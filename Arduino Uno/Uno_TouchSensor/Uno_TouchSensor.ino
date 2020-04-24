/*
   Uno_TouchSensor.ino

   Using Touch Sensor Module as input device
   https://www.az-delivery.de/products/touch-sensor-modul-1

   2019-09-23 Claus Kühnel info@ckuehnel.ch

*/

const int SIG = 2; // pin for Touch Sensor signal

void setup() 
{
  pinMode(SIG, INPUT);
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
}

void loop ()
{
  boolean val = digitalRead(SIG) ; 
  
  if (val == HIGH) digitalWrite (LED_BUILTIN, HIGH);
  else             digitalWrite (LED_BUILTIN, LOW);
}
