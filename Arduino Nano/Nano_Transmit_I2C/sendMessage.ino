/*
 * Build message and send it serially
 */
#define STX 2
#define ETX 3
#define SlaveAddr 5

//<STX><byte><byte><byte><ETX>
void sendMessage(float temp, float hum)
{
  uint32_t tmp = (uint32_t)(temp * 1000);  
  uint8_t temp_lo = lowByte(tmp); 
  uint8_t temp_hi = highByte(tmp); 
  uint8_t _hum = (byte) hum;

  if (DEBUG)
  {
    // reconstruct float similar to receiver
    float temp = (float) (((temp_hi<<8) + temp_lo)/1000.0); 
    
    Serial.print("Temp = "); Serial.print(temp,3); Serial.println(" *C"); 
    Serial.print("Hum  = "); Serial.print(_hum); Serial.println(" %RH"); 
    Serial.println();
  }
  Wire.beginTransmission(SlaveAddr);
  Wire.write(STX); 
  Wire.write(_hum);
  Wire.write(temp_lo);
  Wire.write(temp_hi);
  Wire.write(ETX);
  Wire.endTransmission();
}
