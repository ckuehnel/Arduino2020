/*
 * File: mi_temp3.ino
 * 
 * Query several LYWSD02MMC sensors and create MQTT messages sending to HiveMQ MQTT broker
 * M5StickC Plus (ESP32) is used as BLE MQTT Gateway.
 * https://github.com/nutpathakorn/mi-temperature-and-humidity-esp32-LYWSD03MMC
 */
#include <BLEDevice.h>
#include <SimpleTimer.h>
#include <WiFi.h>
#include <PubSubClient.h>
#include "config.h"

float temp, humi, batt, volt;

TaskHandle_t hibernateTaskHandle = NULL;

bool connectionSuccessful = false;
String baseTopic = MQTT_BASE_TOPIC + "/";

WiFiClient espClient;
PubSubClient client(espClient);

// ESP32 MAC address
char macAddr[18];

static BLEUUID serviceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6");
static BLEUUID    charUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6");

void connectWifi() 
{
  Serial.println("Connecting to WiFi...");
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);

  while (WiFi.status() != WL_CONNECTED) 
  {
    delay(500);
    Serial.print(".");
  }
  Serial.println("WiFi connected");
  byte ar[6];
  WiFi.macAddress(ar);
  sprintf(macAddr,"%02X:%02X:%02X:%02X:%02X:%02X",ar[0],ar[1],ar[2],ar[3],ar[4],ar[5]);
}

void disconnectWifi() 
{
  WiFi.disconnect(true);
  Serial.println("WiFi disonnected");
}

void connectMqtt() 
{
  Serial.println("Processing to send the deta to mqtt...");
  Serial.println(""); 
  Serial.println("Connecting to MQTT...");
  client.setServer(MQTT_HOST, MQTT_PORT);

  while (!client.connected()) 
  {
    if (!client.connect(MQTT_CLIENTID, MQTT_USERNAME, MQTT_PASSWORD)) 
    {
      Serial.print("MQTT connection failed:");
      Serial.print(client.state());
      Serial.println("Retrying...");
      delay(MQTT_RETRY_WAIT);
    }
  }
  Serial.println("MQTT connected");
  Serial.println("");
}

void disconnectMqtt() 
{
  client.disconnect();
  Serial.println("MQTT disconnected");
  Serial.println("");
}

void hibernate() 
{
  esp_sleep_enable_timer_wakeup(SLEEP_DURATION * 1000000ll);
  Serial.println("Going to sleep now.");
  delay(100);
  esp_deep_sleep_start();
}

void delayedHibernate(void *parameter) 
{
  delay(EMERGENCY_HIBERNATE * 1000); // delay for five minutes
  Serial.println("Something got stuck, entering emergency hibernate...");
  hibernate();
}

class MyClientCallback : public BLEClientCallbacks 
{
  void onConnect(BLEClient* pclient) 
  {
    Serial.println("Connected");
  }

  void onDisconnect(BLEClient* pclient) 
  {
    Serial.println("Disconnected");
    if (!connectionSuccessful) 
    {
      Serial.println("RESTART");
      ESP.restart();
    }
  }
};

static void notifyCallback(
  BLERemoteCharacteristic* pBLERemoteCharacteristic,
  uint8_t* pData,
  size_t length,
  bool isNotify) 
{
  Serial.print("Notify callback for characteristic ");
  Serial.println(pBLERemoteCharacteristic->getUUID().toString().c_str());
  
  temp = NULL; humi = NULL; volt = NULL; batt = NULL;
  
  temp = (pData[0] | (pData[1] << 8)) * 0.01; //little endian 
  humi = pData[2];
  volt = ((pData[4] * 256) + pData[3]) / 1000.0;
  batt = (volt - 2.1) * 100.0;
  Serial.printf("temp = %.1f , humidity = %.1f , volt = %.1f , batt = %.1f \n", temp, humi, volt, batt);
  Serial.println("Disconnecting Bluetooth...");
  delay(5000);
  pClient->disconnect(); 
}

void registerNotification() 
{
  BLERemoteService* pRemoteService = pClient->getService(serviceUUID);
  if (pRemoteService == nullptr) 
  {
    Serial.print("Failed to find our service UUID: ");
    Serial.println(serviceUUID.toString().c_str());
    pClient->disconnect();
  }
  Serial.println(" - Found our service");
  BLERemoteCharacteristic* pRemoteCharacteristic = pRemoteService->getCharacteristic(charUUID);
  if (pRemoteCharacteristic == nullptr) 
  {
    Serial.print("Failed to find our characteristic UUID: ");
    Serial.println(charUUID.toString().c_str());
    pClient->disconnect();
  }
  Serial.println(" - Found our characteristic");  
  pRemoteCharacteristic->registerForNotify(notifyCallback);
}

void setup() 
{
  Serial.begin(115200);

  xTaskCreate(delayedHibernate, "hibernate", 4096, NULL, 1, &hibernateTaskHandle);

  Serial.println("Initialize BLE-MI-T&H client...");
  delay(500);
  BLEDevice::init("ESP32");
  createBleClientWithCallbacks();
  Serial.print("Total Devices = ");
  Serial.println(deviceCount);
  
  delay(1000);
  for (int i = 0; i < deviceCount; i++) 
  {
    delay(1000);
    Serial.print("");
    Serial.print("Device MacAddress = ");
    Serial.println(LYWSD03MMC_ADDR[i]);
    connectSensor(LYWSD03MMC_ADDR[i]);
    if(connectionSuccessful)
    {
       registerNotification(); 
    }   
    delay(15000);
    connectWifi();
    connectMqtt();

    char buffer1[64];
    char buffer2[64];
    char buffer3[64];
    char buffer4[64];
    snprintf(buffer1, 64, "%2.1f", temp);
    if (client.publish((baseTopic + LYWSD03MMC_ADDR[i] + "/temperature").c_str(), buffer1)) 
    {
      Serial.println("   >> Published temp");
      //Serial.println(baseTopic + LYWSD03MMC_ADDR[i] + "/temperature");
      //Serial.println(buffer1);
    }
    snprintf(buffer2, 64, "%2.1f", humi);
    if (client.publish((baseTopic + LYWSD03MMC_ADDR[i] + "/humidity").c_str(), buffer2)) 
    {
      Serial.println("   >> Published humi");
    }
    snprintf(buffer3, 64, "%2.1f", volt);
    if (client.publish((baseTopic + LYWSD03MMC_ADDR[i] + "/voltage").c_str(), buffer3)) 
    {
      Serial.println("   >> Published voltage");
    }
    snprintf(buffer4, 64, "%2.1f", batt);
    if (client.publish((baseTopic + LYWSD03MMC_ADDR[i] + "/battery").c_str(), buffer4)) 
    {
      Serial.println("   >> Published battery");
    }
    Serial.println("");
    Serial.println("Disconnecting the internet and logout mqtt service...");
    delay(15000);
    disconnectWifi();
    disconnectMqtt(); 
   delay(15000); 
 }
 delay(15000);
 Serial.println("Delete all visual tasks and preparing to sleep..");
 vTaskDelete(hibernateTaskHandle);
 hibernate();
}

void loop() 
{
  // do nothing
  delay(10000);
}

void createBleClientWithCallbacks() 
{
  pClient = BLEDevice::createClient();
  pClient->setClientCallbacks(new MyClientCallback());
}

void connectSensor(std::string devicesMacAdd) 
{
  BLEAddress htSensorAddress(devicesMacAdd);
  pClient->connect(htSensorAddress);
  connectionSuccessful = true;
}
