/*
 * File: Uno_BLDC.ino
 * 
 * Controlling BLDC motor by SZ-X11F-1 BLDC controller w/ PWM
 * Disconnect jumper on board and close soldering bridge.
 * Claus Kühnel 2020-05-01 info@ckuehnel.ch
 */

const int pwm_ctrl = 6;
const int dir_ctrl = 7; 
int val = 0;

void setup() 
{
  Serial.begin(9600);
  delay(1000);
  Serial.println("BLDC Motor Control");
  Serial.setTimeout(10000);
  pinMode(pwm_ctrl, OUTPUT);
  pinMode(dir_ctrl, OUTPUT);
  digitalWrite(dir_ctrl, HIGH); 
}

void loop() 
{
  Serial.println("Input PWM value (0 - 255, negative inverts direction):");
  val = Serial.parseInt();
  if (val > 255) val = 255;
  if (val < 0) 
  {
    val = abs(val);
    digitalWrite(dir_ctrl, !digitalRead(dir_ctrl));
    Serial.println("Change direction now.");   
  }
  Serial.print("PWM = ");
  Serial.println(val);
  analogWrite(pwm_ctrl, val);
}
