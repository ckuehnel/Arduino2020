/*
 * File: Uno_Receive_Serial.ino
 * 
 * Receive SHT31 measuring data from serial interface.
 * On transmitter side use NanoEvery_Transmit_Serial.ino 
 * to build the message 
 * Claus Kühnel 2020-03-27 info@ckuehnel.ch
 */
#include <SoftwareSerial.h>

SoftwareSerial SW_Serial(2, 3); // RX, TX

#define STX 2
#define ETX 3

void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("\nReceiving serial messages...");
  SW_Serial.begin(19200);
}

void loop() 
{
  byte val;
  
  if (SW_Serial.available()) 
  {
    do
    {
      val = SW_Serial.read();
    } while (val != 0x02);
//    Serial.println("\nMessage detected..."); 
    delay(2);
    uint8_t hum = SW_Serial.read(); delay(2);
    uint8_t temp_lo = SW_Serial.read(); delay(2);
    uint8_t temp_hi = SW_Serial.read(); delay(2);
    uint8_t ctrl = SW_Serial.read(); delay(2);
//    if (ctrl == ETX) Serial.println("Message received.");
    
    float temp = (float) (((temp_hi<<8) + temp_lo)/1000.0);
    Serial.print("Temp = "); Serial.print(temp,3); Serial.println(" *C"); 
    Serial.print("Hum  = "); Serial.print(hum); Serial.println(" %RH"); 
    Serial.println();
  }
}
