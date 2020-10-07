/*
 * File: M5StickC_PIR.ino
 * 
 * PIR Hat connected to M5StickC to count objects
 * 
 * 2020-08-31 Claus Kühnel (info@ckuehnel.ch)
 */

#include <M5StickC.h>

#define INFO 0
#define DEBUG 1
#define BAUDRATE 115200

const int pLED = 10;
const uint16_t PERIOD = 10000;
unsigned long now = millis();
uint16_t count = 0;

void setup() 
{
  Serial.begin(BAUDRATE);
  delay(2000);              // wait for serial monitor
  if (INFO)  printInfo();
  
  if (DEBUG) 
  {
    Serial.println(F("Object counting by PIR Hat on M5StickC..."));
    Serial.println(F("\nInitializing..."));
  }

  M5.begin();
  M5.Lcd.begin();
  M5.Lcd.setRotation(0);
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setCursor(0, 10, 1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("ObjectCount");

  Wire.begin(); // internal I2C Bus

  pinMode(pLED, OUTPUT); digitalWrite(pLED, HIGH);
  pinMode(36,INPUT_PULLUP);
  
  if (DEBUG) Serial.println(F("Running...")); //last line in setup()
}

void loop() 
{
  M5.Lcd.setCursor(10, 80, 1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.setTextColor(WHITE);
  
  if (digitalRead(36)) 
  {
    M5.Lcd.fillRect(0, 80, 160,16, BLACK);
    M5.Lcd.println(++count);
  }
  
  while(digitalRead(36))
  {
    digitalWrite(pLED, LOW);
    delay(500);
    digitalWrite(pLED, HIGH);
    delay(500);
  }
  if (millis() - now > PERIOD)
  {
    now = millis();
    float vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
    M5.Lcd.fillRect(0, 136, 160,16, BLACK);
    M5.Lcd.setCursor(0, 136, 1);
    M5.Lcd.setTextSize(1);
    m5.Lcd.setTextColor(GREEN);
    M5.Lcd.printf("Vbat %.3f V\r\n", vbat);
    int charge = M5.Axp.GetIchargeData() / 2;
    M5.Lcd.printf("Icharge %d mA\r\n", charge);
    Serial.printf("Vbat: %.3f V  Icharge: %d mA\r\n",vbat, charge);
  }
  M5.update();
}

/*--------------------------------------------------------------*/

void printInfo()
{
  // Compile file path (internal file of compilation proccess)
  Serial.print("\nFile "); Serial.println(__FILE__);
  
  String s1 = __DATE__;
  String s2 = __TIME__;

  // Date of compilation
  Serial.print("Compilation @ "); Serial.println(s1 + " " + s2);
  
  // Arduino IDE SW version
  Serial.print("ARDUINO IDE v"); Serial.println(ARDUINO);
  
  // Oscillator frequency
  Serial.print("CPU Clock in MHz: "); Serial.println(F_CPU/1E6);
}
