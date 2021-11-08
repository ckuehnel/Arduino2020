
void displayValues(uint16_t bootCount, float voltage)
{
  InkPageSprite.drawString(56, 16, "* PIR Sense *", &AsciiFont8x16);
  
  sprintf(buffer, "%d", bootCount);
  
  int x;
  if (bootCount < 100000) x = 42;
  if (bootCount < 10000) x = 54;
  if (bootCount < 1000) x = 66;
  if (bootCount < 100) x = 78;
  if (bootCount < 10) x = 90;
  
  InkPageSprite.drawString(x, 80, buffer, &AsciiFont24x48);
  sprintf(buffer, "Vbat: %0.3f V", voltage);
  
  InkPageSprite.drawString(56, 170, buffer, &AsciiFont8x16);
  InkPageSprite.pushSprite();
}
