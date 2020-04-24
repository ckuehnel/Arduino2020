/*
 * File: Uno_WriteMFRC522.ino
 * 
 * Write data to MIFARE Tag
 * Claus Kuehnel 2020-02-18 info@ckuehnel.ch
 * 
 * based on: read_write_personal_data.ino @ https://github.com/miguelbalboa/rfid
 * 
 * Typical pin layout used:
 * -----------------------------------------------------------------------------------------
 *             MFRC522      Arduino       Arduino   Arduino    Arduino          Arduino
 *             Reader/PCD   Uno/101       Mega      Nano v3    Leonardo/Micro   Pro Micro
 * Signal      Pin          Pin           Pin       Pin        Pin              Pin
 * -----------------------------------------------------------------------------------------
 * RST/Reset   RST          9             5         D9         RESET/ICSP-5     RST
 * SPI SS      SDA(SS)      10            53        D10        10               10
 * SPI MOSI    MOSI         11 / ICSP-4   51        D11        ICSP-4           16
 * SPI MISO    MISO         12 / ICSP-1   50        D12        ICSP-1           14
 * SPI SCK     SCK          13 / ICSP-3   52        D13        ICSP-3           15
 */

#include <SPI.h>
#include <MFRC522.h>
#include <stdlib.h>

#define DEBUG 1

const int RST_PIN = 9;               // Configurable, see typical pin layout above
const int SS_PIN = 10;               // Configurable, see typical pin layout above

const int bufferSize = 16;

MFRC522 mfrc522(SS_PIN, RST_PIN);    // Create MFRC522 instance

void setup() 
{
  Serial.begin(115200);      // Initialize serial communications with the PC
  Serial.setTimeout(30000);
  delay(2000);               // wait for Serial Monitor
  SPI.begin();               // Init SPI bus
  mfrc522.PCD_Init();        // Init MFRC522 card
  Serial.println(F("Write data on a MIFARE PICC "));
  if (DEBUG) Serial.println(F("Program running in DEBUG Mode"));
}

void loop() 
{            
  // Prepare key - all keys are set to FFFFFFFFFFFFh at chip delivery from the factory.
  MFRC522::MIFARE_Key key;
  for (byte i = 0; i < 6; i++) key.keyByte[i] = 0xFF;
        
  // Look for new cards
  if ( ! mfrc522.PICC_IsNewCardPresent()) return; 

  // Select one of the cards
  if ( ! mfrc522.PICC_ReadCardSerial())  return;
        
  Serial.print(F("Card UID:"));    //Dump UID
  for (byte i = 0; i < mfrc522.uid.size; i++) 
  {
    Serial.print(mfrc522.uid.uidByte[i] < 0x10 ? " 0" : " ");
    Serial.print(mfrc522.uid.uidByte[i], HEX);
  } 
  Serial.println();
  Serial.print(F("PICC type: "));   // Dump PICC type
  MFRC522::PICC_Type piccType = mfrc522.PICC_GetType(mfrc522.uid.sak);
  Serial.println(mfrc522.PICC_GetTypeName(piccType));
        
  byte buffer[bufferSize];
  for (byte i = 0; i < bufferSize; i++) buffer[i]= ' ';    // Clear buffer

  while (Serial.available() > 0) Serial.read(); // Clean buffer
  Serial.println("Eingabe:");
  Serial.readBytes(buffer, bufferSize);
    
  if (DEBUG)
  {  
    Serial.print(F("Temp Buffer: "));    //Dump Temp Buffer
    for (byte i = 0; i < bufferSize; i++) 
    {
      //if (buffer[i]==0) break;
      Serial.print(buffer[i] < 0x10 ? " 0" : " ");
      Serial.print(buffer[i], HEX);
    }
    Serial.println();
  }
        
  byte block = 1;
  MFRC522::StatusCode status;
  byte len;
            
  //Serial.println(F("Authenticating using key A..."));
  status = mfrc522.PCD_Authenticate(MFRC522::PICC_CMD_MF_AUTH_KEY_A, block, &key, &(mfrc522.uid));
  if (status != MFRC522::STATUS_OK) 
  {
    Serial.print(F("PCD_Authenticate() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else Serial.println(F("PCD_Authenticate() success: "));
        
  // Write block
  status = mfrc522.MIFARE_Write(block, buffer, bufferSize);
  if (status != MFRC522::STATUS_OK) 
  {
    Serial.print(F("MIFARE_Write() failed: "));
    Serial.println(mfrc522.GetStatusCodeName(status));
    return;
  }
  else 
  {
    Serial.print("Block " + String(block));
    Serial.println(F(" MIFARE_Write() success: "));
    Serial.println("Data written.");
  }
        
  if (DEBUG)
  {
    Serial.println("Dump written sector...");
    // Dump debug info about the card; PICC_HaltA() is automatically called
    mfrc522.PICC_DumpMifareClassicSectorToSerial(&(mfrc522.uid), &key, 0);
  }
        
  mfrc522.PICC_HaltA();       // Halt PICC
  mfrc522.PCD_StopCrypto1();  // Stop encryption on PCD     
}
