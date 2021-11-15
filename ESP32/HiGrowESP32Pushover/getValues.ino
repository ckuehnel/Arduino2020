/*
 * The access to the sensors of LILYGO®TTGO T-Higrow is organized here.
 * There are following sensors: 
 *   BME280
 *   BH1750
 * 
 * 2021-11-14 Claus Kühnel info@ckuehnel.ch
 */
 
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include <BH1750.h>

#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme;
BH1750 lightMeter;

void initSensors()
{
  unsigned status;
    
  status = bme.begin();  
  if (!status) 
  {
    Serial.println("Could not find a valid BME280 sensor, check wiring, address, sensor ID!");
    Serial.print("SensorID was: 0x"); Serial.println(bme.sensorID(),16);
    Serial.print("        ID of 0xFF probably means a bad address, a BMP180 or BMP085\n");
    Serial.print("   ID of 0x56-0x58 represents a BMP280,\n");
    Serial.print("        ID of 0x60 represents a BME280.\n");
    Serial.print("        ID of 0x61 represents a BME680.\n");
    while (1) delay(10);
  }
  lightMeter.begin();
}

void getBMEValues()
{
  bmeTemperature = bme.readTemperature();
  bmeHumidity = bme.readHumidity();
  bmePressure = bme.readPressure()/100.0;
  bmeAltitude = bme.readAltitude(SEALEVELPRESSURE_HPA);

  if (DEBUG)
  {
    Serial.print("BME280 Temperature = " + String(bmeTemperature,1) + " °C\n");
    Serial.print("BME280 Humidity    = " + String(bmeHumidity,0) + " %rH\n");
    Serial.print("BME280 Pressure    = " + String(bmePressure,0) + " hPa\n");
    Serial.print("BME280 Altitude    = " + String(bmeAltitude,1) + " m\n");
  }
}

void getLightValue()
{
  if (lightMeter.measurementReady(true)) 
  {    
    lux = lightMeter.readLightLevel();
    if (DEBUG) Serial.print("BH1750 Illuminance = " + String(lux,0) + " lx\n");
    
    if (lux < 0) Serial.println(F("Error condition detected"));
    else 
    {
      if (lux > 40000.0) 
      {
        // reduce measurement time - needed in direct sun light
        if (lightMeter.setMTreg(32)) Serial.println(F("Setting MTReg to low value for high light environment"));
        else                         Serial.println(F("Error setting MTReg to low value for high light environment"));
      }
      else 
      {
        if (lux > 10.0) 
        {
          // typical light environment
          if (lightMeter.setMTreg(69)) Serial.println(F("Setting MTReg to default value for normal light environment"));
          else                         Serial.println(F("Error setting MTReg to default value for normal light environment"));
        }
        else 
        {
          if (lux <= 10.0) 
          {
            //very low light environment
            if (lightMeter.setMTreg(138)) Serial.println(F("Setting MTReg to high value for low light environment"));
            else                          Serial.println(F("Error setting MTReg to high value for low light environment"));
          }
        }
      }
    }
  }
}

void getBatteryVoltage()
{
  const int vref = 1100;
  delay(2000);
  uint16_t volt = analogRead(BAT_ADC);
  vBat = ((float)volt / 4096.0) * 2.0 * 3.3 * (vref) / 1000;
  if (DEBUG) Serial.print("Battery Voltage    = " + String(vBat,3) + " V\n");
}

void getSoilMoisture()
{
  soilMoisture = analogRead(SOIL);
  if (DEBUG) Serial.print("SoilMoisture Raw Data = "); Serial.println(soilMoisture);
  soilMoisture = map(soilMoisture, waterValue, airValue, 100, 0); 
  soilMoisture = constrain(soilMoisture, 0, 100);  // map analog value to 0 - 100
  if (DEBUG) Serial.print("Soil Moisture = "); Serial.print(soilMoisture); Serial.println(" %");
}

void getValues()
{
  getBMEValues();
  getLightValue();
  getSoilMoisture();
  getBatteryVoltage();
}
