/*
 * File: Uno_Stepper.ino
 */

#include <AFMotor.h>

// Number of steps per output rotation
// Change this as per your motor's specification
const int stepsPerRevolution = 200; // NEMA 17 200 steps

// connect motor to port #2 (M3 and M4)
AF_Stepper motor(stepsPerRevolution, 2);

void setup() 
{
  Serial.begin(9600);
  Serial.println("Stepper test...");

  motor.setSpeed(20);  // 10 rpm   
}

void loop() 
{
  Serial.println("Single coil steps");
  motor.step(200, FORWARD, SINGLE); 
  motor.step(200, BACKWARD, SINGLE); 

  Serial.println("Double coil steps");
  motor.step(200, FORWARD, DOUBLE); 
  motor.step(200, BACKWARD, DOUBLE);

  Serial.println("Interleave coil steps");
  motor.step(400, FORWARD, INTERLEAVE); 
  motor.step(400, BACKWARD, INTERLEAVE); 

  Serial.println("Micrsostep steps");
  motor.step(200, FORWARD, MICROSTEP); 
  motor.step(200, BACKWARD, MICROSTEP); 
}
