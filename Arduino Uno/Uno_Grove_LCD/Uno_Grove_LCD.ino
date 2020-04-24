/*
 * File: Uno_Grove_LCD.ino
 * 
 * This program controls the Grove_LCD via I2C-bus
 * 
 * Works with
 * Grove - 16x2 LCD (White on Blue)
 * Grove - 16x2 LCD (Black on Red)
 * Grove - 16x2 LCD (Black on Yellow)
 * Grove - LCD RGB Backlight (set color of background)
 */

#include <Wire.h>
#include "rgb_lcd.h"

rgb_lcd lcd;

const int colorR = 255;
const int colorG = 0;
const int colorB = 0;

void setup() 
{
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  // Set up background
  lcd.setRGB(colorR, colorG, colorB);
  // Print a message to the LCD.
  lcd.print("Grove - 16x2 LCD");
  lcd.blinkLED();
  delay(1000);
}

void loop() 
{
  // set the cursor to column 0, line 1
  lcd.setCursor(0, 1);
  String str = "Running "; 
  str += (millis()/1000);
  str += " s";
  lcd.print(str);
  delay(250);
}
