/*
 * File: Uno_TouchPanel1.ino
 * 
 */

int X, Y;
#define   YP A2  // must be an analog pin, use "An" notation!
#define   XM A3  // must be an analog pin, use "An" notation!
const int YM = 8;  // can be a digital pin
const int XP = 9;  // can be a digital pin

void setup()
{
  Serial.begin(115200);
  delay(2000); // wait for Serial Monitor
  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  // Read Y
  pinMode(YP, INPUT);
  pinMode(YM, INPUT);
  pinMode(XP, OUTPUT);
  pinMode(XM, OUTPUT);

  digitalWrite(XM, LOW);
  digitalWrite(XP, HIGH);
  delay(5); // wait for stable voltage
  Y = analogRead(YP);
  //Y = map(ValueX, 65, 960, 1, 64);
  //if(Y < 0 || Y > 64) Y = 0;
  //Serial.print(Y);
  //Serial.print(";");

  // Read X
  pinMode(XP, INPUT);
  pinMode(XM, INPUT);
  pinMode(YP, OUTPUT);
  pinMode(YM, OUTPUT);

  digitalWrite(YM, LOW);
  digitalWrite(YP, HIGH);
  delay(5); // wait for stable voltage
  X = analogRead(XM);
  //X = map(X, 150, 820, 1, 36);
  //if(X < 0 || X > 36) X = 0;
  // Output
  Serial.print("X = "); Serial.print(X);
  Serial.print("\tY = "); Serial.print(Y);
  // Blink Led
  if (X > 0 && Y > 0) digitalWrite(LED_BUILTIN, HIGH);
  else                digitalWrite(LED_BUILTIN, LOW);
  delay(50);
}
