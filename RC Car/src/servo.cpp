#include "servo.h"
#include <ESP32Servo.h>

Servo s1;
int servoPin = 13;
int minAngle = 60;
int maxAngle = 120;

void servoInit()
{
    s1.attach(servoPin);
    Serial.println("Servo attached");
    s1.write(90);
}

void setServo(int angle)
{
  angle = constrain(angle, minAngle, maxAngle);
  s1.write(angle);
}

void servoTest()
{
  Serial.println("SERVO RIGHT");
  for (int angle = minAngle; angle <= maxAngle; angle++)
  {
    s1.write(angle);
    delay(15);
  }
  Serial.println("SERVO LEFT");
  for (int angle = maxAngle; angle >= minAngle; angle--) {
    s1.write(angle);
    delay(15);
  }
}