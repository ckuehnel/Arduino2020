void reconnectWiFi()
{
  while(WiFi.status() != WL_CONNECTED && wifi_retry < 5 ) 
  {
    wifi_retry++;
    Serial.println("WiFi not connected. Try to reconnect");
    WiFi.disconnect();
    WiFi.begin(ssid, pass);
    delay(500); Serial.print(".");
  }
  Serial.println("");
 
  if(wifi_retry >= 5) 
  {
    Serial.println("\nReboot");
    resetFunc();
  }
}
