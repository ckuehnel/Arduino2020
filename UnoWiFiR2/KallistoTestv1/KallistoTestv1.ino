/*
 * KallistoTestv1.ino
 * 
 * Read out ambient data from Sensry Kallisto Sensor
 * based on Arduino 101 data logger using TI SensorTag CC2650                          
 * Software to accompany the article:                                         
 * "Build an Arduino 101 Data Logger with the TI SensorTag" using Arduino 101 based on Intel Curie
 * 
 * Adaptions to Arduino Uno WiFi Rev2 & Kallisto
 * 2012-08-05 Claus Kuehnel info@ckuehnel.ch
 * 
 * The base UUID of the Kallisto is 0xXXXXXXXX-702b-69b5-b243-d6094a2b0e24.
 */
#include <ArduinoBLE.h>

#define DEBUG 0  // for debug output set to 1 else 0

#define BLEperipheral "Kallisto_Sensing_E6_ED"

// Global structure to hold the sensor data
struct DATA 
{
  char  devInfo[sizeof(BLEperipheral)] = BLEperipheral; // Device Info
  uint8_t batt;  // Battery
  float temp;    // Temperature
  float humi;    // Humidity
  float press;   // Pressure
  float eCO2;    // eCO2
  float TVOC;    // TVOC   // TMP007 object temperature F
};

typedef struct DATA DATA;
DATA SensorData;

//period defines the length of time between measurements in milliseconds
//long period=600000L; // 10 minutes
long period = 30000L;  // .5 minute for testing

BLEDevice peripheral;

//Battery
BLECharacteristic BattValCharacteristic;

// Temperature
BLECharacteristic TempConCharacteristic;
BLECharacteristic TempValCharacteristic;

// Humidity
BLECharacteristic HumiConCharacteristic;
BLECharacteristic HumiValCharacteristic;

// Pressure
BLECharacteristic PressConCharacteristic;
BLECharacteristic PressValCharacteristic;

// eCO2
BLECharacteristic eCO2ConCharacteristic;
BLECharacteristic eCO2ValCharacteristic;

// TVOC
BLECharacteristic TVOCConCharacteristic;
BLECharacteristic TVOCValCharacteristic;

void setup() {
  Serial.begin(9600);
  while (!Serial);
  Serial.println("Kallisto Sensing");
  Serial.println();
  // initialize the BLE hardware
  BLE.begin();
  BLE.scan();
}

void loop() {
  long lastMillis = 0;  // for period test
  long nowMillis = 0;   // for period test

  // check if a peripheral has been discovered
  peripheral = BLE.available();
  if (peripheral) {
    // discovered a peripheral, print out address and local name
    Serial.print("Found ");
    Serial.print(peripheral.address());
    Serial.print(" '");
    Serial.print(peripheral.localName());
    Serial.println("' ");
    if (peripheral.localName() == BLEperipheral) {
      // stop scanning
      BLE.stopScan();
      // connect to the peripheral
      Serial.print("Connecting to Kallisto...");
      if (peripheral.connect()) {
        Serial.println("Connected");
        do_discovery(peripheral);
        subscribe_Temp(peripheral);
        subscribe_Humi(peripheral);
        subscribe_Press(peripheral);
        subscribe_eCO2(peripheral);
        subscribe_TVOC(peripheral);
        Serial.println("Reading sensors...");
        Serial.println();
      } else Serial.println("Failed to connect!");
    }
    // main while connected loop
    while (peripheral.connected()) 
    {
      read_Batt(peripheral);
      read_Temp(peripheral);
      read_Humi(peripheral);
      read_Press(peripheral);
      read_eCO2(peripheral);
      read_TVOC(peripheral);
      print_data();
      lastMillis = millis();
      // stay here until the period is up
      while ( ( (nowMillis = millis()) - lastMillis) <= period);
    }
    // peripheral disconnected, start scanning again
    Serial.println("rescan...");
    BLE.scan();
  }
}

