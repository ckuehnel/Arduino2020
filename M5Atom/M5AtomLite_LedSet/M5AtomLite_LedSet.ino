/*
 * File: M5AtomLite_LedSet.ino
 * 
 * Test button and RGB Led of Atom Lite
 * Select ESP32 Pico Kit as Board
 */
#include <M5Atom.h>

void setup()
{
  M5.begin(true, true, true); // Serial, I2C, Display
  delay(50);
  Serial.println("\nM5Atom Lite test...");
  Serial.println("Initial State Green");
  M5.dis.drawpix(0, 0xf00000);
}

uint8_t FSM = 0;

void loop()
{
  if (M5.Btn.wasPressed())
  {
    switch (FSM)
    {
      case 0:
        Serial.println("Green");
        M5.dis.drawpix(0, 0xf00000);
        break;
      case 1:
        Serial.println("Red");
        M5.dis.drawpix(0, 0x00f000);
        break;
      case 2:
        Serial.println("Blue");
        M5.dis.drawpix(0, 0x0000f0);
        break;
      case 3:
        Serial.println("All colors reduced brightness");
        M5.dis.drawpix(0, 0x707070);
        break;
      default:
        break;
    }
    FSM++;
    if (FSM >= 4) FSM = 0;
  }
  delay(50);
  M5.update();
}
