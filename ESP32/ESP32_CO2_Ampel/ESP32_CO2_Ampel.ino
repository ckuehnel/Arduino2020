/*
 * File: ESP32_CO2_Ampel.ino
 * 
 * Read measuring data of SGP30 and BME280 sensors to monitor
 * TVOC and eCO2 in room air
 * 
 * 2021-01-28 Claus Küehnel info@ckuehnel.ch
 *
 * Make sure all the display driver and pin comnenctions are correct by
 * editting the User_Setup.h file in the TFT_eSPI library folder.

 #########################################################################
 ###### DON'T FORGET TO UPDATE THE User_Setup.h FILE IN THE LIBRARY ######
 #########################################################################
 */

#include "SPI.h"
#include "TFT_eSPI.h"
#include "Free_Fonts.h" // Include the header file attached to this sketch
#include "Tone32.h"
#include <BMx280I2C.h>
#include "Adafruit_SGP30.h"

#define TONE 1
#define DEBUG 1

#define I2C_ADDRESS 0x76

//create a BMx280I2C object using the I2C interface with I2C Address 0x76
BMx280I2C bmx280(I2C_ADDRESS);

Adafruit_SGP30 sgp;

// Use hardware SPI
TFT_eSPI tft = TFT_eSPI();

unsigned int i = 0;

const int LED_32 = 15;  // TFT LED
const int BEEPER = 21;  // Beeper

float    temp, humi;
uint32_t press = 999;
uint16_t eCO2, TVOC;

// Function prototypes
void melody(void);

void setup() 
{
  pinMode(LED_32, OUTPUT);
  digitalWrite(LED_32, LOW); // TFT LED On

  Wire.begin(25, 32, 100000);  // SDA = GPIO25, SCL = GPIO32
  
  Serial.begin(115200);
  while (!Serial);
  Serial.println(F("\nCO2 Display built w/ AZ-Touch ESP")); 
  Serial.print(F("\nRead M5Stack TVOC/eCO2 Unit to get Environmental Data\n"));

  sensorInit();   // Initialize CCS811 & BME280        
 
  tft.init();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);

  tft.setFreeFont(FSB9);
  tft.setCursor(210, 12);
  tft.println(F("CO2 Display"));
  tft.setCursor( 12, 120);

  if (TONE) melody();
  delay(1000);
}

void loop(void)
{
  boolean flag = false;
  getEnvData();   // Read sensors

  if (eCO2 >= 1500)
  {
    tft.fillScreen(TFT_RED);
    flag = true;
  }
  if (eCO2 >= 800 && eCO2 < 1500) tft.fillScreen(TFT_ORANGE);
  if (eCO2 < 800) tft.fillScreen(TFT_DARKGREEN);

  tft.setFreeFont(FSB9);
  tft.setCursor(210, 12);
  tft.println(F("CO2 Display"));

  tft.setFreeFont(FSB18);       // Select Free Serif 18 point font
  tft.setCursor(0, 50);

  tft.print(F(" Temp   ")); tft.print(temp,1); tft.println(" C");
  if (bmx280.isBME280()) 
  {
    tft.print(F(" Hum    ")); tft.print(humi,0); tft.println(" % rH");  
  }
  else                   
    tft.print(F(" Hum - no BME280"));
  
  tft.print(F(" Press   ")); tft.print(press/100); tft.println(" hPa");

  tft.print(F(" TVOC    ")); tft.print(TVOC); tft.println(" ppb");
  tft.print(F(" eCO2   ")); tft.print(eCO2); tft.println(" ppm");
  if (TONE && flag) beeps();

	delay(15 * 1000L);
}
