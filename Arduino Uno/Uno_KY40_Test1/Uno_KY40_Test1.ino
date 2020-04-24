/* 
 *  File: Uno_KY40_Test1.ino
 *  
 *  based on Arduino New Rotary Encoder Debounce
 *  Created by Yvan / https://Brainy-Bits.com
 *  
 */

// Rotary Encoder Module connections
const int PinSW=4;    // Rotary Encoder Switch
const int PinDT=3;    // DATA signal
const int PinCLK=2;   // CLOCK signal

// Variables to debounce Rotary Encoder
long TimeOfLastDebounce = 0;
int DelayofDebounce = 1;

// Store previous Pins state
int PreviousCLK;   
int PreviousDATA;

int counter=0; // Store current counter value
boolean pressed = false;

void setup() 
{
  Serial.begin(115200);
  delay(2000);
  
  // Put current pins state in variables
  PreviousCLK=digitalRead(PinCLK);
  PreviousDATA=digitalRead(PinDT);

  // Set the Switch pin to use Arduino PULLUP resistors
  pinMode(PinSW, INPUT_PULLUP);
}

void loop() 
{
  // If enough time has passed check the rotary encoder
  if ((millis() - TimeOfLastDebounce) > DelayofDebounce) 
  {
    check_rotary();  // Rotary Encoder check routine below
    
    PreviousCLK=digitalRead(PinCLK);
    PreviousDATA=digitalRead(PinDT);
    
    TimeOfLastDebounce=millis();  // Set variable to current millis() timer
  }
  
  // Check if Rotary Encoder switch was pressed
  if (!digitalRead(PinSW) && !pressed) 
  {
    pressed = true;
    counter=0;  // Reset counter to zero
    Serial.print("Counter = "); Serial.println(counter);
  }
  if (digitalRead(PinSW)) pressed = false;
}


// Check if Rotary Encoder was moved
void check_rotary() 
{
  if ((PreviousCLK == 0) && (PreviousDATA == 1)) 
  {
    if ((digitalRead(PinCLK) == 1) && (digitalRead(PinDT) == 0)) 
    {
      counter++;
      Serial.print("Counter = "); Serial.println(counter);
    }
    if ((digitalRead(PinCLK) == 1) && (digitalRead(PinDT) == 1)) 
    {
      counter--;
      Serial.print("Counter = "); Serial.println(counter);
    }
  }
  if ((PreviousCLK == 1) && (PreviousDATA == 0)) 
  {
    if ((digitalRead(PinCLK) == 0) && (digitalRead(PinDT) == 1)) 
    {
      counter++;
      Serial.print("Counter = "); Serial.println(counter);
    }
    if ((digitalRead(PinCLK) == 0) && (digitalRead(PinDT) == 0)) 
    {
      counter--;
      Serial.print("Counter = "); Serial.println(counter);
    }
  }
  if ((PreviousCLK == 1) && (PreviousDATA == 1)) 
  {
    if ((digitalRead(PinCLK) == 0) && (digitalRead(PinDT) == 1)) 
    {
      counter++;
      Serial.print("Counter = "); Serial.println(counter);
    }
    if ((digitalRead(PinCLK) == 0) && (digitalRead(PinDT) == 0)) 
    {
      counter--;
      Serial.print("Counter = "); Serial.println(counter);
    }
  }  
  if ((PreviousCLK == 0) && (PreviousDATA == 0)) 
  {
    if ((digitalRead(PinCLK) == 1) && (digitalRead(PinDT) == 0)) 
    {
      counter++;
      Serial.print("Counter = "); Serial.println(counter);
    }
    if ((digitalRead(PinCLK) == 1) && (digitalRead(PinDT) == 1)) 
    {
      counter--;
      Serial.print("Counter = "); Serial.println(counter);
    }
  }            
 }
