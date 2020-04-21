// File: tone.ino

void setup() 
{
  tone(5, 523);
  delay(300);
  tone(5, 659);
  delay(300);
  tone(5, 784);
  delay(300);
  tone(5, 1047);
  delay(600);
  noTone(5);
  tone(5, 523, 300);
  delay(1000);
  tone(5, 1047, 1000); // dieser Ton ist nicht hörbar
  tone(5, 523, 1000); 
}

void loop() {}
