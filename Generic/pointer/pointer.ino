// File: pointer.ino

void setup() 
{
  Serial.begin(115200);
  
  int i=0, j=1, k=2;
  int *a, *b;         // a und b sind Zeiger auf Integer-Variable
  a = &i;             // die Adresse von i wird dem Zeiger a zugewiesen
  b = &k;             // die Adresse von k wird dem Zeiger b zugewiesen
  Serial.println(*a); // Wert der Variablen i
  Serial.println(*b); // Wert der Variablen k
}

void loop() { }
