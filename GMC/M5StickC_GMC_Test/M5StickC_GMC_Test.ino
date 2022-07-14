/*
 * File: M5StickC_GMC_Test.ino
 * 
 * Testprogram for Geiger-Müller-Zähler
 * based on makerGeiger.ino by Bernd Laquai https://github.com/opengeiger/makerGeiger
 * 
 * GMC Output must be connected to G26 @ M5StickC. Do not used G36. There is no internal pull-up.
 * 
 * 2022-04-16 Claus Kühnel info@ckuehnel.ch
 */
 
#include <M5StickC.h>

// Stock font and GFXFF reference handle
#define GFXFF 1
#define FF12 &FreeSans9pt7b
#define FF18 &FreeSans12pt7b

#define CalFactor 1

const uint8_t LED = 10;
const uint8_t displayWidth = 80;
const uint8_t displayHeight = 160;
const uint16_t countPin = 26;     // GPIO26 for counting GMC pulses
const uint16_t MAXCNT = 100;
volatile uint16_t counter = 0;
unsigned long tick, oldTime = 0;
volatile boolean iflag = false;

void IRAM_ATTR counterISR() 
{
  counter++;
  iflag = true;
  digitalWrite(LED, LOW);
  tick = millis();
}

void setup() 
{
  M5.begin();
  Serial.printf("\nReading GMC connected to M5StickC G26\n");
  initLCD();
  pinMode(LED, OUTPUT); digitalWrite(LED, HIGH);
  pinMode(countPin, INPUT_PULLUP);
  attachInterrupt(digitalPinToInterrupt(countPin), counterISR, FALLING);
  M5.Lcd.drawRect(0, 70, displayHeight, 10, BLUE);
} 
  
void loop() 
{
  if (iflag)
  {
    iflag = false;
    M5.Lcd.fillRect(1, 71, 1 + (int) counter * 1.77f, 8, BLUE);
  }
  
  if (millis() - tick > 10) digitalWrite(LED, HIGH);
  
  if (counter >= MAXCNT) 
  { 
    detachInterrupt(digitalPinToInterrupt(countPin)); 
    unsigned long now = millis(); 
    unsigned long dt = now - oldTime; 
    float rate = (float) MAXCNT * 60.0 * 1000.0 / (float) dt/CalFactor; 
    Serial.printf("Count rate = %5.1f cmp\n", rate);
    dispValue(round(rate));
    oldTime = millis(); 
    counter = 0;   
    M5.Lcd.fillRect(1, 71, displayHeight-2, 8, BLACK); 
    attachInterrupt(digitalPinToInterrupt(countPin), counterISR, FALLING);   
  } 
}
