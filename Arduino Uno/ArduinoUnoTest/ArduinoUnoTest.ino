/*
  Arduino Uno Test
  (c) Claus Kuehnel 2019-10-12 info@ckuehnel.ch
*/

byte count = 0;

void setup() 
{
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200);
  delay(1000); // waiting to start the monitor
  Serial.print("Hi there, here is Arduino Uno"); 
}

void loop() 
{
  Serial.print(".");
  count++;
  if (count == 25)
  {
    Serial.println();
    count=0;
  }
  digitalWrite(LED_BUILTIN, HIGH);
  delay(500);
  digitalWrite(LED_BUILTIN, LOW);
  delay(500);
}
