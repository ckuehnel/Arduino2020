/*
 * File: ESP8266_AZ-Envy_Data.ino
 * 
 * An AZ-Envy board is used to measure temperature and humidity by SHT30 and several gases by MQ-2.
 * The program calculates gas equivalents and sends the measured values to the console.
 * 
 * Claus Kühnel 2021-09-16 info@ckuehnel.ch
 */
#include <Wire.h>
#include <SHTSensor.h> // https://github.com/Sensirion/arduino-sht
#include <MQ2.h>       // https://github.com/labay11/MQ-2-sensor-library

SHTSensor sht;

const int pinMQ2 = A0;   //ADC-pin of AZ-Envy for the gas sensor

MQ2 mq2(pinMQ2);

// Kalibrierwerte AZ-Envy Tempertur
const float m = 0.6699;
const float n = 4.6533;

// MQ-2 values
float lpg, co, smoke;

void setup()
{
  Wire.begin();
  mq2.begin();          // calibrate MQ-2
  Serial.begin(115200);
  //while(!Serial);
  delay(1000);          // wait for serial monitor

  Serial.println(F("AT-Envy Test"));

  if (sht.init()) Serial.print(F("init(): success\n"));
  else            Serial.print(F("init(): failed\n"));

  sht.setAccuracy(SHTSensor::SHT_ACCURACY_MEDIUM); // only supported by SHT3x

  pinMode(pinMQ2,INPUT); //set ADC-pin as a input
  pinMode(LED_BUILTIN, OUTPUT); //set the integrated led as a output
}

void loop()
{
  if (sht.readSample()) 
  {
    float humidity = sht.getHumidity();
    float temperature = sht.getTemperature();
    
    //calibrate your temperature values - due to heat reasons from the MQ-2 (normally up to 4°C)
    float temperature_calibrated = m * temperature + n; 
    
    Serial.println(F("SHT30:"));
    Serial.print(F("  RH:\t"));
    Serial.print(humidity, 1);
    Serial.println(F(" %rH"));
    Serial.print(F("  T:\t"));
    Serial.print(temperature_calibrated, 2);
    Serial.println(F(" *C"));
  } else Serial.println(F("Error in readSample()"));

  /*
   * read the values from the sensor, it returns
   * an array which contains 3 values.
   * 0 : LPG in ppm
   * 1 : CO in ppm
   * 2 : SMOKE in ppm
   */
  float* values= mq2.read(false); //set it false if you don't want to print the values to the Serial
  
  // lpg = values[0];
  lpg = mq2.readLPG();
  // co = values[1];
  co = mq2.readCO();
  // smoke = values[2];
  smoke = mq2.readSmoke();
  Serial.println(F("MQ-2:"));
  Serial.print(F("  LPG:\t"));
  Serial.print(lpg, 1);
  Serial.println(F(" ppm"));
  Serial.print(F("  CO:\t"));
  Serial.print(co, 1);
  Serial.println(F(" ppm"));
  Serial.print(F(" Smoke:\t"));
  Serial.print(smoke, 1);
  Serial.println(F(" ppm"));
  Serial.println(F("----------------------------------------------")); 
 
  digitalWrite(LED_BUILTIN, HIGH);    //turn the integrated led on
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);     //turn the integrated led off  
}
 
