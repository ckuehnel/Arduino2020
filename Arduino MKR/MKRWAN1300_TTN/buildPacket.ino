/*
 * Get SHT31 sensor data & build LoRaWAN data package
 */

void buildPacket() 
{
  if (DEBUG) 
  {
    Serial.println("---------------------------------------");
    Serial.println("Get SHT31 sensor data and build package");
  }
  float temp = sht31.getTemperature();
  float hum  = sht31.getHumidity();

  if (DEBUG)
  {
    Serial.print("Temp = "); Serial.print(temp,2); Serial.println(" *C"); 
    Serial.print("Hum  = "); Serial.print(hum,1); Serial.println(" %RH"); 
  }

  int16_t tmp = (int16_t)((temp * 100) + 0.5); // tmp = temp * 100
  uint8_t temp_lo = lowByte(tmp); 
  uint8_t temp_hi = highByte(tmp);
   
  uint16_t _hum = (uint16_t)((hum * 10) + 0.5);   // _hum = hum * 10
  uint8_t hum_lo = lowByte(_hum);
  uint8_t hum_hi = highByte(_hum);
  
  // Tx Package
  txBuffer[0] = hum_lo;
  txBuffer[1] = hum_hi;
  txBuffer[2] = temp_lo;
  txBuffer[3] = temp_hi;
}
