/*
 * File: Uno_WriteNFCTag.ino
 * based on  : WriteTag example in NDEF library
 * Modifications: Claus Kühnel 2020-02-29 info@ckuehnel.ch
 */
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

String messagetext = "This is a message written by Arduino Uno";
String URL = "http://www.rheinwerk-verlag.de";

void setup() 
{
  Serial.begin(115200);
  Serial.println("NDEF Writer");
  nfc.begin();
}

void loop() 
{
  boolean success;
  
  Serial.println(F("\nPlace a NFC tag on the reader."));
  if (nfc.tagPresent()) 
  {
    success = nfc.clean();
    if (success) Serial.println(F("\nSuccess, tag restored to factory state."));
    else Serial.println(F("\nError, unable to clean tag."));
            
    NdefMessage message = NdefMessage();
        
    Serial.println("\nWrite " + messagetext + " to NFC tag");    
    message.addTextRecord(messagetext);
    Serial.print(message.getRecordCount()); Serial.println(" records written.");

    Serial.println("\nWrite " + URL + " to NFC tag");    
    message.addUriRecord(URL);
    Serial.print(message.getRecordCount()); Serial.println(" records written.");

    success = nfc.write(message);
    if (success) Serial.println(F("Success. Verify by reading this tag with your phone or Uno_ReadNFCTag.ino."));        
    else Serial.println(F("Write failed."));
  }
  delay(5000);
}
