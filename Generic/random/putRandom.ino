void putRandom()
{
  for(int i; i <= 10; i++)
  {
    Serial.print(random(10));
    Serial.print("\t");
  }
}
