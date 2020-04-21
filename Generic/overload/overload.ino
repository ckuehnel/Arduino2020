int summe(int a, int b, int c, int d) 
{
  return (a + b + c + d);
}

int summe(int a, int b, int c)
{
  return (a + b + c);
}

int summe(int a, int b) 
{
  return(a + b);
}

void setup() 
{
  Serial.begin(115200);
  Serial.println(summe(2, 3, 4, 5));
  Serial.println(summe(2, 3, 4));
  Serial.println(summe(2, 3));
}

void loop() {}
