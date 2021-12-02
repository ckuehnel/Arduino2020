/*
 * File: FlipDot.ino
 * 
 * Controlling Flip Dot Status Indicator by H-Bridge
 * short video: https://youtu.be/UxTjOamWLgs
 * 
 * 2021-12-01 claus Kühnel info@ckuehnel.ch
 */
 
#define DEBUG 1

const int IN1 = 2;
const int IN2 = 3;

void setup() 
{
  Serial.begin(115200);
  delay(2000); // wait for serial monitor
  
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);

  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
}

void loop() 
{
  FlipDotOn();
  delay(2500);
  FlipDotOff();
  delay(2500);
}

bool FlipDotOff(void)
{
  if (DEBUG) Serial.println("FlipDot Off");
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  delayMicroseconds(1500);
  digitalWrite(IN1, LOW);
  return true;
}

bool FlipDotOn(void)
{
  if (DEBUG) Serial.println("FlipDot On");
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  delayMicroseconds(1500);
  digitalWrite(IN2, LOW);
  return false;
}
