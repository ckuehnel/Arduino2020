/*
 * File: mi_temp3_display.ino
 * 
 * Query several LYWSD02MMC sensors and display the measured values on M5StickC Plus display.
 * based on
 * https://github.com/nutpathakorn/mi-temperature-and-humidity-esp32-LYWSD03MMC
 * 
 * 2021-05-27 Claus Kühnel info@ckuehnel.ch
 */
#include <M5StickCPlus.h>
#include <BLEDevice.h>
#include <SimpleTimer.h>
#include "config.h"

float temp, humi, batt, volt;
String sensor;

TaskHandle_t hibernateTaskHandle = NULL;

bool connectionSuccessful = false;

char macAddr[18];  // ESP32 MAC address

static BLEUUID serviceUUID("ebe0ccb0-7a0a-4b0c-8a1a-6ff2997da3a6");
static BLEUUID    charUUID("ebe0ccc1-7a0a-4b0c-8a1a-6ff2997da3a6");

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
    if (pclient->getPeerAddress().toString() == "a4:c1:38:3c:e4:e2") sensor = "S1";
    if (pclient->getPeerAddress().toString() == "a4:c1:38:76:97:b3") sensor = "S2";
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
  Serial.printf("Sensor: %s : ", sensor);
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

void displayValues()
{
  if (sensor == "S1")
  {
    M5.Lcd.fillRect(0, 30, 320, 16, BLACK);
    M5.Lcd.setCursor(0, 30);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.printf("S1: %.1f C %.0f%% %.1f V\n", temp, humi, volt);
  }
  if (sensor == "S2")
  {
    M5.Lcd.fillRect(0, 50, 320, 16, BLACK);
    M5.Lcd.setCursor(0, 50);
    M5.Lcd.setTextColor(WHITE);
    M5.Lcd.printf("S2: %.1f C %.0f%% %.1f V\n", temp, humi, volt);
  }
}

void setup() 
{
  M5.begin();

  //M5.Lcd.setRotation(1);  // Landscape orientation
  M5.Lcd.setRotation(3);
  M5.Lcd.fillScreen(BLACK);
  M5.Lcd.fillRect(0, 0, 320, 22, BLUE);
  M5.Lcd.setCursor(0, 5);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setTextSize(2);
  M5.Lcd.printf("BLE Central");
}

void loop() 
{
  xTaskCreate(delayedHibernate, "hibernate", 4096, NULL, 1, &hibernateTaskHandle);

  Serial.println("Initialize BLE client...");
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
    displayValues(); 
 }
 Serial.println("Waiting for next cycle...");
 delay(45000);
 //Serial.println("Delete all visual tasks and preparing to sleep..");
 vTaskDelete(hibernateTaskHandle);
 //hibernate();;
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
