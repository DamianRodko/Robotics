#include "sensors.h"
#include <Adafruit_VL53L0X.h>
#include <Wire.h>
#include <ICM_20948.h>
//ToF
Adafruit_VL53L0X lox;

//IMU
ICM_20948_I2C imu;
void sensorsInit()
{
  //I2C
  Wire.begin(15, 16);
  //Wire.setClock(100000);
  
  //ToF
  if (!lox.begin())
  {
    Serial.println("Sensor not found");
  }
  else
  {
    Serial.println("VL53L0X Ready");
  }
  delay(100);

  //IMU
  imu.begin(Wire, 1); // 0 for GND, 1 for +
  if (imu.status != ICM_20948_Stat_Ok)
  {
    Serial.println("IMU not found");
  }
  else
  {
    Serial.println("IMU Ready");
  }
}

void readToF()
{
  VL53L0X_RangingMeasurementData_t measure;
  lox.rangingTest(&measure, false);
  if (measure.RangeStatus != 4 && measure.RangeMilliMeter < 2000)
  {
    Serial.print("Distance: ");
    Serial.print(measure.RangeMilliMeter);
    Serial.println(" mm");
  }
}

void readIMU()
{
  if (imu.dataReady())
  {
    imu.getAGMT();
    Serial.print("Accel X: ");
    Serial.print(imu.accX());
    Serial.print(" | Y: ");
    Serial.print(imu.accY());
    Serial.print(" | Z: ");
    Serial.println(imu.accZ());
    Serial.print("Gyro  X: ");
    Serial.print(imu.gyrX());
    Serial.print(" | Y: "); 
    Serial.print(imu.gyrY());
    Serial.print(" | Z: ");
    Serial.println(imu.gyrZ());
  }
}