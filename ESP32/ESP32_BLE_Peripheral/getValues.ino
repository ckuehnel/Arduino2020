/*
 * File getValues.ino
 * 
 * Get measuring value from SHT31 sensor and 
 * prepare a string for BLE output
 * 
 */
void getValues(void)
{
  float temp = sht31.getTemperature();
  float hum  = sht31.getHumidity(); 

  temp = ((temp * 1000) + 0.5)/1000.0;
  
  char buff[10];
  
  if (DEBUG)
  {
    Serial.println("--------------------------------");
    Serial.print("Temp = "); Serial.print(temp,3); Serial.println(" *C"); 
    Serial.print("Hum  = "); Serial.print(hum,1); Serial.println(" %RH"); 
    Serial.println();
  }
  // print as CSV data
  values = "";
  dtostrf(temp, 7, 3, buff);  //4 is mininum width, 6 is precision // -10.000
  values += buff; values += ", ";
  dtostrf(hum, 4, 1, buff);  //4 is mininum width, 6 is precision // 25.1
  values += buff;
  values += "\n";
  Serial.print("Send BLE message: "); Serial.print(values); 
}
