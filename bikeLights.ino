#include <Adafruit_NeoPixel.h>
#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(8, 1, NEO_GRB + NEO_KHZ800);

const int motorPin = 0;
int ledDelay = 1000;
int ledState = LOW;
unsigned long prevTime = 0;

int stripDelay = 20;
unsigned long stripPrevTime = 0;
int stripColorJ = 0;


int stripBlinkDelay = 100;
int stripBlinkPrevTime = 0;
int stripBlinkState = 255;

unsigned long time = 0;


void setup() {
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(motorPin, OUTPUT);

}
    
void loop() {
  time = millis();
  blink();
  rainbow();
//  stripBlink();
}

void rainbow() {
  uint16_t i, j;

  if ( time - stripPrevTime >= stripDelay ) {

      stripPrevTime = time;
      for(i=0; i<strip.numPixels(); i++) {
        strip.setPixelColor(i, Wheel((i+stripColorJ) & 255));
      }
      strip.show();
      
      if ( stripColorJ <= 255 ) {
        stripColorJ++;
      } else {
        stripColorJ = 0;
      }
  }

}

void stripBlink() {

  if ( time - stripBlinkPrevTime >= stripBlinkDelay ) {
    stripBlinkPrevTime = time;
    if ( stripBlinkState == 255 )
      stripBlinkState = 0;
    else
      stripBlinkState = 255;
    
    for(uint16_t i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, strip.Color(stripBlinkState, 0, 0));
    }
    strip.show();

  }
}

void blink() {

  if ( time - prevTime >= ledDelay ) {
    prevTime = time;
    
    if ( ledState == LOW )
      ledState = HIGH;
    else
      ledState = LOW;
    digitalWrite(motorPin, ledState);
    
//    stripBlink();
  }
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
