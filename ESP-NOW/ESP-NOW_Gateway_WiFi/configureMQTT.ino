void configureMQTT()
{
  /* configure the MQTT server with IPaddress and port */
  client.setServer(BROKER, BRPORT);
  /* this receivedCallback function will be invoked 
  when client received subscribed topic */
  client.setCallback(receivedCallback);
}

void receivedCallback(char* topic, byte* payload, unsigned int length) 
{
  Serial.print("Message received: ");
  Serial.println(topic);

  Serial.print("payload: ");
  for (int i = 0; i < length; i++) Serial.print((char)payload[i]);
  Serial.println();
}

void mqttconnect() {
  /* Loop until reconnected */
  while (!client.connected()) {
    Serial.println("\nMQTT connecting ...");
    
    /* connect now */
    if (client.connect(CLIENTID /*, BRUSER, BRPWD*/)) 
    {
      Serial.println("connected");
    } else {
      Serial.print("failed, status code =");
      Serial.print(client.state());
      Serial.println("   try again in 5 seconds");
      /* Wait 5 seconds before retrying */
      delay(5000);
    }
  }
}
