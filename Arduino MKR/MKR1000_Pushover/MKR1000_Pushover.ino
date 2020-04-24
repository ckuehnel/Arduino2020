/*

   Pushover sketch by M.J. Meijer 2014

   Send pushover.net messages from the arduino

*/

#include <SPI.h>
#include <Ethernet.h>

byte mac[] = {0xDE,0xAC,0xBF,0xEF,0xFE,0xAA};

// Pushover settings
char pushoversite[] = "api.pushover.net";
char apitoken[] = "your30characterapitokenhere123";
char userkey [] = "your30characteruserkeygoeshere";

int length;

EthernetClient client;

void setup()
{
  Serial.begin(9600);
  Serial.print(F("Starting ethernet..."));
  if(!Ethernet.begin(mac)) Serial.println("failed");
  else Serial.println(Ethernet.localIP());

  delay(5000);
  Serial.println("Ready");
}

void loop()
{
  pushover("OMG, Yes it works!!!",0);  
  delay(60000); 
}

byte pushover(char *pushovermessage, int priority)
{
  String message = pushovermessage;

  length = 113 + message.length();

  if(client.connect(pushoversite,80))
  {
    client.println("POST /1/messages.json HTTP/1.1");
    client.println("Host: api.pushover.net");
    client.println("Connection: close\r\nContent-Type: application/x-www-form-urlencoded");
    client.print("Content-Length: ");
    client.print(length);
    client.println("\r\n");;
    client.print("token=");
    client.print(apitoken);
    client.print("&user=");
    client.print(userkey);
    client.print("&message=");
    client.print(message);
    client.print("&priority=");
    client.print(priority);
    client.print("&retry=60");
    client.print("&expire=3600");
    while(client.connected())  
    {
      while(client.available())
      {
        char ch = client.read();
        Serial.write(ch);
      }
    }
    client.stop();
  }  
}
