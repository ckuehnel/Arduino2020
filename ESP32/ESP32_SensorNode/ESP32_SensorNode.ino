/*
 * File: ESP32_SensorNode.ino
 * 
 * Compiled for DFRobot FireBeetle ESP32-E
 */

#include <esp_now.h>
#include <WiFi.h>
#include "driver/adc.h"
#include <esp_wifi.h>
#include <esp_bt.h>

#define DEBUG 1   // set to 0 to avoid display of measuring results on serial monitor
#define SLEEP (1000 * 1000 * 15)  // Sleeptime 15 sec
#define ID 1      // this ID must be unique for the connected sensors

float sht31Temperature, sht31Humidity;
float bmp280Temperature, bmp280Pressure;

// Variable to store if sending data was successful
String success;

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

// REPLACE WITH THE RECEIVER'S MAC Address
uint8_t gatewayAddress[] = {0xF0, 0x08, 0xD1, 0x80, 0xE2, 0xCC};

// Create peer interface
esp_now_peer_info_t peerInfo;

/*
// callback when data is sent
void OnDataSent(const uint8_t *mac_addr, esp_now_send_status_t status) 
{
  Serial.print("\r\nLast Packet Send Status:\t");
  Serial.println(status == ESP_NOW_SEND_SUCCESS ? "Delivery Success" : "Delivery Fail");
}
*/


void setup()
{
  Serial.begin(115200);
  delay(2000);
  Serial.printf("\nESP32 FireBeetle SensorNode\n");
  Serial.printf("Sensors used SHT31 @ 0x44 & BMP280 @ 0x76\n");
  
  initSensor(); // initialize ENV.II sensor

  float volt = analogRead(A2) * 6.6 /4096;
  if (DEBUG) Serial.printf("Battery      = %5.3f V", volt);
  Serial.println();

  getValues(); // get measuring results of ENV.II sensor

  message.id = ID;   
  message.battery = (uint16_t) (volt * 1000 + .5);
  message.sht31Temp = sht31Temperature;
  message.sht31Humi = (uint8_t) (sht31Humidity + .5);
  message.bmp280Temp = bmp280Temperature;
  message.bmp280Pres = (uint16_t) (bmp280Pressure + .5);

  // Set device as a WiFi Station
  WiFi.mode(WIFI_STA);

  // Init ESP-NOW
  if (esp_now_init() != ESP_OK) 
  {
    Serial.println("Error initializing ESP-NOW");
    return;
  }

  // Once ESPNow is successfully Init, we will register for Send CB to
  // get the status of Trasnmitted packet
//  esp_now_register_send_cb(OnDataSent);
  
  // Register peer
  memcpy(peerInfo.peer_addr, gatewayAddress, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  // Add peer        
  if (esp_now_add_peer(&peerInfo) != ESP_OK)
  {
    Serial.println("Failed to add peer");
    return;
  }

  // Send message via ESP-NOW
  esp_err_t result = esp_now_send(gatewayAddress, (uint8_t *) &message, sizeof(message));
   
  if (result == ESP_OK) Serial.println("Sent with success");
  else Serial.println("Error sending the data");
  
  WiFi.disconnect(true);
  WiFi.mode(WIFI_OFF);
  
  btStop();
  adc_power_off();
  esp_wifi_stop();
  esp_bt_controller_disable();

  Serial.printf("Going to sleep for %d seconds\n", SLEEP/1000/1000);

  esp_sleep_enable_timer_wakeup(SLEEP);
  esp_deep_sleep_start();
}


void loop()
{
  delay(1000);
}
