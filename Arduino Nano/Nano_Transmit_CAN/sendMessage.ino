/*
 * Build message and send it serially
 */
#define STX 2
#define ETX 3

unsigned char msg[8] = {0};
unsigned long id = 0x56;

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

  unsigned long id = random(0x50, 0x60);
  Serial.print("Send message with ID: "); Serial.println(id, HEX);
  msg[0] = STX; 
  msg[1] = _hum;
  msg[2] = temp_lo;
  msg[3] = temp_hi;
  msg[4] = ETX;
  can.send(id, 0, 0, sizeof(msg), msg);   // SEND W/ ID:0X5 - 0x60
  Serial.println("----------------------------"); 
}
