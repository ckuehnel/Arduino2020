/* 
 * File: M0_BMP28x.ino 
 * 
 * Shows how to use the BMP180MI library with the sensor connected using I2C.
 * https://bitbucket.org/christandlg/bmx280mi/src/master/
 * Copyright (c) 2018 Gregor Christandl
 * 
 * connect the BMx280 to the Arduino like this:
 *     Arduino - BMP180
 *     3.3 V ----- VCC  for GY-BMP280 and GY-BME280
 *       GND ----- GND
 *       SDA ----- SDA
 *       SCL ----- SCL
*/

#include <Wire.h>
#include <BMx280TwoWire.h>

#define Serial SerialUSB  // for Arduino M0

#define I2C_ADDRESS 0x76

//create a BMx280I2C object using the I2C interface with I2C Address 0x76
BMx280TwoWire bmx280(&Wire, I2C_ADDRESS);

void setup() 
{
  // put your setup code here, to run once:
  Serial.begin(115200);

  //wait for serial connection to open (only necessary on some boards)
  while (!Serial);

  Wire.begin();

  //begin() checks the Interface, reads the sensor ID (to differentiate between BMP280 and BME280)
  //and reads compensation parameters.
  if (!bmx280.begin())
  {
    Serial.println("begin() failed. check your BMx280 Interface and I2C Address.");
    while (1);
  }
  else Serial.println("BMx280 Breakout Board connected.");

  if (bmx280.isBME280())
    Serial.println("Sensor is a BME280");
  else
    Serial.println("Sensor is a BMP280");

  //reset sensor to default parameters.
  bmx280.resetToDefaults();

  //by default sensing is disabled and must be enabled by setting a non-zero
  //oversampling setting.
  //set an oversampling setting for pressure and temperature measurements. 
  bmx280.writeOversamplingPressure(BMx280MI::OSRS_P_x16);
  bmx280.writeOversamplingTemperature(BMx280MI::OSRS_T_x16);

  //if sensor is a BME280, set an oversampling setting for humidity measurements.
  if (bmx280.isBME280())
    bmx280.writeOversamplingHumidity(BMx280MI::OSRS_H_x16);
}

void loop() 
{
  delay(1000);

  //start a measurement
  if (!bmx280.measure())
  {
    Serial.println("could not start measurement, is a measurement already running?");
    return;
  }

  //wait for the measurement to finish
  do
  {
    delay(100);
  } while (!bmx280.hasValue());

  Serial.print("\nPressure: "); Serial.print((int)bmx280.getPressure()); Serial.println(" Pa");
  Serial.print("Pressure (64 bit): "); Serial.println(bmx280.getPressure64()); Serial.println(" Pa");
  Serial.print("Temperature: "); Serial.print(bmx280.getTemperature()); Serial.println(" *C");

  if (bmx280.isBME280())
  {
    Serial.print("Humidity: "); 
    Serial.print((int) bmx280.getHumidity()); Serial.println(" % RH");
  }
}
