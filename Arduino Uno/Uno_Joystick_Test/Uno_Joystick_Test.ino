/*
 * File: Uno_Jostick_Test.ino
 * 
 * Test of Analog joystick @ Arduino Uno
 * Claus Kühnel 2020-02-17 info@ckuehnel.ch
 * 
 * Connections:
 * Joystick  Arduino Uno
 *     GND -- GND
 *     VCC -- 5 V
 *     VRx -- A0
 *     VRy -- A1
 *       S -- A2
 */
#define CONVERT 0
 
int x,y,s;  // Joystick coordinates

void readJoystick(int &x, int &y, int &s)
{  
  x = analogRead(0);
  y = analogRead(1);
  s = analogRead(2);
}
 
void setup() 
{
  Serial.begin(115200);
  delay(2000); // wait for Serial Monitor
}
 
void loop() 
{
  readJoystick(x, y, s);
  if (CONVERT)
  {
    x = map(x, 0, 677, 0, 1023);
    y = map(y, 0, 677, 0, 1023);
    if (s > 10) s = 1; else s = 0; 
  }
  Serial.print("(x, y, s) = ");
  Serial.print(x);
  Serial.print("\t");
  Serial.print(y);
  Serial.print("\t");
  Serial.println(s);
  delay(500);
}  
  
