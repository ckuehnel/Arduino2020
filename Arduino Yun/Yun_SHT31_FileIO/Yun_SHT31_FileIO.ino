/*
 * File: Yun_SHT31_FileIO.ino
 * 
 * Get temperatur and humidity from SHT31 for
 * Arduino Yún or Dragino Yún shield w/ Arduino Leonardo
 * save data to files on Linux-Device
 * 
 * created by Claus Kühnel 2020-04-13 info@ckuehnel.ch
 */
#include <Arduino.h>
#include <Wire.h>
#include <SHT31.h>
#include <Console.h>
#include <FileIO.h>

const unsigned int cycle = 15000; // measuring cycle 60 sec

SHT31 sht31 = SHT31();

void setup() 
{
  Bridge.begin();
  Console.begin(); 
  sht31.begin();
  FileSystem.begin();

  while (!Console); // wait for Console port to connect.
  Console.println("You're connected to the Console");
}

void loop() 
{
  digitalWrite(LED_BUILTIN, HIGH);   // sets the LED on
  float temp = sht31.getTemperature();
  float hum = sht31.getHumidity();
  Console.print("Temp = "); 
  Console.print(temp, 1);
  Console.println(" C"); 
  Console.print("Hum  = "); 
  Console.print(hum, 0);
  Console.println(" %"); 
  Console.println();
  File f = FileSystem.open("/tmp/TEMP", FILE_WRITE);
  f.print(temp);
  f.close();
  File fff = FileSystem.open("/tmp/HUMI", FILE_WRITE);
  fff.print(hum);
  fff.close();
  digitalWrite(LED_BUILTIN, LOW);   // sets the LED off
  delay(cycle);
}
