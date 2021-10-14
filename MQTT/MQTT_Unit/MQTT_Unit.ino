/*
 * File: MQTT_Unit.ino
 * 
 * M5Stack MQTT Unit with RJ45 Port (APM32F103) handles MQTT messages over Ethernet controlled by UART Interface
 * Microcontrollers without Ethernet / WiFi can use this module to establish MQTT connections. 
 * I used an M5Stack Atom Lite for a simple implementation. But, using Arduino Uno is possible, too.
 * 
 * based on https://github.com/m5stack/UNIT_MQTT/blob/master/examples/MQTT/MQTT.ino
 * 
 * 2021-10-14/ck
 */
#include "UNIT_MQTT.h"

UNIT_MQTT unit;
void setup()
{
    Serial.begin(115200);
    //INIT UNIT MQTT
    unit.Init(&Serial2, 9600, 32, 26);
    Serial.println("Waiting LAN Connect");
    while (!unit.isConnectedLAN())Serial.print('.');

    Serial.println("LAN Connected");

    Serial.println("Config MQTT");

    unit.configMQTT(
        "broker.hivemq.com",//host
        "1883",//port
        "client",//client id
        "",//user name
        "",//password
        "60"//keepalive
    );

    Serial.println("Subcribe Topic");
    unit.subscribe({
        "1",//No 1~-4
        "UNIT_MQTT_TOPIC_1",//Topic
        "0"//QoS
    });

    unit.subscribe({
        "2",
        "UNIT_MQTT_TOPIC_2",
        "0"
    });

    Serial.println("Save config and reset");
    unit.configSave();
    unit.startMQTT();
    Serial.println("Start MQTT Connect");
    while(!unit.isConnectedMQTT());
    Serial.println("MQTT Server Connected");
}

void loop()
{
    if(unit.isConnectedMQTT())
    {   
        String readstr = unit.waitMsg(1000);
        Serial.print(readstr);
        unit.publish({
            "UNIT_MQTT_TOPIC_1",//Topic
            "This is M5Stack UNIT MQTT: "+String(millis()),//Data
            "0"//QoS
        });
        delay(7000); // for a cycle of 10 sec
    }
    else
    {
        Serial.print("MQTT disconnect");
    }
}
