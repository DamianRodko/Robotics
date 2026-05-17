#include <Arduino.h>
#include "motor.h"
//MOTOR
int motorF = 5;
int motorR = 6;
int motorPwmPin = 7;
//PWM
int pwmFreq = 5000;  //tune
int pwmRes = 8;

void motorInit()
{
    pinMode(motorF, OUTPUT);
    pinMode(motorR, OUTPUT);
    //PWM
    ledcAttach(motorPwmPin, pwmFreq, pwmRes);
    ledcWrite(motorPwmPin, 0);
    Serial.println("Motor Initialized");
}
void motorForward(int speed)
{
  digitalWrite(motorF, HIGH);
  digitalWrite(motorR, LOW);
  ledcWrite(motorPwmPin, speed);
}
void motorReverse(int speed)
{
  digitalWrite(motorF, LOW);
  digitalWrite(motorR, HIGH);
  ledcWrite(motorPwmPin, speed);
}
void motorStop()
{
  digitalWrite(motorF, LOW);
  digitalWrite(motorR, LOW);
  ledcWrite(motorPwmPin, 0);
}
void motorTest()
{
  Serial.println("MOTOR FORWARD");
  digitalWrite(motorF, HIGH);
  digitalWrite(motorR, LOW);
  for (int i = 0; i <= 255; i++)
  {
    ledcWrite(motorPwmPin, i);
    Serial.println(i);
    delay(15);
  }

  Serial.println("MOTOR STOP");
  motorStop();
  delay(10000);

  Serial.println("MOTOR REVERSE");
  digitalWrite(motorF, LOW);
  digitalWrite(motorR, HIGH);
  for (int i = 0; i <= 255; i++)
  {
    ledcWrite(motorPwmPin, i);
    Serial.println(i);
    delay(15);
  }

  Serial.println("Motor test: stop");
  motorStop();
  Serial.println("Motor test: done");
}