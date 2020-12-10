// I2C bus connected BMP280

#include <BMP280.h>

BMP280 bmp;

void initBMP280()
{
  
  
  if (!bmp.begin(BMP280_ADDRESS_ALT)) // SDO open
  {
    if (DEBUG) Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  /* Default settings from datasheet. */
  bmp.setSampling(BMP280::MODE_NORMAL,     /* Operating Mode. */
                  BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  BMP280::FILTER_X16,      /* Filtering. */
                  BMP280::STANDBY_MS_500); /* Standby time. */
}

float getTemperature()
{
  float temp = bmp.readTemperature();
  return temp;
}
