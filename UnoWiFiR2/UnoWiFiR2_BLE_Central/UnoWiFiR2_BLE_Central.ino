/*
 * File: UnoWiFiR2_BLE_Central.ino
 * 
 * Receiving temperature and humidity data sent by a BLE peripheral
 * based on ESP32 w/ connected SHT31 sensor.
 * 
 * Program based ob SensorTag Button
 * 
 * The program scans for BLE peripherals until the ESP32 BLE UART Service is discovered.
 * It then connects to it, discovers the attribute of the requested service,
 * subscribes to the TxCharacteristi. 
 * 
 * Claus Kühnel 2020-04-03 info@ckuehnel.ch
 */
#include <ArduinoBLE.h>

void setup() 
{
  Serial.begin(9600);
  while (!Serial);

  // begin initialization
  if (!BLE.begin()) 
  {
    Serial.println("starting BLE failed!");
    while (1);
  }

  Serial.println("BLE Central");
  Serial.println("Make sure peripheral is on");

  // start scanning for peripheral
  BLE.scan();
}

void loop() 
{
  // check if a peripheral has been discovered
  BLEDevice peripheral = BLE.available();

  if (peripheral) 
  {
    // discovered a peripheral, print out address, local name, and advertised service
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.print("' ");
    Serial.print(peripheral.advertisedServiceUuid());
    Serial.println();

    // Check if the peripheral has the local name: "ESP32 BLE UART Service"
    if (peripheral.localName() == "ESP32 BLE UART Service") 
    {
      // stop scanning
      BLE.stopScan();

      readValues(peripheral);

      // peripheral disconnected, start scanning again
      BLE.scan();
    }
  }
}

void readValues(BLEDevice peripheral) 
{
  // connect to the peripheral
  Serial.println("Connecting ...");
  if (peripheral.connect()) Serial.println("Connected");
  else 
  {
    Serial.println("Failed to connect!");
    return;
  }

  // discover peripheral attributes
  Serial.println("Discovering service 6E400001-B5A3-F393-E0A9-E50E24DCCA9E...");
  if (peripheral.discoverService("6E400001-B5A3-F393-E0A9-E50E24DCCA9E")) 
  {
    Serial.println("Service discovered");
  } else 
  {
    Serial.println("Attribute discovery failed.");
    peripheral.disconnect();

    while (1);
    return;
  }

  // retrieve the simple key characteristic
  BLECharacteristic TxCharacteristic = peripheral.characteristic("6E400003-B5A3-F393-E0A9-E50E24DCCA9E");

  // subscribe to the characteristic
  Serial.println("Subscribing to characteristic ...");
  if (!TxCharacteristic) 
  {
    Serial.println("no Tx characteristic found!");
    peripheral.disconnect();
    return;
  } else if (!TxCharacteristic.canSubscribe()) 
  {
    Serial.println("Tx characteristic is not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!TxCharacteristic.subscribe()) 
  {
    Serial.println("subscription failed!");
    peripheral.disconnect();
    return;
  } else 
  {
    Serial.println("Subscribed");
  }

  while (peripheral.connected()) 
  {
    // while the peripheral is connected
    // check if the value of the simple key characteristic has been updated
    if (TxCharacteristic.valueUpdated()) 
    {
      // yes, get the value, characteristic is 1 byte so use byte value
      byte value = 0;
      
      TxCharacteristic.readValue(value);
      Serial.print(char(value));
    }
  }
  Serial.println("Disconnected!");
}
