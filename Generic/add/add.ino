
unsigned int add(unsigned int a, unsigned int b);     // Deklaration der Funktion

void setup() 
{
  Serial.begin(115200);

  unsigned int a = 35, b = 10; // Deklaration und Initialisierung der Variablen
  unsigned int x = add(a, b);  // Aufruf der Funktion add(a, b)
  Serial.println(x);           // serielle Ausgabe des Rückgabewertes der Funktion
}

void loop() {}

unsigned int add(unsigned int a, unsigned int b)      // Definition der Funktion
{
  unsigned int x = a+b; 
  return x;
}
