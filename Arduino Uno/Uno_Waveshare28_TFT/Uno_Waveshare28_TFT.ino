/*
 * File: Uno_Waveshare28_TFT.ino
 * This program shows alpha-numeric output as an overlay
 * of a background image loaded from microSD card.
 * 
 * Claus Kühnel 2020-03-09 info@ckuehnel.ch
 * 
 * Waveshare HX8347D shield has microSD on 3x2 SPI header.  SD_CS on pin 5
 * Uno SPI is shared with 11,12,13.  
 * Copy all your BMP files to the root directory on the microSD with your PC
 * 
 */
#include <SPI.h>            // Wavesgare 2.8" TFT Touch shield uses SPI 
#include <SD.h>             // Use the official SD library on hardware pins
#include <HX8347D_kbv.h>    // https://github.com/prenticedavid/HX8347D_kbv
#include <XPT2046_Touchscreen.h>
#include <Adafruit_GFX.h>   // Hardware-specific library
#include <Fonts/FreeSans9pt7b.h>

HX8347D_kbv tft;

const int PIN_SD_CS = 5;    // CS pin of sd card

#define LIGHT_BLUE     0X64FB           //Light blue background color

#define BUFFPIXEL      20    //increase buffer if sufficient SRAM

// regular 24-bit colour BMP (and 16-bit) are always supported
//#define PALETTEDEPTH   0     // no Palette.
#define PALETTEDEPTH   4     // support 2-color, 16-color Palette
//#define PALETTEDEPTH   8     // support 256-colour Palette.  Unsafe for Uno.

char filename[] = "/Station.bmp";    // File name of background image

File bmpFile;

void setup()
{ 
  Serial.begin(115200);
  delay(1000);  // wait for Serial Monitor
  Serial.println("\nDisplay data w/ Waveshare 2.8 Touch TFT");
    
  // Initialize SD Card
  pinMode(PIN_SD_CS,OUTPUT);
  digitalWrite(PIN_SD_CS,HIGH);
  Sd2Card card;
  card.init(SPI_FULL_SPEED, PIN_SD_CS); 
  if(!SD.begin(PIN_SD_CS))  
  { 
    Serial.println("SD init failed!");
    while(1);                               // init fail, die here
  }
  else Serial.println("SD OK!");

  // Initialize SPI
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.begin();

  uint16_t ID = tft.readID(); //
  tft.begin(ID);

  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.setFont(&FreeSans9pt7b);
  // tft.setTextSize(1);
  tft.setCursor(50, 120); tft.println("Starting");
  tft.setCursor(50, 140); tft.println("Weather Station");
  tft.setCursor(50, 160); tft.println("Display... ");
  delay(1000);

  Serial.print("Open "); Serial.println(filename);
  showBMP(filename, 0, 0);
  Serial.println("Done.");

  tft.setCursor(35, 255); tft.print("Local Weather Station");
  tft.setCursor(10, 280); tft.print("Temperature: ");
  tft.setCursor(47, 300); tft.print("Humidty: ");
}

void loop()
{
  float temp = getTemperature();
  float humi = getHumidity();
  
  char buffer [10];
  dtostrf(temp, 3, 1, buffer);
  strcat (buffer," *C");
  tft.fillRect(120, 267, 80, 16, LIGHT_BLUE); 
  tft.setCursor(120, 280); tft.print(buffer);
  Serial.print("\nTemperature = \t"); Serial.println(buffer);

  dtostrf((int) humi, 2, 0, buffer);
  strcat (buffer," % RH");
  tft.fillRect(120, 287, 80, 16, LIGHT_BLUE); 
  tft.setCursor(120, 300); tft.print(buffer);
  Serial.print("Humidity = \t"); Serial.println(buffer);

  delay(1000);
}
