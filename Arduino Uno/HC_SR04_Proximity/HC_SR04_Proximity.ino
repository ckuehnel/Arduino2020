/*
 * File: HC_SR04_Proximity.ino
 * http://wolles-elektronikkiste.de/hc-sr04-und-jsn-sr04t-2-0-abstandssensoren
 * 
 */

int triggerPin = 8;
int echoPin = 9;

int cmDistance, tCorrectedCmDistance;
//int temperature = 0;
unsigned long duration;

void setup()
{
  pinMode(triggerPin, OUTPUT);
  pinMode(echoPin, INPUT);
  Serial.begin(115200);
  delay(10);
}

void loop()
{
  digitalWrite(triggerPin, HIGH);
  delayMicroseconds(20);
  digitalWrite(triggerPin, LOW);
  duration = pulseIn(echoPin, HIGH);
 
  // Schallgeschwindigkeit in Luft: 343m/S (bei 20°C)
  // 343,2 m/S --> 34,32cm / ms --> 0,03432cm / µs
  // durch 2 wg. Echo
  cmDistance = duration * 0.03432 / 2; 
//  tCorrectedCmDistance = duration *(0.03315 + 0.00006 * temperature)/2;
  Serial.print("Distanz [cm]: ");
  Serial.println(cmDistance);
//  Serial.print("T-korrigierte Distanz [cm]: ");
//  Serial.println(tCorrectedCmDistance);
  delay(500);
}
