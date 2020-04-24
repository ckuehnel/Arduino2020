/*
 * File: DS2438.ino
 * 
 * Measuring Temperature and voltage @ VAD.
 * The voltage @ VAD represents the relavie humidity measured by HIH-4031.
 * Based on: https://github.com/TaaraLabs/OneWireHumidityLight-Demo by Märt Maiste, TaaraLabs 2017
 */
#include <OneWire.h>
#include <DallasTemperature.h>

// Data wire is plugged into port 2 on the Arduino
#define ONE_WIRE_BUS 2

OneWire ds(ONE_WIRE_BUS); // 1-Wire data line @ I/O-Pin 2

// Pass our oneWire reference to Dallas Temperature. 
DallasTemperature sensors(&ds);

int DS18Count = 0;

#define DEBUG 0

void setup(void) 
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("\Read data from DS18B20 and 1-Wire Module w/ DS18B20 und DS2438 & HIH-4031...");
  initDS18B20();
}

void loop(void)
{
  Serial.print  ("----------------------------------");
  Serial.println("----------------------------------");
  readDS18B20();
  readDS2438();
  delay(1000);
}
