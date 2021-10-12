/*
 * File: ESP-NOW Gateway WiFi.ino
 * 
 */
#include <WiFi.h>
#include <PubSubClient.h>
#include <FastLED.h>
#include "arduino_secrets.h"

/* create an instance of PubSubClient client */
WiFiClient espClient;
PubSubClient client(espClient);

#define BAUDRATE 9600
#define GATEWAY_RXD2 14
#define GATEWAY_TXD2 15

#define ESP_LED_PIN 32
#define LED_COUNT   2
CRGB leds[LED_COUNT];

char msg[20];
char cstr[5];

//Structure example to send data
//Must match the receiver structure
typedef struct EspNowMessage 
{
    uint8_t  id;
    uint16_t battery;
    float    sht31Temp;
    uint8_t  sht31Humi;
    float    bmp280Temp;
    uint16_t bmp280Pres;
} EspNowMessage;

struct EspNowMessage *message = (EspNowMessage *) malloc(sizeof *message);

void showStatus(CRGB led1, CRGB led2) 
{
    FastLED.clear();
    leds[0] = led1;
    leds[1] = led2;
    FastLED.show();
}

void setup()
{
  Serial.begin(115200);
  Serial.println();
  Serial.println();
  FastLED.addLeds<WS2812B, ESP_LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(100);
  Serial2.begin(BAUDRATE, SERIAL_8N1, GATEWAY_RXD2, GATEWAY_TXD2);
  showStatus(CRGB::White, CRGB::White);
  connectWiFi();
  configureMQTT();
}

void loop()
{
  // Receiving message. Stop 
  while (Serial2.available()) 
  {
    FastLED.setBrightness(0);
    FastLED.show();
    Serial2.readBytes((char*) message, sizeof(EspNowMessage));
    Serial.println("\nMessage received from Node #" + String(message->id));
    Serial.printf("Battery voltage =   %d mV\n", message->battery);
    Serial.printf("sht31Temperature =  %3.1f °C\n", message->sht31Temp);
    Serial.printf("sht31Humidity =     %d %rH\n", message->sht31Humi);
    Serial.printf("bmp280Temperature = %3.1f °C\n", message->bmp280Temp);
    Serial.printf("bmp280Pressure =    %d hPa\n", message->bmp280Pres);
    
    /* if client was disconnected then try to reconnect again */
    if (!client.connected()) mqttconnect();

    // Prepare & publish messages
    Serial.println("\nPrepare MQTT messages & send...");
    String t = "ESPNOW/" + String(message->id) + "/battery";
    char topic[t.length() + 1]; strcpy(topic, t.c_str());
    snprintf(msg, 20, "%d", message->battery);
    Serial.print(topic); Serial.print(" : "); Serial.println(msg);
    client.publish(topic, msg);
    
    t = "ESPNOW/" + String(message->id) + "/sht31Temp";
    topic[t.length() + 1]; strcpy(topic, t.c_str());
    snprintf(msg, 20, "%3.1f", message->sht31Temp);
    Serial.print(topic); Serial.print(" : "); Serial.println(msg);
    client.publish(topic, msg);

    t = "ESPNOW/" + String(message->id) + "/sht31Humi";
    topic[t.length() + 1]; strcpy(topic, t.c_str());
    snprintf(msg, 20, "%d", message->sht31Humi);
    Serial.print(topic); Serial.print(" : "); Serial.println(msg);
    client.publish(topic, msg);

     t = "ESPNOW/" + String(message->id) + "/bmp280Temp";
    topic[t.length() + 1]; strcpy(topic, t.c_str());
    snprintf(msg, 20, "%3.1f", message->bmp280Temp);
    Serial.print(topic); Serial.print(" : "); Serial.println(msg);
    client.publish(topic, msg);

    t = "ESPNOW/" + String(message->id) + "/bmp280Pres";
    topic[t.length() + 1]; strcpy(topic, t.c_str());
    snprintf(msg, 20, "%d", message->bmp280Pres);
    Serial.print(topic); Serial.print(" : "); Serial.println(msg);
    client.publish(topic, msg);
    Serial.println("MQTT Messages published.");
    FastLED.setBrightness(100);
    FastLED.show();
  }
}
