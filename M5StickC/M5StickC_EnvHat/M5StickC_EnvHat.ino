/*
    note: need add library Adafruit_BMP280 from library manage
    Github: https://github.com/adafruit/Adafruit_BMP280_Library
*/
#include <M5StickC.h>
#include "DHT12.h"
#include <Wire.h>
#include "Adafruit_Sensor.h"
#include <Adafruit_BMP280.h>

DHT12 dht12; 
Adafruit_BMP280 bme;

void setup() 
{
  M5.begin();
  Wire.begin(0,26);
  M5.Lcd.setRotation(4);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.setCursor(0, 0, 2);
  M5.Lcd.setTextColor(BLUE);
  M5.Lcd.println("  M5Stick-C   ENV Hat");
  M5.Lcd.setTextColor(WHITE,BLACK);

  if (!bme.begin(0x76)){  
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      while (1);
  }
}

void loop() 
{
  float tmp = dht12.readTemperature();
  float hum = dht12.readHumidity();
  M5.Lcd.fillRect(0, 50, 160, 16, BLACK);
  M5.Lcd.setCursor(0, 50, 4);
  if (tmp < 10 && tmp >= 0) M5.Lcd.printf("  ");
  M5.Lcd.printf("%2.1f*C", tmp);
  M5.Lcd.fillRect(0, 90, 160, 16, BLACK);
  M5.Lcd.setCursor(0, 90, 4);
  if (hum < 10) M5.Lcd.printf("  ");
  M5.Lcd.printf("   %2.0f%%", hum);
  
  float pressure = bme.readPressure();
  M5.Lcd.fillRect(0, 130, 160, 10, BLACK);
  M5.Lcd.setCursor(0, 130, 2);
  M5.Lcd.printf("  %2.1f", pressure);
  delay(1000);

  if (!bme.begin(0x76)){  
      Serial.println("Could not find a valid BMP280 sensor, check wiring!");
      while (1);
  }
}
