//oef 13) Simuleer de beroemde Night-Rider led-sequentie.
#include <Adafruit_NeoPixel.h>

#define PIN 29

#define NUMPIXELS 8

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);



void setup() {
  pixels.begin();
}

void loop() {
  for (int i = 0 ; i < 8 ; i++) {
    pixels.setPixelColor(i, pixels.Color(255,0,0));
    pixels.show();
    delay(75);
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }
  for (int i = 8 ; i >= 0 ; i--) {
    pixels.setPixelColor(i, pixels.Color(255,0,0));
    pixels.show();
    delay(75);
    pixels.setPixelColor(i, pixels.Color(0,0,0));
  }

}
