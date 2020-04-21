const int K = 123;

void setup()
{
  Serial.begin(115200);

  Serial.println(K);
  //K = 456;  // if you uncomment compiler error: "assignment of read-only variable 'K'" follows
  inc(); inc(); inc(); 
} 
  

void loop() {}

void inc()
{
  int X;
  static int Y;
  X++; Y++;
  Serial.print(X); Serial.print("\t"); Serial.println(Y);
}
