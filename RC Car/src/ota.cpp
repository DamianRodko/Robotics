#include "ota.h"
#include "motor.h"
#include "servo.h"
#include <Arduino.h>
#include <ArduinoOTA.h>

namespace
{
unsigned int lastProgressPercent = 101;
}

void otaInit()
{
  ArduinoOTA.setHostname("damians-esp32");
  ArduinoOTA.setPassword("DRCOTA");

  ArduinoOTA.onStart([]()
  {
    motorStop();
    centerServo();
    lastProgressPercent = 101;
    Serial.println("OTA started; controls stopped");
  });
  ArduinoOTA.onEnd([]()
  {
    Serial.println("OTA complete");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
    if (total > 0)
    {
      const unsigned int percent = static_cast<unsigned int>(
          (static_cast<uint64_t>(progress) * 100U) / total);
      if (percent != lastProgressPercent)
      {
        lastProgressPercent = percent;
        Serial.printf("OTA progress: %u%%\n", percent);
      }
    }
  });
  ArduinoOTA.onError([](ota_error_t error)
  {
    Serial.printf("OTA error: %u\n", error);
  });
  ArduinoOTA.begin();
  Serial.println("OTA ready");
}

void otaHandle()
{
  ArduinoOTA.handle();
}
