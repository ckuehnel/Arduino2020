#include <SoftwareSerial.h>

SoftwareSerial mySerial(7, 8);
#include<stdio.h>
#include<string.h>
#define DEBUG true
 
void setup()
{
  Serial.begin(9600);
  mySerial.begin(9600);
  delay(1000);
  int pon=9;
  int poff=6;
  int lowp=5;
  pinMode(pon, OUTPUT);
  pinMode(poff, OUTPUT);
  pinMode(lowp, OUTPUT);
  digitalWrite(poff, LOW);
  digitalWrite(lowp, HIGH);   
  digitalWrite(pon, HIGH);
  delay(3000);
  digitalWrite(pon, LOW);
  delay(1000); 
  baudrate();
  Serial.println("After 3s, test begin!!");
  delay(3000);  
}

void loop()
{
     sendData("AT+CCID",3000,DEBUG); 
     sendData("AT+CREG?",3000,DEBUG);     
     sendData("AT+CGATT=1",1000,DEBUG);
     sendData("AT+CGDCONT=1,\"IP\",\"CMNET\"",1000,DEBUG);
     sendData("AT+CGACT=1,1",1000,DEBUG);
     sendData("AT+CIPSTART=\"TCP\",\"gtech.co\",80",3000,DEBUG);
     sendData("AT+CIPSEND=80",1000,DEBUG);
     sendData("GET http://gtech.co/gtech.php?hello HTTP/1.0\r\n HOST:gtech.co\r\n\r\n",100,DEBUG);
     delay(10000);
     sendData("AT+CIPCLOSE ",1000,DEBUG);
}
void baudrate()
{
      int i=0;
      for(i=0;i<10;i++)
      {
           String msg = String("");
           msg=sendData("AT",1000,DEBUG);
           if( msg.indexOf("ok") >= 0 )
           {
                return;
            }
       }
}

String sendData(String command, const int timeout, boolean debug)
{
    String response = "";    
    mySerial.println(command); 
    long int time = millis();   
    while( (time+timeout) > millis())
    {
      while(mySerial.available())
      {       
        char c = mySerial.read(); 
        response+=c;
      }  
    }    
    if(debug)
    {
      Serial.print(response);
    }    
    return response;
}
