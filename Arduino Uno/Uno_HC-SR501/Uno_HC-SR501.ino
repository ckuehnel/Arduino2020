/*
 * File: Uno_HC-SR501.ino
 * Set delay to minimum (3 s) and sensitivity to middle position.
 * Claus Kuehnel 2020-02-11 info@ckuehnel.ch
 */

const int pPIR = 8; // Input for HC-S501

int pirValue; // Place to store read PIR Value
int lastValue = 0;


void setup() 
{
  Serial.begin(115200);
  delay(2500);
  Serial.println("Motion Sensor working...");
  
  pinMode(LED_BUILTIN, OUTPUT);
  digitalWrite(LED_BUILTIN, LOW);
  pinMode(pPIR, INPUT);
}

void loop() 
{
  pirValue = digitalRead(pPIR);
  digitalWrite(LED_BUILTIN, pirValue);
  if (!pirValue) lastValue = 0;
  if (pirValue && !lastValue) 
  {
    Serial.print(millis());
    Serial.println(" ms - Motion detected");
    lastValue = pirValue;
  }
}
