/*
 * File: Uno_Waveshare28_Touch.ino
 * This program shows graphic output and aksk for
 * using Touch. Differences in positions are display on Serial Monitor
 * 
 * Claus Kühnel 2020-03-09 info@ckuehnel.ch
 * 
 * Waveshare HX8347D shield has microSD on 3x2 SPI header.  SD_CS on pin 5
 * Uno SPI is shared with 11,12,13.  
 * Copy all your BMP files to the root directory on the microSD with your PC
 * 
 */
#include <SPI.h>            // Wavesgare 2.8" TFT Touch shield uses SPI 
#include <HX8347D_kbv.h>    // https://github.com/prenticedavid/HX8347D_kbv
#include "Adafruit_GFX.h"   // Hardware-specific library
#include <XPT2046_Touchscreen.h>

HX8347D_kbv tft;

#define XPT_CS  4
#define XPT_IRQ 255       //use 3 if you fix interrupts in library source code

#define TS_LEFT 3900      //The XPT2046_Touchscreen works in Landscape
#define TS_RT   300       //I would expect Touch in Portrait
#define TS_TOP  360
#define TS_BOT  3800

XPT2046_Touchscreen ts(XPT_CS, XPT_IRQ);

void setup()
{ 
  Serial.begin(115200);
  delay(1000);  // wait for Serial Monitor
  Serial.println("\nDisplay data position data on Waveshare 2.8 Touch TFT for using Touch.");
    
  // Initialize SPI
  SPI.setDataMode(SPI_MODE3);
  SPI.setBitOrder(MSBFIRST);
  SPI.setClockDivider(SPI_CLOCK_DIV4);
  SPI.begin();

  uint16_t ID = tft.readID(); //
  tft.begin(ID);
  tft.setRotation(1);   //LANDSCAPE
  tft.fillScreen(TFT_BLUE);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.setCursor(70,  90); tft.println("    Starting   ");
  tft.setCursor(70, 110); tft.println("   Touch Demo  ");
  tft.setCursor(70, 130); tft.println("      ...      ");
  delay(2000);
  tft.setTextColor(TFT_RED);
  tft.setTextSize(1);
}

void loop()
{
  tft.fillScreen(TFT_WHITE);
  tft.setCursor(10, 230); tft.println("Press Touch near marked point...");
  int x = getX();
  int y = getY();
  draw_big_point(x, y, TFT_RED);
  while (!ts.touched());  // Threshold adapted in XP
  TS_Point p = ts.getPoint(); //XPT_2046_touchscreen returns in Landscape
  uint16_t ym = map(p.y, TS_TOP, TS_BOT, 0, tft.height());
  uint16_t xm = map(p.x, TS_LEFT, TS_RT, 0, tft.width());
  Serial.print("(");
  Serial.print(x); Serial.print(","); 
  Serial.print(y); Serial.print(")\t");
  Serial.print("(");
  Serial.print(xm); Serial.print(","); 
  Serial.print(ym); Serial.print(")\t");
  Serial.print("Pressure = "); Serial.println(p.z);

}
