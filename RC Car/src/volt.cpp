#include "volt.h"
#include <Arduino.h>

namespace
{
constexpr uint8_t voltagePin = 10;
}

void voltInit()
{
  analogSetPinAttenuation(voltagePin, ADC_11db);
  Serial.print("Battery voltage ADC initialized on GPIO");
  Serial.println(voltagePin);
}

float voltRead()
{
  // Scale the ADC voltage back through the 330k/100k divider.
  return (analogReadMilliVolts(voltagePin) / 1000.0f) * 4.3f;
}
