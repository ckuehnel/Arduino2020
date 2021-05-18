/*
 * File: RP2040_WiFi.ino
 * 
 * Measuring air condition by Sensirion SGP30 and RP2040. 
 * ESP-01 connected to hardware UART for WiFi connection to Thingspeak.
 * Using https://github.com/earlephilhower/arduino-pico
 * needs several Adafruit libraries and https://github.com/bportaluri/WiFiEsp/, too.
 * 
 * 2021-05-17 Claus Kühnel info@ckuehnel.ch
 */

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Adafruit_SGP30.h>
#include <Adafruit_NeoPixel.h>
#include <WiFiEsp.h>
#include <WiFiEspClient.h>
#include "arduino_secrets.h"

unsigned long now = 0;
const unsigned long PERIODE = 60000;
uint16_t TVOC, eCO2;
int status = WL_IDLE_STATUS;     // the Wifi radio's status

// Which pin on the Arduino is connected to the NeoPixels?
#define PIN       28 // for Maker Pi Pico Board

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 1 // one Neopixel on board

Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire);

Adafruit_SGP30 sgp;

// Initialize the Ethernet client object
WiFiEspClient client;

void setup() 
{
  Serial.begin(115200);
  delay(5000);
  Serial.println(F("\nRP2040 Air Condition Measurement"));

  //I2C0 for SSD1306 connection
  Wire.setSDA(0);
  Wire.setSCL(1);
  Wire.begin();

  // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
  if(!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS)) 
  {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);  // Don't proceed, loop forever
  }

  // Prepare the display buffer
  display.clearDisplay();
  display.setTextSize(1);               // Normal 1:1 pixel scale
  display.setTextColor(SSD1306_WHITE);  // Draw white text
  display.setCursor(0,0);               // Start at top-left corner
  display.println(F("Raspberry Pi Pico"));
  display.println(F("Measure Air Condition"));
  display.println(F("by Sensirion SGP30"));
  display.display();

  // I2C1 for SGP30 connection
  Wire1.setSDA(2);
  Wire1.setSCL(3);
  Wire1.begin();

  if (!sgp.begin(&Wire1))
  {
    Serial.println(F("SGP30 Sensor not found"));
    while (1); 
  }

  Serial.print("Found SGP30 serial #");
  Serial.print(sgp.serialnumber[0], HEX);
  Serial.print(sgp.serialnumber[1], HEX);
  Serial.println(sgp.serialnumber[2], HEX);

  // Serial1 for ESP-01 connection
  Serial1.setRX(17);
  Serial1.setTX(16);
  Serial1.begin(115200);
  WiFi.init(&Serial1);

   // check for the presence of the ESP-01
  if (WiFi.status() == WL_NO_SHIELD) 
  {
    Serial.println("ESP-01 shield not present");
    while (1); // Don't proceed, loop forever
  }
  
  // attempt to connect to WiFi network
  while ( status != WL_CONNECTED) 
  {
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    status = WiFi.begin(ssid, pass);
  }

  // you're connected now, so print out the data
  Serial.println("You're connected to the network");
  
  printWifiStatus();
  
  Serial.println(F("Initialization finished."));
}

void loop() 
{
  if (!sgp.IAQmeasure()) 
  {
    Serial.println("Measurement failed");
    return;
  }

  TVOC = sgp.TVOC; 
  eCO2 = sgp.eCO2;
  
  Serial.print("TVOC "); Serial.print(TVOC); Serial.print(" ppb\t");
  Serial.print("eCO2 "); Serial.print(eCO2); Serial.println(" ppm");

  display.fillRect(0, 30, display.width(), 16, SSD1306_BLACK);
  display.setCursor(0,30);
  display.println("TVOC " + String(TVOC) + " ppb");
  display.println("eCO2 " + String(eCO2) + " ppm");
  display.display();

  if (eCO2 <= 800)
  {
    pixels.setPixelColor(0, pixels.Color(0, 80, 0));
    pixels.show();   
  }
  else if ((eCO2 > 800) && (eCO2 <= 1200))
  { 
    pixels.setPixelColor(0, pixels.Color(126, 100, 2));
    pixels.show();
  }
  else
  {
    pixels.setPixelColor(0, pixels.Color(100, 0, 0));
    pixels.show();
  }

  // if there's incoming data from the net connection send it out the serial port
  // this is for debugging purposes only
  while (client.available()) 
  {
    char c = client.read();
    Serial.write(c);
  }
  
  if (millis() > now + PERIODE) 
  {
    now = millis();
    sendMessage();
  }
  else delay(5000);

}
