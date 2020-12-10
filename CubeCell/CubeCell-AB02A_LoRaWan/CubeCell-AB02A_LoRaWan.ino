/*
 * File: CubeCell-AB02A_LoRaWan.ino
 * 
 * Board: CubeCell 1/2 AA
 */
#include "Arduino.h"
#include <LoRaWan_APP.h>
#include <Wire.h>

#define DEBUG 0

float temp;

/* OTAA para*/
uint8_t devEui[] = { 0x00, 0x00, 0xE3, 0xA0, 0xFE, 0x63, 0x2D, 0x33 }; // comes from Chip ID of CubeCell
uint8_t appEui[] = { 0x70, 0xB3, 0xD5, 0x7E, 0xD0, 0x03, 0x89, 0xF7 };  
uint8_t appKey[] = { 0x0F, 0x1C, 0x7C, 0x4C, 0x69, 0x2D, 0x11, 0xB9, 0xAC, 0x8E, 0xEF, 0xFD, 0x82, 0x5C, 0x57, 0xB0 };

/* ABP para*/
uint8_t nwkSKey[] = { 0x15, 0xb1, 0xd0, 0xef, 0xa4, 0x63, 0xdf, 0xbe, 0x3d, 0x11, 0x18, 0x1e, 0x1e, 0xc7, 0xda,0x85 };
uint8_t appSKey[] = { 0xd7, 0x2c, 0x78, 0x75, 0x8c, 0xdc, 0xca, 0xbf, 0x55, 0xee, 0x4a, 0x77, 0x8d, 0x16, 0xef,0x67 };
uint32_t devAddr =  ( uint32_t )0x007e6ae1;

/*LoraWan region, select in arduino IDE tools*/
LoRaMacRegion_t loraWanRegion = ACTIVE_REGION;

/*LoraWan Class, Class A and Class C are supported*/
DeviceClass_t  loraWanClass = LORAWAN_CLASS;

/*the application data transmission duty cycle.  value in [ms].*/
uint32_t appTxDutyCycle = 15000;

/*OTAA or ABP*/
bool overTheAirActivation = LORAWAN_NETMODE;

/*ADR enable*/
bool loraWanAdr = LORAWAN_ADR;

/* set LORAWAN_Net_Reserve ON, the node could save the network info to flash, when node reset not need to join again */
bool keepNet = LORAWAN_NET_RESERVE;

/* Indicates if the node is sending confirmed or unconfirmed messages */
bool isTxConfirmed = LORAWAN_UPLINKMODE;

/*The application data transmission duty cycle.  value in [ms].*/
uint32_t APP_TX_DUTYCYCLE = 30 * 60000; // 15 min 

/* Application port */
uint8_t appPort = 2;
/*!
* Number of trials to transmit the frame, if the LoRaMAC layer did not
* receive an acknowledgment. The MAC performs a datarate adaptation,
* according to the LoRaWAN Specification V1.0.2, chapter 18.4, according
* to the following table:
*
* Transmission nb | Data Rate
* ----------------|-----------
* 1 (first)       | DR
* 2               | DR
* 3               | max(DR-1,0)
* 4               | max(DR-1,0)
* 5               | max(DR-2,0)
* 6               | max(DR-2,0)
* 7               | max(DR-3,0)
* 8               | max(DR-3,0)
*
* Note, that if NbTrials is set to 1 or 2, the MAC will not decrease
* the datarate, in case the LoRaMAC layer did not receive an acknowledgment
*/
uint8_t confirmedNbTrials = 8;

/*LoraWan channelsmask, default channels 0-7*/ 
uint16_t userChannelsMask[6]={ 0x00FF,0x0000,0x0000,0x0000,0x0000,0x0000 };

/* Prepares the payload of the frame */
static void prepareTxFrame( uint8_t port )
{
  digitalWrite(Vext, LOW);
  
	/*appData size is LORAWAN_APP_DATA_MAX_SIZE which is defined in "commissioning.h".
	*appDataSize max value is LORAWAN_APP_DATA_MAX_SIZE.
	*if enabled AT, don't modify LORAWAN_APP_DATA_MAX_SIZE, it may cause system hanging or failure.
	*if disabled AT, LORAWAN_APP_DATA_MAX_SIZE can be modified, the max value is reference to lorawan region and SF.
	*for example, if use REGION_CN470, 
	*the max value for different DR can be found in MaxPayloadOfDatarateCN470 refer to DataratesCN470 and BandwidthsCN470 in "RegionCN470.h".
	*/
  uint16_t vBat = getBatteryVoltage();
  uint8_t loBat = lowByte(vBat);
  uint8_t hiBat = highByte(vBat);

  //temp = getTemperature(); // read external sensor
  temp =  23.4;
  if (DEBUG) Serial.println("Temperature: " + String(temp,1) + " *C");
  
  uint16_t Tmp = (uint16_t) (temp * 10 + .5); // send temp*10 as integer
  uint8_t loTmp = lowByte(Tmp);
  uint8_t hiTmp = highByte(Tmp);

  Wire.end();
  digitalWrite(Vext, HIGH);
  
  appDataSize = 4;
  appData[0] = hiBat;
  appData[1] = loBat;
  appData[2] = hiTmp;
  appData[3] = loTmp;
}


void setup() 
{
	boardInitMcu();
  pinMode(Vext, OUTPUT);
  Wire.begin();
	if (DEBUG) Serial.begin(115200);
  
  //initBMP280();
  
#if(AT_SUPPORT)
	enableAt();
#endif
	deviceState = DEVICE_STATE_INIT;
	LoRaWAN.ifskipjoin();
}

void loop()
{
	switch( deviceState )
	{
		case DEVICE_STATE_INIT:
		{
#if(AT_SUPPORT)
			getDevParam();
#endif
			printDevParam();
			LoRaWAN.init(loraWanClass,loraWanRegion);
			deviceState = DEVICE_STATE_JOIN;
			break;
		}
		case DEVICE_STATE_JOIN:
		{
			LoRaWAN.join();
			break;
		}
		case DEVICE_STATE_SEND:
		{
			prepareTxFrame( appPort );
			LoRaWAN.send();
			deviceState = DEVICE_STATE_CYCLE;
			break;
		}
		case DEVICE_STATE_CYCLE:
		{
			// Schedule next packet transmission
			txDutyCycleTime = APP_TX_DUTYCYCLE + randr( 0, APP_TX_DUTYCYCLE_RND );
			LoRaWAN.cycle(txDutyCycleTime);
			deviceState = DEVICE_STATE_SLEEP;
			break;
		}
		case DEVICE_STATE_SLEEP:
		{
			LoRaWAN.sleep();
			break;
		}
		default:
		{
			deviceState = DEVICE_STATE_INIT;
			break;
		}
	}
}
