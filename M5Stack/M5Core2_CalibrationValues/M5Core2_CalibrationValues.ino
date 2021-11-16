/*
 * File: M5Core2_CalibrationValues.ino
 * 
 * Get the calibration value airValue & waterValue for Soil Moisture Sensor
 * 
 * 2021-11-16 Claus Kühnel info@ckuehnel.ch
 */

void setup() 
{
  Serial.begin(115200); 
  while(!Serial);
  Serial.println("Get calibration values for Soil Moisture Sensors");
  Serial.println("Hold the sensor in air to get the airValue and notify it.");
  Serial.println("Hold the sensor in water to get the waterValue afterwards and notify it");
}
  
void loop() 
{
  uint16_t val;
  val = analogRead(36); //connect sensor to PortB
  Serial.println(val);  //print the value to serial port
  delay(1000);
}