// discover the peripheral's attributes that we want
void do_discovery(BLEDevice peripheral) 
{
  // Battery
  Serial.print("Discovering attributes for Battery Service...");
  if (peripheral.discoverService("180f")) 
  {
    Serial.println("discovered");
    BattValCharacteristic = peripheral.characteristic("2a19");
  }
  else  
  {
    Serial.println("ERROR: Battery Service discovery failed.");
    peripheral.disconnect();
    return;
  }
  // Temperature
  Serial.print("Discovering attributes for Ambient Service(Temperature)...");
  if (peripheral.discoverService("00001120-702b-69b5-b243-d6094a2b0e24")) 
  {
    Serial.println("discovered");
    TempConCharacteristic = peripheral.characteristic("00001134-702b-69b5-b243-d6094a2b0e24");
    TempValCharacteristic = peripheral.characteristic("00001133-702b-69b5-b243-d6094a2b0e24");
  }
  else
  { 
    Serial.println("ERROR: Temperature Service failed");
    peripheral.disconnect();
    return;
  }
  // Humidity
  Serial.print("Discovering attributes for Ambient Service(Humidity)...");
  if (peripheral.discoverService("00001120-702b-69b5-b243-d6094a2b0e24")) 
  {
    Serial.println("discovered");
    HumiConCharacteristic = peripheral.characteristic("00001138-702b-69b5-b243-d6094a2b0e24");
    HumiValCharacteristic = peripheral.characteristic("00001137-702b-69b5-b243-d6094a2b0e24");
  }
  else
  { 
    Serial.println("ERROR: Humidity Service failed");
    peripheral.disconnect();
    return;
  } 
  // Pressure 
  Serial.print("Discovering attributes for Ambient Service(Pressure)...");
  if (peripheral.discoverService("00001120-702b-69b5-b243-d6094a2b0e24")) 
  {
    Serial.println("discovered");
    PressConCharacteristic = peripheral.characteristic("00001136-702b-69b5-b243-d6094a2b0e24");
    PressValCharacteristic = peripheral.characteristic("00001135-702b-69b5-b243-d6094a2b0e24");
  }
  else
  { 
    Serial.println("ERROR: Pressure Service failed");
    peripheral.disconnect();
    return;
  } 
  // Air Quality eCO2   
  Serial.print("Discovering attributes for Ambient Service(eCO2)...");
  if (peripheral.discoverService("00001120-702b-69b5-b243-d6094a2b0e24")) 
  {
    Serial.println("discovered");
    eCO2ConCharacteristic = peripheral.characteristic("00001140-702b-69b5-b243-d6094a2b0e24");
    eCO2ValCharacteristic = peripheral.characteristic("00001139-702b-69b5-b243-d6094a2b0e24");
  }
  else
  { 
    Serial.println("ERROR: eCO2 Service failed");
    peripheral.disconnect();
    return;
  }
  // Air Quality TVOC
  Serial.print("Discovering attributes for Ambient Service(TVOC)...");
  if (peripheral.discoverService("00001120-702b-69b5-b243-d6094a2b0e24")) 
  {
    Serial.println("discovered"); 
    TVOCConCharacteristic = peripheral.characteristic("00001142-702b-69b5-b243-d6094a2b0e24");
    TVOCValCharacteristic = peripheral.characteristic("00001141-702b-69b5-b243-d6094a2b0e24");
  }
  else  
  {
    Serial.println("ERROR: TVOC Service discovery failed.");
    peripheral.disconnect();
    return;
  }
}

