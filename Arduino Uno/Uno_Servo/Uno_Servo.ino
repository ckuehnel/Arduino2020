/*
 * File: Uno_Servo.ino
 * 
 * Direct control of a mini servo 
 *  
 * Claus Kühnel 2020-03-14 info@ckuehnel.ch
 */
#include <Servo.h>

Servo myServo;  // create servo object to control a servo

const int pServo = 9;
int pos = 0;    // variable to store the servo position

void setup() 
{
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  Serial.println("\nTest Servo...");
  
  myServo.attach(pServo, 540, 2400);  // attaches the servo to an I/O pin

  myServo.write(0); 
  delay(2000);  // to reach the position
  int pulse = myServo.readMicroseconds();
  Serial.print("Initial position (  0 degrees) - Pulse width = ");
  Serial.print(pulse); Serial.println(" us");

  myServo.write(180); 
  delay(2000);  // to reach the position
  pulse = myServo.readMicroseconds();
  Serial.print("Initial position (180 degrees) - Pulse width = ");
  Serial.print(pulse); Serial.println(" us");
}

void loop() 
{
  for (pos = 180; pos >= 0; pos -= 1) // goes from 180 degrees to 0 degrees
  {
    myServo.write(pos);               // tell servo to go to position in variable 'pos'
    delay(6);                         // waits 6ms for the servo to reach the position
  }
  delay(1000);
  for (pos = 0; pos <= 180; pos += 1) // goes from 0 degrees to 180 degrees
  {
    myServo.write(pos);               // tell servo to go to position in variable 'pos'
    delay(6);                         // waits 6ms for the servo to reach the position
  }
  delay(1000);
}
