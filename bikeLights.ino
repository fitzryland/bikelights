#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, 0, NEO_GRB + NEO_KHZ800);
unsigned long curTime = 0;

// Light Switching Input
int frontLightSwitchPin = 13;
int backLightSwitchPin = 12;



// Front Light Functions
const int frontLightPin = 1;
const int frontLightDelay = 50;
int frontLightState = LOW;
unsigned long prevTime = 0;
void frontLightBlink() {
  if ( curTime - prevTime >= frontLightDelay ) {
    prevTime = curTime;

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
int rainbowDelay = 20;
unsigned long rainbowPrevTime = 0;
int rainbowColorJ = 0;
void rainbow() {
  uint16_t i, j;

  if ( curTime - rainbowPrevTime >= rainbowDelay ) {

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

int stripBlinkDelay = 100;
unsigned int stripBlinkPrevTime = 0;
int stripBlinkState = 255;
void stripBlink() {

  if ( curTime - stripBlinkPrevTime >= stripBlinkDelay ) {
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



void setup() {
  // input
  pinMode(frontLightSwitchPin, INPUT_PULLUP);
  pinMode(backLightSwitchPin, INPUT_PULLUP);

  // front light
  pinMode(frontLightPin, OUTPUT);

  // setup strip (back light)
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'

}

void loop() {
  curTime = millis();

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

    // stripBlink();
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
