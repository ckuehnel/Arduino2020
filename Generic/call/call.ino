/*
 * Call-by-value vs. call-by-reference
 * (c) Claus Kuehnel 2019-10-12 info@ckuehnel.ch
*/

void f1(int x) 
{
  x = 2 * x;
  Serial.print("> ");
  Serial.println(x);
}

void f2(int &y)
{
  y = 2 * y;
  Serial.print("> ");
  Serial.println(y);
}

void setup() 
{
  Serial.begin(115200);
  
  int x = 10, y = 20;
  
  // Call-by-value
  Serial.println("Call-by-value");
  Serial.println(x);
  f1(x);
  Serial.println(x);
  Serial.println("-----------------");
  
  // Call-by-reference
  Serial.println("Call-by-reference");
  Serial.println(y);
  f2(y);
  Serial.println(y);  
  
}

void loop() {}
