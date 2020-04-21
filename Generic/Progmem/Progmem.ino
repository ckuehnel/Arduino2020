/* File: Progmem.ino
 *  
 * The following code fragments illustrate how to read and write 
 * unsigned chars (bytes) and ints (2 bytes) from/to PROGMEM.
 */

// save some unsigned ints
const uint16_t charSet[] PROGMEM = { 1111, 2222, 3333, 4444, 5555};

// save some chars
const char signMessage[] PROGMEM = {"This text is saved on Flash"};

unsigned int myInt;
char myChar;


void setup() 
{
  Serial.begin(115200);
  while (!Serial);  // wait for serial port to connect. Needed for native USB

  Serial.println(F("Test of reading from flash memory\n"));

  // read back a 2-byte int
  for (byte k = 0; k < 5; k++) 
  {
    myInt = pgm_read_word_near(charSet + k);
    Serial.println(myInt);
  }
  Serial.println();

  // read back a char
  for (byte k = 0; k < strlen_P(signMessage); k++) 
  {
    myChar = pgm_read_byte_near(signMessage + k);
    Serial.print(myChar);
  }

  Serial.println();
}

void loop() {}
