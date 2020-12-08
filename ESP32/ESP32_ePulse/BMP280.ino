// I2C bus connected BMP280

#include <Wire.h>
#include <Adafruit_BMP280.h>

Adafruit_BMP280 bmp; // I2C

void initBMP280()
{
  Wire.begin(21, 22);
  
  if (!bmp.begin(BMP280_ADDRESS_ALT)) // SDO open
  {
    if (DEBUG) Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
}

float getTemperature()
{
  float temp = bmp.readTemperature();
  return temp;
}
