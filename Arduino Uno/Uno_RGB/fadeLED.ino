void fadeLED(int RGB)
{
  int i = 0;
  while (i <= 255)
  {
    analogWrite(RGB, i);
    delay(10);
    i += 5;
  } 
  
  i = 255;
  while (i >=0)
  {
    analogWrite(RGB, i);
    delay(10);
    i -= 5;
  } 
  delay(1000);
}
