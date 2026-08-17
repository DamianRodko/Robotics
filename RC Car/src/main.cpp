#include <Arduino.h>
#include "motor.h"

void setup()
{
  Serial.begin(115200);
  motorInit();

  while (Serial.available())
  {
    Serial.read();
  }

  Serial.println("Motor bench test ready");
  Serial.println("ESC mode: F/B; wheels must be off the ground");
  Serial.println("Type T for 5% forward for 300 ms");
  Serial.println("Type S to force neutral");
}

void loop()
{
  if (!Serial.available())
  {
    return;
  }

  const char command = Serial.read();
  if (command == 't' || command == 'T')
  {
    motorTest();
    while (Serial.available())
    {
      Serial.read();
    }
  }
  else if (command == 's' || command == 'S')
  {
    motorStop();
    Serial.println("ESC neutral");
  }
}
