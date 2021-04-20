/*
 UDPSendReceiveString:
 This sketch receives UDP message strings, prints them to the serial port
 and sends an "acknowledge" string back to the sender
 A Processing sketch is included at the end of file that can be used to send
 and received messages for testing with a computer.
 created 21 Aug 2010
 by Michael Margolis
 This code is in the public domain.
 */

// NETWORK
#define NET_ID 69

#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
#include "FastLED.h"

// LED constants
#define NUM_DATA_OUT 5
#define NUM_LEDS_PER_DATA_OUT 150
#define NUM_LEDS_TOTAL NUM_LEDS_PER_DATA_OUT *NUM_DATA_OUT
#define NUM_LEDS_MAX_PER_DATA_OUT 480
#define MHZ 14

// Data pin that led data will be written out over
#define DATA_PIN_0 0
#define DATA_PIN_1 1
#define DATA_PIN_2 2
#define DATA_PIN_3 3
#define DATA_PIN_4 18

// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define CLOCK_PIN_0 4
#define CLOCK_PIN_1 5
#define CLOCK_PIN_2 6
#define CLOCK_PIN_3 7
#define CLOCK_PIN_4 19

CRGB leds[NUM_DATA_OUT][NUM_LEDS_PER_DATA_OUT];

// UDP
const uint16_t BUFFERSIZE = NUM_LEDS_TOTAL * 3 + 4; // The +4 is for the header
uint8_t packetBufferPrimary[BUFFERSIZE];

byte mac[] = {0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED};
IPAddress ip(10, 0, 0, NET_ID);

unsigned int localPort = 8888; // local port to listen on

// An EthernetUDP instance to let us send and receive packets over UDP
EthernetUDP Udp;

void setupLEDs()
{
  FastLED.addLeds<APA102, DATA_PIN_0, CLOCK_PIN_0, BGR, DATA_RATE_MHZ(MHZ)>(leds[0], NUM_LEDS_PER_DATA_OUT);
  FastLED.addLeds<APA102, DATA_PIN_1, CLOCK_PIN_1, BGR, DATA_RATE_MHZ(MHZ)>(leds[1], NUM_LEDS_PER_DATA_OUT);
  FastLED.addLeds<APA102, DATA_PIN_2, CLOCK_PIN_2, BGR, DATA_RATE_MHZ(MHZ)>(leds[2], NUM_LEDS_PER_DATA_OUT);
  FastLED.addLeds<APA102, DATA_PIN_3, CLOCK_PIN_3, BGR, DATA_RATE_MHZ(MHZ)>(leds[3], NUM_LEDS_PER_DATA_OUT);
  FastLED.addLeds<APA102, DATA_PIN_4, CLOCK_PIN_4, BGR, DATA_RATE_MHZ(MHZ)>(leds[4], NUM_LEDS_PER_DATA_OUT);
}

void setupUDP()
{
  // start the Ethernet
  Ethernet.begin(mac, ip);

  // Open serial communications and wait for port to open:
  Serial.begin(9600);
  // while (!Serial)
  // {
  //   ; // wait for serial port to connect. Needed for native USB port only
  // }

  // Check for Ethernet hardware present
  if (Ethernet.hardwareStatus() == EthernetNoHardware)
  {
    Serial.println("Ethernet shield was not found.  Sorry, can't run without hardware. :(");
    while (true)
    {
      delay(1); // do nothing, no point running without Ethernet hardware
    }
  }
  if (Ethernet.linkStatus() == LinkOFF)
  {
    Serial.println("Ethernet cable is not connected.");
  }

  // start UDP
  Udp.begin(localPort);
}

void setup()
{
  setupUDP();
  delay(100);
  setupLEDs();
}

void prettyPrint(int packetSize)
{
  Serial.print("Received packet of size ");
  Serial.println(packetSize);
  Serial.print("From ");

  IPAddress remote = Udp.remoteIP();
  for (int i = 0; i < 4; i++)
  {
    Serial.print(remote[i], DEC);
    if (i < 3)
    {
      Serial.print(".");
    }
  }
  Serial.print(", port ");
  Serial.println(Udp.remotePort());
}

int count = 0;

void DefaultPattern()
{
   // Move a single white led 
   for(int idx = 0; idx < NUM_LEDS_PER_DATA_OUT; idx = idx + 1) 
   {
      // Turn our current led on to white, then show the leds
      leds[0][idx] = CRGB::Red;
      leds[1][idx] = CRGB::Green;
      leds[2][idx] = CRGB::Blue;
      leds[3][idx] = CRGB::Yellow; // / 128;
      leds[4][idx] = CRGB::White;
      digitalWrite(13, HIGH);

      // Show the leds (only one of which is set to white, from above)
      FastLED.show();

      // Wait a little bit
      delay(35);

      // Turn our current led back to black for the next loop around
      leds[0][idx] = CRGB::Black;
      leds[1][idx] = CRGB::Black;
      leds[2][idx] = CRGB::Black;
      leds[3][idx] = CRGB::Black;
      leds[4][idx] = CRGB::Black;
      digitalWrite(13, LOW);
      

      
   }
}

void loop()

{
  // if there's data available, read a packet
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    prettyPrint(packetSize);

    // read the packet into packetBufffer
    // memset(packetBufferPrimary, 0, sizeof packetBufferPrimary);
    Udp.read(packetBufferPrimary, BUFFERSIZE);

    for (uint16_t i = 0; i < packetSize; i++)
    {
      // Serial.print((byte)buf[i]);
    }
    Serial.println(count++);

    // // send a reply to the IP address and port that sent us the packet we received
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write((byte)0);
    Udp.endPacket();
  }
  else {
     DefaultPattern();
  }
  
}

