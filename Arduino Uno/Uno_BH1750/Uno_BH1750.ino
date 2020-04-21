/*
 * File Uno_BH1750.ino
 * 
 * Reading light level from BH1750 light sensor usung GY-30 breakout board
 * based on the BH1750 library by
 * https://github.com/claws/BH1750
 * 
 * This example initialises the BH1750 object using the default high resolution
 * one shot mode and then makes a light level reading every five seconds.
 * 
 * After the measurement the MTreg value is changed according to the result:
 * lux > 40000 ==> MTreg =  32
 * lux < 40000 ==> MTreg =  69  (default)
 * lux <    10 ==> MTreg = 138
 * Remember to test your specific sensor! Maybe the MTreg value range from 32
 * up to 254 is not applicable to your unit.
 * 
 * Connection:
 *  BH1750 - Arduino
 *     VCC -> 5V
 *     GND -> GND
 *     SCL -> SCL
 *     SDA -> SDA
 *     ADD -> (not connected) or GND
 *     
 *     ADD pin is used to set sensor I2C address. 
 *     If HIGH address is 0x5C, if LOW its 0x23.
 */

#include <Wire.h>
#include <BH1750.h>

#define DEBUG 0  // Set to 0 for plotting data (Ctrl-Shift-L)

BH1750 lightMeter;

void setup()
{
  Serial.begin(115200);
  delay(2000);
  if (DEBUG) Serial.println(F("Reading BH1750 Light Sensor..."));
  Wire.begin();

  lightMeter.begin(BH1750::ONE_TIME_HIGH_RES_MODE);
  //lightMeter.setMTreg(69);  // not needed, only mentioning it
}

void loop() 
{
  //we use here the maxWait option due fail save
  float lux = lightMeter.readLightLevel(true);
  if (DEBUG) Serial.print(F("Light: "));
  Serial.print(lux);
  if (DEBUG) Serial.println(F(" lx"));

  if ((lux < 0) && (DEBUG)) Serial.println(F("Error condition detected"));
  else 
  {
    if (lux > 40000.0) 
    {
      // reduce measurement time - needed in direct sun light
      boolean flag = lightMeter.setMTreg(32);
      if (DEBUG)
      {
        if (flag) Serial.println(F("Setting MTReg to low value for high light environment"));
        else      Serial.println(F("Error setting MTReg to low value for high light environment"));
      }
    }
    else 
    {
      if (lux > 10.0) 
      {
        // typical light environment
        boolean flag = lightMeter.setMTreg(69);
        if (DEBUG)
        {
          if (flag) Serial.println(F("Setting MTReg to default value for normal light environment"));
          else      Serial.println(F("Error setting MTReg to default value for normal light environment"));
        }
      }
      else 
      {
        if (lux <= 10.0) 
        {
          //very low light environment
          boolean flag = lightMeter.setMTreg(138);
          if (DEBUG)
          { 
            if (flag) Serial.println(F("Setting MTReg to high value for low light environment"));
            else      Serial.println(F("Error setting MTReg to high value for low light environment"));
          }
        }
      }
    }
  }
  Serial.println();
  delay(5000);
}
