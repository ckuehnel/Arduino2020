/*
 * Measuring battery voltage of LiPo connected to CubeCell
 *
 */
uint16_t batteryVoltage() 
{ 
  pinMode(VBAT_ADC_CTL,OUTPUT);
  digitalWrite(VBAT_ADC_CTL,LOW);
  uint16_t voltage=analogRead(ADC)*2;
  digitalWrite(VBAT_ADC_CTL,HIGH);
  Serial.print("VBAT = ");
  Serial.print(voltage);
  Serial.println(" mV");
  return voltage;
}
