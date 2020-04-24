/*
 * File: NanoEvery_Transmit_Serial.ino
 * 
 * Get SHT31 measuring data to send serial.
 * On receiver side use Uno_Receive_Serial.ino to decode 
 * and print SHT31 data 
 * Claus Kühnel 2020-03-27 info@ckuehnel.ch
 */
#include <Arduino.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "SHT31.h"

#define DEBUG 1

float temp, hum;

SHT31 sht31 = SHT31();
SoftwareSerial SW_Serial(2, 3); // RX, TX

// Function prototypes
void sendMessage();

void setup() 
{  
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Read SHT31 measuring data...");  
  SW_Serial.begin(19200);
  sht31.begin();  
}

void loop() 
{
  float temp = sht31.getTemperature();
  float hum  = sht31.getHumidity();

  sendMessage(temp, hum);
  delay(1000);
}
