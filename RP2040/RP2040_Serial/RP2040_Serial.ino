/*
 * File: RP2040_Serial.ino
 * 
 * Test of Serial1 initialization to connect ESP-01 for WiFi connection later
 * using https://github.com/earlephilhower/arduino-pico
 * 
 * 2021-05-17 Claus Kühnel info@ckuehnel.ch
 */

void setup() 
{
  Serial.begin(115200);
  delay(5000);
  Serial.println(F("RP2040 Serial Test"));

  Serial1.setRX(17);
  Serial1.setTX(16);
  Serial1.begin(115200);

  Serial.println(F("Initialization finished"));
  Serial.println(F("Short GPIO16 and GPIO17 to receive characters sent"));
}
  
void loop()
{
  if (Serial1.available()) Serial.write(Serial1.read());
  if (Serial.available())  Serial1.write(Serial.read());
}
