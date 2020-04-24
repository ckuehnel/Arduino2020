/*
 * File: readDS2438.ino
 * 
 * Measuring temperature and voltage @ VAD.
 * The voltage @ VAD represents the relavie humidity measured by HIH-4031.
 * Based on: https://github.com/TaaraLabs/OneWireHumidityLight-Demo by Märt Maiste, TaaraLabs 2017
 */

void readDS2438()
{
  byte i;
  byte data[12];
  byte addr[8];

  ds.target_search(0x26); // family code for DS2438

  while (!ds.search(addr)) 
  {
    ds.reset_search();
    //Serial.println("No more sensors");
    delay(1000);
  }
  if (DEBUG)
  {
    for ( i = 0; i < 8; i++)  // we need 9 bytes, last one for CRC
    {
      Serial.print(addr[i], HEX); Serial.print(":");
    }
    Serial.print(" ");
  }

  // Configure chip to read VAD and not VDD
  ds.reset();
  ds.select(addr);
  ds.write(0x4E);
  ds.write(0x00);
  ds.write(0x00);

  // Convert voltage
  ds.reset();
  ds.select(addr);
  ds.write(0xB4);
  delay(20);

  // Start temperature conversion
  ds.reset();
  ds.select(addr);
  ds.write(0x44);
  delay(20);
  
  // Current readings are updated automatically
  
  // Recall
  ds.reset();
  ds.select(addr);
  ds.write(0xB8);
  ds.write(0x00);

  // Read Scratchpad
  ds.reset();
  ds.select(addr);
  ds.write(0xBE);
  ds.write(0x00);
  
  for ( i = 0; i < 9; i++) { // we need 9 bytes, last one for CRC
    data[i] = ds.read();
    // Serial.print(data[i], HEX);
    // Serial.print(" ");
  }

  // CRC
  if ( OneWire::crc8(data, 8) != data[8]) {
    Serial.println("CRC is not valid!");
    return;
  }
  
  Serial.print("DS2438 Temp = ");
  float temp = (((((int16_t) data[2]) << 8) | data[1]) >> 3) * 0.03125;
  Serial.print(temp); Serial.print(" *C ");

  Serial.print(" VAD = ");
  float vad = ((((int16_t)data[4]) << 8) | data[3]) * 0.01;
  Serial.print(vad); Serial.print(" V ");

  /*Serial.print(" IAD=");
  float iad = ((((int16_t)data[6]) << 8) | data[5] << 2) * 0.2441;
  Serial.print(iad);*/

  // Configure chip to read VDD and not VAD
  ds.reset();
  ds.select(addr);
  ds.write(0x4E);
  ds.write(0x00);
  ds.write(0x0F);

  // Convert voltage
  ds.reset();
  ds.select(addr);
  ds.write(0xB4);
  delay(20);

  // Recall
  ds.reset();
  ds.select(addr);
  ds.write(0xB8);
  ds.write(0x00);

  // Read Scratchpad
  ds.reset();
  ds.select(addr);
  ds.write(0xBE);
  ds.write(0x00);
  
  Serial.print(" ");
    for ( i = 0; i < 9; i++) { // we need 9 bytes, last one for CRC
    data[i] = ds.read();
  }

  // CRC
  if ( OneWire::crc8(data, 8) != data[8]) {
    Serial.println("CRC is not valid!");
    return;
  }
  
  Serial.print(" VDD = ");
  float vdd = ((((int16_t)data[4]) << 8) | data[3]) * 0.01;
  Serial.print(vdd); Serial.print(" V ");

  Serial.print(" Hum = ");
  float stdHum = ((vad / vdd) - 0.16) / 0.0062;
  float trueHum = stdHum / (1.0546 - (0.00216 * temp));
  Serial.print(trueHum); Serial.print(" %RH");
  
  Serial.println();
}
