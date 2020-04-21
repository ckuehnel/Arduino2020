// File: random.ini

void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  
  Serial.println("Test Pseudo-Zufallszahlen");
  Serial.println("Initialize randomSeed() by any fix value");
  randomSeed(0);
  putRandom();
  Serial.println("\nInitialize randomSeed() by analog value");
  randomSeed(analogRead(A0));
  putRandom();
  Serial.println();
}

void loop() {}
