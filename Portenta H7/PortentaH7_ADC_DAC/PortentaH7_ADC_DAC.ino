/*
 * File: PortentaH7_ADC_DAC.ino
 * 
 * Arduino Portenta H7 Test of DAC & ADC
 * (c) Claus Kuehnel 2021-11-01 info@ckuehnel.ch
 * 
 * Arduino Portenta H7 has 3× ADC with 16-bit max. resolution
 * and 2× 12-bit DAC (1 MHz)
 * 
 * Connect A0 & A6 for this test.
 */

#define MEAN 1
#define DAC A6
#define ADC A0

uint16_t dacValue, adcValue;

void setup() 
{
  analogWriteResolution(12);    
  analogReadResolution(12);
  Serial.begin(115200);
  pinMode(LEDG, OUTPUT); digitalWrite(LEDG, LOW);
  delay(5000);
  Serial.println("Test of Arduino Potenta H7 DAC-ADC-Subsystem");
  Serial.println("\n DAC\tADC\t DIFF");
}

void loop()
{
  int16_t diff;
  
  for (dacValue = 0; dacValue < 4096; dacValue += 16)
  {
    digitalWrite(LEDG, LOW);
    delay(100);
    analogWrite(DAC, dacValue);
    delay(100);
  
    if (MEAN) adcValue = meanValue(10);
    else      adcValue = analogRead(ADC);
    
    diff = dacValue - adcValue;
    Serial.print(dacValue); Serial.print("\t");
    Serial.print(adcValue); Serial.print("\t");
    Serial.println(diff);
    digitalWrite(LEDG, HIGH);
    delay(900);
  }
}

uint16_t meanValue(uint8_t number)
{
  uint16_t ivalue = 0;
  for (int i = 0; i < number; i++)
  { 
    ivalue +=analogRead(ADC);
  }
  float fvalue = ivalue / number;
  return (uint16_t) (fvalue + .5);
}
