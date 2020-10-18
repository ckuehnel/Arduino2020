 /*
  * File  : M0_BC66_ATtest.ino
  * Brief : This example use the serial port to send AT command to control the BC66-Module.
  *         With initialization completed, we can enter AT command to BC66-Module directly.
  *         Common AT commands : 
  *         AT+CPIN? :  Check SIM card
  *         AT+CSQ    : Check signal quality
  *         AT+CGATT? : Check net attached state
  *         AT+COPS?  : Check Operator
  *         AT+QBAND? : Get mobile operation band      
  * Note  : Ihe AT command must end with CRLF
  */

void setup() 
{
    SerialUSB.begin(115200); // Serial Interface to PC
    Serial1.begin(115200);   // Serial Interface to BC66-Module
    while(!SerialUSB);
    SerialUSB.println("\nSend AT commands to BC66-Module......");
    SerialUSB.println("For example, if you type AT\\r\\n, OK\\r\\n will be responsed!");
    SerialUSB.println("Enter your AT command :");
}

void loop() 
{
  while(Serial1.available())   SerialUSB.write(Serial1.read());
  while(SerialUSB.available()) Serial1.write(SerialUSB.read());
}
