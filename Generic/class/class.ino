// File class.ino

class Struktur
{
  public:
    bool a;
    int b;
    float c;
    Struktur(bool x, double z) // Constructor
    {
      a = x; c = z;
    }
    void about() 
    {  
      Serial.println("This is an object of class Struktur");
    }
};

void setup() 
{
  Serial.begin(115200);
  while(!Serial); // wait for serial port to connect. Nedded for native USB only

  Struktur s(true,1.23);  // Create an object s

  s.about();

  // Access to initialized member by constructor
  Serial.print("s.c: "); Serial.println(s.c); 
   
  // Access to uninitialized member
  Serial.print("s.b: "); Serial.println(s.b);     
  
  // Initialising integer member
  s.b = 123;
  Serial.print("s.b: "); Serial.println(s.b); 
}

void loop() {}
