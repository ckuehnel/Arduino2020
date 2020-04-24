//////////////////////////////////////////////////////////////////
//
// (c) 2016, Prof. Dr. Michael Stal
// Purpose: Conversion of wavelengths (in nanometers) to
// RGB-Value (each color ranges from 0 .. 255 intensity)
// This way it is possible to draw a color spectrum
// such as the sequence of colors found in rainbows
//
// Source of Pascal- Solution:
// http://www.efg2.com/Lab/ScienceAndEngineering/Spectra.htm
// Original solution is written in FORTRAN
// Wavelength = 400 nm --------- Magenta
// Wavelength = 450 nm --------- Blue
// Wavelength = 500 nm --------- Green
// Wavelength = 550 nm --------- Light Green
// Wavelength = 600 nm --------- Orange
// Wavelength = 650 nm --------- Light  Red
// Wavelength = 700 nm --------- Medium Red
// Wavelength = 750 nm --------- Darker Red
//
// Dedicated to the Public Domain
// Have fun!
//
//////////////////////////////////////////////////////////////////

#include <math.h> // math required

//////////////////////////////////////////////////////////////////
 // waveLengthInInterval
 // Purpose: checks containment for intervals of type [left, right[
 //////////////////////////////////////////////////////////////////
#define wavelengthInInterval(VAL, LEFT, RIGHT)  ((LEFT) <= (VAL)) && ((VAL) < (RIGHT))

//////////////////////////////////////////////////////////////////
// calculate RGB - values from wavelengths in nanometers
//
// Result:    contains blue, green, and red values 
//            with red = byte 2, green = byte 1,  blue = byte 0
// Parameter: wavelength in nanometers
//            wavelength should be within interval [380, 780]
//////////////////////////////////////////////////////////////////

uint32_t wavelengthToRGB(uint32_t wavelength) { 
    double blue;  // blue  value
    double green; // green value
    double red;   // red  value
    double factor;// factor to adjust for boundaries
    const double gamma = 0.45;    // according to Poynton
    // const double gamma = 0.80; // according to Bruton
    const double maxIntensity = 255.0;
    // calculations depend on wavelength
    if (wavelengthInInterval(wavelength, 380, 440)) {
       red   =  -1.0 * (wavelength - 440.0) / (440.0 - 380.0);
       green = 0.0;
       blue  = 1.0;
    } else 
    if (wavelengthInInterval(wavelength, 440, 490)) {
       red   = 0.0;
       green = (wavelength - 440.0) / (490.0 - 440.0);
       blue  = 1.0;
    } else
    if (wavelengthInInterval(wavelength, 490, 510)) {
       red   = 0.0;
       green = 1.0;
       blue  = -1.0 * (wavelength - 510.0) / (510.0 - 490.0);
    } else    
    if (wavelengthInInterval(wavelength, 510, 580)) {
       red   = (wavelength - 510.0) / (580.0 - 510.0);
       green = 1.0;
       blue  = 0.0;
    } else
    if (wavelengthInInterval(wavelength, 580, 645)) {
       red   = 1.0;
       green =  -1.0 * (wavelength - 645.0) / (645.0 - 580.0);
       blue  = 0.0;
    } else {
       red   = 1.0;
       green = 0.0;
       blue  = 0.0;
    }

    // Adjust for the boundaries of the visible spectrum
    if (wavelengthInInterval(wavelength, 380, 420)) { // lower spectrum
       factor = 0.3 + 0.7 * (wavelength - 380.0) / (420.0 - 380.0);
    } else
    if (wavelengthInInterval(wavelength, 700, 780)) { // higher spectrum
       factor = 0.3 + 0.7 * (780.0 - wavelength) / (780.0 - 700.0);
    } else { // main spectrum
       factor = 1.0;
    }

    // re-calculate values of red, green, blue accordingly
    red   = pow(factor *   red, gamma);
    green = pow(factor * green, gamma);
    blue  = pow(factor *  blue, gamma);

    // we place Red/Green/Blue - values 
    // into different bytes of the 32 Bit
    // RGB result.
    // byte 0 => Blue
    // byte 1 => Green
    // byte 2 => Red
    
    uint32_t word32 = 0;
    uint8_t *bytes = (uint8_t *) &word32;
    bytes[0] = (uint8_t)(blue  * maxIntensity);
    bytes[1] = (uint8_t)(green * maxIntensity);
    bytes[2] = (uint8_t)(red   * maxIntensity);
    bytes[3] = 0; 
    return word32;
}


//////////////////////////////////////////////////////////////////
 // printRGBToSerial
 // Purpose: diagnostic output of RGB values on serial line
 //////////////////////////////////////////////////////////////////
void printRGBToSerial(uint32_t color) 
{
  uint8_t *bytes = (uint8_t *)& color;
  Serial.print("(");
  Serial.print(bytes[2], DEC); // Red
  Serial.print(", ");
  Serial.print(bytes[1], DEC); // Green
  Serial.print(", ");
  Serial.print(bytes[0], DEC); // Blue
  Serial.print(")");
}


//////////////////////////////////////////////////////////////////
 // printRGBToNeoPixel
 // Purpose: visualize of RGB values on NeoPixel
 //////////////////////////////////////////////////////////////////
void printRGBToNeoPixel(uint16_t wavelength) 
{
  // Alle vorhandenen Pixel durchlaufen:
  for(uint16_t pixno=0; pixno<strip.numPixels(); pixno++) 
  {
    // RGB-Pixelfarbe aus Wellenlänge berechnen:
    strip.setPixelColor(pixno, wavelengthToRGB(wavelength));
    strip.show();
    delay(10); // Gegebene Zeit warten
  }
}

//////////////////////////////////////////////////////////////////
// rainbowWipe(wait)
//
// Zweck:     Pixel für Pixel beginnend mit Violett und mit 
//            Rot endend in Regenbogenfarben erleuchten.
//      
//
// Parameter: wait = Wartezeit zwischen Pixeln
//
// Hinweis:   Je mehr Pixel, desto feinere Übergänge möglich!
//           
//////////////////////////////////////////////////////////////////

void rainbowWipe(uint8_t wait) {
  // Wir wollen in delta-Schritten den Bereich sichtbaren
  // Lichts in den Pixeln abbilden.
  uint32_t delta = (780.0-380.0) / (strip.numPixels());
  uint32_t wavelength;

  // Alle vorhandenen Pixel durchlaufen:
  for(uint16_t pixno=0; pixno<strip.numPixels(); pixno++) {
    // Wellenlänge in etwa zwischen 380 nm und 780 nm:
    wavelength = 380 + pixno * delta;

    // RGB-Pixelfarbe aus Wellenlänge berechnen:
    strip.setPixelColor(pixno, wavelengthToRGB(wavelength));
    // Licht an:
    strip.show();
    delay(wait); // Gegebene Zeit warten
  }
}
