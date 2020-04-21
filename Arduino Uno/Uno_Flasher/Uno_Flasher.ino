/*
 * File: Uno_Flasher.ino
 * 
 * Flashing LEDs as class example
 * Source: https://learn.adafruit.com/multi-tasking-the-arduino-part-1/a-classy-solution
 * Adaption to Easy Module Shield v1 Claus Kuehnel 2020-01-23 info@ckuehnel.ch
 */

class Flasher
{
	// Class Member Variables
	// These are initialized at startup
	int ledPin;      // the number of the LED pin
	long OnTime;     // milliseconds of on-time
	long OffTime;    // milliseconds of off-time

	// These maintain the current state
	int ledState;             		  // ledState used to set the LED
	unsigned long previousMillis;  	// will store last time LED was updated

  // Constructor - creates a Flasher 
  // and initializes the member variables and state
  public:
  Flasher(int pin, long on, long off)
  {
	  ledPin = pin;
	  pinMode(ledPin, OUTPUT);     
	  
	  OnTime = on;
	  OffTime = off;
	
	  ledState = LOW; 
	  previousMillis = 0;
  }

  void Update()
  {
    // check to see if it's time to change the state of the LED
    unsigned long currentMillis = millis();
     
    if((ledState == HIGH) && (currentMillis - previousMillis >= OnTime))
    {
    	ledState = LOW;  // Turn it off
      previousMillis = currentMillis;  // Remember the time
      digitalWrite(ledPin, ledState);  // Update the actual LED
    }
    else if ((ledState == LOW) && (currentMillis - previousMillis >= OffTime))
    {
      ledState = HIGH;  // turn it on
      previousMillis = currentMillis;   // Remember the time
      digitalWrite(ledPin, ledState);	  // Update the actual LED
    }
  }
};

Flasher led1(11, 10, 400);
Flasher led2(10, 35, 350);
Flasher led3( 9, 20, 500);
Flasher led4(13, 20, 150);

void setup() {}

void loop()
{
	led1.Update();
	led2.Update();
  led3.Update();
  led4.Update();
}
