/*
 * File Uno_extFRAM.ino
 * 
 * Write & read structured datat to/from external FRAM 
 * using Adafruit FRAM library
 * 
 * Claus Kühnel 2020-03-26 info@ckuehnel.ch
 */
#include <Wire.h>
#include <MB85_FRAM.h>

MB85_FRAM_Class myFRAM;

int ival = 123;
float fval = 123.456;
char str[] = "@ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void setup() 
{
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  Serial.println("\nTest external FRAM");
  Wire.begin();

  uint8_t chips = myFRAM.begin(I2C_FAST_MODE); // Return number of memories found
  Serial.print("Detected ");
  Serial.print(chips);
  Serial.println(" MB85xxx memories.");
  Serial.print("Size of external FRAM = ");
  Serial.print(myFRAM.totalBytes());
  Serial.println(" Byte");

  Serial.println("\nWrite data to external FRAM...");
  int addr = 0x100;
  myFRAM.write(addr, ival);   // write int 
  addr += sizeof(int);      // incr. addr w/ size of int
  myFRAM.write(addr, fval);   // write float
  addr += sizeof(float);    // incr. addr w/ size of float
  myFRAM.write(addr, str);    // write string

  Serial.println("\nHexdump EEPROM...");
  for (addr = 0x100; addr < 0x100 + sizeof(int) + sizeof(float) + sizeof(str); addr++)
  {
    byte val = 0;
    myFRAM.read(addr, val);
    if (val < 0x10) Serial.print("0");
    Serial.print(val,HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.println("\nRead data back from external FRAM...");
  addr = 0x100;                         // restore addr
  ival = 0; myFRAM.read(addr, ival);     // read int back
  Serial.print(ival); Serial.print(" ");
  addr += sizeof(int);                  // incr. addr w/ size of int
  fval = 0.0; myFRAM.read(addr, fval);   // read float back
  Serial.print(fval, 3); Serial.print(" ");
  addr += sizeof(float);                // incr. addr w/ size of float
  strcpy(str, ""); myFRAM.read(addr, str); // read string back
  Serial.println(str); 
}

void loop() {}
