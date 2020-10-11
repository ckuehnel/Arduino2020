/*
 * File: ESP32_ReadEnvData.ino
 * 
 * Reads temperature, humidity and presure from BME280 sensor and
 * the concentration of carbon dioxide and TVOC from CCS811 sensor.
 * Both sensors are installed on a breakout board manufactured by DFRobot.
 * 
 * Two libraries from DFRobots were used (see below). 
 * This program is based on program samples in both libraries.
 * 
 * copyright	Copyright (c) 2010 DFRobot Co.Ltd (http://www.dfrobot.com)
 * licence     The MIT License (MIT)
 * author [LuoYufeng](yufeng.luo@dfrobot.com)
 * version  V0.1
 * date  2019-07-19
 * get from https://www.dfrobot.com
 * 
 * adaptions 2020-10-11 Claus Kühnel info@ckuehnel.ch
 */
#include "DFRobot_CCS811.h"
#include "DFRobot_BME280.h"
//#include "Wire.h"

// I2C Address
// BME280   0x76 (default) | 0x77
// CCS811   0x5A (default) | 0x5B

typedef DFRobot_BME280_IIC    BME;    // ******** use abbreviations instead of full names ********

BME   bme(&Wire, 0x76);   // select TwoWire peripheral and set sensor address - 0x76 default

//DFRobot_CCS811 sensor(&Wire, /*IIC_ADDRESS=*/0x5A);
DFRobot_CCS811 sensor;

#define SEA_LEVEL_PRESSURE    1019.0f  // https://kachelmannwetter.com/ch/messwerte/zuerich-stadt/luftdruck-stationshoehe/20201011-1400z.html

// show last sensor operate status
void printLastOperateStatus(BME::eStatus_t eStatus)
{
  switch(eStatus) 
  {
    case BME::eStatusOK:    Serial.println("everything ok"); break;
    case BME::eStatusErr:   Serial.println("unknow error"); break;
    case BME::eStatusErrDeviceNotDetected:    Serial.println("device not detected"); break;
    case BME::eStatusErrParameter:    Serial.println("parameter error"); break;
    default: Serial.println("unknow status"); break;
  }
}


void setup(void)
{
  Serial.begin(115200);
  /*Wait for the CCS811 chip to be initialized completely, and then exit*/
  while(sensor.begin())
  {
    Serial.println("failed to init chip, please check if the chip connection is fine");
    delay(1000);
  }
  /*
   * @brief Set measurement cycle
   * @param cycle:in typedef enum{
   *                  eClosed,      //Idle (Measurements are disabled in this mode)
   *                  eCycle_1s,    //Constant power mode, IAQ measurement every second
   *                  eCycle_10s,   //Pulse heating mode IAQ measurement every 10 seconds
   *                  eCycle_60s,   //Low power pulse heating mode IAQ measurement every 60 seconds
   *                  eCycle_250ms  //Constant power mode, sensor measurement every 250ms
   *                  }eCycle_t;
   */
  sensor.setMeasCycle(sensor.eCycle_1s);

  bme.reset();
  while(bme.begin() != BME::eStatusOK) 
  {
    Serial.println("bme begin faild");
    printLastOperateStatus(bme.lastOperateStatus);
    delay(2000);
  }
  Serial.printf("\nRead CCS811 & BME280 to get Environmental Data\n");
  Serial.printf("using Environmental Modul by DFRobot and ESP32\n\n");
}

void loop() 
{
  float   temp = bme.getTemperature();
  uint32_t    press = bme.getPressure();
  float   alti = bme.calAltitude(SEA_LEVEL_PRESSURE, press);
  float   humi = bme.getHumidity();
    
  if(sensor.checkDataReady() == true)
  {
    uint16_t CO2 = sensor.getCO2PPM();
    uint16_t TVOC = sensor.getTVOCPPB(); 

    Serial.printf("----------------------------");
    Serial.printf("\nTemperature %5.1f °C\n", temp);
    Serial.printf("Humidity    %5.1f %%rH\n", humi);
    Serial.printf("Pressure    %5d hPa\n", press/100);
    Serial.printf("Altidude    %5.1f m over sea\n", alti);
    Serial.printf("\nCO2  %5d ppm\n", CO2);
    Serial.printf("TVOC %5d ppb\n", TVOC);
    Serial.printf("----------------------------\n\n");
      
  } else Serial.println("Data is not ready!");
  sensor.writeBaseLine(0xD47B);
  
  //delay cannot be less than measurement cycle
  delay(5000);
}
