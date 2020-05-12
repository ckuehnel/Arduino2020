/*
 Basic ESP8266 MQTT example
 This sketch demonstrates the capabilities of the pubsub library in combination
 with the ESP8266 board/library.
 It connects to an MQTT server then:
  - publishes "hello world" to the topic "outTopic" every two seconds
  - subscribes to the topic "inTopic", printing out any messages
    it receives. NB - it assumes the received payloads are strings not binary
  - If the first character of the topic "inTopic" is an 1, switch ON the ESP Led,
    else switch it off
 It will reconnect to the server if the connection is lost using a blocking
 reconnect function. See the 'mqtt_reconnect_nonblocking' example for how to
 achieve the same result without blocking the main loop.
 To install the ESP8266 board, (using Arduino 1.6.4+):
  - Add the following 3rd party board manager under "File -> Preferences -> Additional Boards Manager URLs":
       http://arduino.esp8266.com/stable/package_esp8266com_index.json
  - Open the "Tools -> Board -> Board Manager" and click install for the ESP8266"
  - Select your ESP8266 in "Tools -> Board"
*/

#include <ESP8266WiFi.h>
#include <PubSubClient.h>
#include <Adafruit_NeoPixel.h>
#include "arduino_secrets.h"

uint8_t Red = 0, Green = 0, Blue = 0;

#define WHITE 100, 100, 100
#define OFF     0,   0,   0

#define PIN   D4
#define LED_NUM 7

Adafruit_NeoPixel leds = Adafruit_NeoPixel(LED_NUM, PIN, NEO_GRB + NEO_KHZ800);

WiFiClient espClient;
PubSubClient client(espClient);
unsigned long lastMsg = 0;
#define MSG_BUFFER_SIZE  (50)
char msg[MSG_BUFFER_SIZE];
int value = 0;

void setup_wifi() 
{
  delay(10);
  Serial.println();
  Serial.print("Connecting to ");
  Serial.println(mySSID);

  WiFi.begin(mySSID, myPW);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }

  randomSeed(micros());

  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Message arrived [");
  Serial.print(topic);
  Serial.print("] ");
  for (int i = 0; i < length; i++) {
    Serial.print((char)payload[i]);
  }
  Serial.println();
  if (strcmp(topic, "D1Lamp/R") == 0) 
  {
    //Serial.print("RED "); 
    byte val = 0;
    for (int i = 0; i < length; i++)
    {
      val += (payload[i] - '0') * pow(10, (length-1)-i);
    }
    //Serial.println(val);
    Red = val; led_set(Red, Green, Blue);
  }
  if (strcmp(topic, "D1Lamp/G") == 0) 
  {
    //Serial.println("GREEN");
    byte val = 0;
    for (int i = 0; i < length; i++)
    {
      val += (payload[i] - '0') * pow(10, (length-1)-i);
    }
    //Serial.println(val);
    Green = val; led_set(Red, Green, Blue);
  }
  if (strcmp(topic, "D1Lamp/B") == 0) 
  {
    //Serial.println("BLUE");
    byte val = 0;
    for (int i = 0; i < length; i++)
    {
      val += (payload[i] - '0') * pow(10, (length-1)-i);
    }
    //Serial.println(val);
    Blue = val; led_set(Red, Green, Blue);
  }
}

void reconnect() 
{
  // Loop until we're reconnected
  while (!client.connected()) 
  {
    Serial.print("Attempting MQTT connection...");
    // Create a random client ID
    String clientId = "ESP8266Client-";
    clientId += String(random(0xffff), HEX);
    // Attempt to connect
    if (client.connect(CLIENTID, BRUSER, BRPWD)) 
    {
      Serial.println("connected");
      // Once connected, publish an announcement...
      client.publish("D1Lamp/Life", "D1Lamp in operation");
      // ... and resubscribe
      client.subscribe("D1Lamp/#");
    } else {
      Serial.print("failed, rc=");
      Serial.print(client.state());
      Serial.println(" try again in 5 seconds");
      // Wait 5 seconds before retrying
      delay(5000);
    }
  }
}

void setup() 
{
  pinMode(BUILTIN_LED, OUTPUT);     // Initialize the BUILTIN_LED pin as an output
  leds.begin(); // Initialize the NeoPixel library.
  Serial.begin(115200);
  delay(1000); // wait for Serial Monitor
  Serial.println("\nD1Lamp MQTT Control...");
  flash(WHITE); delay(500); flash(WHITE); delay(500); flash(WHITE); delay(500); flash(OFF);
  setup_wifi();
  client.setServer(BROKER, BRPORT);
  client.setCallback(callback);
}

void loop() 
{
  if (!client.connected()) reconnect();
  client.loop();

  unsigned long now = millis();
  if (now - lastMsg > 60000) 
  {
    lastMsg = now;
    ++value;
    snprintf (msg, MSG_BUFFER_SIZE, "D1Lamp is live #%ld", value);
    Serial.print("Publish message: "); Serial.println(msg); client.publish("D1Lamp/Life", msg);
  }
}
