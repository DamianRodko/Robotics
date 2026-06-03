#include "volt.h"
#include <Arduino.h>
int voltPin = 4;
float dividerRatio = 100.0f/ 330.0f+ 100.0f; 

void voltInit()
{
  analogSetAttenuation(ADC_11db);
}

float voltRead()
{
  // Read the voltage from the sensor
  float vADC = analogReadMilliVolts(voltPin) / 1000.0f; // Convert mV to V
  return vADC / dividerRatio; // Replace with actual voltage reading
}