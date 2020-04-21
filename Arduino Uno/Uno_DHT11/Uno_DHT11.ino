/*
   File: Uno_DHT11.ino - Testprogram for DHTxx, AM23xx
*/
 
#include "DHT.h"
 
// Uncomment whatever type you're using!
//#define DHTTYPE DHT11     // DHT 11
#define DHTTYPE DHT22     // DHT 22, AM2302, AM2321
//#define DHTTYPE DHT21     // DHT 21, AM2301

// DHTxx connection, do not forget PullUp between VCC and signal
#define DHTPIN 10 
 
DHT dht(DHTPIN, DHTTYPE);
 
void setup() 
{
  Serial.begin(115200);
  Serial.println("Testing DHTxx/AM23xx...");
  dht.begin();
}
 
void loop() 
{
  // Wait a few seconds between measurements.
  delay(2000);
 
  float h = dht.readHumidity();
  float t = dht.readTemperature();     // in C
  float f = dht.readTemperature(true); // in F
 
  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) 
  {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
 
  Serial.print("Humidity: "); Serial.print((int) h); Serial.print(" %\t");
  Serial.print("Temperature: "); Serial.print(t,1);  Serial.print(" *C; "); 
  // Serial.print(f); Serial.print(" *F");
  Serial.println();
}
