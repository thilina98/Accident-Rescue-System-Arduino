#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>
#include "Accelerometer.h"

Adafruit_MPU6050 gy_521;

Accelerations acc;

void setup(void)
{
    Serial.begin(9600);

    Serial.print("GY 521 Acceleromter\n");

    if (gy_521.begin())
    {
        Serial.print("Accelerometer found\n");
    }
    else
    {
        Serial.print("Error\n");
    }

    delay(100);
}

void loop(void)
{
    sensors_event_t a, g, temp;
    gy_521.getEvent(&a, &g, &temp);

    float acc_x = a.acceleration.x;
    float acc_y = a.acceleration.y;
    float acc_z = a.acceleration.z;

    Serial.print("Acceleration X: ");
    Serial.print(acc_x);
    Serial.print(", Y: ");
    Serial.print(acc_y);
    Serial.print(", Z: ");

    Serial.print(acc_z);
    Serial.print(" m/s^2");

    Serial.print("\n");

    delay(1000);
}
