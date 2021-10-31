void LEDsOn()
{
  for (int i = 0; i < LEDS_NUM; i++)
  {
    ledsBuff[i].setRGB(20, 20, 20);
  }
  FastLED.show();
}

void LEDsOff()
{
  for (int i = 0; i < LEDS_NUM; i++)
  {
    ledsBuff[i].setRGB(0, 0, 0);
  }
  FastLED.show();
}
