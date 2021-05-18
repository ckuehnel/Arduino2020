// Send Message to Thingspeak

void printWifiStatus()
{
  // print the SSID of the network you're attached to
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print your WiFi shield's IP address
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);

  // print the received signal strength
  long rssi = WiFi.RSSI();
  Serial.print("Signal strength (RSSI):");
  Serial.print(rssi);
  Serial.println(" dBm");
}

void sendMessage()
{
  // close any connection before send a new request
  // this will free the socket on the WiFi shield
  client.stop();
  
  Serial.println("Starting connection to server...");
  // if you get a connection, report back via serial
  if (client.connect(server, 80)) 
  {
    Serial.println("Connecting...");
    // Make a HTTP request
    client.println("GET /update?api_key=" + api_key + "&field1=" + String(TVOC) +  "&field2=" + String(eCO2) + " HTTP/1.1");
    client.println("Host: api.thingspeak.com");
    client.println("Connection: close");
    client.println();
  }
  
  // if there are incoming bytes available
  // from the server, read them and print them
  while (client.available()) 
  {
    char c = client.read();
    Serial.write(c);
  }

  // if the server's disconnected, stop the client
  if (!client.connected()) 
  {
    Serial.println();
    Serial.println("Disconnecting from server...");
    client.stop();
  }
}
