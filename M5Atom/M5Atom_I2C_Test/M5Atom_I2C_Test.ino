/*
 * File: M5Atom_I2C_Test.ino
 * Test both I2C busses on M5Atom
    
*/
#include <M5Atom.h>
#include <i2cdetect.h>

#define INFO 1
#define DEBUG 1

void setup() 
{
  M5.begin(true, true, true);
  delay(1000);              // wait for serial monitor
  if (INFO)  printInfo();
  if (DEBUG) Serial.println(F("Initializing..."));
  Serial.println("Test I2C Busses...");
  if (DEBUG) Serial.println(F("Running...")); //last line in setup()
}

void loop() 
{
  Wire.begin(25,21,10000);
  Serial.println("\nScanning internal I2C Bus");
  i2cdetect(1,127);
  delay(2000);

  Wire.begin(26,32,10000);
  Serial.println("\nScanning I2C Bus@GROVE");
  i2cdetect(1,127);
  delay(2000);
}

/*--------------------------------------------------------------*/

void printInfo()
{
  // Compile file path (internal file of compilation proccess)
  Serial.print("File "); Serial.println(__FILE__);
  
  String s1 = __DATE__;
  String s2 = __TIME__;

  // Date of compilation
  Serial.print("Compilation @ "); Serial.println(s1 + " " + s2);
  
  // Arduino IDE SW version
  Serial.print("ARDUINO IDE v"); Serial.println(ARDUINO);
  
  // Oscillator frequency
  Serial.print("CPU Clock in MHz: "); Serial.println(F_CPU/1E6);
}
