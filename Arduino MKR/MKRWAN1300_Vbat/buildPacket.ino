/*
 * Get Vbat & SHT31 sensor data & build LoRaWAN data package
 */

const float KF = 2.355646355; // Correction factor for volatage divider

void buildPacket() 
{
  if (DEBUG) Serial.println("Get VBAT & SHT31 sensor data and build package");

  float vbat = KF * analogRead(ADC_BATTERY) * 3300./4096;
  float temp = sht31.getTemperature();
  float hum  = sht31.getHumidity();

  if (DEBUG)
  {
    Serial.print("VBat = "); Serial.print(vbat,1); Serial.println(" mV");
    Serial.print("Temp = "); Serial.print(temp,2); Serial.println(" *C"); 
    Serial.print("Hum  = "); Serial.print(hum,1); Serial.println(" %RH"); 
  }

  uint16_t _vbat = (uint16_t) (vbat + 0.5);
  uint8_t vbat_lo = lowByte(_vbat); 
  uint8_t vbat_hi = highByte(_vbat);

  int16_t tmp = (int16_t)((temp * 100) + 0.5); // tmp = temp * 100
  uint8_t temp_lo = lowByte(tmp); 
  uint8_t temp_hi = highByte(tmp);
   
  uint16_t _hum = (uint16_t)((hum * 10) + 0.5);   // _hum = hum * 10
  uint8_t hum_lo = lowByte(_hum);
  uint8_t hum_hi = highByte(_hum);
  
  // Tx Package
  txBuffer[0] = vbat_hi;
  txBuffer[1] = vbat_lo;
  txBuffer[2] = temp_hi;
  txBuffer[3] = temp_lo;
}
