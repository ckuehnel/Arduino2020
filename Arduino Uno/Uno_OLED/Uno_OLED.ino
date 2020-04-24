/*
 * File Uno_OLED.ino
 * This file implements a temperature display.
 * Temperatur is simulated here.
 * Claus Kühnel 2020-03-06 info@ckuehnel.ch
 */
#include<Arduino.h>
#include<U8g2lib.h>
 
#ifdef U8X8_HAVE_HW_SPI
#include<SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include<Wire.h>
#endif
 
U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// Copy the contents of your .xbm file below
#define Thermometer_small_width 17
#define Thermometer_small_height 32
#define Thermometer_small_x_hot 0
#define Thermometer_small_y_hot 15
static const unsigned char Thermometer_small_bits[] PROGMEM = {
   0x00, 0x00, 0x00, 0xc0, 0x07, 0x00, 0xf0, 0x0f, 0x00, 0xf0, 0x1f, 0x00,
   0x78, 0x1c, 0x00, 0x38, 0x3c, 0x00, 0x38, 0x38, 0x00, 0x38, 0x38, 0x00,
   0x38, 0x38, 0x00, 0x38, 0x38, 0x00, 0x38, 0x38, 0x00, 0x38, 0x3b, 0x00,
   0x38, 0x3b, 0x00, 0x38, 0x3b, 0x00, 0x38, 0x3b, 0x00, 0x38, 0x3b, 0x00,
   0x38, 0x3b, 0x00, 0x38, 0x3b, 0x00, 0x38, 0x3b, 0x00, 0x3c, 0x7b, 0x00,
   0x9e, 0x73, 0x00, 0xce, 0xf7, 0x00, 0xee, 0xe7, 0x00, 0xee, 0xe7, 0x00,
   0xee, 0xe7, 0x00, 0xce, 0xf3, 0x00, 0x1e, 0x70, 0x00, 0x3c, 0x7c, 0x00,
   0xf8, 0x3f, 0x00, 0xf0, 0x1f, 0x00, 0xc0, 0x07, 0x00, 0x00, 0x00, 0x00 };

   
const char DEGREE_SYMBOL[] = { 0xB0, '\0' };

float getValue()
{
  // Simulate room temperature (in celsius)...
  float temp = 25 + random(-5, 5)/10.0;
  return temp;
}

void drawLogo(void) 
{
 // graphic commands to redraw the complete screen should be placed here  
 u8g2.drawXBMP(Thermometer_small_x_hot, Thermometer_small_y_hot, Thermometer_small_width, Thermometer_small_height, Thermometer_small_bits);
}
 
void setup(void) 
{
 u8g2.begin();
 u8g2.enableUTF8Print();
}
 
void loop(void) 
{
 // picture loop
 u8g2.firstPage();
 do 
 {
   float temp = getValue();
   u8g2.setFont(u8g2_font_10x20_tf);
   u8g2.setCursor(40, 40);
   u8g2.print(temp,1);
   u8g2.drawUTF8(90, 40, DEGREE_SYMBOL);
   u8g2.drawStr(100, 40, "C");
   drawLogo();
 } while( u8g2.nextPage() );
 
 // rebuild the picture after some delay
 delay(1000);
}
