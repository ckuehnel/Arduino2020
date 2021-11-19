/*
 * File: M5Atom_Display.ino
 * 
 * Test of I2C-driven M5Stack Displays
 * - M5UnitOLED     https://docs.m5stack.com/en/unit/oled
 * - M5UnitLCD      https://docs.m5stack.com/en/unit/lcd
 * - M5AtomDisplay  https://docs.m5stack.com/en/atom/atom_display
 * 
 * based on TFT_graphictest_PDQ.ino aus der TFT_eSPI-Library von Bodmer
 * used board: M5Atom
 * 
 * 2021-11-19 Claus Kühnel info@ckuehnel.ch
 */

#include "M5Atom.h"
#include <M5GFX.h>
#include "image.h"
//M5GFX display;

//#include <M5UnitLCD.h>
//M5UnitLCD display ( 26, 32, 400000 ); // SDA, SCL, FREQ

//#include <M5AtomDisplay.h>
//M5AtomDisplay display;

#include <M5UnitOLED.h>
M5UnitOLED display ( 26, 32, 400000 ); // SDA, SCL, FREQ

void setup()
{
  // Clear the serial port buffer and set the serial port baud rate to 115200. Initialize I2C. Initialize the LED matrix not.
  M5.begin(/* Serial */ true, /* I2C */ true, /* Matrix */ false); 
  display.begin();

  Serial.print("M5Stack Display Test: "); 
  uint8_t d = display.getBoard();
  switch (d)
  {
    case 15: Serial.println("M5UnitOLED"); break;
    case 14: Serial.println("M5UnitLCD"); break;
    case 13: Serial.println("M5AtomDisplay"); break;
    default: Serial.println("Unknown display");
  } 
  Serial.println(" Screen height = " + String(display.height()) + " Pixel");
  Serial.println(" Screen width  = " + String(display.width())  + " Pixel");
  
  if (display.width() < display.height())
  {
    display.setRotation(display.getRotation() ^ 1);
  }
}

void loop()
{
  display.clearDisplay();
  display.setTextDatum(textdatum_t::middle_center);
  display.drawString("M5Stack Display Test", display.width()/2, display.height()/2);
  display.setTextDatum(textdatum_t::top_left);
  delay(1000);
  Serial.print(F("Test JPG         : "));
  Serial.print(testJPG()); Serial.println (" us");
  delay(1000);
  Serial.print(F("Fill screen      : "));
  Serial.print(testFillScreen()); Serial.println (" us");
  delay(1000);
  Serial.print(F("Text             : "));
  Serial.print(testText()); Serial.println (" us");
  delay(1000);
  Serial.print(F("Pixels           : "));
  Serial.print(testPixels()); Serial.println (" us");
  delay(1000);
  Serial.print(F("Lines            : "));
  Serial.print(testLines(TFT_BLUE)); Serial.println (" us");
  delay(1000);
  Serial.print(F("Fast Lines       : "));
  Serial.print(testFastLines(TFT_RED, TFT_BLUE)); Serial.println (" us");
  delay(1000);
  Serial.print(F("Rectangles       : "));
  Serial.print(testRects(TFT_GREEN)); Serial.println (" us");
  delay(1000);
  Serial.print(F("Filled Rectangles: "));
  Serial.print(testFilledRects(TFT_YELLOW, TFT_MAGENTA)); Serial.println (" us");
  delay(1000); 
  Serial.print(F("Circles          : "));
  Serial.print(testCircles(10, TFT_WHITE)); Serial.println (" us");
  delay(1000);
  Serial.print(F("Filled Circles   : "));
  Serial.print(testFilledCircles(10, TFT_MAGENTA)); Serial.println (" us");
  delay(1000);
  Serial.print(F("Triangles        : "));
  Serial.print(testTriangles()); Serial.println (" us");
  delay(1000);
  Serial.print(F("Filled Triangles : "));
  Serial.print(testFilledTriangles()); Serial.println (" us");
  delay(1000);  
  Serial.print(F("Rounded rect     : "));
  Serial.print(testRoundRects()); Serial.println (" us");
  delay(1000); 
  Serial.print(F("Rounded rects (filled): "));
  Serial.print(testFilledRoundRects()); Serial.println (" us");
  delay(1000); 
  Serial.println();
}

static inline uint32_t micros_start() __attribute__ ((always_inline));
static inline uint32_t micros_start()
{
  uint8_t oms = millis();
  while ((uint8_t)millis() == oms);
  return micros();
}

