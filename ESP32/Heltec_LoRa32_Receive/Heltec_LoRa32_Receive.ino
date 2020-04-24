/*
 * File: Heltec_LoRa32_Receive.ino
 * 
 * based on Library examples
 * Claus Kühnel 2020-04-06 info@ckuehnel.ch
 */
#include <SPI.h>
#include <LoRa.h>
#include "SSD1306.h"
#include <Arduino.h>

SSD1306  display(0x3c, 4, 15);

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
  Serial.println("DHT11 - LoRa Receiver");
  display.init();
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10);
  display.setTextAlignment(TEXT_ALIGN_LEFT);
  
  display.drawString(0, 0,"DHT11 LoRa Receiver");
  display.display();
  
  SPI.begin(5,19,27,18);
  LoRa.setPins(SS,RST,DI0);
  Serial.println("LoRa Receiver");
  if (!LoRa.begin(BAND)) 
  {
    Serial.println("Starting LoRa failed!");
    while (1);
  }
  Serial.println("LoRa Initial OK!");
  display.drawString(0,15,"LoRa Initializing OK!");
  display.display();
  delay(2000);
}

String received;

void loop() 
{
  // try to parse packet
  int packetSize = LoRa.parsePacket();
  if (packetSize) 
  {
    digitalWrite(25, HIGH);
    // received a packet
    Serial.print("Received packet '");
    // read packet
    while (LoRa.available()) received = LoRa.readString();
    Serial.print(received);
    // print RSSI of packet
    Serial.print("' with RSSI ");
    Serial.println(LoRa.packetRssi());
    display.clear();
    display.drawString(0, 0, "DHT11 LoRa Receiver");
    display.drawString(0, 15, "Received packet:");
    display.drawString(0, 30, "(Temp,Hum) = (");
    display.drawString(70, 30, received);
    display.drawString(105, 30, ")");
    display.drawString(0, 45, "RSSI = ");
    display.drawString(45,45, (String) LoRa.packetRssi());  
    display.display();
  }
  digitalWrite(25, LOW);
}
