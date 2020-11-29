/*
 * File: CoreInk_EnvII_Node.ino
 * 
 * IoT node w/ CoreInk including 2.13" ePaper display. The node is powered by internal 3.7 V LiPo (390 mAh) or  
 * USB alternatively. The battery voltage is measured internally. 
 * A M5 ENV II Unit is connected via I2C to measure temperature.
 * The measuring values are send each hour by Telegram.
 * 
 * Board: M5Stack-CoreInk
 * 
 * 2020-11-29 Claus Kuehnel info@ckuehnel.ch
 */
 
#include <M5CoreInk.h>
#include <Wire.h>
//#include "Adafruit_Sensor.h"
//#include <Adafruit_BMP280.h>
#include "SHT3X.h"
#include "esp_adc_cal.h"

// WiFi connection 
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "arduino_secrets.h"

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

Ink_Sprite InkPageSprite(&M5.M5Ink);
SHT3X sht30;
//Adafruit_BMP280 bme;

// General
#define DEBUG 0   // enable/disable serial output
#define WIFI 1    // enable/disable WiFi connection

#define uS_TO_S_FACTOR 1000000  // Conversion factor for micro seconds to seconds 
#define TIME_TO_SLEEP  1800     // Time ESP32 will go to sleep (in seconds)

const int LED  = 10;  // LED @ GPIO10
float voltage, tmp, hum;

float getVbat(void);

void setup() 
{
  M5.begin();
  if (DEBUG)
  {
    while(!Serial);
    print_wakeup_reason(); //Print the wakeup reason for ESP32
  
    Serial.print(F("Initializing..."));
  }
  Wire.begin(32, 33); // Grove I2C Connector

  if( !M5.M5Ink.isInit())
  {
    if (DEBUG) Serial.printf("Ink Init faild");
    while (1) delay(100);   
  }
  M5.M5Ink.clear();
  delay(1000);
  //creat ink refresh Sprite
  if( InkPageSprite.creatSprite(0,0,200,200,true) != 0 )
  {
    if (DEBUG) Serial.printf("Ink Sprite creat faild");
  }
  InkPageSprite.drawString(20,70,"Initializing...");
  InkPageSprite.pushSprite();

  pinMode(LED, OUTPUT);
  digitalWrite(LED, 0);
  delay(100);
  digitalWrite(LED,1);

  InkPageSprite.drawString(20,70,"Initializing...done");
  InkPageSprite.pushSprite();
  if (DEBUG) Serial.println("done.");
  delay(500);
}

void loop()
{
  M5.M5Ink.clear();
  
  if(sht30.get()==0)
  {
    tmp = sht30.cTemp;
    hum = sht30.humidity;
  }
  voltage = getVbat();
  if (DEBUG) Serial.printf("Temperature: %2.2f *C  Humidity: %0.2f %%  Vbat: %0.3f V\r\n", tmp, hum, voltage);

  InkPageSprite.drawString(10, 10, "Measuring values:");
  InkPageSprite.pushSprite();

  char Buffer[64];
  sprintf(Buffer,"LiPo Voltage: %2.3f V", voltage);
  InkPageSprite.drawString(10, 60, Buffer, &AsciiFont8x16);

  sprintf(Buffer,"Temperature : %2.1f *C", tmp);
  InkPageSprite.drawString(10, 120, Buffer, &AsciiFont8x16);

  sprintf(Buffer,"Humidity    : %2.1f %rH", hum);
  InkPageSprite.drawString(10, 140, Buffer, &AsciiFont8x16);

  InkPageSprite.pushSprite();

  if (WIFI) sendMessage();
  
  // goto sleep
  M5.shutdown(TIME_TO_SLEEP);
  //esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  if (DEBUG) Serial.println("Setup CoreInk to sleep for every " + String(TIME_TO_SLEEP) + " s.\n");

  //esp_deep_sleep_start();
}


// Method to print the reason by which ESP32 has been awaken from sleep
void print_wakeup_reason()
{
  esp_sleep_wakeup_cause_t wakeup_reason;
  wakeup_reason = esp_sleep_get_wakeup_cause();
  switch(wakeup_reason)
  {
    case ESP_SLEEP_WAKEUP_EXT0 :     Serial.println("\nWakeup caused by external signal using RTC_IO"); break;
    case ESP_SLEEP_WAKEUP_EXT1 :     Serial.println("\nWakeup caused by external signal using RTC_CNTL"); break;
    case ESP_SLEEP_WAKEUP_TIMER :    Serial.println("\nWakeup caused by timer"); break;
    case ESP_SLEEP_WAKEUP_TOUCHPAD : Serial.println("\nWakeup caused by touchpad"); break;
    case ESP_SLEEP_WAKEUP_ULP :      Serial.println("\nWakeup caused by ULP program"); break;
    default :                        Serial.printf("\nWakeup was not caused by deep sleep: %d\n",wakeup_reason); break;
  }
}
