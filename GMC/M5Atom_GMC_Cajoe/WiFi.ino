// WiFi Initialization

void initWiFi() 
{
  WiFi.mode(WIFI_STA);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.disconnect();
  WiFi.begin(ssid,password);
  Serial.print("MAC: " ); Serial.println(WiFi.macAddress());
  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    M5.dis.drawpix(0, 0x0000f0);  // BLUE
    delay(500);
    M5.dis.drawpix(0, 0x000000);  // DARK
    delay(500);
  }
  Serial.println("\nWiFi connected");
  M5.dis.drawpix(0, 0x0000f0);  // BLUE
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
