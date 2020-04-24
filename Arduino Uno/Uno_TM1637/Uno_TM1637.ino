/*
 * File: Uno_TM1637.ino
 * 
 * Display numeric value on 7-Segment-Display controlled by TM1637
 * using library on https://github.com/avishorp/TM1637
 * 2020-02-28 Claus Kühnel info@ ckuehnel.ch 
 */
#include <TM1637Display.h>

// Module connection pins (Digital Pins)
#define CLK 2
#define DIO 3

const uint8_t SEG_DEC[] = 
{
	SEG_B | SEG_C | SEG_D | SEG_E | SEG_G,    // d
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,    // E
	SEG_A | SEG_D | SEG_E | SEG_F,            // C
  SEG_D                                     // _  
};

const uint8_t SEG_HEX[] = 
{
  SEG_C | SEG_E | SEG_F | SEG_G,            // h
  SEG_A | SEG_D | SEG_E | SEG_F | SEG_G,    // E
  SEG_B | SEG_C | SEG_E | SEG_F | SEG_G,    // H
  SEG_D                                     // _  
};

TM1637Display display(CLK, DIO);

void setup() 
{
  display.setBrightness(2);
}

void loop()
{
  // dEC - display decimal numbers
  display.setSegments(SEG_DEC);
  delay(1000);

  // simulates a display from -40.0 to 75.0
  for (int i = -40; i < 80; i +=5) 
  {
    display.showNumberDec(i, false); 
    delay(500);
  }
  // heH - display hex numbers
  display.setSegments(SEG_HEX);
  delay(1000);

  // simulates a display from 0000 to FFFF
  unsigned int i = 0;
  while (i < 0xF000)
  {
    if (i < 0x10) i+= 1;
    else if (i < 0x100) i+= 0x10;
    else if (i < 0x1000) i+= 0x100;
    else i+= 0x1000;
    display.showNumberHexEx(i, 0, true);
    delay(250);
  }
}
