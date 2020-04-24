/*
 * File: MKRWAN1300_getEUI.ino
 * 
 * based on https://github.com/gonzalocasas/arduino-mkr-wan-1300
 * Claus Kühnel 2020-04-04 info@ckuehnel.ch
 */
#include <MKRWAN.h>

// Select your region (AS923, AU915, EU868, KR920, IN865, US915, US915_HYBRID)
_lora_band region = EU868;

LoRaModem modem(Serial1);

void setup() 
{
  Serial.begin(115200);
  while (!Serial);
  Serial.println("\nGet the EUI of your Arduino MKR WAN 1300 Board"); 
  if (!modem.begin(region)) 
  {
    Serial.println("Failed to start module");
    while (1) {}
  };
  Serial.print("Your device EUI is: ");
  Serial.println(modem.deviceEUI());
  Serial.println("Notify this EUI for integration into LoRaWAN server, like TTN");
}

void loop() {}
