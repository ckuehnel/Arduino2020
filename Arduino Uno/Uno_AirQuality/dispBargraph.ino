void dispBargraph(byte i)
{
   LSB = lowByte(bar[i]);
   MSB = highByte(bar[i]);
   digitalWrite(LATCH, LOW); // blocage du registre de sortie des 74HC595 
   shiftOut(SDIN, SCLK, MSBFIRST, MSB); // envoi des 8 bits de poids forts sur le second 74HC595 
   shiftOut(SDIN, SCLK, MSBFIRST, LSB); // envoi des 8 bits de poids faibles sur le premier 74HC595 
   digitalWrite(LATCH, HIGH); // déblocage du registre de sortie des 74HC595 
}
