/*
 * File: Nano_Transmit_I2C.ino
 * 
 * Get SHT31 measuring data to send via I2C.
 * On receiver side use Uno_Receive_I2C.ino to decode 
 * and print SHT31 data 
 * Claus Kühnel 2020-03-27 info@ckuehnel.ch
 */
#include <Arduino.h>
#include <Wire.h>
#include "SHT31.h"

#define DEBUG 1

SHT31 sht31 = SHT31();

// Function prototypes
void sendMessage();

void setup() 
{  
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Read SHT31 measuring data...");  
  Wire.begin();
  sht31.begin();
}

void loop() 
{
  float temp = sht31.getTemperature();
  float hum  = sht31.getHumidity();

  sendMessage(temp, hum);
  delay(1000);
}
