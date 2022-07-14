/*
 * File: M5StickC_GMC_PiGI.ino
 * 
 * Program for Geiger-Müller-Counter PiGI collects GMC pulses and calculates CPM and uSv/h
 * to send them to Thingspeak for visualization afterwards.
 * Based on makerGeiger.ino by Bernd Laquai https://github.com/opengeiger/makerGeiger
 * 
 * Board used is M5StickC.
 * GMC pulse output is connected to GPIO26. 
 * 
 * 2022-04-22 Claus Kühnel info@ckuehnel.ch
 */
#include <M5StickC.h>
#include <WiFi.h>
#include "arduino_secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

// Stock font and GFXFF reference handle
#define GFXFF 1
#define FF12 &FreeSans9pt7b
#define FF18 &FreeSans12pt7b

#define calFactor 1

float convFactor = 53.0;          // cpm/μSv/h
const uint16_t LED = 10;
const uint8_t displayWidth = 80;
const uint8_t displayHeight = 160;
const uint16_t countPin = 26;     // GPIO26 for counting GMC pulses

const uint16_t MAXCNT = 100;
volatile uint8_t counter = 0;
unsigned long tick, oldTime = 0;
volatile boolean iflag = false;

unsigned long previous_time = 0;
const unsigned long DELAY = 30000;  // 30 seconds delay

WiFiClient client;

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
  Serial.printf("\nReading PiGI GMC connected to M5StickC G26\n");
  initLCD();
  initWiFi();
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  pinMode(LED, OUTPUT); digitalWrite(LED, HIGH);
  pinMode(countPin, INPUT_PULLUP);  
  attachInterrupt(digitalPinToInterrupt(countPin), counterISR, FALLING);
} 
  
void loop() 
{
  unsigned long current_time = millis(); // number of milliseconds since the upload

  // checking for WIFI connection
  if ((WiFi.status() != WL_CONNECTED) && (current_time - previous_time >= DELAY)) 
  {
    Serial.print(millis());
    Serial.println(" - Reconnecting to WIFI network");
    WiFi.disconnect();
    WiFi.reconnect();
    previous_time = current_time;
  }
  
  if (iflag)
  {
    iflag = false;
    Serial.print(".");
    M5.Lcd.fillRect(1, 71, 1 + (int) counter * 1.77f, 8, BLUE);
  }
  if (millis() - tick > 50) digitalWrite(LED, HIGH);
  
  if (counter >= MAXCNT) 
  { 
    detachInterrupt(digitalPinToInterrupt(countPin)); 
    unsigned long now = millis(); 
    unsigned long dt = now - oldTime; 
    float rate = (float) MAXCNT * 60.0 * 1000.0 / (float) dt/calFactor; 
    Serial.printf("\nCount rate = %5.1f CPM", rate);
    dispValue(round(rate));
    float dose = rate/convFactor;
    Serial.printf("\t\tDosis = %5.3lf uSv/h\n", dose);

    // set the fields with the values
    ThingSpeak.setField(1, rate);
    ThingSpeak.setField(2, dose);

    // write to the ThingSpeak channel
    int x = ThingSpeak.writeFields(myChannelNumber, myWriteAPIKey);
    if(x == 200) Serial.println("Channel update successful.");
    else         Serial.println("Problem updating channel. HTTP error code " + String(x));

    oldTime = millis(); 
    counter = 0;   
    M5.Lcd.fillRect(1, 71, displayHeight-2, 8, BLACK); 
    attachInterrupt(digitalPinToInterrupt(countPin), counterISR, FALLING);   
  } 
}
