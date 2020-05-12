void led_set(uint8 R, uint8 G, uint8 B) 
{
  for (int i = 0; i < LED_NUM; i++) leds.setPixelColor(i, leds.Color(R, G, B));
  leds.show();
}

void flash(uint8 R, uint8 G, uint8 B)
{
  led_set(R, G, B);
  delay(200);
  led_set(0, 0, 0);
}
