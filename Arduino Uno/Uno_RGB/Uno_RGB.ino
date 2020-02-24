/*
   Uno_RGB.ino
   
   Controlling RGB LED w/ common cathode & single LEDs
   Created 1019-09-15 Claus Kühnel info@ckuehnel.ch 
*/

enum{Red = 9, Green, Blue,SingleRed, SingleBlue};

void setup()
{
  pinMode(Red, OUTPUT); digitalWrite(Red,LOW);
  pinMode(Green, OUTPUT); digitalWrite(Green, LOW);
  pinMode(Blue, OUTPUT); digitalWrite(Blue, LOW);
  pinMode(SingleRed, OUTPUT); digitalWrite(Green, LOW);
  pinMode(SingleBlue, OUTPUT); digitalWrite(Blue, LOW);
}

void loop()
{
  blinkLED(Red);
  blinkLED(Green);
  blinkLED(Blue);
  blinkLED(SingleRed);
  blinkLED(SingleBlue);
  fadeLED(Red);
  fadeLED(Green);
  fadeLED(Blue);
  fadeLED(SingleRed);
  fadeLED(SingleBlue);
}
