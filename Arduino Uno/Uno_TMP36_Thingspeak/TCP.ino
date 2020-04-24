// Connection to Thingspeak Server via GPRS

void getResponse() 
{
  delay(midZ);
  while (SIM800_Serial.available()) Serial.write(SIM800_Serial.read());
}

void sendUpdate()
{
  if (DEBUG) Serial.println("Prepare Connection to Thingspeak...");
  SIM800_Serial.println("at+csq");
  delay(minZ);
  getResponse();
  SIM800_Serial.println("at+cipshut");
  delay(maxZ);
  getResponse();
  SIM800_Serial.println("at+cstt=\""+ APN +"\",\"\",\"\"");
  getResponse();
  SIM800_Serial.println("at+ciicr");//start wireless connection cellular network
  getResponse();
  SIM800_Serial.println("at+cifsr");//enquire regarding the IP address allocated
  getResponse();
  SIM800_Serial.println("at+cipstart=\"TCP\",\""+ URL +"\",\"80\"");  //connect to the ThingSpeak update URL (https://api.thingspeak.com)
  getResponse();
  SIM800_Serial.println("at+cipsend=80");//declare the number of bytes (characters) I want to send
  getResponse();
  SIM800_Serial.print("GET https://");    //this is a constant beginning for the GET command and is as provided by ThingSpeak
  SIM800_Serial.print(URL);
  SIM800_Serial.print("/update?");
  SIM800_Serial.print(APIKEY); //the channel API key provided by ThingSpeak
  getResponse();
  SIM800_Serial.print("&field1=");
  getResponse();
  SIM800_Serial.println(String(temp));//the value to post to field 
  getResponse();
  SIM800_Serial.println("at+cipack");//ask for acknowledge details
  getResponse();
  SIM800_Serial.println("at+cipclose");//close the IP connection
  getResponse();
}
