#include <Adafruit_NeoPixel.h>
#define PIN 8
#define NUMPIXELS 8

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
unsigned long curTime = 0;

// Light Switching Input
int frontLightSwitchPin = 2;
int backLightSwitchPin = 1;

// Front Light Vars
const int frontLightPin = 5;
const int frontLightDelay = 50;
int frontLightState = LOW;
unsigned long frontLightPrevTime = 0;
int frontLightTimeDelta = 0;

// Back Light Vars
int rainbowDelay = 20;
unsigned long rainbowPrevTime = 0;
int rainbowColorJ = 0;
int rainbowTimeDelta = 0;

int stripBlinkDelay = 100;
unsigned int stripBlinkPrevTime = 0;
int stripBlinkState = 255;
int stripBlinkTimeDelta = 0;

// Charging Status
int batStat1Pin = 9;
int batStat2Pin = 10;
int batStat1 = LOW;
int batStat2 = LOW;

void setup() {
  // input
  // light switching
  pinMode(frontLightSwitchPin, INPUT_PULLUP);
  pinMode(backLightSwitchPin, INPUT_PULLUP);
  // charge status
  pinMode(batStat1Pin, INPUT);
  pinMode(batStat2Pin, INPUT);

  // front light
  pinMode(frontLightPin, OUTPUT);

  // setup strip (back light)
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

  digitalWrite(frontLightPin, frontLightState);

}

// Charging Functions
void setFirstPixel(uint32_t color) {
  for(uint16_t ii=0; ii<strip.numPixels(); ii++) {
    if ( ii == 0 ) {
      strip.setPixelColor(ii, color);
    } else {
      strip.setPixelColor(ii, strip.Color(0, 0, 0));
    }
  }
  strip.show();
  digitalWrite(frontLightPin, LOW);
}
void batStatCharging() {
  setFirstPixel(strip.Color(10, 10, 0));
}
void batStatCharged() {
  setFirstPixel(strip.Color(0, 10, 0));
}


// Front Light Functions
void frontLightBlink() {

  frontLightTimeDelta = curTime - frontLightPrevTime;

  if ( frontLightTimeDelta >= frontLightDelay ) {
    frontLightPrevTime = curTime;

    if ( frontLightState == LOW )
      frontLightState = HIGH;
    else
      frontLightState = LOW;

    digitalWrite(frontLightPin, frontLightState);

  }

}
void frontLightSolid() {
  frontLightState = HIGH;
  digitalWrite(frontLightPin, frontLightState);
}


// Back Light Functions
void rainbow() {
  uint16_t i, j;

  rainbowTimeDelta = curTime - rainbowPrevTime;

  if ( rainbowTimeDelta > rainbowDelay ) {

      rainbowPrevTime = curTime;
      for(i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i+rainbowColorJ) & 255));
      }
      strip.show();

      if ( rainbowColorJ <= 255 ) {
        rainbowColorJ++;
      } else {
        rainbowColorJ = 0;
      }
  }

}
void stripBlink() {

  stripBlinkTimeDelta = curTime - stripBlinkPrevTime;

  if ( stripBlinkTimeDelta > stripBlinkDelay ) {
    stripBlinkPrevTime = curTime;
    if ( stripBlinkState == 255 )
      stripBlinkState = 0;
    else
      stripBlinkState = 255;

    for(uint16_t ii=0; ii<strip.numPixels(); ii++) {
      strip.setPixelColor(ii, strip.Color(stripBlinkState, 0, 0));
    }
    strip.show();

  }
}



void loop() {
  curTime = millis();

  batStat1 = digitalRead(batStat1Pin);
  batStat2 = digitalRead(batStat2Pin);

  if ( batStat1 == HIGH ) {
    batStatCharged();
  } else if ( batStat2 == HIGH ) {
    batStatCharging();
  } else {
    if (digitalRead(frontLightSwitchPin) == LOW) {
      frontLightBlink();
    } else {
      frontLightSolid();
    }
    if (digitalRead(backLightSwitchPin) == LOW) {
      rainbow();
    } else {
      stripBlink();
    }
  }

  // digitalWrite(frontLightPin, HIGH);


}





// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  if(WheelPos < 85) {
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  } else if(WheelPos < 170) {
   WheelPos -= 85;
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
}
