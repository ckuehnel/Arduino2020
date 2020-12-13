/*
 * File: MKRWAN1300_Vbat.ino
 * 
 * Measuring battery voltage and temperatur and send it to TTN.
 * 
 * based on https://github.com/gonzalocasas/arduino-mkr-wan-1300
 * Claus Kühnel 2020-12-012 info@ckuehnel.ch
 * 
 */
#include <MKRWAN.h>
#include <RTCZero.h>
#include <pins_arduino.h>
#include "arduino_secrets.h" 

#include <Wire.h>
#include "SHT31.h"

SHT31 sht31 = SHT31();

// LoRaWAN
// Select your region (AS923, AU915, EU868, KR920, IN865, US915, US915_HYBRID)
_lora_band region = EU868;
LoRaModem modem(Serial1);

#define DEBUG 1

uint8_t txBuffer[4];

// Flashy stuff
#define VERY_FAST 50
#define FAST 200
#define SLOW 500
#define FOREVER -1

#define ADC_RESOLUTION_BITS 12

/* Create an rtc object */
RTCZero rtc;

/* Change these values to define the wakeup periode */
byte wakeupSeconds = 0;
byte wakeupMinutes = 30;
byte wakeupHours = 0;

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 0;
const byte hours = 0;

/* Change these values to set the current initial date */
const byte day = 1;
const byte month = 1;
const byte year = 2020;

bool matched = true;

void flash(int times, unsigned int speed) 
{
  while (times == -1 || times--) 
  {
    digitalWrite(LED_BUILTIN, HIGH);
    delay(speed);
    digitalWrite(LED_BUILTIN, LOW);
    delay(speed);
  }
}

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  analogReadResolution(ADC_RESOLUTION_BITS);
  
  Serial.begin(115200);
  sht31.begin();

  flash(10, VERY_FAST);

  // Wait a maximum of 5s for Serial Monitor serial
  while (!Serial && millis() < 5000);
  Serial.println(F("\nStarting up..."));
  flash(2, SLOW);
  
  // Begin LoRa modem
  if (!modem.begin(region)) 
  {
    Serial.println(F("Failed to start module"));
    flash(FOREVER, VERY_FAST);
  };

  Serial.print(F("Your device EUI is: "));
  Serial.println(modem.deviceEUI());
  flash(2, SLOW);

  int connected = modem.joinOTAA(appEui, appKey);
  if (!connected) {
    Serial.println(F("Something went wrong; are you indoor? Move near a window and retry"));
    flash(FOREVER, VERY_FAST);
  }
  Serial.println(F("Successfully joined the network!"));

  Serial.println(F("Enabling ADR and setting low spreading factor"));
  modem.setADR(true);
  modem.dataRate(5);

  flash(2, SLOW);

  initRTC();
}

void loop() 
{
  if (matched) // send message to TTN after wake up from deep sleep
  {
    matched = false;
    
    Serial.begin(115200);
    if (DEBUG) Serial.println("\nSend message...");
    sendValues();
    if (DEBUG) Serial.println("Message sent.");
    
    rtc.setTime(hours, minutes, seconds);
    rtc.setAlarmTime(hours + wakeupHours, minutes + wakeupMinutes, seconds + wakeupSeconds);
    rtc.standbyMode();    // Sleep until next alarm match
  }
}

void sendValues() 
{
  digitalWrite(LED_BUILTIN, HIGH);
  buildPacket();
  modem.beginPacket();
  modem.write(txBuffer, sizeof(txBuffer));
  int err = modem.endPacket(false);
  if (err > 0) 
  {
    Serial.println("Upload successful");
    flash(3, FAST);
  } else 
  {
    Serial.println("Upload Error");
    flash(10, VERY_FAST);
  }
  if (DEBUG) Serial.println("TX done");

  delay(3000);
  digitalWrite(LED_BUILTIN, LOW);
}
