/*
 * Graphic Primitives
 */

void draw_big_point(uint16_t xpos, uint16_t ypos, uint16_t color)
{
  tft.fillRect(xpos-2, ypos-2, 5, 5, color);
  tft.drawFastVLine(xpos, ypos-10, 21, color); 
  tft.drawFastHLine(xpos-10, ypos, 21, color); 
}
