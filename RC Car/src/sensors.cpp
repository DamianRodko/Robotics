#include "sensors.h"
#include <Arduino.h>
#include <Adafruit_VL53L0X.h>
#include <ICM_20948.h>
#include <Wire.h>

namespace
{
constexpr uint8_t tofShutdownPin = 17;
Adafruit_VL53L0X tofSensor;
ICM_20948_I2C imu;
bool tofReady = false;
bool imuReady = false;
}

void sensorsInit()
{
  pinMode(tofShutdownPin, OUTPUT);
  digitalWrite(tofShutdownPin, LOW);
  delay(10);
  digitalWrite(tofShutdownPin, HIGH);
  delay(10);

  Wire.begin(11, 12);

  tofReady = tofSensor.begin();
  if (!tofReady)
  {
    Serial.println("VL53L0X not found");
  }
  else
  {
    Serial.println("VL53L0X ready");
  }
  delay(100);

  imu.begin(Wire, 0);
  imuReady = imu.status == ICM_20948_Stat_Ok;
  if (!imuReady)
  {
    Serial.println("ICM-20948 not found");
  }
  else
  {
    Serial.println("ICM-20948 ready");
  }
}

void readDistance()
{
  if (!tofReady)
  {
    return;
  }

  VL53L0X_RangingMeasurementData_t measurement;
  tofSensor.rangingTest(&measurement, false);
  if (measurement.RangeStatus == 4 || measurement.RangeMilliMeter >= 2000)
  {
    return;
  }

  Serial.print("Distance: ");
  Serial.print(measurement.RangeMilliMeter);
  Serial.println(" mm");
}

void readImu()
{
  if (!imuReady || !imu.dataReady())
  {
    return;
  }

  imu.getAGMT();
  Serial.print("Accel X: ");
  Serial.print(imu.accX());
  Serial.print(" | Y: ");
  Serial.print(imu.accY());
  Serial.print(" | Z: ");
  Serial.println(imu.accZ());
  Serial.print("Gyro X: ");
  Serial.print(imu.gyrX());
  Serial.print(" | Y: ");
  Serial.print(imu.gyrY());
  Serial.print(" | Z: ");
  Serial.println(imu.gyrZ());
}
