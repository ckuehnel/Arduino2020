// Display routines

void initLCD(void)
{
  M5.Lcd.setRotation(3);
  M5.Lcd.setFreeFont(FF12);                 // Select the font
  M5.Lcd.setTextDatum(BL_DATUM);
  M5.Lcd.fillRect(0,0,displayHeight,18, BLUE);
  M5.Lcd.setTextColor(TFT_ORANGE, BLUE);
  M5.Lcd.setCursor(0, 16);
  M5.Lcd.print("GMC PulseCount");
  M5.Lcd.setFreeFont(FF18);
  M5.Lcd.setTextColor(WHITE, BLACK);
}

void dispValue(uint32_t number)
{
  M5.Lcd.fillRect(0,40,displayHeight,20, BLACK);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.drawString(String(number)+ " cpm", 20, 60, GFXFF); 
}
