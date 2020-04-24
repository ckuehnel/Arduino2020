/*
 * File: Nano_Transmit_CAN.ino
 * 
 * Get SHT31 measuring data to send via CAN bus.
 * On receiver side use Uno_Receive_CAN.ino to decode 
 * and print SHT31 data 
 * Used library: https://github.com/Longan-Labs/Serial_CAN_Arduino
 * Claus Kühnel 2020-03-27 info@ckuehnel.ch
 */
#include <Serial_CAN_Module.h>
#include <SoftwareSerial.h>
#include <Wire.h>
#include "SHT31.h"

Serial_CAN can;

#define can_tx  2           // tx of serial can module connect to D2
#define can_rx  3           // rx of serial can module connect to D3

#define DEBUG 1

SHT31 sht31 = SHT31();

// Function prototypes
void sendMessage();

void setup() 
{  
  Serial.begin(115200);
  while(!Serial);
  Serial.println("\nRead SHT31 measuring data...");
  Serial.println("----------------------------");  
  can.begin(can_tx, can_rx, 9600); // tx, rx
  sht31.begin();
}

void loop() 
{
  float temp = sht31.getTemperature();
  float hum  = sht31.getHumidity();

  sendMessage(temp, hum);
  delay(2500);
}
