/*
 *  File: M5Core2_EnvMonitor.ino
 *
 *  Environmental Monitoring using M5Stack ENV.II Unit to measure temperature and humidity 
 *  and EARTH Unit to measure soil moisture controlled by M5Stack Core2.
 *  It measures the environmental conditions in a cycle of 5 sec. 
 *  and sends messages via Pushover in y 15 minutes cycle.
 *  
 *  2021-10-29 Claus Kühnel info@ckuehnel.ch
 */
 
#include <M5Core2.h>
#include <WiFi.h>
#include "FastLED.h"
#include "arduino_secrets.h"

#define DEBUG 1

const int LEDS_PIN = 25;
const int LEDS_NUM = 10;
CRGB ledsBuff[LEDS_NUM];

const int soilPin = 36;         // analog input for M5Stack Core2
const int airValue = 4095;      // adapt it to your conditions
const int waterValue = 2000;    // adapt it to your conditions
const int moistureLevel = 50;   // adapt it to your conditions
uint16_t soilMoisture = 0; 

float sht31Temperature, sht31Humidity;
float bmp280Temperature, bmp280Pressure;

float current_humidity = -100;
float previous_humidity = -100;
float current_temperature = -100;
float previous_temperature = -100;
float previous_moisture = -100;
float current_moisture = -100;

const uint16_t mCYCLE = 5000;         // measuring cycle 5000 ms
const uint32_t sCYCLE = 15 * 60000;   // sending cycle 15 minutes

uint16_t maxCount = sCYCLE/mCYCLE;     

int length;
int count = -1;

int previous_rssiValue;
int current_rssiValue;

WiFiClient client;

void setup()
{
  M5.begin(/* LCDEnable */ true, /* SDEnable */ false, /* SerialEnable */ true, /* I2CEnable */ true);
  M5.Lcd.setTextColor(GREEN);
  M5.Lcd.setCursor(80, 0);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("M5Stack Core2");
  M5.Lcd.setCursor(20, 18);
  M5.Lcd.printf("Environmental Monitoring");

  Serial.println("\nM5Stack Core2 Environmental Monitoring");

  initWiFi();

  current_rssiValue = WiFi.RSSI();
  displayRSSI();

  M5.Lcd.setTextColor(WHITE);

  Serial.println("Sensor Inititialization...");
  initSensor();     // initialize ENV.II sensor

  FastLED.addLeds<SK6812, LEDS_PIN>(ledsBuff, LEDS_NUM);
  LEDsOn(); delay(200); LEDsOff();
  Serial.println("Initialization done.\n");
}

void loop() 
{
  count++;
  getValues();
  soilMoisture = map(analogRead(soilPin), waterValue, airValue, 100, 0); // read analog value of EARTH unit and map it to 0 - 100
  Serial.print("Soil Moisture = "); Serial.print(soilMoisture); Serial.println(" %");
  if (soilMoisture < moistureLevel) LEDsOn();
  else LEDsOff();

  current_rssiValue = WiFi.RSSI();
  Serial.print("RSSI = "); Serial.print(current_rssiValue); Serial.println(" dBm\n");
  displayRSSI();
  
  current_temperature = sht31Temperature; displayTemperature();
  current_humidity = sht31Humidity; displayHumidity();
  current_moisture = soilMoisture; displayMoisture();
  
  if (count >= maxCount || count == 0)
  {
    if (WiFi.status() != WL_CONNECTED) 
    {
      Serial.println("Reconnecting to WiFi...");
      ESP.restart();
    }
    
    String message  = "Temperature = ";
           message += String(sht31Temperature, 1);
           message += " °C\n";
           message += "Humidity = ";
           message += String(sht31Humidity, 1);
           message += " rH\n";
           message += "Soil Moisture = ";
           message += String(soilMoisture);
           message += " (0-100)\n";
           message += "RSSI = ";
           message += String(current_rssiValue);
           message += " dBm";
    char arr[message.length() + 1];
    for (int i = 0; i < sizeof(arr); i++) arr[i] = message[i]; 
    pushover(arr, 0); 
    count = 0;
  }
  delay(mCYCLE);
}
