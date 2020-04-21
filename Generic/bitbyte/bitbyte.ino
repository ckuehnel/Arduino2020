// File: bitbyte.ino

void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println("Bitnumber/Bitvalue");
  Serial.print("    i :   ");
  for (int i = 0; i < 8; i++)
  {
    Serial.print(i);
    Serial.print("\t");
  }
  Serial.println();
  Serial.print("bit(i):   ");
  for (int i = 0; i < 8; i++)
  {
    Serial.print(bit(i));
    Serial.print("\t");  
  }

  Serial.println("\n\nbitClear/bitSet");
  word x = 0xFFFF; Serial.println(x, HEX);
  bitClear(x, 3); Serial.println(x, HEX);
  bitSet(x, 3); Serial.println(x, HEX);
  
  Serial.println("\nbitRead/bitWrite");
  x = 0xAAAA; Serial.println(x, HEX);
  Serial.println(bitRead(x, 3), HEX);
  Serial.println(bitRead(x, 4), HEX);
  bitWrite(x, 0, 1); Serial.println(x, HEX);
  bitWrite(x, 2, 1); Serial.println(x, HEX);

  Serial.println("\nlowByte/hiByte");
  x = 0xAABB; Serial.println(x, HEX);
  Serial.println(lowByte(x), HEX);
  Serial.println(highByte(x), HEX);
}

void loop() {}
