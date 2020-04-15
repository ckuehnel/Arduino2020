// This file contains the application code

void send_message()
{
  heartbeat();
  Serial.print("Connecting to WiFi...");
  WiFi.begin(ssid, password);
  while (WiFi.status() != WL_CONNECTED) 
  {
    heartbeat();
  }
  Serial.println("connected.");
  char buf[10];
  long rssi = WiFi.RSSI();
  char * rssi_str;
  rssi_str = ltoa(rssi, buf, 10);
 
  String message = "RSSI = ";
         message += rssi_str; 
         message += " dBm";

  int len = message.length();
  char charBuf[len+1];
  message.toCharArray(charBuf, len+1);
  Serial.println(charBuf);
  pushover(charBuf,0); // Priority 0
   
  WiFi.end();
}
