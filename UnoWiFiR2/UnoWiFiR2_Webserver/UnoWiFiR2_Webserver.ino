/*
  WiFi Web Server

 A simple web server that shows the value of the analog input pins.

 This example is written for a network using WPA encryption. For
 WEP or WPA, change the Wifi.begin() call accordingly.

 Circuit:
 * Analog inputs attached to pins A0 through A5 (optional)

 created 13 July 2010
 by dlf (Metodo2 srl)
 modified 31 May 2012
 by Tom Igoe

 */
#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;             // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(80);

#include <Wire.h>
#include "SHT31.h"

SHT31 sht31 = SHT31();

float temp, hum;
unsigned long lastUpdate = millis();

#define DEBUG 0

// Function prototype
void printWifiStatus(void);
void getValues(void);

void setup() 
{
  //Initialize serial and wait for port to open:
  Serial.begin(9600);
  while (!Serial) {}  // wait for Serial Monitor

  sht31.begin();  // Initialize SHT31

  // check for the WiFi module:
  if (WiFi.status() == WL_NO_MODULE) 
  {
    Serial.println("Communication with WiFi module failed!");
    while (true); // don't continue
  }

  String fv = WiFi.firmwareVersion();
  if (fv < WIFI_FIRMWARE_LATEST_VERSION) Serial.println("Please upgrade the firmware");

  // attempt to connect to Wifi network:
  Serial.print("Attempting to connect to SSID: ");
  Serial.println(ssid);
  while (status != WL_CONNECTED) 
  {
    // Connect to WPA/WPA2 network. Change this line if using open or WEP network:
    status = WiFi.begin(ssid, pass);
    Serial.print(".");
    delay(500);
  }
  server.begin();
  // you're connected now, so print out the status:
  printWifiStatus();
}

void loop() 
{
  // listen for incoming clients
  WiFiClient client = server.available();
  if (client) 
  {
    Serial.println("new client");
    // an http request ends with a blank line
    String currentLine = "";
    while (client.connected()) 
    {
      if (client.available()) 
      {
        char c = client.read();
        Serial.write(c);
        // if you've gotten to the end of the line (received a newline
        // character) and the line is blank, the http request has ended,
        // so you can send a reply
        if (c == '\n') 
        {
          if (currentLine.length() == 0)
          {
            // send a standard http response header
            client.println("HTTP/1.1 200 OK");
            client.println("Content-Type: text/html");
            client.println("Connection: close");  // the connection will be closed after completion of the response
            client.println("Refresh: 5");  // refresh the page automatically every 5 sec
            client.println("<!DOCTYPE HTML>");
            client.println("<html>");
            client.println("<h1>SHT31 Measuring Values</h1>");
            getValues();
            client.print("Temperature is "); 
            client.print(temp, 1);
            client.print(" *C<br>");
            client.print("Humidity is ");
            client.print(hum, 0);
            client.print(" %RH<br>");
            client.println("</html>");
            client.println(); // Last line is empty line
            break;
          }
          else currentLine = "";
        }
        else if (c != '\r') currentLine += c;  // add it to the end of the currentLine
      }
    }
    // give the web browser time to receive the data
    delay(10);
    // close the connection:
    client.stop();
    Serial.println("client disconnected");
  }
}
