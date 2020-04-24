void lcdOutput()
{
  clearLine(0);
  lcd.setCursor(0,0);            // move cursor to second line "1" and 9 spaces over
  lcd.print("  CO2 ");
  lcd.print(CO2, 0);
  lcd.print(" ppm");

  clearLine(1);
  lcd.setCursor(0,1);            // move to the begining of the second line
  lcd.print(temp, 1);
  lcd.print(" *C");
  lcd.setCursor(8,1);
  lcd.print(hum, 1);
  lcd.print(" %rH");
  delay(5000);
}

void clearLine(byte line)
{
  lcd.setCursor(0 ,line);
  lcd.print("                ");
}
