/*
 * File: XIAO_WaterLevel.ino
 * 
 * Grove Water Level Sensor connected to Seeeduino XIAO
 * via Grove Shield for Seeduino XIAO
 * 
 * 2020-07-19 Claus Kühnel info@ckuehnel.ch
 */

#include <Wire.h>

unsigned char low_data[8] = {0};
unsigned char high_data[12] = {0};
 
#define NO_TOUCH       0xFE
#define THRESHOLD      100
#define ATTINY1_HIGH_ADDR   0x78
#define ATTINY2_LOW_ADDR   0x77
#define DEBUG 0
 
void getHigh12SectionValue(void)
{
  memset(high_data, 0, sizeof(high_data));
  Wire.requestFrom(ATTINY1_HIGH_ADDR, 12);
  while (12 != Wire.available());
 
  for (int i = 0; i < 12; i++) {
    high_data[i] = Wire.read();
  }
  delay(10);
}
 
void getLow8SectionValue(void)
{
  memset(low_data, 0, sizeof(low_data));
  Wire.requestFrom(ATTINY2_LOW_ADDR, 8);
  while (8 != Wire.available());
 
  for (int i = 0; i < 8 ; i++) {
    low_data[i] = Wire.read(); // receive a byte as character
  }
  delay(10);
}
 
void checkWaterLevel()
{
  int sensorvalue_min = 250;
  int sensorvalue_max = 255;
  int low_count = 0;
  int high_count = 0;
  while (1)
  {
    uint32_t touch_val = 0;
    int trig_section = 0;
    low_count = 0;
    high_count = 0;
    getLow8SectionValue();
    getHigh12SectionValue();

    if (DEBUG) Serial.println("low 8 sections value = ");
    for (int i = 0; i < 8; i++)
    {
      if (DEBUG) Serial.print(low_data[i]);
      if (DEBUG) Serial.print(".");
      if (low_data[i] >= sensorvalue_min && low_data[i] <= sensorvalue_max)
      {
        low_count++;
      }
      if (low_count == 8)
      {
        if (DEBUG) Serial.print("      ");
        if (DEBUG) Serial.print("PASS");
      }
    }
    if (DEBUG) 
    {
      Serial.println("  ");
      Serial.println("  ");
      Serial.println("high 12 sections value = ");
    }
    for (int i = 0; i < 12; i++)
    {
      if (DEBUG) Serial.print(high_data[i]);
      if (DEBUG) Serial.print(".");
 
      if (high_data[i] >= sensorvalue_min && high_data[i] <= sensorvalue_max)
      {
        high_count++;
      }
      if (high_count == 12)
      {
        if (DEBUG) Serial.print("      ");
        if (DEBUG) Serial.print("PASS");
      }
    }
 
    if (DEBUG) Serial.println("  ");
    if (DEBUG) Serial.println("  ");
 
    for (int i = 0 ; i < 8; i++) {
      if (low_data[i] > THRESHOLD) {
        touch_val |= 1 << i;
 
      }
    }
    for (int i = 0 ; i < 12; i++) {
      if (high_data[i] > THRESHOLD) {
        touch_val |= (uint32_t)1 << (8 + i);
      }
    }
  
    while (touch_val & 0x01)
    {
      trig_section++;
      touch_val >>= 1;
    }
    Serial.print("water level = ");
    Serial.print(trig_section * 5);
    Serial.println(" % ");
    if (DEBUG) Serial.println(" ");
    if (DEBUG) Serial.println("*********************************************************");
  }
}
 
void setup() 
{
  Serial.begin(115200);
  delay(2000); // wait for Serial Monitor
  Wire.begin();
}
 
void loop()
{
  checkWaterLevel();
  delay(5000000); //  delay in microseconds
}
