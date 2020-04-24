#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
  #include <avr/power.h>
#endif

#define PIN       6   // Pin driving DIN of first NeoPixel
#define NUMPIXELS 8   // Number of NeoPixels in strip or ring

#define DELAYVAL 100  // Time (in milliseconds) to pause between pixels
#define BRIGHTNESS 50 // Brightness of NeoPixel

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.

void setup() 
{
  strip.begin();
  strip.setBrightness(BRIGHTNESS);
  strip.show(); // Initialize all strip to 'off'
}

void loop() 
{
  strip.clear();   // Set all pixel colors to 'off'
  strip.show();    // Send the updated pixel colors to the hardware.
  delay(DELAYVAL); // Pause before next pass through loop

  for(int i=0; i<NUMPIXELS; i++) 
  { 
    strip.setPixelColor(i, strip.Color(40, 0, 0)); //red
    strip.show();    // Send the updated pixel colors to the hardware.
    delay(DELAYVAL); // Pause before next pass through loop
  }
}
