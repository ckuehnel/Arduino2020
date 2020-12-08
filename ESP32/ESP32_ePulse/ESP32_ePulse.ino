/*
 * File: ESP32_ePulse.ino
 * 
 * 2020-11-27 Claus Kuehnel info@ckuehnel.ch
 */
 
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

#define uS_TO_S_FACTOR 1000000  // Conversion factor for micro seconds to seconds 
#define TIME_TO_SLEEP  1800     // Time ESP32 will go to sleep (in seconds)

const uint8_t ADC10 = 36; // ADC1:0 @ GPIO36
const uint8_t VBMP  = 23; // VBMP @ GPIO23
const uint8_t LED   = 19; // LED @ GPIO19

float voltage, temp;

void setup()
{
  pinMode(VBMP, OUTPUT);
  digitalWrite(VBMP, 1); // BMP280 Power on
  
  if (DEBUG)
  {
    Serial.begin(115200);
    while(!Serial);
    
    print_wakeup_reason(); //Print the wakeup reason for ESP32
  
    Serial.print("Setup...");
  }

  initBMP280();

  if (DEBUG)
  {
    pinMode(LED, OUTPUT);
    digitalWrite(LED, 1);
    delay(100);
    digitalWrite(LED,0);
  }

 
  voltage = analogRead(ADC10);
  delay(10);
  voltage = analogRead(ADC10) * 2 * 3.3/4096; // due to voltage divider;     
  if (DEBUG) Serial.println("Vbat: " + String(voltage) + " V");
  
  temp = getTemperature(); // read external sensor
  if (DEBUG) Serial.println("External Temperature: " + String(temp) + " *C");

  digitalWrite(VBMP, 0); // BMP280 Power off
    
  if (WIFI) sendMessage();

  // goto sleep
  esp_sleep_enable_timer_wakeup(TIME_TO_SLEEP * uS_TO_S_FACTOR);
  if (DEBUG) Serial.println("Setup ePulse to sleep for every " + String(TIME_TO_SLEEP) + " s.\n");

  esp_deep_sleep_start();
}

void loop() {}


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
