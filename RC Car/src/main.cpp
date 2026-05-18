//#define	VS int VS const int VS constexpr
//String vs *char
//Get rid of debug print lines once finished
#include <Arduino.h>
#include <LittleFS.h>
#include "motor.h"
#include "servo.h"
#include "ota.h"
#include "network.h"
#include "neopixel.h"
#include "sensors.h"


//SETUP
void setup()
{
  Serial.begin(115200);
  delay(500);
  Serial.println("ESP32 started");
  LittleFS.begin(true);
  Serial.println("LittleFS started");
  motorInit();
  neopixelInit();
  wifiInit();
  otaInit();
  sensorsInit();
  servoInit();
}

//LOOP
void loop()
{
  wifiHandle();
  otaHandle();

  //neopixelTest();
  //motorTest();
  //servoTest();
  //readToF();
  //readIMU();
}