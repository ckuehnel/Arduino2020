/*
  SerialOutBT.ino
  Serial output via Bluetooth for ATmega328 based Arduinos
  (c) Claus Kuehnel 2018-12-16 info@ckuehnel.ch

  Connect
  Arduino®   HC-05
  ----------------
  5 V        VCC
  GND        GND
  D10        Rx
  D12        Tx
*/

#include <SoftwareSerial.h>

const byte rxPin = 12;
const byte txPin = 10;

// set up a new serial object
SoftwareSerial BTSerial (rxPin, txPin);

// define your board here
#define BOARD "Arduino Leonardo"
//#define BOARD "Arduino M0"

//#define Serial SerialUSB // Arduino M0 native port

void setup() 
{
  Serial.begin(115200);
  while(!Serial);       // for native port
  // delay(2000);
  Serial.print(BOARD);
  Serial.println(" starts BT communication...");
  
  BTSerial.begin(9600);          
  BTSerial.print("This is ");
  BTSerial.println(BOARD);
}

void loop() 
{
  Serial.print("*");
  BTSerial.print("*");
  delay(1000);
}
