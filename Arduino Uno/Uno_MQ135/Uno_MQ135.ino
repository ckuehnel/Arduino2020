/*  
 * File Uno_MQ135.ino
 * Get air quality signal from MQ135 sensor
 * (c) Claus Kuehnel 2018-11-27 info@ckuehnel.ch
 *   
 * based on
 * http://davidegironi.blogspot.com/2017/05/mq-gas-sensor-correlation-function.html
 */

int mqInput = A0;        //sensor input PIN
int mqR = 22000;         //pull-down resistor value
long rO = 41763;         //rO sensor value
float minRsRo = 0.358;   //min value for Rs/Ro
float maxRsRo = 2.428;   //max value for Rs/Ro
float a = 116.6020682;   //sensor a coefficient value
float b = -2.769034857;  //sensor b coefficient value
 
void setup() 
{
  pinMode(mqInput, INPUT);
  pinMode(LED_BUILTIN, OUTPUT); // On-board LED
  Serial.begin(115200);
}
 
void loop() 
{
  int adcRaw = analogRead(mqInput);
  
  if (adcRaw >= 100) digitalWrite(LED_BUILTIN, HIGH);
  else digitalWrite(LED_BUILTIN, LOW);
  
  Serial.print("ADC: "); Serial.println(adcRaw);
  long rS = ((1024.0 * mqR) / adcRaw) - mqR;
  Serial.print("Rs: "); Serial.println(rS);
  float rSrO = (float)rS / (float)rO;
  Serial.print("Rs/Ro: "); Serial.println(rSrO);
  if(rSrO < maxRsRo && rSrO > minRsRo) 
  {
    float ppm = a * pow((float)rS / (float)rO, b);
    Serial.print("ppm: "); Serial.println(ppm);
    digitalWrite(LED_BUILTIN, HIGH);
  } 
  if(rSrO < minRsRo) Serial.println("PPM CO2 too high");
  if(rSrO > maxRsRo) Serial.println("PPM CO2 quite low");
  Serial.println();
  
  delay(2000);
}
