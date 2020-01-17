int summe(int a, int b, int c, int d) 
{
  int x = a + b + c + d;
  return x;
}

int summe(int a, int b, int c)
{
  int x = a + b + c;
  return x;
}

int summe(int a, int b)
{
  int x = a + b;
  return x;
}

void setup() 
{
  Serial.begin(115200);
  Serial.println(summe(2, 3, 4, 5));
  Serial.println(summe(2, 3, 4));
  Serial.println(summe(2, 3));
}

void loop() {}
