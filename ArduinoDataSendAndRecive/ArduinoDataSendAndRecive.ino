#include <SPI.h>         
#include <Ethernet.h>
#include <EthernetUdp.h>         
#include <dht.h>

#define DHT11_PIN 7

dht DHT;
int temp=0;
int chk=0;

 


byte mac[] = {
  0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED
};
IPAddress ip(192, 168, 1, 15);

unsigned int localPort = 8888;      // local port to listen on

// buffers for receiving and sending data
char packetBuffer[UDP_TX_PACKET_MAX_SIZE];  //buffer to hold incoming packet,
//char  ReplyBuffer[] = "21°C";       // a string to send back

char  ReplyBuffer[7];
int c=0;
// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setup() {
  // start the Ethernet and UDP:
  Ethernet.begin(mac, ip);
  Udp.begin(localPort);
 
  Serial.begin(9600);
}

void loop() {

  chk = DHT.read11(DHT11_PIN);
  temp=DHT.temperature;
  sprintf(ReplyBuffer, "%d", temp);
  
  // send a packet
    Udp.beginPacket("192.168.1.8", 8888);
    Udp.write(ReplyBuffer);
   
    Udp.endPacket();

   
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    Serial.print("Received packet of size ");
    Serial.println(packetSize);
    Serial.print("From ");
    IPAddress remote = Udp.remoteIP();
    for (int i = 0; i < 4; i++) {
      Serial.print(remote[i], DEC);
      if (i < 3) {
        Serial.print(".");
      }
    }
    Serial.print(", port ");
    Serial.println(Udp.remotePort());

    // read the packet into packetBufffer
    Udp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    Serial.println("Contents:");
    Serial.println(packetBuffer);

 
   
  }

  delay(5000);
}
