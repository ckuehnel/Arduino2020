/*
 * File Uno_Nokia5110.ino
 * include library from https://github.com/carlosefr/pcd8544
 * based on Thermometer.ino included in the library
 */
#include <PCD8544.h>

/*
 * Connections: (otherwise change PCD8544.h)
 * Display Pin | RST | CE | DC | DIN | CLK | VCC  | LIGHT | GND
 * Arduino Pin |  6  |  7 |  5 |  4  |  3  | 3.3V |  13   | GND
 */
 
#define STARTSCREEN 1 // set to display a start screen 

// Controlling backlight
static const byte ledPin = 13;

// The dimensions of the LCD (in pixels)...
static const byte LCD_WIDTH = 84;
static const byte LCD_HEIGHT = 48;

// The number of lines for the temperature chart...
static const byte CHART_HEIGHT = 5;

// A custom "degrees" symbol...
static const byte DEGREES_CHAR = 1;
static const byte degrees_glyph[] = 
  { 0x00, 0x07, 0x05, 0x07, 0x00 };

// A bitmap graphic (10x2) of a thermometer...
static const byte THERMO_WIDTH = 10;
static const byte THERMO_HEIGHT = 2;
static const byte thermometer[] = 
  { 0x00, 0x00, 0x48, 0xfe, 0x01, 0xfe, 0x00, 0x02, 0x05, 0x02,
    0x00, 0x00, 0x62, 0xff, 0xfe, 0xff, 0x60, 0x00, 0x00, 0x00};

static PCD8544 lcd;

// Function prototype
void displayStartScreen(void);

void setup() {
  lcd.begin(LCD_WIDTH, LCD_HEIGHT);
  
  // Register the custom symbol...
  lcd.createChar(DEGREES_CHAR, degrees_glyph);
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW);

  randomSeed(analogRead(0));

  if (STARTSCREEN) displayStartScreen();
}


void loop() {
  // Start beyond the edge of the screen...
  static byte xChart = LCD_WIDTH;
  
  //digitalWrite(ledPin, HIGH);

  // Simulate room temperature (in celsius)...
  float temp = 25 + random(-5, 5)/10.0;
  
  // Print the temperature (using the custom "degrees" symbol)...
  lcd.setCursor(0, 0);
  lcd.print("Temp: ");
  lcd.print(temp, 1);
  lcd.print(" \001C ");

  // Draw the thermometer bitmap at the bottom left corner...
  lcd.setCursor(0, LCD_HEIGHT/8 - THERMO_HEIGHT);
  lcd.drawBitmap(thermometer, THERMO_WIDTH, THERMO_HEIGHT);

  // Wrap the chart's current position...
  if (xChart >= LCD_WIDTH) {
    xChart = THERMO_WIDTH + 2;
  }

  // Update the temperature chart...  
  lcd.setCursor(xChart, 1);
  lcd.drawColumn(CHART_HEIGHT, map(temp, 0, 45, 0, CHART_HEIGHT*8));  // ...clipped to the 0-45C range.
  lcd.drawColumn(CHART_HEIGHT, 0);         // ...with a clear marker to see the current chart position.
  
  xChart++;

  //digitalWrite(ledPin, LOW);  
  delay(500);
}
