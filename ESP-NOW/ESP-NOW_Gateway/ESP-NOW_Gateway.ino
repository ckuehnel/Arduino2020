/*
 * File: ESP-NOW Gateway.ino
 */

#include <esp_now.h>
#include <WiFi.h>
#include <FastLED.h>

//Structure example to send data
//Must match the receiver structure
typedef struct EspNowMessage 
{
    uint8_t  id;
    uint16_t battery;
    float    sht31Temp;
    uint8_t  sht31Humi;
    float    bmp280Temp;
    uint16_t bmp280Pres;
} EspNowMessage;

// Create a struct_message called message
EspNowMessage message;

// Create a structure to hold the readings from each board
EspNowMessage board1;
EspNowMessage board2;
EspNowMessage board3;

// Create an array with all the structures
EspNowMessage boardsStruct[3] = {board1, board2, board3};


#define BAUDRATE 9600
#define GATEWAY_RXD2 14
#define GATEWAY_TXD2 15

#define ESP_LED_PIN 32
#define LED_COUNT   2
CRGB leds[LED_COUNT];


void showStatus(CRGB led1, CRGB led2) 
{
    FastLED.clear();
    leds[0] = led1;
    leds[1] = led2;
    FastLED.show();
}

// Init ESP Now with fallback
void InitESPNow() 
{
  WiFi.disconnect();
  
  if (esp_now_init() == ESP_OK) 
  {
    Serial.println("ESPNow Init Success");
    showStatus(CRGB::Green, CRGB::Green);
  } 
  else 
  {
    Serial.println("ESPNow Init Failed");
    showStatus(CRGB::Red, CRGB::Red);
    // Retry InitESPNow, add a counte and then restart?
    // InitESPNow();
    // or Simply Restart
    ESP.restart();
  }
}

// callback function that will be executed when data is received
void OnDataRecv(const uint8_t * mac_addr, const uint8_t *incomingData, int len) 
{
  char macStr[18];
  Serial.print("Packet received from: ");
  snprintf(macStr, sizeof(macStr), "%02x:%02x:%02x:%02x:%02x:%02x",
           mac_addr[0], mac_addr[1], mac_addr[2], mac_addr[3], mac_addr[4], mac_addr[5]);
  Serial.println(macStr);
  memcpy(&message, incomingData, sizeof(message));
  Serial.printf("  Board ID %u: %u bytes\n", message.id, len);
  
  // Update the structures with the new incoming data
  boardsStruct[message.id-1].battery = message.battery;
  boardsStruct[message.id-1].sht31Temp = message.sht31Temp;
  boardsStruct[message.id-1].sht31Humi = message.sht31Humi;
  boardsStruct[message.id-1].bmp280Temp = message.bmp280Temp;
  boardsStruct[message.id-1].bmp280Pres = message.bmp280Pres;

  // Print received data
  Serial.printf("  Battery:    %d mV\n", boardsStruct[message.id-1].battery);
  Serial.printf("  sht31Temp:  %3.1f °C\n", boardsStruct[message.id-1].sht31Temp);
  Serial.printf("  sht31Humi:  %d %rH\n", boardsStruct[message.id-1].sht31Humi);  
  Serial.printf("  bmp280Temp: %3.1f °C\n", boardsStruct[message.id-1].bmp280Temp);  
  Serial.printf("  bmp280Pres: %d hPa\n", boardsStruct[message.id-1].bmp280Pres);
  
  // Send received message to ESP2
  Serial2.write((uint8_t *) &message, sizeof(message));
  Serial.println("Message transmitted\n");
}

void setup() 
{
  Serial.begin(115200);
  FastLED.addLeds<WS2812B, ESP_LED_PIN, GRB>(leds, LED_COUNT);
  FastLED.setBrightness(100);

  showStatus(CRGB::White, CRGB::White);

  Serial2.begin(BAUDRATE, SERIAL_8N1, GATEWAY_RXD2, GATEWAY_TXD2);

  WiFi.mode(WIFI_STA);
 
  // Init ESPNow with a fallback logic
  InitESPNow();
  // Once ESPNow is successfully Init, we will register for recv CB to
  // get recv packer info.
  esp_now_register_recv_cb(OnDataRecv);
}



void loop() {
  //sendDebugMessage();
  delay(1000);
  // Chill
}
