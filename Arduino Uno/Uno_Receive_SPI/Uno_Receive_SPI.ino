/*
 * File: Uno_Receive_SPI.ino
 * 
 * Receive SHT31 measuring data from SPI interface.
 * On transmitter side use Nano_Transmit_SPI.ino 
 * to build the message 
 * Claus Kühnel 2020-03-27 info@ckuehnel.ch
 */
#include <SPI.h>

#define STX 2
#define ETX 3

volatile boolean process_it;
char msg[5];
byte pos = 0;

ISR (SPI_STC_vect) // SPI interrupt routine
{
  byte c = SPDR;  // grab byte from SPI Data Register
  if (pos < (sizeof(msg) - 1)) msg[pos++] = c;
  // ETX means message end
  if (c == ETX) process_it = true;    
} 

void setup (void)
{
  Serial.begin (115200);   // debugging
  while(!Serial);
  Serial.println("\Wait for SPI transfer...");
  
  // turn on SPI in slave mode
  SPCR |= bit (SPE);
  // have to send on master in, *slave out*
  pinMode(MISO, OUTPUT);
  // get ready for an interrupt 
  pos = 0;   // buffer empty
  process_it = false;
  // now turn on interrupts
  SPI.attachInterrupt();
} 

void loop (void)
{
  if (process_it)
  {
    if (msg[0] == STX)
    {
      uint8_t hum = msg[1];
      uint8_t temp_lo = msg[2];
      uint8_t temp_hi = msg[3];
 
      float temp = (float) (((temp_hi<<8) + temp_lo)/1000.0);
      Serial.print("Temp = "); Serial.print(temp,3); Serial.println(" *C"); 
      Serial.print("Hum  = "); Serial.print(hum); Serial.println(" %RH"); 
      Serial.println();
    }
    pos = 0;
    process_it = false;
  }    
} 
