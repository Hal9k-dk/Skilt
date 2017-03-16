
#include <ESP8266WiFi.h>
#include <RCSwitch.h>

#include <DNSServer.h>
#include <ESP8266WebServer.h>
#include <WiFiManager.h>   

#include <ESP8266mDNS.h>
#include <WiFiUdp.h>
#include <ArduinoOTA.h>

#include <Adafruit_NeoPixel.h>

#define HOSTNAME "hal9k-skilt"     // Friedly hostname
#define AP_NAME "ConfigAP-hal9k-skilt"   // Friedly Autoconfig ap name 

#define PIN 3

int brightness = 50;       // Global variable for storing the brightness (255 == 100%)

// Parameter 1 = number of pixels in strip
// Parameter 2 = Arduino pin number (most are valid)
// Parameter 3 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)
//   NEO_RGBW    Pixels are wired for RGBW bitstream (NeoPixel RGBW products)
Adafruit_NeoPixel strip = Adafruit_NeoPixel(200, PIN, NEO_GRB + NEO_KHZ800);

int temp = 0;
int numpixel = strip.numPixels() - 2;

// IMPORTANT: To reduce NeoPixel burnout risk, add 1000 uF capacitor across
// pixel power leads, add 300 - 500 Ohm resistor on first pixel's data input
// and minimize distance between Arduino and first pixel.  Avoid connecting
// on a live circuit...if you must, connect GND first.


// ***************************************************************************
// Callback for WiFiManager library when config mode is entered
// ***************************************************************************
//gets called when WiFiManager enters configuration mode
void configModeCallback (WiFiManager *myWiFiManager) {
  //Serial.println("Entered config mode");
  //Serial.println(WiFi.softAPIP());
  //if you used auto generated SSID, print it
  //Serial.println(myWiFiManager->getConfigPortalSSID());
}


void setup() {
  // This is for Trinket 5V 16MHz, you can remove these three lines if you are not using a Trinket
  #if defined (__AVR_ATtiny85__)
    if (F_CPU == 16000000) clock_prescale_set(clock_div_1);
  #endif

  WiFiManager wifiManager;
  wifiManager.setAPCallback(configModeCallback);

  if (!wifiManager.autoConnect(AP_NAME)) {
    //Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }
  // End of trinket special code


  strip.begin();
  strip.setBrightness(brightness);
  strip.show(); // Initialize all pixels to 'off'

  if (!wifiManager.autoConnect(AP_NAME)) {
    //Serial.println("failed to connect and hit timeout");
    //reset and try again, or maybe put it to deep sleep
    ESP.reset();
    delay(1000);
  }

  ArduinoOTA.onStart([]() {
    strip.clear();
    strip.show();
    strip.setPixelColor(2,strip.Color(150,0,0)); // Moderately bright green color.
    strip.setPixelColor(3,strip.Color(150,0,0)); // Moderately bright green color.
    strip.setPixelColor(4,strip.Color(150,0,0)); // Moderately bright green color.
    strip.setPixelColor(5,strip.Color(150,0,0)); // Moderately bright green color.
    strip.setPixelColor(6,strip.Color(150,0,0)); // Moderately bright green color.
    strip.setPixelColor(7,strip.Color(150,0,0)); // Moderately bright green color.
    strip.setPixelColor(8,strip.Color(150,0,0)); // Moderately bright green color.
    strip.setPixelColor(9,strip.Color(150,0,0)); // Moderately bright green color.
    strip.setPixelColor(10,strip.Color(150,0,0)); // Moderately bright green color.
    
    strip.show(); // Initialize all pixels to 'off'
  });
  //ArduinoOTA.setHostname((const char *)hostname.c_str());
  ArduinoOTA.begin();
  
}

void loop() {
  ArduinoOTA.handle();

  
  // Some example procedures showing how to display to the pixels:
  //colorWipe(strip.Color(255, 0, 0), 50); // Red
  //colorWipe(strip.Color(0, 255, 0), 50); // Green
  //  colorWipe(strip.Color(0, 0, 255), 50); // Blue
//colorWipe(strip.Color(0, 0, 0, 255), 50); // White RGBW
  // Send a theater pixel chase in...
  //theaterChase(strip.Color(127, 127, 127), 50); // White
  //theaterChase(strip.Color(127, 0, 0), 50); // Red
  //theaterChase(strip.Color(0, 0, 127), 50); // Blue
  /*strip.clear();
  strip.show();
  colorWipe(strip.Color(255, 0, 0), 200); // Red
  ArduinoOTA.handle();
  strip.clear();
  strip.show();
  colorWipe(strip.Color(0, 255, 0), 200); // green
  ArduinoOTA.handle();
  strip.clear();
  strip.show();
  colorWipe(strip.Color(0, 0, 255), 200); // blue
  ArduinoOTA.handle();*/

  rainbow(5);
  ArduinoOTA.handle();
  rainbowCycle(5);
  ArduinoOTA.handle();
  //theaterChaseRainbow(50);

  hal9klogo();
  delay(5000); 
}


// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<numpixel; i++) {
    strip.setPixelColor(i+2, c);
    strip.show();
    delay(wait);
    ArduinoOTA.handle();
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<numpixel; i++) {
      strip.setPixelColor(i+2, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< numpixel; i++) {
      strip.setPixelColor(i+2, Wheel(((i * 256 / numpixel) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
