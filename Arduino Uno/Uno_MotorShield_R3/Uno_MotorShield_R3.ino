/*
 * File: Uno_MotorShield_R3.ino
  * 
 * Bidirectional Motor Control with Arduino Motor Shield R3 using
 * ArduinoMotorShieldR3 Library. 
 * Include the library https://github.com/gallingern/arduino-motor-shield-r3
 * Claus Kühnel 2020-03-18 info@ckuehnel.ch
 */
#include "ArduinoMotorShieldR3.h"

ArduinoMotorShieldR3 md;

void setup()
{
  md.init();  // initialize IO
}

void loop()
{
  md.setM1Speed(400);  // M1 forward speed 100%
  delay(1000);

  md.setM1Speed(0);   // stop
  md.setM1Brake();
  delay(1000);

  md.setM1Speed(-400); // M1 backward speed 100%
  delay(1000);
  
  md.setM1Speed(0);   // stop
  md.setM1Brake();
  delay(1000);
}
