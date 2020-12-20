//#define Serial SerialUSB // for Arduino M0
//#define BOARD "MaixDuino"
//#define BOARD "Arduino Uno WiFi R2"
#define BOARD "Arduino Nano Every"

void setup() 
{
  Serial.begin(115200);
  while(!Serial); // wait for serial port to connect. Needed for native USB.
  Serial.println("Start");

}

void loop() 
{
  Serial.print("Hello World from ");
  Serial.println(BOARD);
  delay(1000);

}
