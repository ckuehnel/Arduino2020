#include <Adafruit_DotStar.h>
#include <SPI.h>         

#define NUMPIXELS 3   // Number of LEDs in strip
#define DELAYVAL 100  // Time (in milliseconds) to pause between pixels

//Adafruit_DotStar strip(NUMPIXELS, DATAPIN, CLOCKPIN, DOTSTAR_BRG);
// The last parameter is optional -- this is the color data order of the
// DotStar strip, which has changed over time in different production runs.
// Your code just uses R,G,B colors, the library then reassigns as needed.
// Default is DOTSTAR_BRG, so change this if you have an earlier strip.

// Hardware SPI is a little faster, but must be wired to specific pins
// (Arduino Uno = pin 11 for data, 13 for clock, other boards are different).
Adafruit_DotStar strip(NUMPIXELS, DOTSTAR_BGR);

void setup() 
{
  strip.begin(); // Initialize pins for output
  strip.show();  // Turn all LEDs off ASAP
}

void loop()
{
  strip.clear();   // Set all pixel colors to 'off'
  strip.show();    // Send the updated pixel colors to the hardware.
  delay(DELAYVAL); // Pause before next pass through loop
  
  for(int i=0; i<NUMPIXELS; i++) 
  { 
    strip.setPixelColor(i, 40, 0, 0); //red
    strip.show();    // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
  }
}
