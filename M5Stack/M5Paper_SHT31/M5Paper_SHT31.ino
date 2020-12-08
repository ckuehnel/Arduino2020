/*
 * File: M5Paper_SHT30.ino
 * 
 * 
 */
#include <M5EPD.h>
#include <Free_Fonts.h>
//#include "Image.h"

// WiFi connection 
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "arduino_secrets.h"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

// General
#define DEBUG 0   // enable/disable serial output
#define WIFI 1    // enable/disable WiFi connection


#define TIME_TO_SLEEP 1800     // Time ESP32 will go to sleep (in seconds)


float temp, humi;
uint32_t voltage;

M5EPD_Canvas canvas(&M5.EPD);


void setup()
{
  M5.begin();
  if (DEBUG) Serial.print("\nSetup M5Paper...");
  M5.SHT30.Begin();
  M5.RTC.begin();
  M5.BatteryADCBegin();
  M5.EPD.SetRotation(90);
  M5.EPD.Clear(true);
  
  canvas.createCanvas(540, 960);
//  canvas.pushImage(0,0, 32, 32,ImageResource_backspace_32x32);
//  canvas.pushCanvas(0,0,UPDATE_MODE_DU4);
  canvas.setFreeFont(FF8);
  canvas.setTextSize(2);
  canvas.drawString("M5Paper",20, 150);
  canvas.pushCanvas(0,0,UPDATE_MODE_DU4);
  canvas.setFreeFont(FSS18);
  canvas.setTextSize(1);
  if (DEBUG) Serial.println("done.");
}

void loop()
{
  voltage = M5.getBatteryVoltage();
  canvas.drawString("Battery Voltage: " + String(voltage/1000., 3) + " V", 20, 400);
  
  M5.SHT30.UpdateData();
  temp = M5.SHT30.GetTemperature();
  humi = M5.SHT30.GetRelHumidity();
  canvas.drawString("Int. Temperature: " + String(temp,1) + " *C", 20, 500);
  canvas.drawString("Rel. Humidity: " + String(humi,1) + " %rH",20, 550);
  canvas.pushCanvas(0,0,UPDATE_MODE_DU4);

  if (DEBUG)
  {
    Serial.printf("Battery Voltage: %4d mV\n", voltage);
    Serial.printf("Int. Temperature: %2.1f *C\n", temp);
    Serial.printf("Rel. Humidity: %2.1f %rH\n\n", humi);
  }

  if(WIFI) sendMessage(); 
  
  // goto sleep
  M5.shutdown(TIME_TO_SLEEP); //Deep Sleep only w/o USB connection
  if (DEBUG) Serial.println("Setup M5Paper to sleep for every " + String(TIME_TO_SLEEP) + " s.\n");
}
