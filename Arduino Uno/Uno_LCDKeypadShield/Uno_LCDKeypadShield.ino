/** this is a slightly changed and configured example from 
 * https://wiki.dfrobot.com/Arduino_LCD_KeyPad_Shield__SKU__DFR0009_
 *    
 *  For german explanation visit 
 *  http://www.fambach.net
 */

//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

#define DEBUG 1

/*******************************************************

This program will test the LCD panel and the buttons
Mark Bramwell, July 2010

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
const int backlight = 10; // Backlight connected to D10
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor 
 if (DEBUG)
 {
   Serial.print("Button value = ");
   Serial.println(adc_key_in);
 }
 // set DEBUG to 1 to read the adc_key_in values via serial monitor
 // my buttons when read are centered at these valies: 0, 99, 255, 407, 639
 // I add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;  
 if (adc_key_in < 150)  return btnUP; 
 if (adc_key_in < 300)  return btnDOWN; 
 if (adc_key_in < 450)  return btnLEFT; 
 if (adc_key_in < 700)  return btnSELECT;  

 return btnNONE;  // when all others fail, return this...
}

void setup()
{
 Serial.begin(115200);
 if (DEBUG) Serial.println("\nTest DFRobot LCD Keypad Shield...");
 pinMode(backlight, OUTPUT);
 digitalWrite(backlight, HIGH);
 lcd.begin(16, 2);              // start the library
 lcd.setCursor(0,0);
 lcd.print("Push the buttons"); // print a simple message
}
 
void loop()
{
 lcd.setCursor(9,1);            // move cursor to second line "1" and 9 spaces over
 lcd.print(millis()/1000);      // display seconds elapsed since power-up


 lcd.setCursor(0,1);            // move to the begining of the second line
 lcd_key = read_LCD_buttons();  // read the buttons

 switch (lcd_key)               // depending on which button was pushed, we perform an action
 {
   case btnRIGHT:
     {
     lcd.print("RIGHT ");
     break;
     }
   case btnLEFT:
     {
     lcd.print("LEFT   ");
     break;
     }
   case btnUP:
     {
     lcd.print("UP    ");
     break;
     }
   case btnDOWN:
     {
     lcd.print("DOWN  ");
     break;
     }
   case btnSELECT:
     {
     lcd.print("SELECT");         // pressing SELECT switches backlight off
     digitalWrite(backlight, LOW);
     break;
     }
     case btnNONE:
     {
     lcd.print("NONE  ");
     digitalWrite(backlight, HIGH);
     break;
     }
 }
 delay(250);
}
