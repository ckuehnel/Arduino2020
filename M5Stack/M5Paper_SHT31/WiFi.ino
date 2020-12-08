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
  WiFi.begin(ssid, pw);

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
         message += "Temp: " + String(temp) + " *C\n"; 
         message += "Hum : " + String(humi) + " %rH";
  bot.sendMessage(CHAT_ID, message, "");
}
