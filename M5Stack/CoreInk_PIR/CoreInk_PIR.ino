/*
 * File: CoreInk_PIR.ino
 * 
 * 
 */
 
#include "M5CoreInk.h"
#include "esp_adc_cal.h"

Ink_Sprite InkPageSprite(&M5.M5Ink);

RTC_DATA_ATTR int bootCount = 0;
RTC_DATA_ATTR float voltage;

// General
#define DEBUG 1   // enable/disable serial output
#define WIFI 0    // enable/disable WiFi connection

#define uS_TO_S_FACTOR 1000000  // Conversion factor for micro seconds to seconds 
#define TIME_TO_SLEEP  1800     // Time ESP32 will go to sleep (in seconds)

char buffer[12];

void setup() 
{
  M5.begin();
  // Green LED - indicates ESP32 is running
  digitalWrite(LED_EXT_PIN, LOW);

  if (DEBUG) print_wakeup_reason();

  if(M5.M5Ink.isInit() == false)
  {
    Serial.printf("Ink Init failed");
    while(1) delay(100);
  }
  if (bootCount % 10 == 0) M5.M5Ink.clear();

  // After every shutdown the sprite is created anew.
  // But the sprite doesn't know about the current image on the
  //  ink display therefore the same time and date, as have been
  //  drawn before the shutdown, are redrawn.
  // This is required, else drawing new time and date only adds
  //  pixels to the already drawn pixels instead of clearing the
  //  previous time and date and then draw the new time and date
  if( InkPageSprite.creatSprite(0,0,200,200,true) != 0 )
  {
    if (DEBUG) Serial.printf("Ink Sprite create failed");
  }

  displayValues(bootCount, voltage);
  InkPageSprite.pushSprite();

  ++bootCount;  //Increment boot number and print it every reboot
  voltage = getVbat();
  if (DEBUG) Serial.printf("Boot Count: %d Vbat: %0.3f V\r\n", bootCount, voltage);

  displayValues(bootCount, voltage);
  InkPageSprite.pushSprite();
  
  esp_sleep_enable_ext0_wakeup(GPIO_NUM_36,1); //1 = High, 0 = Low

  //Go to sleep now
  if (DEBUG) 
  {
    Serial.println("Going to sleep now");
    Serial.flush();
  }
  delay(2000);
  esp_deep_sleep_start();
}

void loop() {}
