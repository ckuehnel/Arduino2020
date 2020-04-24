#include <LEDMatrixDriver.hpp>
#include "Font.h"

// This sketch draws numbers on your LED matrix using the hardware SPI driver Library by Bartosz Bielawski.

// Define the ChipSelect pin for the led matrix (Dont use the SS or MISO pin of your Arduino!)
// Other pins are Arduino specific SPI pins (MOSI=DIN, SCK=CLK of the LEDMatrix) see https://www.arduino.cc/en/Reference/SPI
const uint8_t LEDMATRIX_CS_PIN = 9;

// Number of 8x8 segments you are connecting
const int LEDMATRIX_SEGMENTS = 4;
const int LEDMATRIX_WIDTH    = LEDMATRIX_SEGMENTS * 8;

// The LEDMatrixDriver class instance
LEDMatrixDriver lmd(LEDMATRIX_SEGMENTS, LEDMATRIX_CS_PIN);

char text[5], txt[5];

int x=0, y=0;   // start top left

void setup() 
{
  // init the display
  lmd.setEnabled(true);
  lmd.setIntensity(2);   // 0 = low, 10 = high
}

void loop()
{
  // display i on LED Matrix display
  for (int i=0; i < 10000; i++)
  {
    itoa(i, text, 10);
    int len = strlen(text);
    
    // fill leading spaces
    if (len == 1) strcpy(txt, "   ");
    if (len == 2) strcpy(txt, "  ");
    if (len == 3) strcpy(txt, " ");
    if (len == 4) strcpy(txt, "");
    strcat(txt, text);
    
    len = strlen(txt);
    drawString(txt, len, x, 0);

    // Toggle display of the new framebuffer
    lmd.display();
    delay(200);
  }
}


/*
 * This function draws a string of the given length to the given position.
 */
void drawString(char* text, int len, int x, int y )
{
  for( int idx = 0; idx < len; idx ++ )
  {
    int c = text[idx] - 32;

    // stop if char is outside visible area
    if( x + idx * 8  > LEDMATRIX_WIDTH )
      return;

    // only draw if char is visible
    if( 8 + x + idx * 8 > 0 )
      drawSprite( font[c], x + idx * 8, y, 8, 8 );
  }
}

/*
 * This draws a sprite to the given position using the width and height supplied (usually 8x8)
 */
void drawSprite( byte* sprite, int x, int y, int width, int height )
{
  // The mask is used to get the column bit from the sprite row
  byte mask = B10000000;

  for( int iy = 0; iy < height; iy++ )
  {
    for( int ix = 0; ix < width; ix++ )
    {
      lmd.setPixel(x + ix, y + iy, (bool)(sprite[iy] & mask ));

      // shift the mask by one pixel to the right
      mask = mask >> 1;
    }

    // reset column mask
    mask = B10000000;
  }
}
