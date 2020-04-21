/*  
 * File: Uno_PPD42NS.ino  
 * 
 * Reads measuring data from Grove Dust Sensor.
 * (c) Claus Kuehnel 2018-11-26 info@ckuehnel.ch
 * 
 * based on: http://wiki.seeedstudio.com/Grove-Dust_Sensor/
 */

const int sensor = 8;  // Output P1
unsigned long duration;
unsigned long starttime;
unsigned long sampletime_ms = 30000;  //sample time = 30 s
unsigned long lowpulseoccupancy = 0;
float ratio = 0;
float concentration = 0;

#define DEBUG 0

void setup() 
{
  Serial.begin(115200);
  delay(2000);
  Serial.println("Reading Grove Dust Sensor...");
  pinMode(sensor,INPUT);
  starttime = millis();   //get the current time;
}

void loop() 
{
  duration = pulseIn(sensor, LOW);
  lowpulseoccupancy = lowpulseoccupancy + duration;
  Serial.print(".");

  if ((millis()-starttime) > sampletime_ms) //if the sample time == 30s
  {
    ratio = lowpulseoccupancy/(sampletime_ms*10.0);  // Integer percentage 0=>100
    concentration = 1.1*pow(ratio,3)-3.8*pow(ratio,2)+520*ratio+0.62; // using spec sheet curve
    Serial.println();
    if (DEBUG)
      { 
        Serial.print(lowpulseoccupancy);
        Serial.print(", ");
        Serial.print(ratio);
        Serial.print("   ");
      }
      Serial.print("Dust Concentration: ");
      Serial.print(3.5 * concentration);
      Serial.print(" ppl \t");
      Serial.print(concentration);
      Serial.println(" pp 0.01 ft^3");
      lowpulseoccupancy = 0;
      starttime = millis();
  }
}
