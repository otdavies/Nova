// NETWORK
#define NET_ID 69 // Network id
unsigned int localPort = 8888; // local port to listen on
byte mac[] = {(byte)NET_ID, 0xAD, 0xBE, 0xEF, 0xFE, 0xED}; // Mac address

#include <NativeEthernet.h>
#include <NativeEthernetUdp.h>
#include "FastLED.h"

// LED constants
#define NUM_DATA_OUT 5
#define NUM_LEDS_PER_DATA_OUT 180
#define NUM_LEDS_TOTAL NUM_LEDS_PER_DATA_OUT *NUM_DATA_OUT
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

// UDP constants
const uint16_t BUFFERSIZE = NUM_LEDS_TOTAL * 3 + 4; // The +4 is for the header
uint8_t ledBuffer[BUFFERSIZE];
IPAddress ip(10, 0, 1, NET_ID);
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

void NetworkPattern() {
  int packetSize = Udp.parsePacket();
  if (packetSize)
  {
    prettyPrint(packetSize);

    // read the packet into packetBufffer
    Udp.read(ledBuffer, BUFFERSIZE);
    byte header = (byte)ledBuffer[0];
    for (uint16_t i = 1; i < packetSize; i+=3)
    {
      Serial.print(header);
      leds[header][i    ] = ledBuffer[i    ];
      leds[header][i + 1] = ledBuffer[i + 1];
      leds[header][i + 2] = ledBuffer[i + 2];
    }

    FastLED.show();
    digitalWrite(13, HIGH);

    // Send Ack
    Udp.beginPacket(Udp.remoteIP(), Udp.remotePort());
    Udp.write((byte)0);
    Udp.endPacket();
  } else {
      digitalWrite(13, LOW); 
  }
}

void loop()
{
  if(Ethernet.linkStatus() == LinkOFF) DefaultPattern();
  else NetworkPattern();
}

