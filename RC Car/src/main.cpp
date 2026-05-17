//#define	VS int VS const int VS constexpr
//String vs *char
//Get rid of debug print lines once finished
#include <Arduino.h>
#include <Adafruit_NeoPixel.h>
#include <WiFi.h>
#include <WebServer.h>
#include <WebSocketsServer.h>
#include <Adafruit_VL53L0X.h>
#include <Wire.h>
#include <ICM_20948.h>
#include <LittleFS.h>
#include "motor.h"
#include "servo.h"

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
  Serial.println("LittleFS started");
  motorInit();
  servoInit();
  //NEOPIXEL
  pixel.begin();
  pixel.show();
  Serial.println("NeoPixel ready");

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
    setServo(servoVal);

    int spd = constrain(abs(motorVal), 0, 255);

    if (motorVal > 0)
    {
      motorForward(spd);
    }
    else if (motorVal < 0)
    {
      motorReverse(spd);
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
void rgbCycle()
{
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