/* 
 * File: Nicla_EnvironmentalSensor.ino
 * 
 * Base: https://github.com/arduino-libraries/Arduino_BHY2/tree/main/examples/Standalone
 *  
 * This sketch shows how nicla can be used in standalone mode.
 * Without the need for an host, nicla can run sketches that 
 * are able to configure the bhi sensors and are able to read all 
 * the bhi sensors data.
 * This program queries the environmental sensors and controls the on-board 
 * RGB Led according to the value of eCO2.
 * 
 * 2021-11-10 Claus Kühnel info@ckuehnel.ch
 */
#include "Nicla_System.h"
#include "Arduino_BHY2.h"

// from SensorID.h
//  SENSOR_ID_TEMP                     = 128, /* Temperature */
//  SENSOR_ID_BARO                     = 129, /* Barometer */
//  SENSOR_ID_HUM                      = 130, /* Humidity */
//  SENSOR_ID_GAS                      = 131, /* Gas */
//  SENSOR_ID_BSEC_LEGACY              = 171, /* BSEC 1.x output (legacy, deprecated) */

Sensor temp(SENSOR_ID_TEMP);
Sensor humi(SENSOR_ID_HUM );
Sensor baro(SENSOR_ID_BARO);
Sensor gas(SENSOR_ID_GAS);

SensorBSEC bsec(SENSOR_ID_BSEC_LEGACY);

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  
  BHY2.begin();

  temp.begin();
  humi.begin();
  baro.begin();
  gas.begin();
  bsec.begin();

  nicla::begin();
  nicla::leds.begin();
}

void loop()
{
  static auto printTime = millis();

  // Update function should be continuously polled
  BHY2.update();

  if (millis() - printTime >= 5000) // Measuring cycle 5 sec
  {
    printTime = millis();
    nicla::leds.setColor(blue);
    delay(100);
    Serial.println("\n\nNicla Sense ME as Environmental Sensor\n");
    Serial.println(String("Temperature: ") + String(temp.value(),1) + String(" °C"));
    Serial.println(String("Humidity: ") + String(humi.value(),0) + String(" %rH"));
    Serial.println(String("Pressure: ") + String(baro.value(),0) + String(" hPA"));
    Serial.println(String("MOX Resistance: ") + String(gas.value(),0) + String(" Ohm"));
    Serial.println(String("iAQ: ") + String(bsec.iaq_s()));
    uint32_t eCO2 = bsec.co2_eq();
    Serial.println(String("eCO2: ") + String(eCO2) + String(" ppm"));
    Serial.println(String("bVOC: ") + String(bsec.b_voc_eq(),3) + String(" ppm"));
    Serial.println(String("BSEC: ") + bsec.toString());

    if (eCO2 < 800) nicla::leds.setColor(green);
    if (eCO2 >= 800 && eCO2 < 1200) nicla::leds.setColor(yellow);
    if (eCO2 >= 1200) nicla::leds.setColor(red);
  }
}
