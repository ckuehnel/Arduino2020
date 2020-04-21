/*
 * File: Uno_DS18B20.ino - DS18B20 Test
 * 
 * based on https://www.hackster.io/TheGadgetBoy/ds18b20-digital-temperature-sensor-and-arduino-9cc806  
 * 
 * Hardware Connections:
 *   DS18B20 ---------------- Arduino
 *   black   ---------------- GND
 *   red     ---------------- 3.3V (VCC)
 *   yellow  ---------------- 10
 */

#include <OneWire.h>
#include <DallasTemperature.h>
 
// Data wire is plugged into pin 10 on the Arduino
#define ONE_WIRE_BUS 10
 
// Setup a oneWire instance to communicate with any OneWire devices 
// (not just Maxim/Dallas temperature ICs)
OneWire oneWire(ONE_WIRE_BUS);
 
// Pass our oneWire reference to Dallas Temperature.
DallasTemperature sensors(&oneWire);
 
void setup(void)
{
  // start serial port
  Serial.begin(115200);
  delay(2000); // wait for serial monitor
  Serial.println("Dallas Temperature IC Control Library Demo");

  // Start up the library
  sensors.begin();
}
  
void loop(void)
{
  // call sensors.requestTemperatures() to issue a global temperature
  // request to all devices on the bus
  Serial.print(" Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  Serial.print(" Done\t");
  Serial.print("Temperature: ");
  Serial.print(sensors.getTempCByIndex(0), 1); // Why "byIndex"? 
  // You can have more than one IC on the same bus. 
  // 0 refers to the first IC on the wire
  Serial.println(" *C");
  delay(1000);
}
