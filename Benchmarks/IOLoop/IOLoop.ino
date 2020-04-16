//
// Title        : IO Loop
// Author       : Claus Kuehnel
// Date         : 2018-03-01
// Id           : IOLoop.ino
// Version      : 1.8.5
// based on     : 
//
// DISCLAIMER:
// The author is in no way responsible for any problems or damage caused by
// using this code. Use at your own risk.
//
// LICENSE:
// This code is distributed under the GNU Public License
// which can be found at http://www.gnu.org/licenses/gpl.txt
//

//#include <M5STACK>
//#define BOARD "Arduino Uno"  // describe your board here
//#define BOARD "Arduino Uno WiFi R2"
//#define BOARD "Arduino Due"
//#define BOARD "Arduino MKR 1000"
//#define BOARD "ESPduino-8266"  // repeated resets 2019-08-15
#define BOARD "ESPduino-32" 
//#define BOARD "M5STACK"
//#define BOARD "BBC Micro:bit"
//#define BOARD "NUCLEO-L476RG"

const int IOPin = 7;    
unsigned long t = 0;

void setup() 
{ 
  pinMode(IOPin, OUTPUT);

  Serial.begin(115200);
  delay(2500);
  Serial.print(F("I/O Loop Test for "));
  Serial.println(BOARD);
  Serial.print(F("Arduino SW Version "));
  Serial.println(ARDUINO);  // Arduino SW version
  Serial.print(F("Clock frequency "));
  Serial.print(F_CPU/1000000); // Oscillator frequency
  Serial.println(F(" MHz"));
  Serial.println("Measure IO frequency on Pin 7");
  t = micros();
  digitalWrite(IOPin, HIGH);                  
  digitalWrite(IOPin, LOW);
  t = micros() - t;
    
  Serial.print("Runtime = ");
  Serial.print(t);
  Serial.println(" us\n");
} 
  
void loop() 
{  
  digitalWrite(IOPin, HIGH);                    
  digitalWrite(IOPin, LOW); 
  digitalWrite(IOPin, HIGH);                    
  digitalWrite(IOPin, LOW); 
  digitalWrite(IOPin, HIGH);                    
  digitalWrite(IOPin, LOW); 
}
