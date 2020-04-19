/*
 * File: M5Atom_ENV_Unit.ino
 * 
 * ENV Unit connected to M5Atom Lite via Grove Connector
 * Created 2020-04-19 Claus Kühnel info@ckuehnel.ch   
*/
#include <M5Atom.h>
#include "DHT12.h"
#include <Wire.h> //The DHT12 uses I2C comunication.
#include <BMx280I2C.h>

#define INFO 1
#define DEBUG 1

DHT12 dht12; //Preset scale CELSIUS and ID 0x5c.

#define I2C_ADDRESS 0x76
//create a BMx280I2C object using the I2C interface with I2C Address 0x76
BMx280I2C bmx280(I2C_ADDRESS);

void setup() 
{
  M5.begin(true, true, true); // Serial, I2C, Display
  delay(3000);              // wait for serial monitor
  if (INFO)  printInfo();
  if (DEBUG) Serial.println(F("Initializing..."));
  M5.dis.drawpix(0, 0xf00000);
  Serial.println(F("ENV Unit(DHT12 and BMP280) test...")); 
  Wire.begin(26,32);
  //begin() checks the Interface, reads the sensor ID (to differentiate between BMP280 and BME280)
  //and reads compensation parameters.
  if (!bmx280.begin())
  {
    Serial.println("Error: Check your BMx280 Interface and I2C Address.");
    Serial.println("Program stopped.");
    M5.dis.drawpix(0, 0x00f000);
    while (1);
  }
  if (bmx280.isBME280())
    Serial.println("Sensor is BME280");
  else
    Serial.println("Sensor is BMP280");

  //reset sensor to default parameters.
  bmx280.resetToDefaults();

  //by default sensing is disabled and must be enabled by setting a non-zero
  //oversampling setting.
  //set an oversampling setting for pressure and temperature measurements. 
  bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
  bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

  //if sensor is a BME280, set an oversampling setting for humidity measurements.
  if (bmx280.isBME280()) bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);
  Serial.println("Initializing of sensors done."); 
  if (DEBUG) Serial.println(F("Running...")); //last line in setup()
}

void loop() 
{
  Wire.begin(26,32);
  M5.dis.drawpix(0, 0x0000F0);
  delay(100);
  M5.dis.drawpix(0, 0x000000);
  float tmp = dht12.readTemperature();
  float hum = dht12.readHumidity();
//  float pressure = bme.readPressure();
  Serial.println("\nDHT12  Sensor Date:");
  Serial.printf("Temperature: %2.1f *C\r\n", tmp);
  Serial.printf("Humidity: %2.0f %%\r\n", hum);

  Serial.println("\nBMx280 Sensor Data:");
  //start a measurement
  if (!bmx280.measure())
  {
    Serial.println("could not start measurement, is a measurement already running?");
    return;
  }
  //wait for the measurement to finish
  do
  {
    delay(100);
  } while (!bmx280.hasValue());

  Serial.printf("Pressure: %2.1f Pa\r\n", bmx280.getPressure());
  Serial.printf("Pressure (64 bit): %2.2f Pa\r\n", bmx280.getPressure64()); 
  Serial.printf("Temperature: %2.1f *C\r\n", bmx280.getTemperature());

  if (bmx280.isBME280())
  {
    Serial.print("Humidity: "); 
    Serial.println(bmx280.getHumidity());
  }
  delay(2500);
}

/*--------------------------------------------------------------*/

void printInfo()
{
  // Compile file path (internal file of compilation proccess)
  Serial.print("\nFile "); Serial.println(__FILE__);
  
  String s1 = __DATE__;
  String s2 = __TIME__;

  // Date of compilation
  Serial.print("Compilation @ "); Serial.println(s1 + " " + s2);
  
  // Arduino IDE SW version
  Serial.print("ARDUINO IDE v"); Serial.println(ARDUINO);
  
  // Oscillator frequency
  Serial.print("CPU Clock in MHz: "); Serial.println(F_CPU/1E6);
}
