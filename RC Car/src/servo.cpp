#include "servo.h"
#include <Arduino.h>

namespace
{
  constexpr uint8_t servoPin = 13;
  constexpr uint8_t servoChannel = 0;
  bool servoReady = false;

  int angleToPulse(int angle)
  {
    return map(angle, 0, 180, 1000, 2000);
  }
}

void servoInit()
{
  servoReady = ledcSetup(servoChannel, 50, 14) != 0;
  if (!servoReady)
  {
    Serial.println("ERROR: steering servo PWM setup failed");
    return;
  }

  ledcAttachPin(servoPin, servoChannel);
  centerServo();
  Serial.print("Steering servo initialized on GPIO");
  Serial.println(servoPin);
}

void setServo(int angle)
{
  if (!servoReady)
  {
    return;
  }

  angle = constrain(angle, servoMinAngle, servoMaxAngle);
  ledcWrite(servoChannel, (angleToPulse(angle) * 16383UL) / 20000UL);
}

void centerServo()
{
  setServo(servoCenterAngle);
}

void servoTest()
{
  Serial.println("SERVO TEST STARTED");

  for (int angle = servoMinAngle; angle <= servoMaxAngle; angle += 5)
  {
    setServo(angle);
    Serial.print("Servo: ");
    Serial.print(angle);
    Serial.print(" | pulse: ");
    Serial.print(angleToPulse(angle));
    Serial.println(" us");
    delay(1500);
  }
  centerServo();
  Serial.println("SERVO TEST COMPLETE");
}