uint32_t testJPG(void)
{
  uint32_t start = micros_start();
  display.fillScreen(TFT_BLACK);

  display.drawJpg( jpg  // data_pointer
                 , ~0u  // data_length (~0u = auto)
                 , 0    // X position
                 , 0    // Y position
                 , display.width()  // Width
                 , display.height() // Height
                 , 0    // X offset
                 , 0    // Y offset
                 , 1  // X magnification(default = 1.0 , 0 = fitsize , -1 = follow the Y magni)
                 , 1  // Y magnification(default = 1.0 , 0 = fitsize , -1 = follow the X magni)
                 , datum_t::middle_center
                 );

  uint32_t t = micros() - start;
  delay(3000);
  return t;
}
  
uint32_t testFillScreen()
{
  uint32_t start = micros_start();
    
    display.fillScreen(TFT_WHITE);
    display.fillScreen(TFT_RED);
    display.fillScreen(TFT_GREEN);
    display.fillScreen(TFT_BLUE);
    display.fillScreen(TFT_BLACK);

  return (micros() - start)/5;
}

uint32_t testText()
{
  display.fillScreen(TFT_BLACK);
  uint32_t start = micros_start();
  display.setCursor(0, 0);
  display.setTextColor(TFT_WHITE,TFT_BLACK);  display.setTextSize(1);
  display.println(F("Hello World!"));
  display.setTextSize(2);
  display.setTextColor(display.color565(0xff, 0x00, 0x00));
  display.print(F("RED "));
  display.setTextColor(display.color565(0x00, 0xff, 0x00));
  display.print(F("GREEN "));
  display.setTextColor(display.color565(0x00, 0x00, 0xff));
  display.println(F("BLUE"));
  display.setTextColor(TFT_YELLOW); display.setTextSize(2);
  display.println(1234.56);
  display.setTextColor(TFT_RED);    display.setTextSize(3);
  display.println(0xDEADBEEF, HEX);
  display.println();
  display.setTextColor(TFT_GREEN);
  display.setTextSize(5);
  display.println(F("Groop"));
  display.setTextSize(2);
  display.println(F("I implore thee,"));
  display.setTextColor(TFT_GREEN);
  display.setTextSize(1);
  display.println(F("my foonting turlingdromes."));
  display.println(F("And hooptiously drangle me"));
  display.println(F("with crinkly bindlewurdles,"));
  display.println(F("Or I will rend thee"));
  display.println(F("in the gobberwarts"));
  display.println(F("with my blurglecruncheon,"));
  display.println(F("see if I don't!"));
  display.println(F(""));
  display.println(F(""));
  display.setTextColor(TFT_MAGENTA);
  display.setTextSize(6);
  display.println(F("Woot!"));
  uint32_t t = micros() - start;
  return t;
}

uint32_t testPixels()
{
  int32_t w = display.width();
  int32_t h = display.height();

  uint32_t start = micros_start();
  display.startWrite();
  for (uint16_t y = 0; y < h; y++)
  {
    for (uint16_t x = 0; x < w; x++)
    {
      display.drawPixel(x, y, display.color565(x<<3, y<<3, x*y));
    }
  }
  display.endWrite();
  return micros() - start;
}

uint32_t testLines(uint16_t color)
{
  uint32_t start, t;
  int32_t x1, y1, x2, y2;
  int32_t w = display.width();
  int32_t h = display.height();

  display.fillScreen(TFT_BLACK);

  x1 = y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    display.drawLine(x1, y1, x2, y2, color);
  }

  x2 = w - 1;

  for (y2 = 0; y2 < h; y2 += 6)
  {
    display.drawLine(x1, y1, x2, y2, color);
  }

  t = micros() - start; // fillScreen doesn't count against timing

  display.fillScreen(TFT_BLACK);

  x1 = w - 1;
  y1 = 0;
  y2 = h - 1;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    display.drawLine(x1, y1, x2, y2, color);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    display.drawLine(x1, y1, x2, y2, color);
  }

  t += micros() - start;

  display.fillScreen(TFT_BLACK);

  x1 = 0;
  y1 = h - 1;
  y2 = 0;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    display.drawLine(x1, y1, x2, y2, color);
  }
  x2 = w - 1;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    display.drawLine(x1, y1, x2, y2, color);
  }
  t += micros() - start;

  display.fillScreen(TFT_BLACK);

  x1 = w - 1;
  y1 = h - 1;
  y2 = 0;

  start = micros_start();

  for (x2 = 0; x2 < w; x2 += 6)
  {
    display.drawLine(x1, y1, x2, y2, color);
  }

  x2 = 0;
  for (y2 = 0; y2 < h; y2 += 6)
  {
    display.drawLine(x1, y1, x2, y2, color);
  }

  t += micros() - start;

  return t;
}

