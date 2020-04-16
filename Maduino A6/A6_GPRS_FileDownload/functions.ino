void poweron()
{
  int i=0;
  boolean result = false;
  digitalWrite(pon, HIGH); 
  delay(3000);       
  digitalWrite(pon, LOW);
 //   SerialAT.flush();
 //   SerialAT.setTimeout(3000);
  SerialAT.println("AT");
  SerialAT.println("AT");
  SerialAT.println("AT");
  SerialAT.println("AT");
  SerialAT.println("AT");
  result = SerialAT.find("OK");
}
