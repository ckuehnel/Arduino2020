/* 
 *  File: Uno_KY40_Test2.ino
 *  
 *  based on Encoder Library - Basic Example
 *  http://www.pjrc.com/teensy/td_libs_Encoder.html
 *
 * Midifications: Claus Kühnel 2020-02-17 info@ckuehnel.ch
 */
#include <Encoder.h>

// Change these two numbers to the pins connected to your encoder.
//   Best Performance: both pins have interrupt capability
//   Good Performance: only the first pin has interrupt capability
//   Low Performance:  neither pin has interrupt capability
//   avoid using pins with LEDs attached
Encoder encoder(2, 3);
const int SW = 4;    // Rotary Encoder Switch
boolean pressed = false;

int buttonState;              // the current reading from the input pin
int lastButtonState = HIGH;   // the previous reading from the input pin

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50;    // the debounce time; increase if the output flickers

void setup() 
{
  Serial.begin(115200);
  delay(2000); // wait for Serial Monitor
  Serial.println("Basic Encoder Test:");
  
  // Set the Switch pin to use Arduino PULLUP resistors
  pinMode(SW, INPUT_PULLUP);
}

long oldPosition  = -999;

void loop() 
{
  // Check if Rotary Encoder switch was pressed
  int reading = digitalRead(SW);
  if (reading != lastButtonState) lastDebounceTime = millis();
  if ((millis() - lastDebounceTime) > debounceDelay) 
  {
    if (reading != buttonState) buttonState = reading;
  }
  lastButtonState = reading;  

  if (!buttonState && !pressed) 
  {
    pressed = true;
    Serial.println("Encoder switch pressed.");
  }
  if (buttonState) pressed = false;
  
  long newPosition = encoder.read();
  if (newPosition != oldPosition) 
  {
    oldPosition = newPosition;
    Serial.println(newPosition);
  }
}
