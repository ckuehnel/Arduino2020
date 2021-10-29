/*
 * The access to ENV.II Sensor Unit is organized here.
 * The ENV.II Sensor contains a SHT31 sensor to measure temperature & humidity,
 * and a BMP280 sensor to get temperature & pressure.
 * The results are saved in global float variables for each sensor.
 * 
 * 2021-06-04 Claus Kühnel info@ckuehnel.ch
 */
 
#include <Wire.h>
#include <Adafruit_SHT31.h>
#include <Adafruit_BMP280.h>

Adafruit_SHT31 sht31 = Adafruit_SHT31();

Adafruit_BMP280 bmp; // use I2C interface
Adafruit_Sensor *bmp_temp = bmp.getTemperatureSensor();
Adafruit_Sensor *bmp_pressure = bmp.getPressureSensor();

void initSensor()
{
   if (!sht31.begin(0x44)) // Set to 0x45 for alternate i2c addr
  {   
    Serial.println("Couldn't find SHT31");
    while (1) delay(10);
  }
  
  sht31.heater(false);
  Serial.print("SHT31 Heater Enabled State: ");
  if (sht31.isHeaterEnabled()) Serial.println("ENABLED");
  else                         Serial.println("DISABLED");

  if (!bmp.begin(BMP280_ADDRESS_ALT, BMP280_CHIPID)) // I2C Addr 0x76
  {
    Serial.println("Could not find BMP280");
    while (1) delay(10);
  }
  /* Default settings from datasheet. */
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Operating Mode. */
                  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
                  Adafruit_BMP280::SAMPLING_X16,    /* Pressure oversampling */
                  Adafruit_BMP280::FILTER_X16,      /* Filtering. */
                  Adafruit_BMP280::STANDBY_MS_500); /* Standby time. */
  // bmp_temp->printSensorDetails();
}

void getSHT31Values()
{
  sht31Temperature = sht31.readTemperature();
  sht31Humidity = sht31.readHumidity();
  if (DEBUG)
  {
    if (!isnan(sht31Temperature))  // check if 'is not a number'
    {
      Serial.print("SHT31  Temp  = "); Serial.print(sht31Temperature); Serial.print(" *C\t\t");
    } else Serial.println("Failed to read temperature");
  
    if (!isnan(sht31Humidity))  // check if 'is not a number'
    {
      Serial.print("SHT31 Hum.   = "); Serial.print(sht31Humidity); Serial.println(" %rH");
    } 
    else Serial.println("Failed to read humidity");
  }
}

void getBMP280Values()
{
  sensors_event_t temp_event, pressure_event;
  bmp_temp->getEvent(&temp_event);
  bmp_pressure->getEvent(&pressure_event);

  bmp280Temperature = temp_event.temperature;
  bmp280Pressure = pressure_event.pressure;

  if (DEBUG)
  {
    Serial.print("BMP280 Temp  = "); Serial.print(bmp280Temperature); Serial.print(" *C\t\t");
    Serial.print("BMP280 Press = "); Serial.print(bmp280Pressure); Serial.println(" hPa");
  }
}

void getValues()
{
  getSHT31Values();
  getBMP280Values();
}
