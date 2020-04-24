#include <FastLED.h>

#define NUM_LEDS 3

#define DATA_PIN 11
#define CLOCK_PIN 13

#define DELAYVAL 250  // Time (in milliseconds) to pause between pixels
#define BRIGHTNESS 40 // Brightness of NeoPixel

// Define the array of leds
CRGB leds[NUM_LEDS];

void setup() 
{ 
  // Uncomment/edit one of the following lines for your leds arrangement.
  // ## Clockless types ##
//  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);  // GRB ordering is assumed
  // ## Clocked (SPI) types ##
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN, BGR>(leds, NUM_LEDS);  // BGR ordering is typical
    
  FastLED.setBrightness(BRIGHTNESS);
  FastLED.show(); // Initialize all strip to 'off'
}

void loop() 
{
  FastLED.clear();   // Set all pixel colors to 'off'
  FastLED.show();    // Send the updated pixel colors to the hardware.
  FastLED.delay(DELAYVAL); // Pause before next pass through loop

  for(int i=0; i<NUM_LEDS; i++) 
  { 
    switch (i)
    {
      case 0: leds[i] = CRGB::Red;   break;
      case 1: leds[i] = CRGB::Green; break;
      case 2: leds[i] = CRGB::Blue;  break;
    }
    FastLED.show();
    FastLED.delay(DELAYVAL); // Pause before next pass through loop
  }
}
