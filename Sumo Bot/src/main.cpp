#include <Arduino.h>

int motor1F, motor1R, button, motor2F, motor2R, irF, irB, irFs, irBs, speed;
boolean move, start;
void playGame();
void readSensors();
void setup()
{
  Serial.begin(9600);
  speed = 150;
  motor1F = 10;
  motor1R = 11;
  motor2F = 6;
  motor2R = 5;
  button = 9;
  irFs = 0;
  irBs = 0;
  irF = 14;
  irB = 15;
  move = true;
  start = false;
  pinMode(button, INPUT);
  pinMode(motor1F, OUTPUT);
  pinMode(motor1R, OUTPUT);
  pinMode(motor2F, OUTPUT);
  pinMode(motor2R, OUTPUT);
  pinMode(irF, INPUT);
  pinMode(irB, INPUT);
}

void loop()
{
  playGame();
}

void motorForward()
{
  analogWrite(motor1F, speed);
  analogWrite(motor1R, 0);
  analogWrite(motor2F, speed);
  analogWrite(motor2R, 0);
}

void motorReverse()
{
  analogWrite(motor1F, 0);
  analogWrite(motor1R, speed);
  analogWrite(motor2F, 0);
  analogWrite(motor2R, speed);
}

void motorTurn()
{
  analogWrite(motor1F, speed);
  analogWrite(motor1R, 0);
  analogWrite(motor2F, 0);
  analogWrite(motor2R, speed);
}
void motorStop()
{
  analogWrite(motor1F, 0);
  analogWrite(motor1R, 0);
  analogWrite(motor2F, 0);
  analogWrite(motor2R, 0);
}
void playGame()
{
  if(digitalRead(button) == 1)
  {
    start = true;
  }
  if(start) 
  {
    readSensors();
    //update move variable based off sensors
    if(move && irFs == 1)
    {
      move = false;
    }
    if(!move && irBs == 1)
    {
      move = true;
    }
    //move the motor
    if(move)
    {
      motorForward();
    }
    else
    {
      motorReverse();
    }
  }
  delay(10);
}
void readSensors()
{
  irFs = digitalRead(irF);
  irBs = digitalRead(irB);
    Serial.print("F:");
    Serial.print(irFs);
    Serial.print("B:");
    Serial.print(irBs);
    Serial.print("M:");
    Serial.println(move);
}