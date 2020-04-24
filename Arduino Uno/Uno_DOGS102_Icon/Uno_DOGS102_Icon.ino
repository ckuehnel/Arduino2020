/*
    Uno_DOGS102_Icon.ino

    Test of display shield w/ EA DOGS102-6 display on EA PCBARDDOG1701 board
    Based on dogs102_GR_backlight.ino
    https://www.lcd-module.de/fileadmin/downloads/development%20service/Arduino/Arduino%20meets%20EA%20DOGS102.zip
    Install dog_1701 library first.
    
    Modifications and logo 2019-09.24 Claus Kühnel info@ckuehnel.ch
 */
#include <SPI.h>
#include <dog_1701.h>
#include <font_16x32nums.h>
#include <font_6x8.h>
#include <font_8x16.h>
#include <font_8x8.h>
#include "logo_BMP.h"

/*Available functions in dog_1701 Libraray:
  void initialize  (byte p_cs, byte p_si, byte p_clk, byte p_a0, byte p_res, byte type);
  void clear       (void);
  void contrast    (byte contr);
  void view        (byte direction);
  void string      (byte column, byte page, const byte *font_adress, const char *str);
  void rectangle   (byte start_column, byte start_page, byte end_column, byte end_page, byte pattern);
  void picture     (byte column, byte page, const byte *pic_adress);
*/

dog_1701 DOG;

//the following port definitions are used by the demo board "EA PCBARDDOG1701"
const int led_green = 5;
const int led_red   = 3;
int led = led_red;
void init_backlight(boolean mono);
void mono_backlight(byte brightness);
void gr_backlight(byte green, byte red);

void sample_screen(void);

//initialize the backlight and the display
void setup()
{
  init_backlight(false); //use RGB backlight in this sample code. Please change it to your configuration
  DOG.initialize(10,0,0,9,4,DOGS102);   //SS = 10, 0,0= use Hardware SPI, 9 = A0, 4 = RESET, EA DOGS102-6 (=102x64 dots)
}

//create a sample sceen content
void sample_screen(void)
{
  DOG.clear();  //clear whole display
  DOG.picture(0,0,Image_logo_BMP);
  DOG.string(30,0,font_8x16,"DOGS102-6");      //font size 8x16 first page         
  DOG.rectangle(30,2,102,2,0x03);              //show small line (filled pattern = 0x03), to the left and right        
  DOG.string(0,4,font_6x8,"Further info:");
  DOG.string(0,5,font_6x8,"Arduino meets ");
  DOG.string(0,6,font_6x8,"EA DOGS102");
  DOG.string(0,7,font_6x8,"@ lcd-module.com");
}

void loop()
{
  rgb_backlight(random(256), random(256));    //BL random color (green, red);
  DOG.view(VIEW_BOTTOM);  //default viewing direction
  sample_screen();        //show content
  delay(2000);
  /*rgb_backlight(random(255), random(255));    //BL random color (green, red)
  DOG.view(VIEW_TOP);    //alternate viewing direction
  sample_screen();
  delay(2000);*/
}

//The following functions controll the backlight with a PWM. Not needed for the display content
void init_backlight(boolean mono)
{
  if(mono) //EA LED39X41-W, EA LED39X41-A
  {
    pinMode(led,  OUTPUT);
    mono_backlight(255);
  }
  else  //EA LED39X41-GR
  {
    pinMode(led_green, OUTPUT);
    pinMode(led_red,   OUTPUT);
    rgb_backlight(255,0);
  }
}
//Use this funtion for monochrome backlight
void mono_backlight(byte brightness)
{
  analogWrite(led, brightness);  
}
//Use this function for RGB backlight
void rgb_backlight(byte green, byte red)
{
  analogWrite(led_red,  red);  
  analogWrite(led_green, green);  
}
