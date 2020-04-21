// File: struct.ino

struct Struktur1
{
  bool a;
  int b;
  float c;
};

class Struktur2
{
  public:
    bool a;
    int b;
    float c;
};

void setup() 
{
	Serial.begin(115200);
  while(!Serial);

  Struktur1 s1;
  s1.a = true;
  s1.b = 123;
  s1.c = 1.23;

	Serial.println(s1.a);     
	Serial.println(s1.b); 
	Serial.println(s1.c);

  Struktur2 s2;
  s2.a = false;
  s2.b = 456;
  s2.c = 4.56;

  Serial.println(s2.a);     
  Serial.println(s2.b); 
  Serial.println(s2.c);
}


void loop() {	}
