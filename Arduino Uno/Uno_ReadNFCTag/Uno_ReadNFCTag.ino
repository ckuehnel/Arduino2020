/*
 * File: Uno_ReadNFCTag.ino
 * based on  : ReadTag example in NDEF library
 * Modifications: Claus Kühnel 2020-02-29 info@ckuehnel.ch
 */
#include <Wire.h>
#include <PN532_I2C.h>
#include <PN532.h>
#include <NfcAdapter.h>

PN532_I2C pn532_i2c(Wire);
NfcAdapter nfc = NfcAdapter(pn532_i2c);

void setup(void) 
{
  Serial.begin(115200);
  Serial.println(F("NDEF Reader"));
  nfc.begin();
}

void loop(void) 
{
  Serial.println(F("\nScan a NFC tag\n"));
  if (nfc.tagPresent())
  {
    NfcTag tag = nfc.read();
                
    Serial.print(F("NFC Tag - ")); Serial.println(tag.getTagType());
    Serial.print(F("UID "));       Serial.println(tag.getUidString());
        
    if (tag.hasNdefMessage()) // every tag won't have a message
    {
      NdefMessage message = tag.getNdefMessage();
        
      int recordCount = message.getRecordCount();
      Serial.print(F("Number of records = ")); Serial.println(recordCount);
        
      for (int i = 0; i < recordCount; i++)
      {
        Serial.print(F("\nNDEF Record "));Serial.println(i+1);
        NdefRecord record = message.getRecord(i);

        int payloadLength = record.getPayloadLength();
        byte payload[payloadLength];
        record.getPayload(payload);
        
        String payloadAsString = "";
        for (int c = 0; c < payloadLength; c++) 
        {
          payloadAsString += (char)payload[c];
        }
            
        Serial.print(F("Payload (as String): "));
        Serial.println(payloadAsString);
      }
    }
  }
  delay(5000);
}
