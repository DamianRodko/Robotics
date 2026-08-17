#include <Arduino.h>
#include <LittleFS.h>

#include "neopixel.h"
#include "network.h"
#include "servo.h"
#include "volt.h"

void setup()
{
  Serial.begin(115200);
  delay(1500);

  Serial.println();
  Serial.println("RC firmware starting");

  if (!LittleFS.begin(true))
  {
    Serial.println("ERROR: LittleFS mount failed");
  }
  else
  {
    Serial.println("LittleFS ready");
  }

  servoInit();
  voltInit();
  neopixelInit();
  wifiInit();
}

void loop()
{
  wifiHandle();
}
