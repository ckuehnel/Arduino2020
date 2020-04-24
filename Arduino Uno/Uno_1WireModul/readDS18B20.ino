/*
 * File: readDS18B20.ino
 * 
 * Read all DS18B20 Temperatur Sensors connected to the 1-Wire bus
 * 
 */
void initDS18B20()
{
  sensors.begin();
  Serial.print("1-Wire Device Count:   "); Serial.println(sensors.getDeviceCount());
  DS18Count = sensors.getDS18Count(); 
  Serial.print("DS18B20 on 1-Wire bus: "); Serial.println(DS18Count);
  if (DS18Count == 0)
  {
    Serial.println("No DS18B20 connected!");
    while(true); 
  }
  Serial.print("Global Resolution:     "); Serial.println(sensors.getResolution());
}

void readDS18B20()
{
  if (DEBUG) Serial.print("Requesting temperatures...");
  sensors.requestTemperatures(); // Send the command to get temperatures
  if (DEBUG) Serial.println("Done");
  for (int i = 0; i < DS18Count; i++)
  {
    Serial.print("DS18B20["); Serial.print(i); Serial.print("]: "); 
  //  Serial.print("Temperature for the device 1 (index 0) is: ");
    Serial.print(sensors.getTempCByIndex(i)); Serial.println(" *C");
  }
}
