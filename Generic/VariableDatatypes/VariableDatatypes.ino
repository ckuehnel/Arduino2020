#define TEST "Variables & Datatypes"

bool flag = true;
char at = '@';
char s[] = "##########";
short x = 1234;
int y = -4567;
long z = 123456789;
float f = PI;
double g = 3.4028235E+2;
long double h = 1.23;
long long ll = 1234567890;
uint8_t a = 0xFF;
int16_t b = 0xFFFF;
uint32_t c = 0xFFFFFFFF;
uint64_t d = 0xFFFFFFFFFFFFFFFF;

void setup() 
{
  Serial.begin(115200);
  while(!Serial);
  Serial.println(TEST);
  Serial.print("bool ");  Serial.print(flag); Serial.print(" needs "); Serial.print(sizeof(flag));  Serial.println(" Byte in RAM");
  Serial.print("char ");  Serial.print(at);   Serial.print(" needs "); Serial.print(sizeof(at));    Serial.println(" Byte in RAM");
  Serial.print("char ");  Serial.print(s);    Serial.print(" needs "); Serial.print(sizeof(s));     Serial.println(" Byte in RAM");
  Serial.print("short "); Serial.print(x);    Serial.print(" needs "); Serial.print(sizeof(x));     Serial.println(" Byte in RAM");
  Serial.print("int ");   Serial.print(y);    Serial.print(" needs "); Serial.print(sizeof(y));     Serial.println(" Byte in RAM");
  Serial.print("long ");  Serial.print(z);    Serial.print(" needs "); Serial.print(sizeof(z));     Serial.println(" Byte in RAM");
  // Serial.print(long long) does not work, you have to adapt print.cpp and print.h https://forum.arduino.cc/index.php?topic=134116.msg1008917#msg1008917
  Serial.print("long long "); /*Serial.print(ll);  */  Serial.print(" needs "); Serial.print(sizeof(ll));     Serial.println(" Byte in RAM");
  Serial.print("uint8_t "); Serial.print(a);  Serial.print(" needs "); Serial.print(sizeof(a));     Serial.println(" Byte in RAM");
  Serial.print("int16_t "); Serial.print(b);  Serial.print(" needs "); Serial.print(sizeof(b));     Serial.println(" Byte in RAM");
  Serial.print("uint32_t ");Serial.print(c);  Serial.print(" needs "); Serial.print(sizeof(c));     Serial.println(" Byte in RAM");
  Serial.print("uint64_t "); /*Serial.print(ll);  */  Serial.print(" needs "); Serial.print(sizeof(d));     Serial.println(" Byte in RAM");
  Serial.print("float "); Serial.print(f);    Serial.print(" needs "); Serial.print(sizeof(f));     Serial.println(" Byte in RAM");
  Serial.print("double "); Serial.print(g);    Serial.print(" needs "); Serial.print(sizeof(g));     Serial.println(" Byte in RAM");
  Serial.print("long double "); /* Serial.print(h); */   Serial.print(" needs "); Serial.print(sizeof(h));     Serial.println(" Byte in RAM");
}

void loop() {}
