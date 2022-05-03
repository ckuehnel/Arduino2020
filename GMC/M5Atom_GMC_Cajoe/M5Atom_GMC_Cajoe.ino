/*
 * File: M5Atom_GMC_Cajoe.ino
 * 
 * Program for Geiger-Müller-Counter Cajoe collects GMC pulses and calculates CPM and uSv/h
 * to send them to Thingspeak for visualization afterwards.
 * Based on makerGeiger.ino by Bernd Laquai https://github.com/opengeiger/makerGeiger
 * 
 * Board used is M5ATOM-Lite. https://www.fambach.net/wemos-d1-r32-esp32/
 * GMC pulse output is connected to GPIO22. 
 * 
 * 2022-04-22 Claus Kühnel info@ckuehnel.ch
 */
#include <M5Atom.h>
#include <WiFi.h>
#include "arduino_secrets.h"
#include "ThingSpeak.h" // always include thingspeak header file after other header files and custom macros

#define calFactor 1

float convFactor = 154.0;          // cpm/μSv/h
const uint16_t LED = 2;
const uint16_t countPin = 22;     // GPIO22 for counting GMC pulses
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
  tick = millis();
}

void setup() 
{
  M5.begin(/* SerialEnable */ true, /* I2CEnable */ false, /* DisplayEnable */ true);    
  delay(50);   // delay 50ms
  M5.dis.drawpix(0, 0x00ff00); // GREEN
  Serial.printf("\nReading Cajoe GMC connected to ESP32 GPIO22\n");
  initWiFi();
  ThingSpeak.begin(client);  // Initialize ThingSpeak
  pinMode(countPin, INPUT);  // Due to the resistor at output transistor no pullup here
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
    M5.dis.drawpix(0, 0xff0000);  // RED
  }
  if (millis() - tick > 50) M5.dis.drawpix(0, 0x0000f0);  // BLUE
  
  if (counter >= MAXCNT) 
  { 
    detachInterrupt(digitalPinToInterrupt(countPin)); 
    unsigned long now = millis(); 
    unsigned long dt = now - oldTime; 
    float rate = (float) MAXCNT * 60.0 * 1000.0 / (float) dt/calFactor; 
    Serial.printf("\nCount rate = %5.1f CPM", rate);
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
    attachInterrupt(digitalPinToInterrupt(countPin), counterISR, FALLING);   
  } 
}
