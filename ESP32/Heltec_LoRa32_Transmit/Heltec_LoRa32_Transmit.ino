/*
 * File: Heltec_LoRa32_Transmit.ino
 * 
 * based on Library examples
 * Claus Kühnel 2020-04-06 info@ckuehnel.ch
 */
#include <Arduino.h>
#include <SPI.h>
#include <LoRa.h>
#include "SSD1306.h"

SSD1306  display(0x3c, 4, 15);

#include "DHT.h"                
#define DHTPIN 21          // Hier die Pin Nummer eintragen wo der Sensor angeschlossen ist
#define DHTTYPE DHT11 

DHT dht(DHTPIN, DHTTYPE);

#define SS      18
#define RST     14
#define DI0     26
#define BAND    868300000

#define DEBUG 1

void setup() 
{
  pinMode(25,OUTPUT); 
  pinMode(16,OUTPUT);
  digitalWrite(16, LOW);
  delay(50); 
  digitalWrite(16, HIGH);
  
  Serial.begin(115200);
  while (!Serial); 
  Serial.println("DHT11 LoRa Transmitter");
  dht.begin();
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  
  display.drawString(0, 0,"DHT11 LoRa Transmitter");
  display.display();
  
  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
  Serial.println("LoRa Transmitter");
  if (!LoRa.begin(BAND)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");
  display.drawString(0, 15,"LoRa Initializing OK!");
  display.display();
  delay(2000);
}

void loop() 
{
   digitalWrite(25, HIGH);  
  // Read DHT11
    float h = dht.readHumidity();    
    float t = dht.readTemperature();
    if (isnan(h) || isnan(t)) 
    {       
      Serial.println("DHT11 Read Error");
      return;
    }
    if (DEBUG)
    {
      Serial.print("Temperature: ");
      Serial.print(t,1); Serial.print(" *C\t");
      Serial.print("Humidity: ");
      Serial.print(h,0); Serial.print(" %RH\t");
    }
    display.clear();
    display.drawString(0, 0, "DHT11 LoRa Transmitter");
    display.drawString(0, 15, "Temperature: ");
    display.drawString(70, 15, String(t,1));
    display.drawString(90, 15, " *C");
    display.drawString(0, 30, "Humidity: ");
    display.drawString(70, 30, String(h,0));
    display.drawString(80, 30, " %RH");    
    display.display();
    Serial.print("TX: "); Serial.print(t, 1);
    Serial.print(","); Serial.print(h,0);
    Serial.println();

    LoRa.beginPacket();
    LoRa.print(t, 1);
    LoRa.print(",");
    LoRa.print(h,0);
    LoRa.print('\0');
    LoRa.endPacket();
    digitalWrite(25, LOW);
    delay(2000);  // change this to your measuring rate
}
