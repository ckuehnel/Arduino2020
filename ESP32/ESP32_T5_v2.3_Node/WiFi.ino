// Send measuring values to Telegram bot

void sendMessage()
{
  if (DEBUG)
  {
    // attempt to connect to Wifi network:
    Serial.print("Connecting to WiFi: ");
    Serial.println(ssid);
  }
  
  WiFi.mode(WIFI_STA); 
  WiFi.disconnect();
  WiFi.begin("Sunrise_2.4GHz_8AC2A0", "u2u7fgzv31Ds");

  while (WiFi.status() != WL_CONNECTED) 
  {
    if (DEBUG) Serial.print(".");
    delay(500);
  }

  if (DEBUG)
  {
    Serial.println("WiFi connected.");
    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
  }
  String message =  "Vbat: " + String(voltage) + " V\n";
         message += "Temp: " + String(temp) + " *C"; 
  bot.sendMessage(CHAT_ID, message, "");
}
