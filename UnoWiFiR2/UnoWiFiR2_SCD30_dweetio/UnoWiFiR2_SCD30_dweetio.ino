/*
 * File UnoWiFiR2_SCD30_dweetio.ino
 * 
 *  Connecting Uno WiFi Rev2 to WiFi and POST data to Dweet.io
 *  based on https://github.com/openhomeautomation/esp8266-cloud
 *  createtd 2020-04-11 by Claus Kühnel (info@ckuehnel.ch) 
 */
#include <LiquidCrystal.h>
#include <Wire.h>
#include "SparkFun_SCD30_Arduino_Library.h" 

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

SCD30 airSensor;

// define some values used by the panel
const int backlight = 10; // Backlight connected to D10

#include <WiFiNINA.h>
#include <WiFiClient.h>
#include "arduino_secrets.h"

///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int status = WL_IDLE_STATUS;  // the WiFi radio's status

// Host
const char* host = "dweet.io";

float CO2, temp, hum;
int wifi_retry = 0;

void(* resetFunc) (void) = 0; // Reset ATmega4809
 
void setup() 
{
  Serial.begin(9600);
  delay(1000);          // wait for serial monitor
  Wire.begin();
  airSensor.begin();      //This will cause readings to occur every two seconds
  pinMode(backlight, OUTPUT);
  digitalWrite(backlight, HIGH);
  lcd.begin(16, 2);              // start the library for 2 lines a 16 characters
  lcd.setCursor(0,0);
  lcd.print("SCD30 Monitoring"); // print a simple message
  delay(1000);
    
  Serial.print("Connecting to ");
  Serial.println(ssid);

  WiFi.begin(ssid, pass);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500); Serial.print(".");
    wifi_retry++;
    if (wifi_retry > 10) resetFunc();
  }
  Serial.println("");
  Serial.println("WiFi connected");  
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  lcd.clear();
}
 
void loop() 
{
  reconnectWiFi(); // if Wifi connection is no more available
  
  Serial.print("connecting to ");
  Serial.println(host);

  // Use WiFiClient class to create TCP connections
  WiFiClient client;
  
  const int httpPort = 80;
  
  if (!client.connect(host, httpPort)) 
  {
    Serial.println("connection failed");
    resetFunc();  // if there is no socket from Dweet.io
  }

  Serial.print("Requesting URL: ");
  Serial.println(host);

  temp = airSensor.getTemperature();
  hum  = airSensor.getHumidity();
  CO2  = airSensor.getCO2();

  Serial.print("CO2: \t"); Serial.println(CO2, 0);
  Serial.print("t: \t"); Serial.println(temp, 1);
  Serial.print("h: \t"); Serial.println(hum, 1);
  lcdOutput();

  // This will send the request to the server
  client.print(String("GET /dweet/for/RoomClimate?CO2=") + String(CO2) + 
               "&temperature=" + String(temp) +
               "&humidity=" + String(hum) + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" + 
               "Connection: close\r\n\r\n");
  delay(100);
  
  // Read all the lines of the reply from server and print them to Serial
  while(client.available())
  {
    String line = client.readStringUntil('\r');
    Serial.print(line);
  }
  
  Serial.println();
  Serial.println("closing connection");

  delay(15000);  //Send a request every 15 seconds
}
