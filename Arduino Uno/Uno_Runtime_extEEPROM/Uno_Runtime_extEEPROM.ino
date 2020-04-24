/*
 * File: Uno_Runtime_extEEPROM.ino
 * 
 * Measure runtime of code sequences
 * Claus Kühnel 2020-02-17 info@ckuehnel.ch
 * 
 */
#include <Wire.h>
#include "SparkFun_External_EEPROM.h" 

ExternalEEPROM myEEPROM;

#define DEBUG 0 // set to 1 for Hex dump
 
void setup() 
{
  Serial.begin(115200);
  delay(2000); // wait for Serial Monitor
  Serial.println("\nRuntime measurement of code sequence in program ");
  Serial.println(__FILE__);

  Wire.begin();
  myEEPROM.begin(); 
    
  unsigned long starttime = micros();
  // put your code between the comment lines
  /***************************************************/
  int val = 0;
  for (int addr = 0x100; addr < 0x200; addr++)
  {
    myEEPROM.write(addr, val++);
  }
  /***************************************************/
  unsigned long endtime = micros();
  Serial.print("Runtime of code sequence = ");
  Serial.print(endtime - starttime);
  Serial.println(" us");

  if (DEBUG)
  {
    Serial.println("\nHexdump FRAM...");
    for (int addr = 0x100; addr < 0x200; addr++)
    {
      byte val;
      val = myEEPROM.read(addr);
      if (addr % 16 == 0) Serial.println();
      if (val < 0x10) Serial.print("0");
      Serial.print(val, HEX);
      Serial.print(" ");
    }
    Serial.println();
  }
}

void loop() {}
