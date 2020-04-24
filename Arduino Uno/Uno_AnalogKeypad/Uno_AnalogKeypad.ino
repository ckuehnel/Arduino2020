/*
 * 
 */
// Define Analog Pin:
const int pin = A0;

// Number of keys on keypad
const int keys = 16;

// Variable to store button being pressed:
int keyValue = 0;

// Keypad button names:
const char buttonIDs[keys] = {'1', '2', '3', 'A', '4', '5', '6', 'B', '7', '8', '9', 'C', '*', '0', '#', 'D'};

// Analog values that correspond to each button:
int thresholds[keys]; 

void setup() 
{
  Serial.begin(115200);
  delay(2000); // wait doe Serial Monitor
  Serial.println("\nAnalog-out Keypad Test");

  for (int i=0; i < keys; i++)
  {
    Serial.print("Press key "); Serial.print(buttonIDs[i]);
    while(analogRead(A0) < 100);
    keyValue = analogRead(A0);
    Serial.print("\tKeyvalue is "); Serial.println(keyValue);
    thresholds[i] = keyValue;
     while(analogRead(A0) > 100);   
  }
  for (int i=0; i < keys; i++) 
  {
    Serial.print(buttonIDs[i]); Serial.print("\t");
  }
  Serial.println();
  for (int i=0; i < keys; i++) 
  {
    Serial.print(thresholds[i]); Serial.print("\t");
  }
  Serial.println();
  Serial.println("\nInsert the following line into your code before void setup():");
  Serial.print("int myThresholds[16] = {");
  for (int i = 0; i < keys-1; i++) 
  {
    Serial.print(thresholds[i]);
    Serial.print(", ");
  }
  Serial.print(thresholds[15]);
  Serial.println("};");
  Serial.println("\nNow test by pressing any key..."); 
}

void loop()
{
  int idx;
  
  Serial.print("Press any key: "); 
  while(analogRead(A0) < 100);
  keyValue = analogRead(A0);
  Serial.print("\tKeyvalue is "); Serial.println(keyValue);
  for (int i = 0; i< keys; i++)
  {
    if ((keyValue > thresholds[i] - 5) && (keyValue < thresholds[i] + 5)) idx = i;
  }
  Serial.print("You pressed "); Serial.println(buttonIDs[idx]);
  while(analogRead(A0) > 100);
}
