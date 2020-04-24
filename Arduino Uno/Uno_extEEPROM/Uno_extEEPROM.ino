/*
 * File Uno_extEEPROM.ino
 * 
 * Write & read structured datat to/from external EEPROM 
 * using Sparkfun external EEPROM library
 * 
 * Claus Kühnel 2020-03-26 info@ckuehnel.ch
 */
#include <Wire.h>
#include "SparkFun_External_EEPROM.h" 

ExternalEEPROM myEEPROM;

int ival = 123;
float fval = 123.456;
char str[] = "@ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void setup() 
{
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  Serial.println("\nTest external EEPROM");
  Wire.begin();

  if (myEEPROM.begin() == false)
  {
    Serial.println("Error - No memory detected.");
    while (1);
  }
  Serial.println("External EEPROM detected!");

  uint32_t sz = myEEPROM.length();
  Serial.print("Size of external EEPROM = ");
  Serial.print(sz); Serial.println(" Byte");

  Serial.println("\nWrite data to external EEPROM...");
  int addr = 0x100;
  myEEPROM.put(addr, ival);   // write int 
  addr += sizeof(int);      // incr. addr w/ size of int
  myEEPROM.put(addr, fval);   // write float
  addr += sizeof(float);    // incr. addr w/ size of float
  myEEPROM.put(addr, str);    // write string

  Serial.println("\nHexdump EEPROM...");
  for (addr = 0x100; addr < 0x100 + sizeof(int) + sizeof(float) + sizeof(str); addr++)
  {
    byte val = myEEPROM.read(addr);
    if (val < 0x10) Serial.print("0");
    Serial.print(val,HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.println("\nRead data back from external EEPROM...");
  addr = 0x100;                         // restore addr
  ival = 0; myEEPROM.get(addr, ival);     // read int back
  Serial.print(ival); Serial.print(" ");
  addr += sizeof(int);                  // incr. addr w/ size of int
  fval = 0.0; myEEPROM.get(addr, fval);   // read float back
  Serial.print(fval, 3); Serial.print(" ");
  addr += sizeof(float);                // incr. addr w/ size of float
  strcpy(str, ""); myEEPROM.get(addr, str); // read string back
  Serial.println(str); 
}

void loop() {}
