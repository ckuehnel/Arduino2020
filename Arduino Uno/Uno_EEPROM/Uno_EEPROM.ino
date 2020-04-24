/*
 * File Uno_internalEEPROM.ino
 * 
 * Write & read structured datat to/from internal EEPROM 
 * using internal EEPROM library
 * 
 * Claus Kühnel 2020-03-26 info@ckuehnel.ch
 */
#include <EEPROM.h>

int ival = 123;
float fval = 123.456;
char str[] = "@ABCDEFGHIJKLMNOPQRSTUVWXYZ";

void setup() 
{
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  Serial.println("\nTest internal EEPROM");

  int sz = EEPROM.length();
  Serial.print("Size of internal EEPROM = ");
  Serial.print(sz); Serial.println(" Byte");

  Serial.println("\nWrite data to internal EEPROM...");
  int addr = 0x100;
  EEPROM.put(addr, ival);   // write int 
  addr += sizeof(int);      // incr. addr w/ size of int
  EEPROM.put(addr, fval);   // write float
  addr += sizeof(float);    // incr. addr w/ size of float
  EEPROM.put(addr, str);    // write string

  Serial.println("\nHexdump EEPROM...");
  for (addr = 0x100; addr < 0x100 + sizeof(int) + sizeof(float) + sizeof(str); addr++)
  {
    byte val = EEPROM[addr];
    if (val < 0x10) Serial.print("0");
    Serial.print(val,HEX);
    Serial.print(" ");
  }
  Serial.println();

  Serial.println("\nRead data back from internal EEPROM...");
  addr = 0x100;                         // restore addr
  ival = 0; EEPROM.get(addr, ival);     // read int back
  Serial.print(ival); Serial.print(" ");
  addr += sizeof(int);                  // incr. addr w/ size of int
  fval = 0.0; EEPROM.get(addr, fval);   // read float back
  Serial.print(fval, 3); Serial.print(" ");
  addr += sizeof(float);                // incr. addr w/ size of float
  strcpy(str, ""); EEPROM.get(addr, str); // read string back
  Serial.println(str); 
}

void loop() {}
