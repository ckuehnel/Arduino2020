/*
 * File: PaperiNode_SCap_Voltage_LORaWAN.ino
 * 
 * Based on: https://github.com/RobPo/PaperiNode/blob/master/examples/04_MeasVScap/04_MeasVScap.ino
 * 
 */
#include "paperinode.h"
#include "mcp7940.h"
#include "DS2401.h"
#include "spi_flash.h"
#include "RFM95.h"
#include "LoRaMAC.h"
#include "lorawan_def.h"
#include "PL_microEPD44.h"
#include <Adafruit_BMP280.h>

#define DEBUG 0
#define CYCLE 30      // LoRaWAN message cycle
#define SENDLEVEL 605 // means 6.6 * 580/1024 = 3.9 V 

/******** GLOBAL VARIABLES ****************************************************************/
const uint8_t  BMP_SCK  = 13;
const uint8_t  BMP_MISO = 12;
const uint8_t  BMP_MOSI = 11;
const uint8_t  BMP_CS   = 7;

Adafruit_BMP280 bmp(BMP_CS); // hardware SPI
//Adafruit_BMP280 bmp(BMP_CS, BMP_MOSI, BMP_MISO,  BMP_SCK);

PL_microEPD epd(EPD_CS, EPD_RST, EPD_BUSY);     //Initialize the EPD.

sAPP      app;                // Application variable
sLoRaWAN  lora;               // See the lorapaper.h file for the settings 
sTimeDate TimeDate;           // RTC time and date variables
LORAMAC lorawan (&lora);      // Initialize the LoRaWAN stack.  

uint16_t    v_scap, volt, itmp, count=0;        // V_Supercap (ADC counts, mV), intTemp, counter
volatile bool RTC_ALARM = false;                // Interrupt variable
    
/********* INTERRUPTS *********************************************************************/
ISR(INT1_vect)            // Interrupt vector for the alarm of the MCP7940 Real Time
{                 
  RTC_ALARM = true;       // Do not use I2C functions or long delays here.
} 
                          
/********* MAIN ***************************************************************************/
void setup(void) 
{ 
  if (DEBUG) Serial.begin(115200); 
  analogReference(EXTERNAL);          // use AREF for reference voltage
  SPI.begin();                        // Initialize the SPI port
  SPI.beginTransaction(SPISettings(8000000, MSBFIRST, SPI_MODE0));
  
  pinMode(SW_TFT, OUTPUT);            // Switch for V_Scap
  pinMode(DS2401, OUTPUT);            // Authenticating IC DS2401p+
  pinMode(RFM_NSS, OUTPUT);           // RFM95W NSS = CS
  pinMode(SPI_FLASH_CS, OUTPUT);      // External SPI flash
  digitalWrite(DS2401, HIGH);         // to save power...
  digitalWrite(SW_TFT, HIGH);         // to save power...
  digitalWrite(SPI_FLASH_CS, HIGH);   // to save power...

  //initBMP280();                       // Initialize external temperature sensor

  epd.begin();                        // Turn ON & initialize 1.1" EPD
  epd.printText("PaperiNode", 10, 30, 2);
  //epd.saveFBToFlash(ADDR_FRAMEBUFFER);
  epd.update();                       // Update EPD
  epd.end();                          // to save power...
  digitalWrite(RFM_NSS, LOW);         // to save power...
  flash_power_down();                 // To save power...
  SPI.endTransaction();               // to save power...
  SPI.end();                          // to save power...

  v_scap = analogRead(A7);            // 1st Dummy-read which always delivers strange values...(to skip)

  I2C_init();                         // Initialize I2C pins
  mcp7940_init(&TimeDate, app.LoRaWAN_message_interval);   // Generate minutely interrupt
  RTC_ALARM = true;
  if (DEBUG) Serial.println(F("Setup finished."));
}

void loop()
{ 
  if(RTC_ALARM == true)
  { 
    if (DEBUG) Serial.println(F("Loop entry"));
    app.LoRaWAN_message_interval = CYCLE;
    
    // Catch the minute alarm from the RTC.       
    mcp7940_reset_minute_alarm(app.LoRaWAN_message_interval);        
    
    digitalWrite(SW_TFT, LOW);     // Turn ON voltage divider 
    delay(1);                      // To stabilize analogRead
    v_scap = analogRead(A7);       // Measure V_scap
    digitalWrite(SW_TFT, HIGH);    // Turn OFF voltage divider 

    SPI.begin();
    SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
    epd.begin(false);             // Turn ON EPD without refresh to save power
    epd.clear();
    SPI_Write(RFM_NSS, 0x01, 0x00);          // Switch RFM to sleep
    //epd.loadFromFlash(ADDR_FRAMEBUFFER);   // Load last image to pre-buffer
    epd.printText("PaperiNode", 10, 5, 1);
  
    float Vbat = v_scap*6.6/1024;
    volt = (uint16_t) (Vbat * 1000 +0.5);  // SuperCap voltage in mV
    String voltage = "Vscap: " + String(Vbat,3) + " V";
    if (DEBUG) Serial.println("Vscap: " + String(Vbat,3) + " V");
    epd.printText(voltage, 10, 20, 1);
    
    float intTemp = epd.readTemperature();
    itmp = (int16_t) (intTemp * 100);      // internal Temperature
    String temp = "Int.Temp.: " + String(intTemp,0) + " *C";
    if (DEBUG) Serial.println("Int.Temp.: " + String(intTemp,0) + " *C");
    epd.printText(temp, 10, 35, 1);

    temp = "Ext.Temp.: n.a.";
    //temp = "Ext.Temp.: " + String(bmp.readTemperature(),1) + " *C";
    epd.printText(temp, 10, 50, 1);

    temp = "C: " + String(++count);
    epd.printText(temp, 100, 5, 1);
    //epd.saveFBToFlash(ADDR_FRAMEBUFFER);
    if (v_scap > 640) epd.update();               // Trigger a 900ms default update (4GL)
    else              epd.update(EPD_UPD_MONO);   // Trigger a 450ms low power update (2GL)
     
    lora.TX.Data[0] = highByte(volt);
    lora.TX.Data[1] = lowByte(volt);
    lora.TX.Data[2] = highByte(itmp);
    lora.TX.Data[3] = lowByte(itmp);
    lora.TX.Count = 4;
      
    lorawan.init();               // Init the RFM95 module
    delay(50);
    if(v_scap >= SENDLEVEL) 
    {
      if (DEBUG) Serial.print(F("Send message..."));
      lorawan.LORA_Send_Data();                   // LoRaWAN send
      if (DEBUG) Serial.println(F("Message sent."));
    }
    if (DEBUG) Serial.println(F("Loop finished.")); delay(500);
    epd.end();                                   // To save power...
    digitalWrite(RFM_NSS, LOW);                  // To save power...
    flash_power_down();                          // To save power...
    SPI.endTransaction();                        // To save power...
    SPI.end();
    
    RTC_ALARM = false;             // Clear the boolean.
  } else LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);     // To save power... 
}