// subscribe to each peripheral service that we want
void subscribe_Batt(BLEDevice peripheral)
{
  // subscribe to the barometric service
  Serial.print("Subscribing to the Battery data service  ...");
  if (!BattValCharacteristic) {
    Serial.println("Error: Battery data service characteristic not found!");
    peripheral.disconnect();
    return;
  } else if (!BattValCharacteristic.canSubscribe()) {
    Serial.println("Error: Battery data service characteristic not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!BattValCharacteristic.subscribe()) {
    Serial.println("Error: subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed to Battery pressure data service characteristic");
    delay(100);
  }
}

void subscribe_Temp(BLEDevice peripheral) {
  // subscribe to the Temperature service
  Serial.print("Subscribing to the Temperature data service...");
  if (!TempValCharacteristic) {
    Serial.println("Error: Temperature data service characteristic not found!");
    peripheral.disconnect();
    return;
  } else if (!TempValCharacteristic.canSubscribe()) {
    Serial.println("Error: Temperature data service characteristic not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!TempValCharacteristic.subscribe()) {
    Serial.println("Error: subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed to Temperature data service characteristic");
    delay(100);
  }
}

void subscribe_Humi(BLEDevice peripheral) {
  // subscribe to the Humidity service
  Serial.print("Subscribing to the Humidity data service...");
  if (!HumiValCharacteristic) {
    Serial.println("Error: Humidity data service characteristic not found!");
    peripheral.disconnect();
    return;
  } else if (!HumiValCharacteristic.canSubscribe()) {
    Serial.println("Error: Humidity data service characteristic not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!HumiValCharacteristic.subscribe()) {
    Serial.println("Error: subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed to Humidity data service characteristic");
    delay(100);
  }
}

void subscribe_Press(BLEDevice peripheral) {
  // subscribe to the Pressure service
  Serial.print("Subscribing to the Pressure data service...");
  if (!PressValCharacteristic) {
    Serial.println("Error: Pressure data service characteristic not found!");
    peripheral.disconnect();
    return;
  } else if (!PressValCharacteristic.canSubscribe()) {
    Serial.println("Error: Pressure data service characteristic not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!PressValCharacteristic.subscribe()) {
    Serial.println("Error: subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed to Pressure data service characteristic");
    delay(100);
  }
}

void subscribe_eCO2(BLEDevice peripheral) {
  // subscribe to the eCO2 service
  Serial.print("Subscribing to the eCO2 data service...");
  if (!eCO2ValCharacteristic) {
    Serial.println("Error: eCO2 data service characteristic not found!");
    peripheral.disconnect();
    return;
  } else if (!eCO2ValCharacteristic.canSubscribe()) {
    Serial.println("Error: eCO2 data service characteristic not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!eCO2ValCharacteristic.subscribe()) {
    Serial.println("Error: subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed to eCO2 service characteristic");
    delay(100);
  }
}

void subscribe_TVOC(BLEDevice peripheral) {
  // subscribe to the TVOC service
  Serial.print("Subscribing to the TVOC data service...");
  if (!TVOCValCharacteristic) {
    Serial.println("Error: TVOC data service characteristic not found!");
    peripheral.disconnect();
    return;
  } else if (!TVOCValCharacteristic.canSubscribe()) {
    Serial.println("Error: TVOC data service characteristic not subscribable!");
    peripheral.disconnect();
    return;
  } else if (!TVOCValCharacteristic.subscribe()) {
    Serial.println("Error: subscription failed!");
    peripheral.disconnect();
    return;
  } else {
    Serial.println("Subscribed to TVOC service characteristic");
    delay(100);
  }
}

// Sensor reads
void read_Batt(BLEDevice peripheral) 
{
  uint8_t batt;
  
  if (peripheral.connected()) 
  {
    BattValCharacteristic.readValue(batt);

    if (DEBUG) Serial.println(batt);
    
    // save into the structure
    SensorData.batt = batt;
  }
  else Serial.println(" *not connected* ");
}

void read_Temp(BLEDevice peripheral) 
{
  uint8_t holdvalues[16] = {0};
  
  // must be subscribed
  if (peripheral.connected()) 
  {
    // while the peripheral is connected, turn the temperature sensor on by writing a '1' to the characteristic
    TempConCharacteristic.writeValue((byte)0x61);
    delay(2000); // wait for the sensor to do a read

    TempValCharacteristic.readValue(holdvalues, 16);

    // Value: (int32_t) ((D_MSB << 24) | (D_B3 << 16) | (D_B2 << 8) | D_LSB)
    if (DEBUG)
    {
      for (int i = 15; i >=0; i--)
      { 
        Serial.print(holdvalues[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
    
    TempConCharacteristic.writeValue((byte)0x00); // turn the sensor off
    
    uint32_t rawTemp = ((uint32_t)holdvalues[7]) << 24 | ((uint32_t)holdvalues[6]) << 16 | ((uint32_t)holdvalues[5]) << 8 | (uint32_t)holdvalues[4];
    
    // calculate temperature final and
    float temp = ((float)rawTemp/100.); 
    if (DEBUG)
    {
      Serial.println(rawTemp, HEX);
      Serial.println(temp);
    }
    // save into the structure
    SensorData.temp = temp;
  }
  else Serial.println(" *not connected* ");
}

void read_Humi(BLEDevice peripheral) 
{
  uint8_t holdvalues[16] = {0};
  
  // must be subscribed
  if (peripheral.connected()) 
  {
    // while the peripheral is connected, turn the humidity sensor on by writing a '1' to the characteristic
    HumiConCharacteristic.writeValue((byte)0x61);
    delay(2000); // wait for the sensor to do a read

    HumiValCharacteristic.readValue(holdvalues, 16);

    // Value: (int32_t) ((D_MSB << 24) | (D_B3 << 16) | (D_B2 << 8) | D_LSB)
    if (DEBUG)
    {
      for (int i = 15; i >=0; i--)
      { 
        Serial.print(holdvalues[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
    
    HumiConCharacteristic.writeValue((byte)0x00); // turn the sensor off
    
    uint32_t rawHumi = ((uint32_t)holdvalues[7]) << 24 | ((uint32_t)holdvalues[6]) << 16 | ((uint32_t)holdvalues[5]) << 8 | (uint32_t)holdvalues[4];
    
    // calculate temperature final and
    float humi = ((float)rawHumi/1024.0); 
    if (DEBUG)
    {
      Serial.println(rawHumi, HEX);
      Serial.println(humi);
    }
    // save into the structure
    SensorData.humi = humi;
  }
  else Serial.println(" *not connected* ");
}

void read_Press(BLEDevice peripheral) 
{
  uint8_t holdvalues[16] = {0};
  
  // must be subscribed
  if (peripheral.connected()) 
  {
    // while the peripheral is connected, turn the pressure sensor on by writing a '1' to the characteristic
    PressConCharacteristic.writeValue((byte)0x61);
    delay(2000); // wait for the sensor to do a read

    PressValCharacteristic.readValue(holdvalues, 16);

    // Value: (int32_t) ((D_MSB << 24) | (D_B3 << 16) | (D_B2 << 8) | D_LSB)
    if (DEBUG)
    {
      for (int i = 15; i >=0; i--)
      { 
        Serial.print(holdvalues[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
    
    PressConCharacteristic.writeValue((byte)0x00); // turn the sensor off
    
    uint32_t rawPress = ((uint32_t)holdvalues[7]) << 24 | ((uint32_t)holdvalues[6]) << 16 | ((uint32_t)holdvalues[5]) << 8 | (uint32_t)holdvalues[4];
 
    // calculate pressure final and
    float press = ((float)rawPress/100.); 
    if (DEBUG)
    {
      Serial.println(rawPress, HEX);
      Serial.println(press);
    }
    // save into the structure
    SensorData.press = press;
  }
  else Serial.println(" *not connected* ");
}

void read_eCO2(BLEDevice peripheral) 
{
  uint8_t holdvalues[6];
  
  // must be subscribed
  if (peripheral.connected()) 
  {
    // while the peripheral is connected, turn the eCO2 sensor on by writing a '1' to the characteristic
    eCO2ConCharacteristic.writeValue((byte)0x41);
    delay(2000); // wait for the sensor to do a read

    eCO2ValCharacteristic.readValue(holdvalues, 6);

    // Value: (int32_t) ((D_MSB << 8) | D_LSB)
    if (DEBUG)
    {
      for (int i = 5; i >=0; i--)
      { 
        Serial.print(holdvalues[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
    
    eCO2ConCharacteristic.writeValue((byte)0x00); // turn the sensor off
    
    uint32_t raweCO2 = ((uint32_t)holdvalues[5]) << 8 | (uint32_t) holdvalues[4]; 
    
    // calculate pressure final and
    float eCO2 = ((float)raweCO2); 
    if (DEBUG) 
    {
      Serial.println(raweCO2, HEX);
      Serial.println(eCO2);
    }
    // save into the structure
    SensorData.eCO2 = eCO2;
  }
  else Serial.println(" *not connected* ");
}

void read_TVOC(BLEDevice peripheral) 
{
  uint8_t holdvalues[6];
  
  // must be subscribed
  if (peripheral.connected()) 
  {
    // while the peripheral is connected, turn the TVOC sensor on by writing a '1' to the characteristic
    TVOCConCharacteristic.writeValue((byte)0x41);
    delay(2000); // wait for the sensor to do a read

    TVOCValCharacteristic.readValue(holdvalues, 6);

    // Value: (int32_t) ((D_MSB << 8) | D_LSB)
    if (DEBUG)
    {
      for (int i = 5; i >=0; i--)
      { 
        Serial.print(holdvalues[i], HEX);
        Serial.print(" ");
      }
      Serial.println();
    }
    
    TVOCConCharacteristic.writeValue((byte)0x00); // turn the sensor off
    
    uint32_t rawTVOC = (uint32_t)(holdvalues[5]) << 8 | (uint32_t)holdvalues[4]; 
    
    // calculate pressure final and
    float TVOC = ((float)rawTVOC); 
    if (DEBUG)
    {
      Serial.println(rawTVOC, HEX);
      Serial.println(TVOC);
    }
    // save into the structure
    SensorData.TVOC = TVOC;
  }
  else Serial.println(" *not connected* ");
}

// Print the data to the serial monitor
void print_data() 
{
  Serial.print("------------------------------------\n");
  Serial.print("Device      = "); Serial.println(SensorData.devInfo); 
  Serial.print("Battery     = "); Serial.print(SensorData.batt); Serial.println(" %");
  Serial.print("Temperature = "); Serial.print(SensorData.temp,1); Serial.println(" °C");
  Serial.print("Humidity    = "); Serial.print(SensorData.humi,1); Serial.println(" %rH");
  Serial.print("Pressure    = "); Serial.print(SensorData.press, 0); Serial.println(" mbar");
  Serial.print("TVOC        = "); Serial.print(SensorData.TVOC, 0); Serial.println(" ppb");
  Serial.print("eCO2        = "); Serial.print(SensorData.eCO2, 0); Serial.println(" ppm");
  Serial.println("------------------------------------\n");
}
