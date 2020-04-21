/*
 * File: Uno_TMP36.ino
 * 
 * Measuring temperature by TMP36 sensor @ Arduino Uno
 * created 2018-12-20 by Claus Kuehnel (info@ckuehnel.ch  
 */

const int pTMP36 = A0;
const float Vs = 5.0;     // VCC,  5 V for Arduino Uno 

void setup()
{
  Serial.begin(115200);
  delay(2000);  // wait for serial monitor
  Serial.println("Measuring temperature by TMP36");
}

void loop()
{
  int raw = analogRead(pTMP36);
  float volt = raw * Vs/1024;
  Serial.print("Voltage = "); Serial.print(volt, 3); Serial.print(" V\t");
  float temp = (volt - 0.5) * 100; 
  Serial.print("Temperature =  ");
  Serial.print(temp, 1);
  Serial.println(" °C");
  delay(1000);
}
