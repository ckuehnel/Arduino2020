/*
 * File: Nano_Transmit_SPI.ino
 * 
 * Get SHT31 measuring data to send via SPI.
 * On receiver side use Uno_Receive_SPI.ino to decode 
 * and print SHT31 data 
 * Claus Kühnel 2020-03-27 info@ckuehnel.ch
 */
#include <Arduino.h>
#include <SPI.h>
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
  sht31.begin();
  digitalWrite(SS,HIGH);  
  SPI.begin();  
  SPI.setClockDivider(SPI_CLOCK_DIV8);
}

void loop() 
{
  float temp = sht31.getTemperature();
  float hum  = sht31.getHumidity();

  sendMessage(temp, hum);
  delay(1000);
}