uint32_t testFastLines(uint16_t color1, uint16_t color2)
{
  uint32_t start;
  int32_t x, y;
  int32_t w = display.width();
  int32_t h = display.height();

  display.fillScreen(TFT_BLACK);

  start = micros_start();

  for (y = 0; y < h; y += 5)
    display.drawFastHLine(0, y, w, color1);
  for (x = 0; x < w; x += 5)
    display.drawFastVLine(x, 0, h, color2);

  return micros() - start;
}

uint32_t testRects(uint16_t color)
{
  uint32_t start;
  int32_t n, i, i2;
  int32_t cx = display.width() / 2;
  int32_t cy = display.height() / 2;

  display.fillScreen(TFT_BLACK);
  n = min(display.width(), display.height());
  start = micros_start();
  for (i = 2; i < n; i += 6)
  {
    i2 = i / 2;
    display.drawRect(cx-i2, cy-i2, i, i, color);
  }

  return micros() - start;
}
uint32_t testFilledRects(uint16_t color1, uint16_t color2)
{
  uint32_t start, t = 0;
  int32_t n, i, i2;
  int32_t cx = display.width() / 2 - 1;
  int32_t cy = display.height() / 2 - 1;

  display.fillScreen(TFT_BLACK);
  n = min(display.width(), display.height());
  for (i = n; i > 0; i -= 6)
  {
    i2 = i / 2;

    start = micros_start();

    display.fillRect(cx-i2, cy-i2, i, i, color1);

    t += micros() - start;

    // Outlines are not included in timing results
    display.drawRect(cx-i2, cy-i2, i, i, color2);
  }

  return t;
}

uint32_t testFilledCircles(uint8_t radius, uint16_t color)
{
  uint32_t start;
  int32_t x, y, w = display.width(), h = display.height(), r2 = radius * 2;

  display.fillScreen(TFT_BLACK);

  start = micros_start();

  for (x = radius; x < w; x += r2)
  {
    for (y = radius; y < h; y += r2)
    {
      display.fillCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

uint32_t testCircles(uint8_t radius, uint16_t color)
{
  uint32_t start;
  int32_t x, y, r2 = radius * 2;
  int32_t w = display.width() + radius;
  int32_t h = display.height() + radius;

  display.fillScreen(TFT_BLACK);
  
  start = micros_start();

  for (x = 0; x < w; x += r2)
  {
    for (y = 0; y < h; y += r2)
    {
      display.drawCircle(x, y, radius, color);
    }
  }

  return micros() - start;
}

uint32_t testTriangles()
{
  uint32_t start;
  int32_t n, i;
  int32_t cx = display.width()/ 2 - 1;
  int32_t cy = display.height() / 2 - 1;

  display.fillScreen(TFT_BLACK);
  n = min(cx, cy);

  start = micros_start();

  for (i = 0; i < n; i += 5)
  {
    display.drawTriangle(
      cx    , cy - i, // peak
      cx - i, cy + i, // bottom left
      cx + i, cy + i, // bottom right
      display.color565(0, 0, i));
  }

  return micros() - start;
}

uint32_t testFilledTriangles()
{
  uint32_t start, t = 0;
  int32_t i;
  int32_t cx = display.width() / 2 - 1;
  int32_t cy = display.height() / 2 - 1;

  display.fillScreen(TFT_BLACK);

  start = micros_start();

  for (i = min(cx,cy); i > 10; i -= 5) {
    start = micros_start();
    display.fillTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      display.color565(0, i, i));
    t += micros() - start;
    display.drawTriangle(cx, cy - i, cx - i, cy + i, cx + i, cy + i,
      display.color565(i, i, 0));
  }

  return t;
}

uint32_t testRoundRects()
 {
  uint32_t start;
  int32_t w, i, i2;
  int32_t cx = display.width() / 2 - 1;
  int32_t cy = display.height() / 2 - 1;

  display.fillScreen(TFT_BLACK);
  
  w = min(display.width(), display.height());
  
  start = micros_start();

  for (i = 0; i < w; i += 6)
  {
    i2 = i / 2;
    display.drawRoundRect(cx-i2, cy-i2, i, i, i/8, display.color565(i, 0, 0));
  }

  return micros() - start;
}

uint32_t testFilledRoundRects()
{
  uint32_t start;
  int32_t i, i2;
  int32_t cx = display.width() / 2 - 1;
  int32_t cy = display.height() / 2 - 1;

  display.fillScreen(TFT_BLACK);

  start = micros_start();

  for (i = min(display.width(), display.height()); i > 20; i -= 6)
  {
    i2 = i / 2;
    display.fillRoundRect(cx-i2, cy-i2, i, i, i/8, display.color565(0, i, 0));
  }

  return micros() - start;
}
