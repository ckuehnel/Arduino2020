#include <M5Stack.h>
#include <FuGPS.h> // https://github.com/fu-hsi/FuGPS

HardwareSerial GPS(2);

FuGPS fuGPS(GPS);
bool gpsAlive = false;

void setup() 
{
  M5.begin();
  GPS.begin(9600);

  Serial.println("\nReading GPSUnit connected to M5Stack Basic");
  //M5.Lcd.setTextFont(4);
  M5.Lcd.setTextSize(1);
  M5.Lcd.setTextColor(WHITE, BLACK);
  M5.Lcd.setCursor(20, 10, 4);
  M5.Lcd.println("M5Stack GPS Data");
} 

void loop() 
{
  byte quality, satellites;
  float latitude, longitude, _speed;
  
  // Valid NMEA message
    if (fuGPS.read())
    {
        // We don't know, which message was came first (GGA or RMC).
        // Thats why some fields may be empty.

        gpsAlive = true;

        M5.Lcd.setCursor(20, 50);
        M5.Lcd.print("Qual:");
        M5.Lcd.setCursor(110, 50);
        M5.Lcd.print(quality, BLACK);
        quality = fuGPS.Quality;
        M5.Lcd.setCursor(110, 50);
        M5.Lcd.print(quality);
        
        M5.Lcd.setCursor(20, 80);
        M5.Lcd.print("#Sat:");
        M5.Lcd.setCursor(110, 80);
        M5.Lcd.print(satellites, BLACK);
        satellites = fuGPS.Satellites;
        M5.Lcd.setCursor(110, 80);
        M5.Lcd.print(satellites);
        M5.Lcd.print(" ");

        if (fuGPS.hasFix() == true)
        {
            // Data from GGA message
            Serial.print("Accuracy (HDOP): ");
            Serial.println(fuGPS.Accuracy);

            Serial.print("Altitude (above sea level): ");
            Serial.println(fuGPS.Altitude);

            // Data from GGA or RMC
            Serial.print("Location (decimal degrees): ");
            Serial.println("https://www.google.com/maps/search/?api=1&query=" + String(fuGPS.Latitude, 6) + "," + String(fuGPS.Longitude, 6));

            M5.Lcd.setCursor(20, 120, 4);
            M5.Lcd.print("Lat:");
            M5.Lcd.setCursor(110, 120, 4);
            M5.Lcd.printf("%.6f",latitude, BLACK);
            latitude = fuGPS.Latitude;
            M5.Lcd.setCursor(110, 120, 4);
            M5.Lcd.printf("%.6f", latitude);
            
            M5.Lcd.setCursor(20, 150, 4);
            M5.Lcd.print("Long:");
            M5.Lcd.setCursor(110, 150, 4);
            M5.Lcd.printf("%.6f", longitude, BLACK);
            longitude = fuGPS.Longitude;
            M5.Lcd.setCursor(110, 150, 4);
            M5.Lcd.printf("%.6f", longitude);

            M5.Lcd.setCursor(20, 200);
            M5.Lcd.print("Speed: ");
            M5.Lcd.setCursor(120, 200);
            M5.Lcd.printf("%.1f", _speed, BLACK);
            _speed = fuGPS.Speed * 1.852;
            M5.Lcd.setCursor(120, 200);
            M5.Lcd.printf("%.1f", _speed);
        }
    }

    // Default is 10 seconds
    if (fuGPS.isAlive() == false)
    {
        if (gpsAlive == true)
        {
            gpsAlive = false;
            Serial.println("GPS module not responding with valid data.");
            Serial.println("Check wiring or restart.");
        }
    }
}
