/*
 * File: Uno_L293.ino
 * 
 * Bidirectional Motor Control with L293 H-Bridge using
 * L293 library created by Giuseppe Masino, 25 may 2016
 * 
 * Based on MotorControl_Demo.ino
 * Adaptions by Claus Kühnel 2020-03-18 info@ckuehnel.ch
 */
#include <L293.h>

const int pin12EN = 11;	// must be a PWM pin for speed control
const int pin1A = 4;	  // pins for controlling motor state
const int pin2A = 5;	

L293 motor(pin12EN, pin1A, pin2A); // create motor object

void setup() {}

void loop()
{
	motor.forward(255);	// set the direction and maximun speed
	delay(1000);				// wait for 1 second 
  motor.stop();        // stop the motor
  delay(1000);        // wait for 1 second
	motor.back(180);		// set a new direction and a lower speed
	delay(1000);				// wait for 1 second 
	motor.stop();				// stop the motor
	delay(1000);				// wait for 1 second
}
