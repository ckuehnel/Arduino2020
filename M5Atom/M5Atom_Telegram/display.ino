// control pixel for signalization

void MidPoint(uint8_t R, uint8_t G, uint8_t B)
{
  M5.dis.drawpix(2, 2, CRGB(G, R, B));
}

void InnerCircle(uint8_t R, uint8_t G, uint8_t B)
{
  for (int i = 1; i < 4; i++) M5.dis.drawpix(i,1, CRGB(G, R, B));
  M5.dis.drawpix(1, 2, CRGB(G, R, B));
  M5.dis.drawpix(3, 2, CRGB(G, R, B));
  for (int i = 1; i < 4; i++) M5.dis.drawpix(i,3, CRGB(G, R, B));
}

void OuterCircle(uint8_t R, uint8_t G, uint8_t B)
{
  for (int i = 0; i < 5; i++) M5.dis.drawpix(i,0, CRGB(G, R, B));
  M5.dis.drawpix(0, 1, CRGB(G, R, B)); M5.dis.drawpix(4, 1, CRGB(G, R, B));
  M5.dis.drawpix(0, 2, CRGB(G, R, B)); M5.dis.drawpix(4, 2, CRGB(G, R, B));
  M5.dis.drawpix(0, 3, CRGB(G, R, B)); M5.dis.drawpix(4, 3, CRGB(G, R, B));
  for (int i = 0; i < 5; i++) M5.dis.drawpix(i,4, CRGB(G, R, B));
}

void blink(uint8_t R, uint8_t G, uint8_t B)
{
  MidPoint(R, G, B);
  delay(100);
  InnerCircle(R, G, B);
  delay(100);
  OuterCircle(R, G, B);
  delay(200);
  OuterCircle(0,0,0);
  delay(100);
  InnerCircle(0,0,0);
  delay(100);
  MidPoint(0,0,0);
  delay(500);
}
