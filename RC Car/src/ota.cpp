#include "ota.h"
#include <ArduinoOTA.h>

void otaInit()
{
  ArduinoOTA.setHostname("Damian's ESP32");
  ArduinoOTA.setPassword("DRCOTA"); // change this to whatever you want

  ArduinoOTA.onStart([]()
  {
    Serial.println("OTA Start");
  });
  ArduinoOTA.onEnd([]()
  {
    Serial.println("OTA Done");
  });
  ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
  {
    Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
  });
  ArduinoOTA.onError([](ota_error_t error)
  {
    Serial.printf("OTA Error[%u]\n", error);
  });
  ArduinoOTA.begin();
  Serial.println("OTA Ready");
}

void otaHandle()
{
  ArduinoOTA.handle();
}