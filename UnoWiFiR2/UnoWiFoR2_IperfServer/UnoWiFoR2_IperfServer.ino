/*
 * File UnoWiFioR2_IperfServer.ino
 * 
 * based on 
 * 
 */
#include <SPI.h>
#include <WiFiNINA.h>

#include "arduino_secrets.h" 
///////please enter your sensitive data in the Secret tab/arduino_secrets.h
char ssid[] = SECRET_SSID;    // your network SSID (name)
char pass[] = SECRET_PASS;    // your network password (use for WPA, or use as key for WEP)
int keyIndex = 0;             // your network key Index number (needed only for WEP)

int status = WL_IDLE_STATUS;

WiFiServer server(5001);

void setup() 
{  
  //Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {}  // wait for Serial Monitor

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

  Serial.print("\nIperf server address : ");
  Serial.println(WiFi.localIP());
  Serial.println(" ");
  //SPI.setClockDivider(SPI_CLOCK_DIV2);
}

void loop() {
  byte buf[1024];
  // wait for a new client:
  WiFiClient client = server.available();
  //EthernetClient client2 = server2.available();
  // when the client sends the first byte, say hello:
  //if (client | client2) {
    if (client) {
//    Serial.println("Here is new client for check arduino performance");
  //while (client.connected() | client2.connected())
  while (client.connected())
    {
      if (client.available()) client.read(buf, 1024);
    //  if (client2.available()) client2.read(buf,1024);
   }
    // close the connection:
    client.stop();
   // client2.stop();
//    Serial.println("client disonnected");
  }
}
