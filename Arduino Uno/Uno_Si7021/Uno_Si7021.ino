/*  
 * File: Uno_Si7021.ino  
 * 
 * Reads measuring data from Si7021 Sensor.
 * (c) Claus Kuehnel 2020-02-03 info@ckuehnel.ch
 * 
 * based on: SparkFun Si7021 Breakout Example
 * 
 * Hardware Connections:
 *    Si7021 ---------------- Arduino
 *    (-) ------------------- GND
 *    (+) ------------------- 3.3V (VCC)
 *    CL ------------------- SCL
 *    DA ------------------- SDA
 */

#include "SparkFun_Si7021_Breakout_Library.h"
#include <Wire.h>

//Create Instance of Si7021 temp and humidity sensor 
Weather sensor;

void setup()
{
  Serial.begin(115200);   // open serial over USB at 115200 baud
  delay(2000);
  Serial.println("Read Si7021 sensor...");
  sensor.begin(); //Initialize the I2C sensors and ping them
  Serial.println();
}

void loop()
{
  delay(2000);
  getWeather();
}

void getWeather()
{
  // Measure Relative Humidity from the HTU21D or Si7021
  float humi = sensor.getRH();

  // Measure Temperature from the HTU21D or Si7021
  float temp = sensor.getTemp();
  // Temperature is measured every time RH is requested.
  // It is faster, therefore, to read it from previous RH
  // measurement with getTemp() instead with readTemp()
  Serial.print("Humidity: "); Serial.print((int) humi); Serial.print(" %\t");
  Serial.print("Temperature: "); Serial.print(temp, 1); Serial.println(" °C ");
}
