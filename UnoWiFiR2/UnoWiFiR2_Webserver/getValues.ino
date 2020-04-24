/*
 * Get measuring vaule from SHT31 sensor
 */
void getValues()
{
  temp = sht31.getTemperature();
  hum = sht31.getHumidity();

  if (DEBUG)
  {
    Serial.print("Temp = "); 
    Serial.print(temp, 1);
    Serial.println(" *C"); 
    Serial.print("Hum = "); 
    Serial.print(hum, 0);
    Serial.println(" %RH"); 
    Serial.println();
  }
}

 
