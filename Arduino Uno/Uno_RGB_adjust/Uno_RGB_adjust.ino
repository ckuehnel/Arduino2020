/*
   Uno_RGB_adjust.ino
   
   Controlling RGB LED w/ common cathode & adjust brigthnes
   Created 2020-02-26 Claus Kühnel info@ckuehnel.ch 
*/

enum{Red = 9, Green, Blue};
byte brRed, brGreen;

void setup()
{
  Serial.begin(115200);
  Serial.setTimeout(30000);
  delay(2000); // wait for Serial Monitor
  Serial.println("Adjust the brightness of RGB-LED to create the same brightness for all RGB-LEDs...");
  Serial.println("For same resistor values blue LED has lowest brightness.");
  
  pinMode(Red, OUTPUT); digitalWrite(Red,LOW);
  pinMode(Green, OUTPUT); digitalWrite(Green, LOW);
  pinMode(Blue, OUTPUT); digitalWrite(Blue, LOW);

  // switch blue LED on
  analogWrite(Blue, 255);
  Serial.println("Adjust brightness of red LED - input  >255 for end");
  while (true)
  {
    int val = Serial.parseInt();
    if (val > 255) break;
    brRed = (byte) val;
    analogWrite(Blue, 0);
    analogWrite(Red, brRed);
    delay(500);
    analogWrite(Red, 0);
    analogWrite(Blue, 255);
    delay(1000);
    analogWrite(Blue, 0);
    analogWrite(Red, brRed);
    Serial.println(brRed);
  }
  analogWrite(Red, 0);
  analogWrite(Blue, 255);
  Serial.println("Adjust brightness of green LED - input  >255 for end");
  while (true)
  {
    int val = Serial.parseInt();
    if (val > 255) break;
    brGreen = (byte) val;
    analogWrite(Blue, 0);
    analogWrite(Green, brGreen);
    delay(500);
    analogWrite(Green, 0);
    analogWrite(Blue, 255);
    delay(1000);
    analogWrite(Blue, 0);
    analogWrite(Green, brGreen);
    Serial.println(brGreen);
  }
  analogWrite(Green, 0);
  Serial.println("Adjustment finished.");
}

void loop()
{
  analogWrite(Red, brRed);
  delay(1000);
  analogWrite(Red, 0);
  analogWrite(Green, brGreen);
  delay(1000);
  analogWrite(Green, 0);
  analogWrite(Blue, 255);
  delay(1000);
  analogWrite(Blue, 0);
}
