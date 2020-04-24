/*
 * File: Uno_Receive_I2C.ino
 * 
 * Receive SHT31 measuring data from I2C interface.
 * On transmitter side use Nano_Transmit_I2C.ino 
 * to build the message 
 * Claus Kühnel 2020-03-27 info@ckuehnel.ch
 */
#include <Wire.h>

#define STX 2
#define ETX 3
#define SlaveAddr 5

char msg[5];
volatile boolean process_it;

void receiveEvent()
{
  while(Wire.available())
  {
    for (int i = 0; i < sizeof(msg); i++) msg[i]= Wire.read();    
  }
  process_it = true;
}

void setup (void)
{
  Serial.begin (115200);   // debugging
  while(!Serial);
  Serial.println("\Wait for I2C transfer...");
  Wire.begin(SlaveAddr);
  Wire.onReceive(receiveEvent); // Event handler for I2C receive
} 

void loop (void)
{
  if (process_it)
  {
    if ((msg[0] == STX) && (msg[4] == ETX))
    {
      uint8_t hum = msg[1];
      uint8_t temp_lo = msg[2];
      uint8_t temp_hi = msg[3];
 
      float temp = (float) (((temp_hi<<8) + temp_lo)/1000.0);
      Serial.print("Temp = "); Serial.print(temp,3); Serial.println(" *C"); 
      Serial.print("Hum  = "); Serial.print(hum); Serial.println(" %RH"); 
      Serial.println();
    }
    process_it = false;
  }    
} 
