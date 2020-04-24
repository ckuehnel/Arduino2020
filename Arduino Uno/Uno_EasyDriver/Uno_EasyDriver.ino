/*
 * File: Uno_EasyDriver.ino
 * 
 * Testing a stepper motor with an Easy Driver board on Arduino Uno 
 * 
 * Claus Kühnel 2020-03-24 info@ckuehnel.ch
 * 
 */
#include <AccelStepper.h>

// Define a stepper and the pins it will use
AccelStepper stepper(AccelStepper::DRIVER, 2, 5); // stepPin, directionPin

int pos = 1600; // 200 steps @ 8 microsteps

void setup()
{ 
  stepper.setEnablePin(8);
  stepper.setMaxSpeed(625);  // in steps/s (here 1600 steps/3 s)
  stepper.setAcceleration(1000); // in steps/s^2

}

void loop()
{
  if (stepper.distanceToGo() == 0) // true if target position is achieved
  {
    pos = -pos;
    stepper.move(pos); // set the target position relative to the current position
  }
  stepper.run(); // runs the motor
}
