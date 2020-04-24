/*
 * File: Uno_Relais.ino
 * 
 * Ansteuerung einer Last durch ein an IO/-Pin 8
 * angeschlossenes Relais.
 * 
 * Claus Kühnel 2020-03-14 info@ckuehnel.ch
 */

const int Relais = 8; // Relais @ Pin 8

void setup() 
{
  pinMode(Relais, OUTPUT);
  digitalWrite(Relais, LOW);
}

void loop() 
{
  digitalWrite(Relais, HIGH);
  delay(1000);
  digitalWrite(Relais, LOW);
  delay(1000);
}
