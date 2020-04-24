/*
 * File: UnoWiFiR2_DHT11_MQTT.ino
 * 
 * Measuring temperature and humidity by DHT11 sensor and sending data as
 * MQTT topics to CloudMQTT server.
 * 
 * createtd 2020-04-14 by Claus Kühnel (info@ckuehnel.ch) 
 */
#include <SPI.h>
#include <WiFiNINA.h>
#include <PubSubClient.h>
#include "DHT.h"
#include "arduino_secrets.h"

#define INFO 0
#define DEBUG 1

/* create an instance of PubSubClient client */
WiFiClient UnoWiFiClient;
PubSubClient client(UnoWiFiClient);

/* topics */
#define TEMP_TOPIC  "UnoWiFiR2/temp"
#define HUMI_TOPIC  "UnoWiFiR2/humi"

long lastMsg = 0;
char msg[20];
float temp = 0;
float hum  = 100;

/* define DHT pins */
#define DHTPIN 4
#define DHTTYPE DHT11

DHT dht(DHTPIN, DHTTYPE);

void setup() 
{
  Serial.begin(9600);
  delay(1000);
  if (INFO)  printInfo();
  if (DEBUG) Serial.println(F("\nInitializing...")); 
    
  connectWiFi();
  configureMQTT();
  initSensor();
  
  if (DEBUG) Serial.println(F("Running..."));     //last line in setup()
}

void loop() 
{
  /* if client was disconnected then try to reconnect again */
  if (!client.connected()) mqttconnect();
  /* this function will listen for incomming subscribed topic-process-invoke receivedCallback */
  client.loop();
  /* we measure temperature every 10 secs
  we count until 30 secs reached to avoid blocking program if using delay()*/
  long now = millis();
  if (now - lastMsg > 30000) 
  {
    lastMsg = now;
    /* read DHT11/DHT22 sensor and convert to string */
    temp = dht.readTemperature();
    hum  = dht.readHumidity(); 
    if (!isnan(temp)) 
    {
      dtostrf(temp, 3, 1, msg);
      /* publish the message */
      client.publish(TEMP_TOPIC, msg);
      if (DEBUG) Serial.print(msg);
    }
    if (!isnan(hum)) 
    {
      dtostrf(hum, 3, 0, msg);
      /* publish the message */
      client.publish(HUMI_TOPIC, msg); 
      if (DEBUG) Serial.println(msg);
    }
  }
}

/*--------------------------------------------------------------*/

void printInfo()
{
  // Compile file path (internal file of compilation proccess)
  Serial.print("File "); Serial.println(__FILE__);
  
  String s1 = __DATE__;
  String s2 = __TIME__;
  
  // Date of compilation
  Serial.print("Compilation @ "); Serial.println(s1 + " " + s2);
  
  // Arduino IDE SW version
  Serial.print("ARDUINO IDE v"); Serial.println(ARDUINO);
  
  // Oscillator frequency
  Serial.print("CPU Clock in MHz: "); Serial.println(F_CPU/1E6);
}
