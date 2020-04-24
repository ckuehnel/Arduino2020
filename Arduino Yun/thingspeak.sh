#!/bin/sh

echo "Send data to Thingspeak Server"

#Thingspeak
api_key='RW34BSSLBODV2HHK'

DATE="$(date +"%d-%m-%Y")"
read SHT31_TEMP < /tmp/TEMP		
echo "SHT31 Temperature   = $TEMP *C"
read SHT31_HUMI < /tmp/HUMI		
echo "SHT31 Rel. Humidity = $HUMI %RH"
read OWM_TEMP < /root/TEMP		
echo "OWM Temperature    = $TEMP *C"
read OWM_HUMI < /tmp/HUMI		
echo "OWM Rel. Humidity  = $HUMI %RH"

curl --insecure --data \
     "api_key=$api_key&field1=$SHT31_TEMP&field2=$SHT31_HUMI \
	                  &field3=$OWM_TEMP&field4=$OWM_HUMI&field5=$DATE" \
     https://api.thingspeak.com/update  > log 2>&1
