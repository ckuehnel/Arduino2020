/*
 * Arduino Uno WiFi Rev2 Weather
 * 
 * based on ESP8266 -  Weather forecast from https://openweathermap.org/
 * v. 1.0
 * Copyright (C) 2018 Robert Ulbricht https://www.arduinoslovakia.eu
 * 
 * adaptions to Arduino Uno WiFi Rev2 2020-04-08 by Claus Kuehnel info@ckuehnel.ch
 * Board: Arduino Uno WiFi Rev2
 * Libraries:
 * Arduino JSON library: https://github.com/bblanchon/ArduinoJson
 * Version: 6.15.0 or higher
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */
// WiFi - Version: 1.2.6
#include <SPI.h>
#include <WiFiNINA.h>
#include <ArduinoJson.h>
#include "arduino_secrets.h"

char* ssid     = SECRET_SSID;
char* password = SECRET_PASS;
char* appid    = SECRET_APPID;

#define DEBUG 0  // Select debug mode by 1

//http://api.openweathermap.org/data/2.5/weather?q=Altendorf,CH&units=metric&appid=87286fed489ba6a8afcd81443138d510
const char* host = "api.openweathermap.org";
const char* url = "/data/2.5/weather";
const char* openweathermapq = "Altendorf, CH"; // chosse your location - here "Altendorf, CH"
const char* openweathermapunits = "metric"; // Celsius

String param = String(url) +
               "?q=" + openweathermapq +
               "&units=" + openweathermapunits +
               "&appid=" + appid;

String line;

void setup() {
  Serial.begin(9600);
  while(!Serial);
  Serial.println("Get weather data from Openweathermap.org...");
  Serial.print("Connecting to WiFi - ");
  Serial.println(ssid);
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());

  WiFiClient client;
  Serial.print("connecting to ");
  Serial.println(host);
  if (!client.connect(host, 80)) {
    Serial.println("connection failed");
    return;
  }

  Serial.print("requesting URL: ");
  Serial.print(host);
  Serial.println(param);

  client.print(String("GET ") + param + " HTTP/1.1\r\n" +
               "Host: " + host + "\r\n" +
               "User-Agent: MKR1000\r\n" +
               "Connection: close\r\n\r\n");

  Serial.println("request sent\n");

  line = client.readStringUntil('\n');
  if (line != "HTTP/1.1 200 OK\r") {
    Serial.print("Unexpected response: ");
    Serial.println(line);
    return;
  }

  if (client.find("\r\n\r\n")) {
    DynamicJsonDocument doc(4096);
    DeserializationError err = deserializeJson(doc, client);
    if (err) 
    {
      Serial.print(F("deserializeJson() failed: "));
      Serial.println(err.c_str());
    }
    if (DEBUG) 
    {
      serializeJsonPretty(doc, Serial);
      Serial.println();
    }

    // parsed output
    const char* name = doc["name"];
    Serial.print("City: ");Serial.println(name);

    char buffer[64];
    const double temp = doc["main"]["temp"];
//    sprintf(buffer, "Temperature: %4.1f °C", temp);
//    Serial.println(buffer);
    Serial.print("Temperature: "); Serial.print(temp,2); Serial.println(" °C");

    const int humidity = doc["main"]["humidity"];
    sprintf(buffer, "Humidity: %d %%r.H.", humidity);
    Serial.println(buffer);

    const int pressure = doc["main"]["pressure"];
    sprintf(buffer, "Pressure: %d hPa", pressure);
    Serial.println(buffer);

    const double wind = doc["wind"]["speed"];
//    sprintf(buffer, "Wind speed: %.02f m/s", wind);
//    Serial.println(buffer);
    Serial.print("Wind speed: "); Serial.print(wind,2); Serial.println(" m/s");

    const char* weather = doc["weather"][0]["main"];
    const char* description = doc["weather"][0]["description"];
    sprintf(buffer, "Weather: %s (%s)", weather, description);
    Serial.println(buffer);
  }
}

void loop() {}
