/*
 * File: get_ESP_MAC_Address.ino
 * 
 * Get MAC address of ESP32  or ESP8266 microcontroller board
 * 
 */

#ifdef ESP32
  #include <WiFi.h>
#else
  #include <ESP8266WiFi.h>
#endif

void setup()
{
  Serial.begin(115200);
  while(!Serial);
  Serial.print("\nESP Board MAC Address:  ");
  Serial.println(WiFi.macAddress());
}
 
void loop(){}
