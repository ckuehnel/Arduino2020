// WiFi Initialization

void initWiFi() 
{
  WiFi.mode(WIFI_STA);
  Serial.print("\nConnecting to ");
  Serial.println(ssid);
  WiFi.disconnect();
  WiFi.begin(ssid,password);
  Serial.print("MAC: " );Serial.println(WiFi.macAddress());
  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("\nWiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
