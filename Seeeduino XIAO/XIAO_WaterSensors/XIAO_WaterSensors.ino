/*
 * File: XIAO_WaterSensors.ino
 * 
 * Grove Turbidity & TDS Sensor connected to Seeeduino XIAO
 * via Grove Shield for Seeduino XIAO
 * 
 * 2020-07-19 Claus Kühnel info@ckuehnel.ch
 */
 
#define pTurbidity A8
#define pTDS A2

void setup() 
{
  Serial.begin(115200); 
  delay(2000); // wait for Serial Monitor
  Serial.println("Turbidity\tTDS Sensor");
}
 
void loop() 
{
  int sensorValue = analogRead(pTurbidity);// read the voltage of Grove Turbidity Sensor
  float voltage = sensorValue * (3.3 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.print(voltage, 3); // print out the value you read:
  Serial.print("\t\t");
  sensorValue = analogRead(pTDS);// read the voltage of Grove TDS Sensor
  voltage = sensorValue * (3.3 / 1024.0); // Convert the analog reading (which goes from 0 - 1023) to a voltage (0 - 5V):
  Serial.println(voltage, 3); // print out the value you read:
  delay(1000);
}
