/*
  MKR1000_PushOver_Status.ino
  based on
  https://create.arduino.cc/projecthub/ckuehnel/mkr1000-pushover-status-5eac37
  created 2018-07-05 Claus Kuehnel (info@ckuehnel.ch)
*/
#include <SPI.h>
#include <WiFi101.h>
#include <RTCZero.h>
#include "arduino_secrets.h"

char* ssid     = SECRET_SSID;
char* password = SECRET_PASS;

int length;

WiFiClient client;

/* Create an rtc object */
RTCZero rtc;

/* Change these values to define the wakeup periode */
byte wakeupSeconds = 0;
byte wakeupMinutes = 10;
byte wakeupHours = 0;

/* Change these values to set the current initial time */
const byte seconds = 0;
const byte minutes = 0;
const byte hours = 0;

/* Change these values to set the current initial date */
const byte day = 1;
const byte month = 1;
const byte year = 2018;

bool matched = true;

void setup()
{
  Serial.begin(9600);
  while(!Serial);
  Serial.println("RTC driven Pushover message...");
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);

  rtc.begin();

  rtc.setTime(hours, minutes, seconds);
  rtc.setDate(day, month, year);

  rtc.setAlarmTime(hours + wakeupHours, minutes + wakeupMinutes, seconds + wakeupSeconds);
  rtc.enableAlarm(rtc.MATCH_HHMMSS);
  rtc.attachInterrupt(alarmMatch);
}

void loop() 
{
  //---------------------------------------
  // Include appplication code here

  //---------------------------------------
  if (matched) // send Pushover message after alarm raised
  {
    matched = false;
    Serial.println("Send message...");
    send_message();
    Serial.println("done.");
    

    rtc.setTime(hours, minutes, seconds);
    rtc.setAlarmTime(hours + wakeupHours, minutes + wakeupMinutes, seconds + wakeupSeconds);
  }
}

void alarmMatch() 
{
  Serial.println("Alarm raised.");
  matched = true;
}
