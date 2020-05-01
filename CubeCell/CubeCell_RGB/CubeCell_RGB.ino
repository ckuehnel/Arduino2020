#include "Adafruit_NeoPixel.h"

#define PIN     RGB   // Pin driving DIN of first NeoPixel
#define NUMPIXELS 3   // Number of NeoPixels in pixels or ring
#define DELAYVAL 100  // Time (in milliseconds) to pause between pixels
#define BRIGHTNESS 50 // Brightness of NeoPixel

Adafruit_NeoPixel pixels(NUMPIXELS, RGB, NEO_GRB + NEO_KHZ400);

void setup() 
{
  Serial.begin(115200);
  Serial.println("\nHeltec CubeCell test...");

  pinMode(Vext,OUTPUT);
  digitalWrite(Vext,LOW); //SET POWER
  
  pixels.begin();
  //pixels.setBrightness(BRIGHTNESS);
  pixels.show(); // Initialize all pixels to 'off'
}

uint8_t i=0, j=0;

void loop() 
{
  pixels.setPixelColor(j, pixels.Color(i, 0, 0));
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(200); // Pause before next pass through loop

  pixels.setPixelColor(j, pixels.Color(0, i, 0));
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(200); // Pause before next pass through loop

  pixels.setPixelColor(j, pixels.Color(0, 0, i));
  pixels.show();   // Send the updated pixel colors to the hardware.
  delay(200); // Pause before next pass through loop

  pixels.setPixelColor(j, pixels.Color(0, 0, 0));
    
  i+=10; j++; if(j>=NUMPIXELS) j=0;
}
