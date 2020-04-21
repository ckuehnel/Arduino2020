/*
 * File: Uno_Weather.ino
 * 
 * Weather Shield Example for Sparkfun Weather Shield w/ HTU21D
 * based on Nathan Seidle's Weather_Shield_Basic_V12.ino using Si7201
 * https://github.com/sparkfun/Weather_Shield/blob/master/Firmware/Weather_Shield_Basic_V12/Weather_Shield_Basic_V12.ino
 * This example prints the current humidity, air pressure, temperature and light levels.
 * Modification: Claus Kuehnel 2020-02-05 info@ckuehnel.ch
 */

#include <Wire.h> //I2C needed for sensors
#include "SparkFunMPL3115A2.h" //Pressure sensor - Search "SparkFun MPL3115" and install from Library Manager
#include <SparkFunHTU21D.h> // for HTU21D

MPL3115A2 myPressure; //Create an instance of the pressure sensor
HTU21D myHumidity;   //Create an instance of the humidity sensor for HTU21D

//Hardware pin definitions
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
const byte STAT_BLUE = 7;

const byte REFERENCE_3V3 = A3;
const byte LIGHT = A1;
const byte BATT = A2;

//Global Variables
//-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
long lastSecond; //The millis counter to see when a second rolls by

void setup()
{
  Serial.begin(115200);
  Serial.println("Weather Shield Example");

  pinMode(STAT_BLUE, OUTPUT); //Status LED Blue

  pinMode(REFERENCE_3V3, INPUT);
  pinMode(LIGHT, INPUT);

  //Configure the pressure sensor
  myPressure.begin(); // Get sensor online
  myPressure.setModeBarometer(); // Measure pressure in Pascals from 20 to 110 kPa
  myPressure.setOversampleRate(7); // Set Oversample to the recommended 128
  myPressure.enableEventFlags(); // Enable all three pressure and temp event flags
  
  //Configure the humidity sensor
  myHumidity.begin();

  lastSecond = millis();

  Serial.println("Weather Shield in operation...\n");
}

void loop()
{
  //Print readings every second
  if (millis() - lastSecond >= 5000)
  {
    digitalWrite(STAT_BLUE, HIGH); //Blink stat LED

    lastSecond += 5000;

    //Check Humidity Sensor
    float humidity = myHumidity.readHumidity();

    if (humidity >= 10000) //Humidty sensor failed to respond
    {
      Serial.println("I2C communication to sensors is not working. Check solder connections.");

      //Try re-initializing the I2C comm and the sensors
      myPressure.begin();
      myPressure.setModeBarometer();
      myPressure.setOversampleRate(7);
      myPressure.enableEventFlags();
      myHumidity.begin();
    }
    else
    {
      Serial.print("Humidity sensor: ");
      Serial.print(humidity);
      Serial.print("%,");
      float temp_h = myHumidity.readTemperature();
      
      Serial.print("\ttemp_h = ");
      Serial.print(temp_h, 1);
      Serial.print(" *C\n");

      //Check Pressure Sensor
      float pressure = myPressure.readPressure();
      Serial.print("Pressure sensor: ");
      Serial.print(pressure, 1);
      Serial.print(" Pa");

      //Check temp from pressure sensor
      float temp_p = myPressure.readTemp();
      Serial.print("\ttemp_p = ");
      Serial.print(temp_p, 1);
      Serial.print(" *C\n");

      //Calculate temperature deviation
      Serial.print("Temperature deviation = ");
      Serial.print((temp_h - temp_p), 1);
      Serial.print(" K\n");

      //Check light sensor
      float light_lvl = get_light_level();
      Serial.print("Light_lvl = ");
      Serial.print(light_lvl);
      Serial.print(" V\n");

      //Check batt level
      float batt_lvl = get_battery_level();
      Serial.print("Voltage Vin = ");
      Serial.print(batt_lvl);
      Serial.print(" V\n\n");
    }
    digitalWrite(STAT_BLUE, LOW); //Turn off stat LED
  }
}

//Returns the voltage of the light sensor based on the 3.3V rail
//This allows us to ignore what VCC might be (an Arduino plugged into USB has VCC of 4.5 to 5.2V)
float get_light_level()
{
  float operatingVoltage = analogRead(REFERENCE_3V3);

  float lightSensor = analogRead(LIGHT);

  operatingVoltage = 3.3 / operatingVoltage; //The reference voltage is 3.3V

  lightSensor = operatingVoltage * lightSensor;

  return (lightSensor);
}

//Returns the voltage of the raw pin based on the 3.3V rail
//This allows us to ignore what VCC might be (an Arduino plugged into USB has VCC of 4.5 to 5.2V)
//Battery level is connected to the RAW pin on Arduino and is fed through two 5% resistors:
//3.9K on the high side (R1), and 1K on the low side (R2)
float get_battery_level()
{
  float operatingVoltage = analogRead(REFERENCE_3V3);

  float rawVoltage = analogRead(BATT);

  operatingVoltage = 3.30 / operatingVoltage; //The reference voltage is 3.3V

  rawVoltage = operatingVoltage * rawVoltage; //Convert the 0 to 1023 int to actual voltage on BATT pin

  rawVoltage *= 4.90; //(3.9k+1k)/1k - multiple BATT voltage by the voltage divider to get actual system voltage

  return (rawVoltage);
}
