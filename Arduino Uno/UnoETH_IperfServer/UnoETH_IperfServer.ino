/*
 Chat  Server

 A simple server that distributes any incoming messages to all
 connected clients.  To use telnet to  your device's IP address and type.
 You can see the client's input in the serial monitor as well.
 Using an Arduino Wiznet Ethernet shield.

 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 * Analog inputs attached to pins A0 through A5 (optional)

 created 18 Dec 2009
 by David A. Mellis
 modified 9 Apr 2012
 by Tom Igoe

 */

#include <SPI.h>
#include <Ethernet.h>

// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network.
// gateway and subnet are optional:
byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 177);
IPAddress gateway(192, 168, 1, 1);
IPAddress dns_server(8, 8, 8, 8);
IPAddress subnet(255, 255, 0, 0);


// telnet defaults to port 23
EthernetServer server(5001);
//EthernetServer server2(5005);
//boolean alreadyConnected = false; // whether or not the client was connected previously

void setup() {
  // initialize the ethernet device
  Ethernet.begin(mac, ip, dns_server, gateway, subnet);
  // start listening for clients
  server.begin();
  // Open serial communications and wait for port to open:
  //Serial.begin(9600);
  //while (!Serial) {
  //  ; // wait for serial port to connect. Needed for Leonardo only
  //}


  //Serial.print("Iperf server address : ");
  //Serial.println(Ethernet.localIP());
  //Serial.println(" ");
  SPI.setClockDivider(SPI_CLOCK_DIV2);
}

void loop() {
  byte buf[1024];
  // wait for a new client:
  EthernetClient client = server.available();
  //EthernetClient client2 = server2.available();
  // when the client sends the first byte, say hello:
  //if (client | client2) {
    if (client) {
//    Serial.println("Here is new client for check arduino performance");
  //while (client.connected() | client2.connected())
  while (client.connected())
    {
      if (client.available()) client.read(buf, 1024);
    //  if (client2.available()) client2.read(buf,1024);
   }
    // close the connection:
    client.stop();
   // client2.stop();
//    Serial.println("client disonnected");
  }
}
