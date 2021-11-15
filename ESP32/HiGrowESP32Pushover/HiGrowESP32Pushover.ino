/*
 * File: HiGrowESP32Pushover.ino
 * 
 * The ESP32 reads data from HiGrow device and publishes these via Pushover
 * Used Part: LILYGO®TTGO T-Higrow Soil Temperature And Humidity Photometric Electrolyte Sensor
 * http://www.lilygo.cn/prod_view.aspx?TypeId=50063&Id=1172
 * Board: ESP32 Dev Module
 * 
 * 2021-11-13 Claus Kühnel info@ckuehnel.ch
 */

#include <WiFi.h>
#include <Wire.h>
#include "arduino_secrets.h"

#define uS_TO_S_FACTOR 1000000
#define DEBUG 1

const int DEEPSLEEP_SECONDS = 1800; // 30 min
//const int DEEPSLEEP_SECONDS = 180; // 3 min

float bmeTemperature, bmeHumidity;
float bmePressure, bmeAltitude;
float lux, vBat;
int rssiValue;

uint16_t soilMoisture = 0;
const int airValue = 3459;      // adapt it to your conditions
const int waterValue = 1323;    // adapt it to your conditions

char deviceid[21];

const int I2C_SDA = 25;
const int I2C_SCL = 26;
const int BAT_ADC = 33;
const int POWER_EN = 4;
const int SOIL = 32;

#define DS18B20_PIN         21                  //18b20 data pin

WiFiClient client;

// Function prototypes
void initBME(void);
void getBMEValues(void);

void setup() 
{
  Serial.begin(115200);
  delay(2000); // wait for serial monitor
  Serial.println("\nLILYGO®TTGO T-Higrow Soil Temperature And Humidity Photometric Electrolyte Sensor");

  if (DEBUG) print_wakeup_reason();
  
  Serial.println("Inititialization...");

  esp_sleep_enable_timer_wakeup(DEEPSLEEP_SECONDS * uS_TO_S_FACTOR);

  uint64_t chipid = ESP.getEfuseMac();
  sprintf(deviceid, "%" PRIu64, chipid);
  Serial.print("ESP32 DeviceId: ");
  Serial.println(deviceid);

  Wire.begin(I2C_SDA, I2C_SCL);
  
  //! Sensor power enable pin, must set high
  pinMode(POWER_EN, OUTPUT);
  digitalWrite(POWER_EN, 1);
  delay(1000);

  initSensors();
  connectWiFi();
  Serial.println("Initialization done.\n");
}

void loop() 
{
  getValues();

  if (WiFi.status() != WL_CONNECTED) 
  {
    Serial.println("Reconnecting to WiFi...");
    ESP.restart();
  }

  rssiValue = WiFi.RSSI();
  if (DEBUG) Serial.print("RSSI = "); Serial.print(rssiValue); Serial.println(" dBm\n");

  String message  = "Temperature = ";
         message += String(bmeTemperature, 1);
         message += " °C\n";
         message += "Humidity = ";
         message += String(bmeHumidity, 0);
         message += " rH\n";
         message += "Pressure = ";
         message += String(bmePressure, 0);
         message += " hParH\n";
         message += "Altitude = ";
         message += String(bmeAltitude, 1);
         message += " m\n";
         message += "Soil Moisture = ";
         message += String(soilMoisture);
         message += " (0-100)\n";
         message += "Illuminance = ";
         message += String(lux,0);
         message += " lx\n";
         message += "Battery Voltage = ";
         message += String(vBat,3);
         message += " V\n";
         message += "RSSI = ";
         message += String(rssiValue);
         message += " dBm";
  char arr[message.length() + 1];
  for (int i = 0; i < sizeof(arr); i++) arr[i] = message[i]; 
  pushover(arr, 0);

  Serial.println("Going to sleep now");
  delay(1000);
  Serial.flush(); 
  esp_deep_sleep_start();

  //delay(60000);               // used for test
}
