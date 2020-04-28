/*  
   File: Uno_AirQuality.ino
   Get air quality signal from MQ135 sensor
   (c) Claus Kuehnel 2020-04-28 info@ckuehnel.ch

   using 
   Air quality click (PID: MIKROE-1630)
   BarGraph Click    (PID: MIKROE-1423) on
   Arduino UNO click shield (PID: MIKROE-1581)
  
   based on
   https://github.com/ckuehnel/Arduino_Sensorknoten/blob/master/General/MQ135-1.ino
   http://davidegironi.blogspot.com/2017/05/mq-gas-sensor-correlation-function.html
*/

int mqInput = A0;        //sensor input PIN, left slot
int mqR = 20000;         //pull-down resistor value, adjustable between 10 - 20 kOhm
long rO = 41763;         //rO sensor value
float minRsRo = 0.358;   //min value for Rs/Ro
float maxRsRo = 2.428;   //max value for Rs/Ro
float a = 116.6020682;   //sensor a coefficient value
float b = -2.769034857;  //sensor b coefficient value

#define SDIN 11   // DS 74HC595 
#define SCLK 13   // SHCP 74HC595 
#define LATCH 9   // STCP 74HC595, right slot
#define DISPLAY 6 //  CS bargraph, right slot
#define PWM 5  // PWM right, slot

byte MSB, LSB; 
unsigned int bar[] = {0,0x01,0x03,0x07,0x0F,0x1F, 0x3F, 0x7F,0xFF,0x1FF, 0x3FF};

void setup() 
{
  pinMode(mqInput, INPUT);
  pinMode(LED_BUILTIN, OUTPUT); // On-board LED
  pinMode(SDIN, OUTPUT); 
  pinMode(SCLK, OUTPUT); 
  pinMode(LATCH, OUTPUT); 
  pinMode(DISPLAY, OUTPUT); 
  digitalWrite(DISPLAY,HIGH); // CS bargraph right slot
  pinMode(PWM, OUTPUT);
  analogWrite(PWM, 88);  // dimming LEDs
  Serial.begin(9600);
  delay(1000);
  Serial.println("\nMeasuring Air Quality by MQ-135 Sensor...");
}
 
void loop() 
{
  float ppm;
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
    ppm = a * pow((float)rS / (float)rO, b);
    Serial.print("ppm: "); Serial.println(ppm);
    digitalWrite(LED_BUILTIN, HIGH);
  } 
  if(rSrO < minRsRo) Serial.println("PPM CO2 too high");
  if(rSrO > maxRsRo) Serial.println("PPM CO2 quite low");
  Serial.println();
  unsigned int bg = map(int(ppm), 0, 1200, 0, 10);
  Serial.println(bg);
  dispBargraph(bg);
  delay(2000);
}
