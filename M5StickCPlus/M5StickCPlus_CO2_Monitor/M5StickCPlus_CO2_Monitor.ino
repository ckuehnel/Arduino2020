/*
 * File: M5StickCPlus_CO2_Monitor.ino
 * 
 * Air quality measurement using M5Stack TVOC/eCO2 mini Unit w/ Sensirion SGP30 sensor.
 * SGP30 needs 15 seconds to initialize calibration after power on.
 * The M5StickC+ screen will display TVOC and CO2
 * 
 * 2020-10-25 Claus Kuehnel info@ckuehnel.ch
 */
#include "M5StickCPlus.h"
#include "Adafruit_SGP30.h"

Adafruit_SGP30 sgp;

const int LED = 10;
uint8_t i = 15;
long last_millis = 0;
uint16_t eCO2, TVOC;
const uint16_t LIMIT = 1200;

void header(const char *string, uint16_t color)
{
    M5.Lcd.fillScreen(TFT_BLACK);
    M5.Lcd.setTextSize(1);
    M5.Lcd.setTextColor(TFT_WHITE, color);
    M5.Lcd.fillRect(0, 0, 240, 30, color);
    M5.Lcd.setTextDatum(TC_DATUM);
    M5.Lcd.drawCentreString(string, 120, 3, 4); 
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
}

void setup() 
{
  pinMode(LED, OUTPUT);
  digitalWrite(LED, HIGH);
  
  M5.begin();
  M5.Lcd.setRotation(1);
  header("CO2 Monitor",TFT_BLUE);
  
  Serial.begin(115200);
  Serial.println("CO2 Monitor");
  
  Wire.begin(32, 33); // Grove I2C Connector
  if (!sgp.begin())
  {
    Serial.println("Sensor not found :(");
    while (1);
  }
  
  M5.Lcd.drawString("TVOC:", 50, 40, 4);
  M5.Lcd.drawString("eCO2:", 50, 70, 4);
  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);
  M5.Lcd.drawString("Initialization...", 120, 110, 4);
}

void loop() 
{
  while(i > 0) {    
    if(millis()- last_millis > 1000) 
    {
      last_millis = millis();
      i--;
      M5.Lcd.fillRect(0, 110, 40, 20, TFT_BLACK);
      M5.Lcd.drawNumber(i, 20, 110, 4);
    }
  }
  M5.Lcd.fillRect(0, 105, 240, 30, TFT_BLACK);

  if (! sgp.IAQmeasure()) {
    Serial.println("Measurement failed");
    return;
  }
  TVOC = sgp.TVOC;
  eCO2 = sgp.eCO2;
  M5.Lcd.fillRect(100, 40, 75, 90, TFT_BLACK);
  M5.Lcd.drawNumber(TVOC, 140, 40 , 4);
  M5.Lcd.drawString("ppb", 210, 40, 4);
  M5.Lcd.drawNumber(eCO2, 140, 70, 4);
  M5.Lcd.drawString("ppm", 210, 70, 4);
  Serial.print("TVOC "); Serial.print(TVOC); Serial.print(" ppb\t");
  Serial.print("eCO2 "); Serial.print(eCO2); Serial.println(" ppm");

  if(eCO2 >= LIMIT) 
  {
    M5.Beep.tone(4000); digitalWrite(LED, LOW);
    delay(100); 
    M5.Beep.mute(); digitalWrite(LED, HIGH);

    M5.Lcd.setTextColor(TFT_WHITE, TFT_RED);
    M5.Lcd.fillRect(0, 105, 240, 30, TFT_RED);
    M5.Lcd.drawString("Ventilation required!", 120, 110, 4);
    M5.Lcd.setTextColor(TFT_WHITE, TFT_BLACK);
  }
  else M5.Lcd.fillRect(0, 105, 240, 30, TFT_BLACK);
 
  delay(1000);
}
