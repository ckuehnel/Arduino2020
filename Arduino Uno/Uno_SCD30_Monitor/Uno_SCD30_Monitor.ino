/*
 * File: Uno_SCD30_Monitor.ino
 * 
 * Get measuring value from Seeed SCD30 – CO2 & TEMPERATURE & HUMIDITY SENSOR
 * Claus Kühnel 2020-04-10 info@ckuehnel.ch
 */
//Include used libraries
#include <LiquidCrystal.h>
#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h" 

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

SCD30 airSensor;

// define some values used by the panel
const int backlight = 10; // Backlight connected to D10

void setup()
{
  Wire.begin();
  airSensor.begin();      //This will cause readings to occur every two seconds
  pinMode(backlight, OUTPUT);
  digitalWrite(backlight, HIGH);
  lcd.begin(16, 2);              // start the library for 2 lines a 16 characters
  lcd.setCursor(0,0);
  lcd.print("SCD30 Monitoring"); // print a simple message
  delay(2000);
  lcd.clear();
}
 
void loop()
{
  clearLine(0);
  lcd.setCursor(0,0);            // move cursor to second line "1" and 9 spaces over
  lcd.print("  CO2 ");
  lcd.print(airSensor.getCO2());
  lcd.print(" ppm");

  clearLine(1);
  lcd.setCursor(0,1);            // move to the begining of the second line
  lcd.print(airSensor.getTemperature(), 1);
  lcd.print(" *C");
  lcd.setCursor(8,1);
  lcd.print(airSensor.getHumidity(), 1);
  lcd.print(" %rH");
  delay(5000);
}

void clearLine(byte line)
{
  lcd.setCursor(0 ,line);
  lcd.print("                ");
}
  
