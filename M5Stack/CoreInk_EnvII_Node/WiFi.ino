// Send measuring values via Telegram bot

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
  
  String message =  "Vbat: " + String(voltage,3) + " V\n";
         message += "Temp: " + String(tmp,1) + " *C\n"; 
         message += "Hum : " + String(hum,1) + " %rH";
  bot.sendMessage(CHAT_ID, message, "");
}
