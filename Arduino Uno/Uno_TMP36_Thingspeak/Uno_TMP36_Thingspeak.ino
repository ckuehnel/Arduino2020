/*
   File: Uno_TMP36_Thingspeak.ino

   Measuring temperature by TMP36 temperature sensor and sending data
   containing measuring result to the Thingspeak platform.

   For internet access SIM800L Base Band EVB with Arduino Uno is used.
   Connection to Thingspeak based on 
   https://community.thingspeak.com/forum/arduino/uploading-via-sim800-gsm-module-using-at-commands/

   createtd 2019-02-24 by Claus Kühnel (info@ckuehnel.ch) 
 */

#include <SoftwareSerial.h>
#include "arduino_secrets.h"

#define RX 7
#define TX 8
#define BAUD 9600

SoftwareSerial SIM800_Serial(RX, TX); // RX, TX

#define INFO 0
#define DEBUG 1

int minZ=1000;
int midZ=2500;  //time delays
int maxZ=5000;

float temp;

void(* resetFunc) (void) = 0;   // declare reset function at address 0

void setup() 
{  
  Serial.begin(BAUD);
  SIM800_Serial.begin(BAUD);
  delay(2000);  // wait for serial monitor
  if (INFO)  printInfo();
  if (DEBUG) Serial.println(F("\nInitializing..."));
  Serial.println("TMP36 values will be sent to Thingspeak...");
  if (DEBUG) Serial.println(F("Running..."));     //last line in setup()
}

void loop()
{
  temp = getValue();

  Serial.print("TMP36 temperature is ");
  Serial.print(temp); Serial.println(" *C");
  sendUpdate();
  delay(5 * 60000);                   // 5 minutes
  resetFunc();                        //call reset
}


/*--------------------------------------------------------------*/

void printInfo()
{
  // Compile file path (internal file of compilation proccess)
  Serial.print("File "); Serial.println(__FILE__);
  
  String s1 = __DATE__;
  String s2 = __TIME__;
  
  // Date of compilation
  Serial.print("Compilation @ "); Serial.println(s1 + " " + s2);
  
  // Arduino IDE SW version
  Serial.print("ARDUINO IDE v"); Serial.println(ARDUINO);
  
  // Oscillator frequency
  Serial.print("CPU Clock in MHz: "); Serial.println(F_CPU/1E6);
}
