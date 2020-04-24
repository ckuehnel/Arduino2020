/*
 * Generate StartScreen w/ Logo 
 */

static const byte LOGO_WIDTH = 27;
static const byte LOGO_HEIGHT = 4; // 4 lines w/ 8 bit = 32 dots
static const byte logo[] = 
{   0,  0,252,252, 12, 12,204,204,204,204,204,204,204,204,204,204,
  204,204,204,204,204,204,204,204,204,  0,  0,  0,  0,255,255,  0,
    0,255,255,  0,  0,252,252, 12, 12,204,204,204,204,204,204,204,
  204,204,204,204,  0,  0,  0,  0,255,255,  0,  0,255,255,  0,  0,
  255,255,  0,  0,255,255,  0,  0,156,156,156,128,128,252,252,  0,
    0,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,  1,  0,  0,  1,
    1,  0,  0,  1,  1,  1,  1,  1,  1,  1,  0,  0, 0, 0, 0, 0, 0};

void displayStartScreen()
{
  // Draw the logo bitmap at the top left corner...
  lcd.setCursor(0, 0);
  lcd.drawBitmap(logo, LOGO_WIDTH, LOGO_HEIGHT);
  lcd.setCursor(0, 5);
  lcd.setInverseOutput(true);
  lcd.print("Nokia5110  LCD");
  lcd.setInverseOutput(false);
  delay(2000);
  lcd.clear();
}
