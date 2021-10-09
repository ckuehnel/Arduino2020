/*
 * File: ESP8266_AZ-Envy_RawData.ino
 * 
 * An AZ-Envy board is used to measure temperature and humidity by SHT30 and several gases by MQ-2.
 * The program sends the measured values to the console.
 * 
 * Claus Kühnel 2021-09-16 info@ckuehnel.ch
 */
#include <Wire.h>
#include <SHTSensor.h> // https://github.com/Sensirion/arduino-sht

SHTSensor sht;

const int pinMQ2 = A0;   //ADC-pin of AZ-Envy for the gas sensor

// Kalibrierwerte AZ-Envy Tempertur
const float m = 1.0;
const float n = 0;

void setup()
{
  Wire.begin();
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
    Serial.println(humidity, 1);
    Serial.print(F("  T:\t"));
    Serial.println(temperature_calibrated, 2);
  } else Serial.println(F("Error in readSample()"));

  int MQ2Value = analogRead(pinMQ2);  //read the ADC-pin → connected to MQ-2

  Serial.print(F("MQ-2:\n")); 
  Serial.print(F("  ADC:\t"));
  Serial.println(MQ2Value); 
  Serial.println(F("----------------------------------------------")); 
 
  digitalWrite(LED_BUILTIN, HIGH);    //turn the integrated led on
  delay(5000);
  digitalWrite(LED_BUILTIN, LOW);     //turn the integrated led off  
}
 
