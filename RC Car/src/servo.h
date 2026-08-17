#ifndef SERVO_H
#define SERVO_H

constexpr int servoMinAngle = 0;
constexpr int servoCenterAngle = 80;
constexpr int servoMaxAngle = 150;

void servoInit();
void setServo(int angle);
void centerServo();
void servoTest();

#endif
