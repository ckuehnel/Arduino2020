/* Reads TMP36 temperature sensor

   Connections:

   TMP36       | Vs  | Vout | GND |
   --------------------------------
   Arduino Uno | 5 V |  A0  | GND |

*/

int raw;
float volt;

const int pTMP36 = A0;
const float Vs = 5.0;     // VCC,  5 V for Arduino Uno

float getValue()
{
  raw = analogRead(pTMP36);
  volt = raw * Vs/1024;
  temp = (volt - 0.5) * 100;
  if (DEBUG) 
  {
    Serial.print("Voltage = "); Serial.print(volt, 3); Serial.print(" V\t");
    Serial.print("Temperature =  "); Serial.print(temp, 1); Serial.println(" *C");
  }
  return temp;
}
