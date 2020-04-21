// File: union.ino

union Value 
{
    int i;
    double d;
};

void setup() 
{
  Serial.begin(115200);
  while(!Serial);

  Value v;
  Serial.print("Union v umfasst "); Serial.print(sizeof(v)); Serial.println(" Byte\n");
  
  v.d = 987.654;  // v holds a double
  Serial.print("v.d: "); Serial.println(v.d);     
  Serial.print("v.i: "); Serial.println(v.i); 
  Serial.println();

  v.i = 123;     // v holds an int
  Serial.print("v.i: "); Serial.println(v.i); 
  Serial.print("v.d: "); Serial.println(v.d);
}

void loop() {}
