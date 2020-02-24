void blinkLED(int RGB)
{
  digitalWrite(RGB, HIGH);
  delay(1000);
  digitalWrite(RGB, LOW);
  delay(1000);
}
