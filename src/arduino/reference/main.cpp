// Directives
#define FASTLED_ALLOW_INTERRUPTS 0

// Includes
#include "Arduino.h"
#include <WiFi.h>
#include <WiFiUdp.h>
#include <ETH.h>
#include "FastLED.h"

// NETWORKW
#define NET_ID 28

// LED constants
#define NUM_DATA_OUT 3
#define NUM_LEDS_PER_DATA_OUT 144 + 36
#define NUM_LEDS_TOTAL NUM_LEDS_PER_DATA * NUM_DATA_OUT
#define NUM_LEDS_MAX_PER_DATA_OUT 480

// Data pin that led data will be written out over
#define DATA_PIN 33

// Clock pin only needed for SPI based chipsets when not using hardware SPI
#define CLOCK_PIN_0 16
#define CLOCK_PIN_1 17
#define CLOCK_PIN_2 32

// Define LED arrays
CRGB leds[NUM_DATA_OUT][NUM_LEDS_PER_DATA_OUT];

// const uint8_t PROGMEM gamma8[] = 
// {
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
// 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1,
// 1, 1, 1, 1, 1, 1, 1, 1, 1, 2, 2, 2, 2, 2, 2, 2,
// 2, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 5, 5, 5,
// 5, 6, 6, 6, 6, 7, 7, 7, 7, 8, 8, 8, 9, 9, 9, 10,
// 10, 10, 11, 11, 11, 12, 12, 13, 13, 13, 14, 14, 15, 15, 16, 16,
// 17, 17, 18, 18, 19, 19, 20, 20, 21, 21, 22, 22, 23, 24, 24, 25,
// 25, 26, 27, 27, 28, 29, 29, 30, 31, 32, 32, 33, 34, 35, 35, 36,
// 37, 38, 39, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 50,
// 51, 52, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 64, 66, 67, 68,
// 69, 70, 72, 73, 74, 75, 77, 78, 79, 81, 82, 83, 85, 86, 87, 89,
// 90, 92, 93, 95, 96, 98, 99,101,102,104,105,107,109,110,112,114,
// 115,117,119,120,122,124,126,127,129,131,133,135,137,138,140,142,
// 144,146,148,150,152,154,156,158,160,162,164,167,169,171,173,175,
// 177,180,182,184,186,189,191,193,196,198,200,203,205,208,210,213,
// 215,218,220,223,225,228,231,233,236,239,241,244,247,249,252,255 
// };



// UDP
const uint16_t  BUFFERSIZE = NUM_LEDS_MAX_PER_DATA_OUT * 3 + 4; // The +4 is for the header
uint8_t         packetBufferPrimary[BUFFERSIZE];

int             status = WL_IDLE_STATUS;
WiFiUDP         Udp;
static bool     eth_connected = false;
unsigned int    localPort = 6969;

// Method Signatures
void NetworkEvent(WiFiEvent_t event);
void setupLEDs();
void setupUDP();
void ready();
void NetworkData();
void DefaultData();
bool ReadInPackets(uint8_t b[]);

// Setup ethernet
void NetworkEvent(WiFiEvent_t event)
{
  switch (event) {
    case SYSTEM_EVENT_ETH_START:
      Serial.println();
      Serial.println("SYSTEM_EVENT_ETH_START");
      //set eth hostname here
      ETH.setHostname("esp32-ethernet");
      break;
    case SYSTEM_EVENT_ETH_CONNECTED:
      Serial.println();
      Serial.println("SYSTEM_EVENT_ETH_CONNECTED");
      Serial.println("ETH Connected");
      Serial.print(ETH.localIP());
      break;
    case SYSTEM_EVENT_ETH_GOT_IP:
      Serial.println();
      Serial.println("SYSTEM_EVENT_ETH_GOT_IP");
      Serial.print("ETH MAC: ");
      Serial.print(ETH.macAddress());
      Serial.print(", IPv4: ");
      Serial.print(ETH.localIP());
      if (ETH.fullDuplex()) {
        Serial.print(", FULL_DUPLEX");
      }
      Serial.print(", ");
      Serial.print(ETH.linkSpeed());
      Serial.println("Mbps");
      ready();
      
      break;
    case SYSTEM_EVENT_ETH_DISCONNECTED:
      Serial.println();
      Serial.println("SYSTEM_EVENT_ETH_DISCONNECTED");
      Serial.println("ETH Disconnected");
      eth_connected = false;
      break;
    case SYSTEM_EVENT_ETH_STOP:
      Serial.println();
      Serial.println("SYSTEM_EVENT_ETH_STOP");
      Serial.println("ETH Stopped");
      eth_connected = false;
      break;
    default:
      break;
  }
}

void setupLEDs()
{
  delay(1000);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN_0, RGB, DATA_RATE_MHZ(2)>(leds[0], NUM_LEDS_PER_DATA_OUT);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN_1, RGB, DATA_RATE_MHZ(2)>(leds[1], NUM_LEDS_PER_DATA_OUT);
  FastLED.addLeds<APA102, DATA_PIN, CLOCK_PIN_2, RGB, DATA_RATE_MHZ(2)>(leds[2], NUM_LEDS_PER_DATA_OUT);
}

void setupUDP()
{
  Serial.begin(115200);
  WiFi.onEvent(NetworkEvent);
  IPAddress ip(192, 168, 50, NET_ID);
  IPAddress gateway(192,168,50,1);
  IPAddress subnet(255,255,255,0);
  ETH.begin();
  ETH.config(ip, gateway, subnet);
}

void ready()
{
  eth_connected = true;
  Udp.begin(localPort);
  Serial.print(ETH.localIP());
}

void setup() 
{
  setupLEDs();
  setupUDP();
}

void loop() 
{
  if (eth_connected) NetworkData();
  else DefaultData();
}

void NetworkData()
{
  if(ReadInPackets(packetBufferPrimary))
  {
    FastLED.show();
  }
}

bool ReadInPackets(uint8_t b[])
{
  int expected[3] = { 1, 1, 1 };
  do
  {
    int packetSize = Udp.parsePacket();
    if (packetSize) 
    {
      int len = Udp.read(b, packetSize);
      if(len == 0) continue;
  
      uint16_t channel  = (0x0000     | b[0]);
      uint16_t ledCount = (packetSize - 4) / 3;

      // Check if we've already found this
      if(expected[channel] == 0) continue;
      expected[channel] = 0;
      
      int dataCounter = 4;
      for (uint16_t i = 0; i < ledCount; i++)
      {
        leds[channel][i].r = b[dataCounter+2];
        leds[channel][i].g = b[dataCounter+1];
        leds[channel][i].b = b[dataCounter  ];
        dataCounter += 3;
      }
    }
  } 
  while(expected[0] || expected[1] || expected[2]);
  return true;
}

void DefaultData()
{
   // Move a single white led 
   for(int idx = 0; idx < NUM_LEDS_PER_DATA_OUT; idx = idx + 1) 
   {
      // Turn our current led on to white, then show the leds
      leds[0][idx] = CRGB::Red;
      leds[1][idx] = CRGB::Green;
      leds[2][idx] = CRGB::Blue;
      
      // Show the leds (only one of which is set to white, from above)
      FastLED.show();

      // Wait a little bit
      delay(5);

      // Turn our current led back to black for the next loop around
      leds[0][idx] = CRGB::Black;
      leds[1][idx] = CRGB::Black;
      leds[2][idx] = CRGB::Black;
   }
}