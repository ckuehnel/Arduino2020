/*  Here a simple sketch to test the upload of ESP32 S2 Saola 1MI.
 *  The addressable RGB LED (WS2812), driven by GPIO18 need
 *  Adafruit NeoPixel library
 *  by Mischianti Renzo <https://www.mischianti.org>
 *
 *  https://www.mischianti.org/
 *
 */
 
#include <Adafruit_NeoPixel.h>
#define PIN 18
#define NUMPIXELS 1
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
 
enum {NONE, RED, GREEN, BLUE};
int ledColor = NONE;
 
void setup()
{
  pixels.begin();
}
 
void loop()
{
  switch (ledColor) {
    case NONE:
      pixels.setPixelColor(0, pixels.Color(0, 0, 0));
      pixels.show();
      break;
    case RED:
      pixels.setPixelColor(0, pixels.Color(20, 0, 0));
      pixels.show();
      break;
    case GREEN:
      pixels.setPixelColor(0, pixels.Color(0, 20, 0));
      pixels.show();
      break;
    case BLUE:
      pixels.setPixelColor(0, pixels.Color(0, 0, 20));
      pixels.show();
      break;
    default:
      break;
  }
 
  ledColor++;
  if (ledColor == 4) {
    ledColor = NONE;
  }
 
  delay(1000);
}
