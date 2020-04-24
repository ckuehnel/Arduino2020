/*
 * File: Uno_Receive_CAN.ino
 * 
 * Receive SHT31 measuring data from CAN interface.
 * On transmitter side use Nano_Transmit_CAN.ino 
 * to build the message 
 * Used library: https://github.com/Longan-Labs/Serial_CAN_Arduino
 * Claus Kühnel 2020-03-27 info@ckuehnel.ch
 */
#include <Serial_CAN_Module.h>
#include <SoftwareSerial.h>

Serial_CAN can;

#define can_tx  2           // tx of serial can module connect to D2
#define can_rx  3           // rx of serial can module connect to D3

#define DEBUG 0

#define STX 2
#define ETX 3

unsigned long mask[4] = 
{
    0, 0x1FFFFFFC,          // ext, maks 0
    0, 0x1FFFFFFF,          // ext, mask 1
};

unsigned long filt[12] = 
{
    0, 0x00000054,          // ext, filt 0
    0, 0x00,                // ext, filt 1
    0, 0x00,                // ext, filt 2
    0, 0x00,                // ext, filt 3
    0, 0x00,                // ext, filt 4
    0, 0x00,                // ext, filt 5
};

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("\Initialize CAN node...");
  can.begin(can_tx, can_rx, 9600);      // tx, rx
  Serial.println("Set accepteance filter...");
  if(can.setFilt(filt)) Serial.println("Setting OK");
    else Serial.println("Setting failed");
  Serial.println("Set acceptance masks...");     
  if(can.setMask(mask)) Serial.println("Setting OK");
    else Serial.println("Setting failed"); 
}

unsigned long id = 0;
unsigned char dta[8];

// send(unsigned long id, byte ext, byte rtrBit, byte len, const byte *buf);
void loop()
{
  if(can.recv(&id, dta))
  {
    Serial.println("----------------------------"); 
    Serial.print("Got data w/ ID: ");
    Serial.println(id, HEX);
    if (DEBUG)
    {
      for(int i = 0; i < 5; i++)
      {
        Serial.print("0x");
        Serial.print(dta[i], HEX);
        Serial.print('\t');
      }
      Serial.println();
    }
    if ((dta[0] == STX) && (dta[4] == ETX))
    {
      float temp = (float) (((dta[3]<<8) + dta[2])/1000.0);
      Serial.print("Temp = "); Serial.print(temp,3); Serial.println(" *C"); 
      Serial.print("Hum  = "); Serial.print(dta[1]); Serial.println(" %RH"); 
    } 
    //for (int i = 0; i < sizeof(dta); i++) dta[i] = 0;
  }
}
