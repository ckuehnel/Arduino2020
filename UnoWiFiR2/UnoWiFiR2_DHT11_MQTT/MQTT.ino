//   MQTT configuration and connection

void configureMQTT()
{
  /* configure the MQTT server with IPaddress and port */
  client.setServer(BROKER, BRPORT);
  /* this receivedCallback function will be invoked 
  when client received subscribed topic */
  client.setCallback(receivedCallback);
  if (DEBUG)
  {
    Serial.println("Node will publish the topics:");
    Serial.print("> "); Serial.println(TEMP_TOPIC);
    Serial.print("> "); Serial.println(HUMI_TOPIC);
  }
}

void receivedCallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message received: ");
  Serial.println(topic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) Serial.print((char)payload[i]);
}

void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.print("MQTT connecting ...");
    
    /* connected now */
    if (client.connect(CLIENTID, BRUSER, BRPWD)) Serial.println("connected");
    else 
    {
      Serial.print("failed, status code = ");
      Serial.println(client.state());
      Serial.println("try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}
