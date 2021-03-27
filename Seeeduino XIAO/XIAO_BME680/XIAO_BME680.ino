/*
 * File: XIAO_BME680.ino
 * 
 * BME680 test program on Seeeduino XIAO using 
 * Bosch Sensortec Environmental Cluster (BSEC) Software v1.4.7.4 released on July 3rd, 2019
 * The BSEC fusion library has been conceptualized to provide a higher-level signal processing and fusion for the BME680. 
 * The library receives compensated sensor values from the sensor API. 
 * It processes the BME680 signals to provide the requested sensor outputs.
 * 
 * 2021-02-12 Claus Kühnel info@ckuehnel.ch
 */
#include <Arduino.h>
#include <U8g2lib.h>
#include <Wire.h>
#include "bsec.h"

// Helper functions declarations
void checkIaqSensorStatus(void);
void errLeds(void);

U8G2_SSD1306_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* clock=*/ SCL, /* data=*/ SDA, /* reset=*/ U8X8_PIN_NONE);  // High speed I2C

// Create an object of the class Bsec
Bsec iaqSensor;

String output;

// Entry point for the example
void setup(void)
{
  Serial.begin(115200);
  //while(!Serial);
  delay(2000);
  Serial.println("\nAir Quality Monitoring w/ BME680");
  Wire.begin();
  u8g2.begin();
  u8g2.clearBuffer();                   // clear the internal memory
  u8g2.setFont(u8g2_font_7x13_tf); // choose a suitable font
  u8g2.drawStr(0,10,"BME680 Monitor");    // write something to the internal memory
  u8g2.sendBuffer();                    // transfer internal memory to the display
  delay(2000);

  //iaqSensor.begin(BME680_I2C_ADDR_PRIMARY, Wire);
  iaqSensor.begin(BME680_I2C_ADDR_SECONDARY, Wire); // SDO = CS = HIGH
  
  output = "BSEC library version " + String(iaqSensor.version.major) + "." + String(iaqSensor.version.minor) + "." + String(iaqSensor.version.major_bugfix) + "." + String(iaqSensor.version.minor_bugfix);
  Serial.println(output);
  checkIaqSensorStatus();

  bsec_virtual_sensor_t sensorList[10] = {
    BSEC_OUTPUT_RAW_TEMPERATURE,
    BSEC_OUTPUT_RAW_PRESSURE,
    BSEC_OUTPUT_RAW_HUMIDITY,
    BSEC_OUTPUT_RAW_GAS,
    BSEC_OUTPUT_IAQ,
    BSEC_OUTPUT_STATIC_IAQ,
    BSEC_OUTPUT_CO2_EQUIVALENT,
    BSEC_OUTPUT_BREATH_VOC_EQUIVALENT,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_TEMPERATURE,
    BSEC_OUTPUT_SENSOR_HEAT_COMPENSATED_HUMIDITY,
  };

  iaqSensor.updateSubscription(sensorList, 10, BSEC_SAMPLE_RATE_LP);
  checkIaqSensorStatus();

  u8g2.clearBuffer();                   // clear the internal memory
  u8g2.sendBuffer();                    // transfer internal memory to the display
}

// Function that is looped forever
void loop(void)
{
  //unsigned long time_trigger = millis();
  if (iaqSensor.run()) // If new data is available
  { 
    output = "\nRaw temperature [°C]      = " + String(iaqSensor.rawTemperature); Serial.println(output);
    output = "Pressure [hPa]            = " + String(iaqSensor.pressure); Serial.println(output);
    output = "Raw relative Humidity [%] = " + String(iaqSensor.rawHumidity); Serial.println(output);
    output = "Gas resistance [Ohm]      = " + String(iaqSensor.gasResistance); Serial.println(output);
    output = "iAQ                       = " + String(iaqSensor.iaq); Serial.println(output);
    output = "iAQ Accuracy              = " + String(iaqSensor.iaqAccuracy); Serial.println(output);
    output = "Temperature [°C]          = " + String(iaqSensor.temperature); Serial.println(output);
    output = "Relative Humidity [%]     = " + String(iaqSensor.humidity); Serial.println(output);
    output = "Static iAQ                = " + String(iaqSensor.staticIaq); Serial.println(output);
    output = "eCO2                      = " + String(iaqSensor.co2Equivalent); Serial.println(output);
    output = "bVOC                      = " + String(iaqSensor.breathVocEquivalent); Serial.println(output);

    u8g2.clearBuffer();                   // clear the internal memory
    u8g2.sendBuffer();                    // transfer internal memory to the display
    output = "Temp [*C] = " + String(iaqSensor.temperature,1);
    u8g2.setCursor(0,10); u8g2.print(output);
    output = "rHum  [%] = " + String(iaqSensor.humidity,1);
    u8g2.setCursor(0,20); u8g2.print(output);
    if (iaqSensor.iaqAccuracy >= 1)
    {
      output = "   iAQ    = " + String(iaqSensor.staticIaq,0);
      u8g2.setCursor(0,35); u8g2.print(output);
      output = "eCO2[ppm] = " + String(iaqSensor.co2Equivalent,0);
      u8g2.setCursor(0,50); u8g2.print(output);
      output = "bVOC[ppb] = " + String(iaqSensor.breathVocEquivalent,0);
      u8g2.setCursor(0,60); u8g2.print(output);
    }
    else
    { 
      output = "Wait for iAQ valid";
      u8g2.setCursor(0,50); u8g2.print(output);
    }
    u8g2.sendBuffer();                    // transfer internal memory to the display
  } 
  else checkIaqSensorStatus();
}

// Helper function definitions
void checkIaqSensorStatus(void)
{
  if (iaqSensor.status != BSEC_OK) {
    if (iaqSensor.status < BSEC_OK) {
      output = "BSEC error code : " + String(iaqSensor.status);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
      output = "BSEC warning code : " + String(iaqSensor.status);
      Serial.println(output);
    }
  }

  if (iaqSensor.bme680Status != BME680_OK) {
    if (iaqSensor.bme680Status < BME680_OK) {
      output = "BME680 error code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
      for (;;)
        errLeds(); /* Halt in case of failure */
    } else {
      output = "BME680 warning code : " + String(iaqSensor.bme680Status);
      Serial.println(output);
    }
  }
}

void errLeds(void)
{
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, HIGH);
  delay(100);
  digitalWrite(LED_BUILTIN, LOW);
  delay(100);
}
