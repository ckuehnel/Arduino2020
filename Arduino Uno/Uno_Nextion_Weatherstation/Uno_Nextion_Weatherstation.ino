/*
 * File: Uno_Nextion_Weatherstation.ino
 * 
 * This programs simulates a weatherstation and displays data
 * on Nextion HMI equipped w/ special firmware Weatherstation.tft.
 * You will find the firmware at 
 * https://github.com/ckuehnel/newArduino/blob/master/Arduino%20Uno/Uno_NextionHMI/Weatherstation.tft
 * 
 * Claus Kühnel 2020-03-12 info@ckuehnel.ch
 */
#include <SoftwareSerial.h>

// Nextion TX to pin 10 and RX to pin 11 of Arduino
SoftwareSerial HMISerial(10, 11);

#define DEBUG 1

void HMIprint(String str)
{
  HMISerial.print(str);
  HMISerial.write(0xFF);       // Each Nextion HMI command is closed by three bytes 0xFF
  HMISerial.write(0xFF);
  HMISerial.write(0xFF);
}
 
void setup() 
{
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  Serial.println("\nWeatherstation w/ NEXTION HMI");
  HMISerial.begin(9600);  
}

void loop() 
{
  char msg[20];
  
  float temp = getTemperature();
  float humi = getHumidity();

  Serial.print(temp); Serial.print(" °C\t");
  Serial.print(humi); Serial.print(" % RH\n");

  dtostrf(10*temp, 3, 0, msg);  //3 is mininum width, 1 is precision
  String toHMI = "temp.val="; 
  toHMI += msg; 
  if (DEBUG) Serial.println(toHMI);
  HMIprint(toHMI);

  dtostrf(humi, 2, 0, msg);  //2 is mininum width, 0 is precision
  toHMI = "humi.val="; 
  toHMI += msg; 
  if (DEBUG) Serial.println(toHMI); 
  HMIprint(toHMI);

  delay(1000);

}
