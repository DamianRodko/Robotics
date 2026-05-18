#include <Arduino.h>
#include "motor.h"
#include <ESP32Servo.h>
//ESC
Servo esc;
int escPin = 7;

void motorInit()
{
    esc.attach(escPin, 1000, 2000);
    esc.writeMicroseconds(1500);//arm ESC
    delay(2000);//wait for ESC to arm
    Serial.println("Motor Initialized");
}
void motorForward(int speed)
{
  int pulse = map(speed, 0, 100, 1500, 2000);
  esc.writeMicroseconds(pulse);
}
void motorReverse(int speed)
{
  int pulse = map(speed, 0, 100, 1500, 1000);
  esc.writeMicroseconds(pulse);
}
void motorStop()
{
  esc.writeMicroseconds(1500);
}
void motorTest()
{
  Serial.println("MOTOR FORWARD");
  for (int i = 0; i <= 100; i++)
  {
    motorForward(i);
    Serial.println(i);
    delay(50);
  }

  Serial.println("MOTOR STOP");
  motorStop();
  delay(3000);

  Serial.println("MOTOR REVERSE");
  for (int i = 0; i <= 100; i++)
  {
    motorReverse(i);
    Serial.println(i);
    delay(50);
  }

  Serial.println("Motor test: stop");
  motorStop();
  Serial.println("Motor test: done");
}