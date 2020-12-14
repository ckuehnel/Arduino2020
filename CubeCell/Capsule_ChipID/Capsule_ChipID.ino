#include "Arduino.h"

// ChipID: 53A0FD032715

void setup() 
{

  Serial.begin(115200);
  delay(2000);    // wait for serial monitor
  
  Serial.println("\nRead ChipID from Heltec Capsule...");
  
  uint64_t chipID=getID();
  Serial.printf("ChipID: %04X%08X\r\n",(uint32_t)(chipID>>32),(uint32_t)chipID);
}

void loop() {}
