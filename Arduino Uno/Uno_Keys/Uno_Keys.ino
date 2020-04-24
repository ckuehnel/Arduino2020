/*
 * File: Uno_Keys.ino
 * 
 * Connections:
 * S1 -- IO2
 * S2 -- IO3
 */
const int S1 = 2, S2 = 3;

void setup() 
{
  Serial.begin(115200);
  delay(2000); // wait for Serial Monitor

  pinMode(S1, INPUT_PULLUP);
  pinMode(S2, INPUT_PULLUP);
}

void loop() 
{
  if (digitalRead(S1)) Serial.print("S1 open ");
  else Serial.print("S1 closed ");
  if (digitalRead(S2)) Serial.println("- S2 open");
  else Serial.println("- S2 closed");
  delay(500);
}
