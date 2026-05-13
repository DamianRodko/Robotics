//#define	VS int VS const int VS constexpr
//String vs *char
//Get rid of debug print lines once finished
#include <Arduino.h>
#include <ESP32Servo.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <Adafruit_VL53L0X.h>
#include <Wire.h>
#include <ICM_20948.h>
#include <LittleFS.h>

void handleRoot();
void handleNotFound();
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length);
void readToF();
void readIMU();

//ToF
Adafruit_VL53L0X lox;

//IMU
ICM_20948_I2C imu;

//WiFi
const char* ssid = "Damian's ESP32";
const char* password = "12345678";
WebServer server(80);
WebSocketsServer ws = WebSocketsServer(81);

//SERVO
Servo s1;
int servoPin = 13;
int sAngle = 90;
int minAngle = 60;
int maxAngle = 120;

//MOTOR
int motorF = 5;
int motorR = 6;
int motorPwmPin = 7;
int speed = 200;

//PWM
int pwmFreq = 5000;  //tune
int pwmRes = 8;

//NEOPIXEL
Adafruit_NeoPixel pixel(1, 48, NEO_GRB + NEO_KHZ800);
int rgbState = 0;

//SETUP
void setup()
{
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP32 started");
  LittleFS.begin(true);
  Serial.println("LittleFS started")

  //SERVO
  s1.attach(servoPin);
  Serial.println("Servo attached");
  s1.write(sAngle);
  //MOTOR
  pinMode(motorF, OUTPUT);
  pinMode(motorR, OUTPUT);
  Serial.println("Motor pins ready");

  //NEOPIXEL
  pixel.begin();
  pixel.show();
  Serial.println("NeoPixel ready");

  //PWM
  ledcAttach(motorPwmPin, pwmFreq, pwmRes);
  ledcWrite(motorPwmPin, 0);
  Serial.println("PWM ready");

  //WiFi
  WiFi.softAP(ssid, password);
  Serial.println("WiFi Started");
  Serial.print("IP: ");
  Serial.println(WiFi.softAPIP());
  ws.begin();
  ws.onEvent(onWebSocketEvent);
  Serial.println("WebSocket Connected");
  server.on("/", handleRoot);
  server.onNotFound(handleNotFound);
  server.begin();
  
  //I2C
  Wire.begin(15, 16);
  //Wire.setClock(100000);
  
  //ToF
  if (!lox.begin())
  {
    Serial.println("Sensor not found");
  }
  else
  {
    Serial.println("VL53L0X Ready");
  }
  delay(100);

  //IMU
  imu.begin(Wire, 1); // 0 for GND, 1 for +
  if (imu.status != ICM_20948_Stat_Ok)
  {
    Serial.println("IMU not found");
  }
  else
  {
    Serial.println("IMU Ready");
  }
}

//LOOP
void loop()
{
  //server.handleClient();
  //ws.loop();
  //readToF();
  //readIMU();
  // rgbCycle();
  delay(1);
  //servoTest();
  //motorTest();
}

//SERVO
void servoTest() {
  Serial.println("SERVO RIGHT");
  for (int angle = minAngle; angle <= maxAngle; angle++) {
    s1.write(angle);
    delay(15);
  }
  Serial.println("SERVO LEFT");
  for (int angle = maxAngle; angle >= minAngle; angle--) {
    s1.write(angle);
    delay(15);
  }
}

//MOTOR FUNCTIONS
void motorForward() {
  digitalWrite(motorF, HIGH);
  digitalWrite(motorR, LOW);
  ledcWrite(motorPwmPin, speed);
}
void motorReverse() {
  digitalWrite(motorF, LOW);
  digitalWrite(motorR, HIGH);
  ledcWrite(motorPwmPin, speed);
}
void motorStop() {
  digitalWrite(motorF, LOW);
  digitalWrite(motorR, LOW);
  ledcWrite(motorPwmPin, 0);
}
void motorTest()
{
  Serial.println("MOTOR FORWARD");
  digitalWrite(motorF, HIGH);
  digitalWrite(motorR, LOW);
  for (int i = 0; i <= 255; i++)
  {
    ledcWrite(motorPwmPin, i);
    Serial.println(i);
    delay(15);
  }

  Serial.println("MOTOR STOP");
  motorStop();
  delay(10000);

  Serial.println("MOTOR REVERSE");
  digitalWrite(motorF, LOW);
  digitalWrite(motorR, HIGH);
  for (int i = 0; i <= 255; i++)
  {
    ledcWrite(motorPwmPin, i);
    Serial.println(i);
    delay(15);
  }

  Serial.println("Motor test: stop");
  motorStop();
  Serial.println("Motor test: done");
}

//ToF
void readToF()
{
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  if (measure.RangeStatus != 4 && measure.RangeMilliMeter < 2000)
  {
    Serial.print("Distance: ");
    Serial.print(measure.RangeMilliMeter);
    Serial.println(" mm");
  }
}

//IMU
void readIMU()
{
  if (imu.dataReady())
  {
    imu.getAGMT();
    Serial.print("Accel X: "); Serial.print(imu.accX());
    Serial.print(" Y: ");      Serial.print(imu.accY());
    Serial.print(" Z: ");      Serial.println(imu.accZ());
    Serial.print("Gyro  X: "); Serial.print(imu.gyrX());
    Serial.print(" Y: ");      Serial.print(imu.gyrY());
    Serial.print(" Z: ");      Serial.println(imu.gyrZ());
  }
}

//WiFi
void onWebSocketEvent(uint8_t num, WStype_t type, uint8_t* payload, size_t length)
{
  if (type == WStype_TEXT)
  {
    String msg = String((char*)payload);
    Serial.println("WS received: " + msg);
    int motorVal = 0;
    int servoVal = 90;

    int mIndex = msg.indexOf("\"motor\":");
    int sIndex = msg.indexOf("\"servo\":");

    if (mIndex != -1)
    {
      motorVal = msg.substring(mIndex + 8).toInt();
    }
    if (sIndex != -1)
    {
      servoVal = msg.substring(sIndex + 8).toInt();
    }
    servoVal = constrain(servoVal, minAngle, maxAngle);
    sAngle = servoVal;
    s1.write(servoVal);

    speed = constrain(abs(motorVal), 0, 255);

    if (motorVal > 20)
    {
      motorForward();
    }
    else if (motorVal < -20)
    {
      motorReverse();
    }
    else 
    {
      motorStop();
    }
  }
}
void handleRoot()
{
  server.serveStatic("/", LittleFS, "/index.html");
}

void handleNotFound()
{
  Serial.print("No handler for: ");
  Serial.println(server.uri());
  server.send(404, "text/plain", "Not found");
}

//NEOPIXEL
void rgbCycle() {
  int r, g, b;
  if (rgbState < 85) {
    r = 255 - rgbState * 3;
    g = rgbState * 3;
    b = 0;
  } else if (rgbState < 170) {
    int temp = rgbState - 85;
    r = 0;
    g = 255 - temp * 3;
    b = temp * 3;
  } else {
    int temp = rgbState - 170;
    r = temp * 3;
    g = 0;
    b = 255 - temp * 3;
  }
  setColor(r, g, b);
  rgbState++;
  if (rgbState > 255) {
    rgbState = 0;
  }
}
void setColor(int r, int g, int b) {
  pixel.setPixelColor(0, pixel.Color(r, g, b));
  pixel.show();
}