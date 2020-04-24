/*
 *name :          LightControlledBlinking
 *programmed by : Claus Kuhnel
 *date :          2008-12-07
 *micro :         ATmega168V / LilyPad Arduino
 *tested with :   Arduino 1.8.11
 */
/*
 * LightControlledBlinking
 *
 * Turns on and off light emitting diodes (LED) in blinking mode depending
 * of the amount of ambient light.
 * The switching point is defined by a constant as default and can be changed
 * by serial input a value between 0 and 255.
 *
 */
#define DEBUG 1 // for debug output set to 1, otherwise to 0
#define DEFAULT_THRESHOLD 120

int sensorPin = 0; // input pin for the light sensor
int ledPin = 13; // pin for internal LED
int led1Pin = 8; // pin for LED1
int led2Pin = 9; // pin for LED2
int val = 0; // variable to store the value coming from sensor
int threshold; // threshold defines activation of blinking

void blinking(void)
{
 digitalWrite(ledPin, HIGH); // sets the LEDs on
 digitalWrite(led1Pin, HIGH);
 digitalWrite(led2Pin, HIGH);
 delay(50); // waits a short moment
 digitalWrite(ledPin, LOW); // sets the LEDs off
 digitalWrite(led1Pin, LOW);
 digitalWrite(led2Pin, LOW);
 delay(100); // waits a short moment
 digitalWrite(ledPin, HIGH); // sets the LEDs on
 digitalWrite(led1Pin, HIGH);
 digitalWrite(led2Pin, HIGH);
 delay(50); // waits a short moment
 digitalWrite(ledPin, LOW); // sets the LEDs off
 digitalWrite(led1Pin, LOW);
 digitalWrite(led2Pin, LOW);
 delay(1000); // waits for a second
}

void setup()
{
 pinMode(ledPin, OUTPUT); // declare ledPin as OUTPUT
 pinMode(led1Pin, OUTPUT); // declare led1Pin as OUTPUT
 pinMode(led2Pin, OUTPUT); // declare led2Pin as OUTPUT
 Serial.begin(19200);
 threshold = DEFAULT_THRESHOLD;
} 

void loop()
{
 if (Serial.available()) // check if data has been sent from host
 {
 threshold = Serial.read(); // read the most recent byte (which will be from 0 to 255)
 }
 val = analogRead(sensorPin); // read the value from light sensor
 if (DEBUG)
 {
 Serial.print("Threshold = ");
 Serial.print(threshold);
 Serial.print("\t");
 Serial.print("Light Value = ");
 Serial.println(val);
 }
 if (val < threshold) blinking();
 else delay(1000);
} 
