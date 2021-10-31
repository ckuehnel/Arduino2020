void displayTemperature()
{
   if(current_temperature != previous_temperature)
   {
      M5.Lcd.setCursor(50, 80);
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.setTextSize(4);
      M5.Lcd.print("T: ");
      M5.Lcd.print(convertFloatToString(previous_temperature));
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(225, 75);
      M5.Lcd.print("o");
      M5.Lcd.setCursor(240, 80);
      M5.Lcd.setTextSize(4);
      M5.Lcd.print("C");
      
      M5.Lcd.setCursor(50, 80);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(4);
      M5.Lcd.print("T: ");
      M5.Lcd.print(convertFloatToString(current_temperature));
      M5.Lcd.setTextSize(2);
      M5.Lcd.setCursor(225, 75);
      M5.Lcd.print("o");
      M5.Lcd.setCursor(240, 80);
      M5.Lcd.setTextSize(4);
      M5.Lcd.print("C");
      
      previous_temperature = current_temperature;
   }
}

void displayHumidity()
{
   if(current_humidity != previous_humidity)
   {
      M5.Lcd.setCursor(50, 140);
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.setTextSize(4);
      M5.Lcd.print("H: ");
      M5.Lcd.print(convertFloatToString(previous_humidity));
      M5.Lcd.print(" %");
      
      M5.Lcd.setCursor(50, 140);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(4);
      M5.Lcd.print("H: ");
      M5.Lcd.print(convertFloatToString(current_humidity));
      M5.Lcd.print(" %");
      
      previous_humidity = current_humidity;
   }
}

void displayMoisture()
{
   if(current_moisture != previous_moisture)
   {
      M5.Lcd.setCursor(50, 200);
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.setTextSize(4);
      M5.Lcd.print("M: ");
      M5.Lcd.print(convertFloatToString(previous_moisture));
      M5.Lcd.print(" %");
      
      M5.Lcd.setCursor(50, 200);
      M5.Lcd.setTextColor(WHITE);
      M5.Lcd.setTextSize(4);
      M5.Lcd.print("M: ");
      M5.Lcd.print(convertFloatToString(current_moisture));
      M5.Lcd.print(" %");
      
      previous_moisture = current_moisture;
   }
}

void displayRSSI()
{
   if(current_rssiValue != previous_rssiValue)
   {
      M5.Lcd.setCursor(80, 36);
      M5.Lcd.setTextColor(BLACK);
      M5.Lcd.setTextSize(2);
      M5.Lcd.print("RSSI: ");
      M5.Lcd.print(previous_rssiValue);
      M5.Lcd.print(" dBm");
      
      M5.Lcd.setCursor(80, 36);
      M5.Lcd.setTextColor(GREEN);
      M5.Lcd.setTextSize(2);
      M5.Lcd.print("RSSI: ");
      M5.Lcd.print(current_rssiValue);
      M5.Lcd.print(" dBm");
      
      previous_rssiValue = current_rssiValue;
   }
}

String convertFloatToString(float f)
{
  String s = String(f,1);
  return s;
}
