/*
 * File: Uno_L293_MotorShield.ino
  * 
 * Bidirectional Motor Control with L293 H-Bridge using
 * Adafruit Motor Shield Library. ased on MotorTest.ino
 * Adaptions by Claus Kühnel 2020-03-18 info@ckuehnel.ch
 */
#include <AFMotor.h>

AF_DCMotor motor(1);  // DC motor on M1

void setup() {}

void loop() 
{
  motor.run(FORWARD);   // forward
  motor.setSpeed(255);  // maximum speed
  delay(1000);
 
  motor.run(RELEASE);   // Stop
  delay(1000);

  motor.run(BACKWARD);  // backward
  motor.setSpeed(180);  // reduced speed
  delay(1000);

  motor.run(RELEASE);   // stop
  delay(1000);
}
