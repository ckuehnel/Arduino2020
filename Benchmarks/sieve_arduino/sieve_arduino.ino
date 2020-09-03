//
// Title        : Sieve of Eratosthenes
// Author       : Claus Kuehnel
// Date         : 2018-03-01
// Id           : sieve_arduino.ino
// Version      : 1.8.9
// based on     : sieve.pde from 2010
//
// DISCLAIMER:
// The author is in no way responsible for any problems or damage caused by
// using this code. Use at your own risk.
//
// LICENSE:
// This code is distributed under the GNU Public License
// which can be found at http://www.gnu.org/licenses/gpl.txt
//
//#define BOARD "Arduino Uno" // define your board here
//#define BOARD "Arduino Uno WiFi R2"
//#define BOARD "Arduino Due"
//#define BOARD "Arduino MKR 1000"
//#define BOARD "ESPduino-8266"
#define BOARD "ESPduino-32"
//#define BOARD "Heltec CubeCell"
//#define BOARD "Arduino M0"

#include <HardwareSerial.h>

HardwareSerial mySerial;

#define TRUE 1
#define FALSE 0

#define SERIAL  mySerial

int i,k, prime,count;

const int SIZE = 1000;    
char flags[SIZE+1];

void setup() 
{ 
  //SERIAL.begin(115200);
  mySerial.begin(115200, SERIAL_8N1);
  delay(2500); // wait for Serial Monitor
  SERIAL.print(F("Sieve of Eratosthenes - "));
  SERIAL.println(BOARD);
  SERIAL.print(F("Arduino SW Version "));
  SERIAL.println(ARDUINO);  // Arduino SW version
  SERIAL.print(F("Clock frequency 48"));
//  SERIAL.print(F_CPU/1000000);      // Oscillator frequency
  SERIAL.println(F(" MHz"));
  
  /*-------------------------------------------------------------------
  The following code is an implementation of the Sieve of Eratosthenes.
  -------------------------------------------------------------------*/
  SERIAL.println("5000 iterations");
  unsigned long time = millis();
  for (unsigned int iter = 1; iter <= 5000; iter++)     /* do program 5000 times */
  { 
    count = 0;               /* initialize prime counter */
    for (i = 0; i <= SIZE; i++)      /* set all flags true */
      flags[i] = TRUE;
    for (i = 0; i <= SIZE; i++)
    {
      if (flags[i])                /* found a prime */
      {
        prime = i + i + 3;         /* twice index + 3 */
        for (k = i + prime; k <= SIZE; k += prime)
          flags[k] = FALSE;        /* kill all multiples */
        count++;                   /* primes found */
      }
    }
  }
  time = millis() - time;
  SERIAL.print(count);
  SERIAL.println(" primes.");     
  SERIAL.print("Runtime = ");
  SERIAL.print(time);
  SERIAL.println(" ms\n");
} 
  
void loop()
{
  // nothing to do
}
