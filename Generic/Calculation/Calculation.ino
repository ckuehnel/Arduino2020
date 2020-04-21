#define TEST "Calculation"

void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println(TEST);

  Serial.println((1+2/3)*4-5);
  Serial.print((1+2./3)*4-5);
} 

void loop() 
{
  // nothing to do here
}
