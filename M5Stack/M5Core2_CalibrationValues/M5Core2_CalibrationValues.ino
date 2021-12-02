/*
 * File: M5Core2_CalibrationValues.ino
 * 
 * Get the calibration value airValue & waterValue for Soil Moisture Sensor
 * 
 * 2021-11-16 Claus Kühnel info@ckuehnel.ch
 */

#include <M5Core2.h>

const int soilPin = 36;

void setup() 
{
  M5.begin();       //Init M5Stack.  
  while(!Serial);
  Serial.println("Get calibration values for Soil Moisture Sensors");
  Serial.println("Hold the sensor in air to get the airValue and notify it.");
  Serial.println("Hold the sensor in water to get the waterValue afterwards and notify it");
}
  
void loop() 
{
  uint16_t val;
  val = analogRead(soilPin); //connect sensor to PortB
  Serial.println(val);  //print the value to serial port
  Serial.print("\t");
  Serial.print(val*3300/4096);
  Serial.println(" mV");
  delay(1000);
}
