void connectWiFi()
{
  int count = 0;
  Serial.print("Connecting to ");
  Serial.println(SSID);
  WiFi.begin(SSID, PASS);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
    count++;
    if (count > 15) ESP.restart();
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
}
