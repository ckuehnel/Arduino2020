// Test of External Interrupt for Arduino Uno
// 2020-01-26 Claus Kühnel info@ckuehnel.ch

const int pINT0 = 2;             // INT0 at Pin2
const int pINT1 = 3;             // INT1 at Pin3

volatile boolean iflag = true;

int idx;

void setup()
{
  Serial.begin(115200);
  
  pinMode(LED_BUILTIN, OUTPUT);
  pinMode(pINT0, INPUT_PULLUP);
  pinMode(pINT1, INPUT_PULLUP);
  
  attachInterrupt(digitalPinToInterrupt(pINT0), stop_serial, FALLING);   // INT0 stops serial output
  Serial.print("EICRA: "); Serial.println(EICRA, HEX);
  attachInterrupt(digitalPinToInterrupt(pINT1), resume_serial, FALLING); // INT1 resumes serial output
  Serial.print("EICRA: "); Serial.println(EICRA, HEX);
  Serial.println("Setup finished.");
}

void loop()
{ 
  if (iflag) Serial.println(idx);  // iflag controls serial output
  idx++;
  delay(500);
}

void stop_serial()           
{
  iflag = false;
  digitalWrite(LED_BUILTIN, HIGH);
}

void resume_serial()
{
  iflag = true;
  digitalWrite(LED_BUILTIN, LOW);
}
