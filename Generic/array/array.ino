// File: array.ino

void setup() 
{
  Serial.begin(115200);
  
  int array[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
  for(int i = 0; i < 10; ++i)
  {
    Serial.println(array[i]);
  }
  Serial.println();
  Serial.println(array[5]);     // Zugriff über Index
  Serial.println(*(array + 5)); // Zugriff über Zeiger
  Serial.println();
  Serial.print("    Array Size: "); Serial.println(sizeof(array));
  Serial.print("  Element Size: "); Serial.println(sizeof(int)); //array[0]));
  Serial.print("Element Number: "); Serial.println(sizeof(array) / sizeof(array[0])); 
}


void loop() { }
