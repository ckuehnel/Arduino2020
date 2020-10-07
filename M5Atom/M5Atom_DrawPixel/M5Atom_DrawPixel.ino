#include "M5Atom.h"

uint8_t Red = 255, Green = 200, Blue = 0;
uint8_t FSM = 0;

bool pressed = false;

void IRAM_ATTR btn_isr()
{
  pressed = true;
}

void setup() 
{
  M5.begin(true, false, true);
  pinMode(39, INPUT_PULLUP);               // Button is G32 pin
  attachInterrupt(39, btn_isr, FALLING);   // Grove G32 pin
  delay(1000);
  M5.dis.clear();
  Serial.println("\nTest...");
}

void loop() 
{
  if (pressed)
  {

    switch (FSM)
    {
      case 0:
        Red = 255, Green = 200, Blue = 0;
        break;
      case 1:
        Red = 0, Green = 200, Blue = 0;    
        break;
      default:
        break;
     }
     FSM++;
     if (FSM >= 2) FSM = 0;
  }
  //M5.update();
  MidPoint(Red, Green, Blue);
  delay(100);
  //M5.update();
  InnerCircle(Red, Green, Blue);
  delay(100);
  //M5.update();
  OuterCircle(Red, Green, Blue);
  delay(200);
  //M5.update();
  OuterCircle(0,0,0);
  delay(100);
  //M5.update();
  InnerCircle(0,0,0);
  delay(100);
  //M5.update();
  MidPoint(0,0,0);
  delay(500);
  //M5.update();
/*  for (int i=0; i<25; i++)
  {
    //                     G  R  B
    M5.dis.drawpix(i, CRGB(0,100,0));
  }
  delay(1000);
  
  delay(1000);*/
}

void MidPoint(uint8_t R, uint8_t G, uint8_t B)
{
  M5.dis.drawpix(2, 2, CRGB(G, R, B));
}

void InnerCircle(uint8_t R, uint8_t G, uint8_t B)
{
  for (int i = 1; i < 4; i++) M5.dis.drawpix(i,1, CRGB(G, R, B));
  M5.dis.drawpix(1, 2, CRGB(G, R, B));
  M5.dis.drawpix(3, 2, CRGB(G, R, B));
  for (int i = 1; i < 4; i++) M5.dis.drawpix(i,3, CRGB(G, R, B));
}

void OuterCircle(uint8_t R, uint8_t G, uint8_t B)
{
  for (int i = 0; i < 5; i++) M5.dis.drawpix(i,0, CRGB(G, R, B));
  M5.dis.drawpix(0, 1, CRGB(G, R, B)); M5.dis.drawpix(4, 1, CRGB(G, R, B));
  M5.dis.drawpix(0, 2, CRGB(G, R, B)); M5.dis.drawpix(4, 2, CRGB(G, R, B));
  M5.dis.drawpix(0, 3, CRGB(G, R, B)); M5.dis.drawpix(4, 3, CRGB(G, R, B));
  for (int i = 0; i < 5; i++) M5.dis.drawpix(i,4, CRGB(G, R, B));
}
