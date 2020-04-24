/*
 * File: Uno_A4988.ino
 * 
 * Testing a stepper motor with a A4988 driver board or equivalent 
 * w/ CNC Shield V3 on Arduino Uno 
 * 
 * Claus Kühnel 2020-03-24 info@ckuehnel.ch
 * 
 */
const int enablePin = 8; // enable stepper

#define XMOTOR // uncomment if YMOTOR is placed

#ifdef XMOTOR
const int directionPin = 5; // direction x-axis
const int stepPin = 2; // step x-axis
#else
const int directionPin = 6; // direction y-axis
const int stepPin = 3; // step y-axis
#endif

const int numberOfSteps = 1600; // 200 steps @ 8 microsteps
const int microsbetweenSteps = 1875; // microseconds 

void setup() 
{
  Serial.begin(9600);
  delay(1000); // wait for Serial Monitor
  Serial.println("Starting A4988 Test");
  Serial.print("Step Pin = "); Serial.println(stepPin);
  Serial.print(" Dir Pin = "); Serial.println(directionPin);
    
  pinMode(directionPin, OUTPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(enablePin, OUTPUT);
  digitalWrite(enablePin, LOW); // enable stepper
  digitalWrite(LED_BUILTIN, LOW);
  
  Serial.println("Forward...");
  digitalWrite(directionPin, HIGH);
  for(int n = 0; n < numberOfSteps; n++) 
  {
    digitalWrite(stepPin, HIGH);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(microsbetweenSteps);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  
  Serial.println("Backward...");  
  digitalWrite(directionPin, LOW);
  for(int n = 0; n < numberOfSteps; n++) 
  {
    digitalWrite(stepPin, HIGH);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(microsbetweenSteps);
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN));
  }
  Serial.println("Stop.");
  digitalWrite(enablePin, HIGH); // disable stepper
}

void loop() {}
