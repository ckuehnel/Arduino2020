/*
 * File: M5StickC_NCIR.ino
 * 
 * NCIR Hat connected to M5StickC to measure object temperatures contectless
 * 
 * 2020-09-04 Claus Kühnel (info@ckuehnel.ch)
 */

#include <M5StickC.h>
#include <Wire.h>

#define INFO 0
#define DEBUG 1
#define BAUDRATE 115200

const uint16_t PERIOD = 10000;
unsigned long now = millis();
uint16_t result;
float temperature;

void setup() 
{
  Serial.begin(BAUDRATE);
  delay(2000);              // wait for serial monitor
  if (INFO)  printInfo();
  
  if (DEBUG) 
  {
    Serial.println(F("\nMeasuring Object Temperature by NCIR Hat on M5StickC..."));
    Serial.println(F("\nInitializing..."));
  }

  M5.begin();
  M5.Lcd.begin();
  M5.Lcd.setRotation(0);
  M5.Lcd.fillScreen( BLACK );
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.setCursor(0, 10, 1);
  M5.Lcd.setTextSize(2);
  M5.Lcd.println("ObjectTemp  in *C");

  
 

 
  if (DEBUG) Serial.println(F("Running...")); //last line in setup()
}

void loop() 
{
  Wire.begin(0,26); // external I2C Bus
  Wire.beginTransmission(0x5A);          // Send Initial Signal and I2C Bus Address
  Wire.write(0x07);                      // Send data only once and add one address automatically.
  Wire.endTransmission(false);           // Stop signal
  Wire.requestFrom(0x5A, 2);             // Get 2 consecutive data from 0x5A, and the data is stored only.
  result = Wire.read();                  // Receive DATA
  result |= Wire.read() << 8;            // Receive DATA
  
  temperature = result * 0.02 - 273.15;

  if (DEBUG) Serial.printf("Temperature = %.1f *C\n", temperature);
   
  M5.Lcd.fillRect(0, 80, 160,24, BLACK);
  M5.Lcd.setCursor(5, 80, 1);
  M5.Lcd.setTextSize(3);
  M5.Lcd.setTextColor(WHITE);
  M5.Lcd.print(temperature,1);

  if (millis() - now > PERIOD)
  {
    now = millis();
    Wire.begin(); // internal I2C Bus
    float vbat = M5.Axp.GetVbatData() * 1.1 / 1000;
    M5.Lcd.fillRect(0, 136, 160, 20, BLACK);
    M5.Lcd.setCursor(0, 136, 1);
    M5.Lcd.setTextSize(1);
    m5.Lcd.setTextColor(GREEN);
    M5.Lcd.printf("Vbat %.3f V\r\n", vbat);
    int charge = M5.Axp.GetIchargeData() / 2;
    M5.Lcd.printf("Icharge %d mA\r\n", charge);
    Serial.printf("Vbat: %.3f V  Icharge: %d mA\r\n",vbat, charge);
  }
  M5.update();
  delay(1000);
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
