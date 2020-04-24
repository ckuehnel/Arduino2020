/*
   File: ESP32_BLE_Server.ino
    
   Based on Neil Kolban example for IDF: https://github.com/nkolban/esp32-snippets/blob/master/cpp_utils/tests/BLE%20Tests/SampleNotify.cpp
   Ported to Arduino ESP32 by Evandro Copercini

   Create a BLE server that, once we receive a connection, will send periodic notifications.
   The service advertises itself as: 6E400001-B5A3-F393-E0A9-E50E24DCCA9E
   Has a characteristic of: 6E400002-B5A3-F393-E0A9-E50E24DCCA9E - used for receiving data with "WRITE" 
   Has a characteristic of: 6E400003-B5A3-F393-E0A9-E50E24DCCA9E - used to send data with  "NOTIFY"

   The design of creating the BLE server is:
   1. Create a BLE Server
   2. Create a BLE Service
   3. Create a BLE Characteristic on the Service
   4. Create a BLE Descriptor on the characteristic
   5. Start the service.
   6. Start advertising.

   In this example rxValue is the data received (only accessible inside that function), and txValue is the data to be sent. 
*/
#include <BLEDevice.h>
#include <BLEServer.h>
#include <BLEUtils.h>
#include <BLE2902.h>

#include <Wire.h>
#include "SHT31.h"

#define DEBUG 1

SHT31 sht31 = SHT31();

BLECharacteristic *pCharacteristic;

bool deviceConnected = false;
uint8_t txValue = 32;
String values = "";  // BLE output string 

// See the following for generating UUIDs: https://www.uuidgenerator.net/

#define SERVICE_UUID           "6E400001-B5A3-F393-E0A9-E50E24DCCA9E" // UART service UUID
#define CHARACTERISTIC_UUID_RX "6E400002-B5A3-F393-E0A9-E50E24DCCA9E"
#define CHARACTERISTIC_UUID_TX "6E400003-B5A3-F393-E0A9-E50E24DCCA9E"

void getValues(void); // Function prototype

class MyServerCallbacks: public BLEServerCallbacks 
{
  void onConnect(BLEServer* pServer)    { deviceConnected = true; };
  void onDisconnect(BLEServer* pServer) { deviceConnected = false; }
};

class MyCallbacks: public BLECharacteristicCallbacks 
{
  void onWrite(BLECharacteristic *pCharacteristic) 
  {
    std::string rxValue = pCharacteristic->getValue();

    if (rxValue.length() > 0) 
    {
      Serial.println("*********");
      Serial.print("Received Value: ");
      for (int i = 0; i < rxValue.length(); i++)
      Serial.print(rxValue[i]);
      Serial.println("*********");
    }
  }
};


void setup() 
{
  Serial.begin(115200);
  delay(1000);  // waiting for Serial Monitor
  Serial.println("\nBLE Server running...");
  sht31.begin();

  // Create the BLE Device
  BLEDevice::init("ESP32 BLE UART Service");

  // Create the BLE Server
  BLEServer *pServer = BLEDevice::createServer();
  pServer->setCallbacks(new MyServerCallbacks());

  // Create the BLE Service
  BLEService *pService = pServer->createService(SERVICE_UUID);

  // Create a BLE Characteristic
  pCharacteristic = pService->createCharacteristic
  (
    CHARACTERISTIC_UUID_TX,
    BLECharacteristic::PROPERTY_NOTIFY
  );
                      
  pCharacteristic->addDescriptor(new BLE2902());

  BLECharacteristic *pCharacteristic = pService->createCharacteristic
  (
    CHARACTERISTIC_UUID_RX,
    BLECharacteristic::PROPERTY_WRITE
  );

  pCharacteristic->setCallbacks(new MyCallbacks());

  // Start the service
  pService->start();

  // Start advertising
  pServer->getAdvertising()->start();
  Serial.println("Waiting a client connection to notify...");
}

void loop() 
{
  getValues();  // measure & build message string
  
  if (deviceConnected) 
  {
    for (int i = 0; i < values.length(); i++)
    { 
      txValue = values[i];
      //Serial.print(txValue);
      pCharacteristic->setValue(&txValue, 1);
      pCharacteristic->notify();
    }
  }
  delay(1000);
}
