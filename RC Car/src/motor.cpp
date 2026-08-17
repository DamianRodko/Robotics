#include "motor.h"
#include <Arduino.h>

namespace
{
constexpr uint8_t escPin = 18;
constexpr uint8_t escChannel = 1;
constexpr int minSpeed = 0;
constexpr int maxSpeed = 100;
constexpr int neutralPulseUs = 1500;
bool escReady = false;

void writeEscPulse(int pulseUs)
{
  if (!escReady)
  {
    return;
  }

  pulseUs = constrain(pulseUs, 1000, 2000);
  ledcWrite(escChannel, (pulseUs * 16383UL) / 20000UL);
}
}

void motorInit()
{
  escReady = ledcSetup(escChannel, 50, 14) != 0;

  if (!escReady)
  {
    Serial.println("ERROR: ESC PWM setup failed");
    return;
  }

  ledcAttachPin(escPin, escChannel);
  writeEscPulse(neutralPulseUs);
  Serial.println("ESC neutral; waiting 3 seconds to arm");
  delay(3000);
  Serial.print("Motor initialized on GPIO");
  Serial.println(escPin);
}

void motorForward(int speed)
{
  speed = constrain(speed, minSpeed, maxSpeed);
  writeEscPulse(map(speed, minSpeed, maxSpeed, neutralPulseUs, 2000));
}

void motorReverse(int speed)
{
  speed = constrain(speed, minSpeed, maxSpeed);
  writeEscPulse(map(speed, minSpeed, maxSpeed, neutralPulseUs, 1000));
}

void motorStop()
{
  writeEscPulse(neutralPulseUs);
}

void motorTest()
{
  Serial.println("Motor test: 5% forward for 300 ms");
  motorForward(5);
  delay(300);
  motorStop();
  Serial.println("Motor test complete; ESC neutral");
}
