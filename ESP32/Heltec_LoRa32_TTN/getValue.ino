// This function read a DHT11 sensor

void getValue()
{
   if (DEBUG) 
  {
    Serial.println("---------------------------------------");
    Serial.println("Get DHT11 sensor data and build package");
  }
  float temperature = dht.readTemperature();
  float humidity = dht.readHumidity();

  if (DEBUG)
  {
    Serial.print("Measured temperature = ");
    Serial.print(temperature, 1); 
    Serial.println(" *C"); 
    Serial.print("Measured humidity = ");
    Serial.print(humidity, 0); 
    Serial.println(" % rH");   
  }

  char buffer[5];
  String s = dtostrf(temperature, 3, 1, buffer);
  display.clear();
  display.drawString(0, 0, "Temp = ");
  display.drawString(72, 0, s);
  display.drawString(104, 0, " °C");

  s = dtostrf(humidity, 3, 0, buffer);
  display.drawString(0, 16, "Hum  = ");
  display.drawString(72, 16, s);
  display.drawString(104, 16, " %");

  display.display();

  int16_t temp = (int) ((temperature *100) + .5);
  uint16_t  humi = (int) humidity * 10; 

  // Tx package
  mydata[0] = lowByte(humi);
  mydata[1] = highByte(humi);
  mydata[2] = lowByte(temp);
  mydata[3] = highByte(temp);
}
