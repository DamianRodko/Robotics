#include <Arduino.h>

//Set Vars
const int setupTime = 5000;
const int autonTime = 15000;
const int stopTime = 2000;

//Motors
const int MotorLForward = 10;
const int MotorLReverse = 11;
const int MotorRForward = 9;
const int MotorRReverse = 8;

//Photoresistor
const int prL = A0;//Left
const int prR = A1;//Right
const int prF = A3;//Forward

//IR Sensor Pins
const int irL = 6;
const int irR = 7;

//Ultrasonic Sensor Pins
const int trig = 2;
const int echo = 3;

//Buttons
const int bPath = 4;

//led
const int ledPath = 13; //on = left, off = right

//Tune
const int baseLightVal = 600;//photoresistor threshold 
const int autoSpeed = 210;
const int turnStrength = 65;//auton IR correction strength 
const int teleSpeed = 200;
const int wallTurnDist = 20; 
const int wallTurnTime = 400; 

//State
bool wallTurnLeft = false;

//Sensor Values
int  prLS, prFS, prRS;
bool irLS, irRS;
long wallDist;

void setup()
{
  Serial.begin(9600);

  pinMode(MotorLForward, OUTPUT);
  pinMode(MotorLReverse, OUTPUT);
  pinMode(MotorRForward, OUTPUT);
  pinMode(MotorRReverse, OUTPUT);

  pinMode(irL, INPUT);
  pinMode(irR, INPUT);

  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);

  pinMode(bPath,  INPUT_PULLUP);

  pinMode(13, OUTPUT); //led indicator
}

void loop()
{
  long mil = millis();
  //Path
  if (digitalRead(bPath) == LOW)
  {
    wallTurnLeft = !wallTurnLeft;
    delay(400);
  }
  if (wallTurnLeft == true)
  {
    digitalWrite(ledPath, HIGH);
  }
  else
  {
    digitalWrite(ledPath, LOW);
  }
  
  //Mode timer
  if (mil < setupTime)
  {

  }
  else if (mil < autonTime + setupTime)
  {
    auton();
  }
  else if (mil < autonTime + setupTime + stopTime)
  {
    motorStop();
  }
  else
  {
    teleOp();
  }
}

void readSensors()
{
  prFS = analogRead(prF);
  prRS = analogRead(prR);
  prLS = analogRead(prL);

  irLS = (digitalRead(irL) == LOW);
  irRS = (digitalRead(irR) == LOW);

  wallDist = getUltrasonicCM();

  Serial.print("RIGHT LIGHT: ");
  Serial.println(prRS);
  Serial.print("LEFT LIGHT: ");
  Serial.println(prLS);
  Serial.print("FORWARDS LIGHT: ");
  Serial.println(prFS);
  Serial.print("RIGHT IR: ");
  Serial.println(irRS);
  Serial.print("LEFT IR: ");
  Serial.println(irLS);
  Serial.print("WALL: ");
  Serial.println(wallDist);
}
long getUltrasonicCM()
{
  // send pulse
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duration = pulseIn(echo, HIGH, 30000);

  // convert to cm 
  if (duration == 0)// no wall detected
  {
    return 999;
  }
  return duration / 58;
}

void auton()
{
  readSensors();
  int left  = autoSpeed;
  int right = autoSpeed;

  if (wallDist <= wallTurnDist)
  {
    if(wallTurnLeft)
    {
      motorDrive(-autoSpeed, autoSpeed);
    }
    else
    {
      motorDrive(autoSpeed, -autoSpeed);
    }
    delay(wallTurnTime);
    return;// Change if needed 
  }
  if (irLS && !irRS)//on left -> turn right
  {
    left  += turnStrength;
    right -= turnStrength;
  }
  else if (irRS && !irLS)//on right -> turn left
  {
    left  -= turnStrength;
    right += turnStrength;

  } else if (irLS && irRS)//
  {
    // Both walls (narrow section) -> stay straight, no correction
    left  = autoSpeed;
    right = autoSpeed;
  }
  motorDrive(left, right);
}

//Front Left -> Forward 
//Front Right -> Backwards
//Front -> Left
//Back -> Right
//None -> Stop
void teleOp()
{
  readSensors();
  bool forward  = (prFS > baseLightVal);
  bool right = (prRS > baseLightVal);
  bool left  = (prLS > baseLightVal);

  if (forward)
  {
    motorForward();
  }
  else if (right)
  {
    motorTurnR();
  }
  else if (left)
  {
    motorTurnL();
  }
  else
  {
    motorStop();
  }
}

void motorDrive(int left, int right)
{
  // Left motor
  analogWrite(MotorLForward, constrain( left, 0, 255));
  analogWrite(MotorLReverse, constrain(-left, 0, 255));
  // Right motor
  analogWrite(MotorRForward, constrain( right, 0, 255));
  analogWrite(MotorRReverse, constrain(-right, 0, 255));
}

void motorForward()
{
  motorDrive(teleSpeed,  teleSpeed);
}
void motorTurnL()  
{
  motorDrive(-teleSpeed,  teleSpeed);
} // pivot left
void motorTurnR() 
{
  motorDrive( teleSpeed, -teleSpeed);
} // pivot right
void motorStop()
{
  motorDrive(0, 0);
}