//
// Sample code to control the single NeoPixel on the ESP32-S2 Saola
// https://www.instructables.com/ESP32-S2-Saola-Making-the-RGB-Work/
//

#include <Adafruit_NeoPixel.h>

// On the ESP32S2 SAOLA GPIO is the NeoPixel.
#define PIN        18 

//Single NeoPixel
Adafruit_NeoPixel pixels(1, PIN, NEO_GRB + NEO_KHZ800);

#define DELAYVAL 25 // Time (in milliseconds) to pause between color change

void setup() 
{
  Serial.begin(115200);
  delay(2000);  // wait for serial monitor
  
  //This pixel is just way to bright, lower it to 10 so it does not hurt to look at.
  pixels.setBrightness(10);
  pixels.begin(); // INITIALIZE NeoPixel (REQUIRED)
}

// Simple function to return a color in the rainbow
// Input a value 0 to 255 to get a color value.
uint32_t Wheel(byte WheelPos)
{
    //Assume the wheel value is less than 85, if so Green value is 0
    uint32_t returnColor = Adafruit_NeoPixel::Color((byte)(255 - (WheelPos * 3)), 0, (byte)(WheelPos * 3));

    //If we are greater than 170 Red value is 0
    if (WheelPos > 84 && WheelPos < 170)
    {
        WheelPos -= 85;
        returnColor = Adafruit_NeoPixel::Color(0, (byte)(WheelPos * 3), (byte)(255 - WheelPos * 3));
    }
    //Finally above 170 and Blue value is 0
    else if (WheelPos >= 170)
    {
        WheelPos -= 170;
        returnColor = Adafruit_NeoPixel::Color((byte)(WheelPos * 3), (byte)(255 - WheelPos * 3), 0);
    }

    return returnColor;
}

//Counter to run from 0-255 to cycle the colors of the rainbow.
int colorCount = 0;
void loop() 
{
  Serial.println(colorCount);

    //Set the new color on the pixel.
    pixels.setPixelColor(0, Wheel(colorCount++));

    // Send the updated pixel colors to the hardware.
    pixels.show();   

    //Cycle the colors at the end.
    if (colorCount > 255)
        colorCount = 0;

    // Pause before next pass through loop
    delay(DELAYVAL); 
}
