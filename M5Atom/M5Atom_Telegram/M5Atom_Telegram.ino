/*
 * File: M5Atom_TelegramBot.ino
 * 
 * Created 2020-07-09 Claus Kühnel info@ckuehnel.ch   
*/
#include <M5Atom.h>
#include <WiFi.h>
#include <WiFiClientSecure.h>
#include <UniversalTelegramBot.h>
#include "arduino_secrets.h"

String ThisChat = "";

WiFiClientSecure client;
UniversalTelegramBot bot(BOTtoken, client);

#define INFO 1
#define DEBUG 1

uint8_t Red = 255, Green = 200, Blue = 0;

void matrix(uint8_t R, uint8_t G, uint8_t B)
{
  for (int i = 0; i < 25; i++) M5.dis.drawpix(i, CRGB(G, R, B));
}

enum states {NOSTATE, MAIL, WA};
uint8_t state;

bool pressed = false;

void IRAM_ATTR btn_isr()
{
  pressed = true;
}

unsigned int numNewMessages = 0;
unsigned int Bot_mtbs = 1000; //mean time between scan messages
long Bot_lasttime;   //last time messages' scan has been done

void matrix(uint8_t R, uint8_t G, uint8_t B);

void handleNewMessages(int numNewMessages) 
{
  String chat_id;
  
  for (int i=0; i<numNewMessages; i++) 
  {
    String chat_id = String(bot.messages[i].chat_id);
    String text = bot.messages[i].text;

    String from_name = bot.messages[i].from_name;
    if (from_name == "") from_name = "Guest";

    if (text == "/mail") 
    {
      state = MAIL;
      bot.sendMessage(ThisChat, "New email is announced", "");
 
      String keyboardJson = "[[\"/clear\", \"/wa\", \"/status\"]]";
      bot.sendMessageWithReplyKeyboard(ThisChat, "Choose one of the following options", "", keyboardJson, true, true);
    }

    if (text == "/wa") 
    {
      state = WA;
      bot.sendMessage(ThisChat, "New WhatsApp is announced", "");
      String keyboardJson = "[[\"/mail\", \"/clear\", \"/status\"]]";
      bot.sendMessageWithReplyKeyboard(ThisChat, "Choose one of the following options", "", keyboardJson, true, true);
    }

    if (text == "/clear")
    {
      state = NOSTATE;
      bot.sendMessage(ThisChat, "Announcements cleared", ""); 
      String keyboardJson = "[[\"/mail\", \"/wa\", \"/status\"]]";
      bot.sendMessageWithReplyKeyboard(ThisChat, "Choose one of the following options", "", keyboardJson, true, true);
    }
    
    if (text == "/status") 
    {
      String msg = "";
      switch (state)
      {
        case 0: msg = "No Announcement";
                break;
        case 1: msg = "Mail announced";
                break;
        case 2: msg = "WhatsApp announced";
                break;
        default: msg = "Connection ok";       
      }
      bot.sendMessage(ThisChat, msg, "");
      String keyboardJson = "[[\"/mail\", \"/wa\", \"/clear\"]]";
      bot.sendMessageWithReplyKeyboard(ThisChat, "Choose one of the following options", "", keyboardJson, true, true);
    }
  
    if (text == "/start") 
    {
      String welcome = "Signalization by M5Atom Matrix\nvia Telegram by " + from_name + ".\n\n";
      welcome += "/mail   : to announce new mail\n";
      welcome += "/wa     : to announce new whatsapp\n";
      welcome += "/clear  : to clear announcements\n";
      welcome += "/status : returns status \n";
      if (DEBUG) Serial.print("ChatID: "); Serial.println(chat_id);
      if (ThisChat=="") ThisChat = chat_id;
      bot.sendMessage(ThisChat, welcome, "Markdown");
      String keyboardJson = "[[\"/mail\", \"/wa\", \"/status\"]]";
      bot.sendMessageWithReplyKeyboard(ThisChat, "Choose one of the following options", "", keyboardJson, true, true);
    }
  }
}

void setup() 
{
  M5.begin(true, true, true); // Serial, I2C, Display
  delay(3000);               // wait for serial monitor
  if (INFO)  printInfo();
  if (DEBUG) Serial.println(F("Initializing..."));
  
  // attempt to connect to Wifi network:
  Serial.print("Connecting to WiFi: ");
  Serial.println(ssid);
  
  
  WiFi.disconnect();
  WiFi.mode(WIFI_STA); 
  WiFi.begin(ssid, pw);

  while (WiFi.status() != WL_CONNECTED) 
  {
    Serial.print(".");
    delay(500);
  }
  Serial.println("WiFi connected.");
  Serial.print("IP address: ");
  Serial.println(WiFi.localIP());
  
  pinMode(39, INPUT_PULLUP);               // Button is G32 pin
  attachInterrupt(39, btn_isr, FALLING);   // Grove G32 pin
  state = NOSTATE;
  if (DEBUG) Serial.println(F("Running..."));
}

void loop() 
{
  WiFi_reconnect();
  if (pressed)
  {
    pressed = false;
    state = NOSTATE;
    Serial.println("Button pressed.");
    bot.sendMessage(ThisChat, "Announcements acknowledged", "");
    String keyboardJson = "[[\"/mail\", \"/wa\", \"/status\"]]";
    bot.sendMessageWithReplyKeyboard(ThisChat, "Choose one of the following options", "", keyboardJson, true, true);
  }
  if (millis() > Bot_lasttime + Bot_mtbs)  
  {
    numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    while(numNewMessages) 
    {
      handleNewMessages(numNewMessages);
      numNewMessages = bot.getUpdates(bot.last_message_received + 1);
    }
    Bot_lasttime = millis();
  }
  if (state == MAIL) 
  {
    Red = 255, Green = 200, Blue = 0;
    //do blink(Red, Green, Blue);
    //while (state != NOSTATE);
    matrix(Red, Green, Blue);
  }
  if (state == WA)
  { 
    Red = 0, Green = 200, Blue = 0;
    //do blink(Red, Green, Blue);
    //while (state != NOSTATE);
    matrix(Red, Green, Blue);
  }
  if (state == NOSTATE)
  {
    Red = 0, Green = 0, Blue = 0;
    matrix(Red, Green, Blue);
  }
}

/*--------------------------------------------------------------*/

void printInfo()
{
  // Compile file path (internal file of compilation proccess)
  Serial.print("\nFile "); Serial.println(__FILE__);
  
  String s1 = __DATE__;
  String s2 = __TIME__;

  // Date of compilation
  Serial.print("Compilation @ "); Serial.println(s1 + " " + s2);
  
  // Arduino IDE SW version
  Serial.print("ARDUINO IDE v"); Serial.println(ARDUINO);
  
  // Oscillator frequency
  Serial.print("CPU Clock in MHz: "); Serial.println(F_CPU/1E6);
}
