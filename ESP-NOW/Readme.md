# Low-Power Sensornetzwerk

Das im Blogbeitrag https://ckarduino.wordpress.com/2021/10/09/low-power-sensornetzwerk/ beschriebene Sensornetzwerk verwendet batterie- bzw. solarbetriebene Sensorknoten, 
die aus *DFRobot FireBeetle ESP32-E* oder *ThingPulse ePulse Low Power ESP32* Boards bestehen. Die erhobenen Messwerte werden über ESP-NOW an ein *ThingPulse ESP-Gateway* gesendet.

Das *ThingPulse ESP-Gateway* besteht aus zwei ESP32-Wrover-IB-Modulen. Detaillierte Informationen zum *ThingPulse ESP-Gateway* sind unter der URL 
https://thingpulse.com/new-product-the-espgateway-design/ zu finden.

Vom ersten ESP32 werden die über ESP-NOW gesendeten Messages empfangen (ESP-NOW Receiver), aufbereitet und seriell an den zweiten ESP32 im Gateway transferiert. 
Dieser übernimmt diese Daten und bereitet sie zum Versenden über MQTT auf.

Die MQTT Messages sende ich an den Public MQTT Broker von HiveMQ (https://www.hivemq.com/public-mqtt-broker/). Mit einem MQTT Client auf einem PC, Tablet oder Smartphone 
können einzelne oder alle MQTT Messages abonniert (subscribed) werden.
