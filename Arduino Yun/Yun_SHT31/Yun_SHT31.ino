/*
 * File: Yun_SHT31.ino
 * 
 * Get temperatur and humidity from SHT31 for
 * Arduino Yún or Dragino Yún shield w/ Arduino Leonardo
 * 
 * created by Claus Kühnel 2020-04-13 info@ckuehnel.ch
 */
#include <Arduino.h>
#include <Wire.h>
#include <SHT31.h>
#include <Console.h>

const unsigned int cycle = 15000; // measuring cycle 15 sec

SHT31 sht31 = SHT31();

void setup() 
{
  Bridge.begin();
  Console.begin(); 
  sht31.begin();

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
  Console.println(" *C"); 
  Console.print("Hum  = "); 
  Console.print(hum, 0);
  Console.println(" %RH"); 
  Console.println();
  digitalWrite(LED_BUILTIN, LOW);   // sets the LED off
  delay(cycle);
}
