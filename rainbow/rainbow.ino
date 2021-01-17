#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

const int LED_PIN = 1;
const int LED_COUNT = 57;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const uint32_t redColor = strip.Color(217, 74, 98);
const uint32_t yellowColor = strip.Color(241, 196, 18);
const uint32_t greenColor = strip.Color(0, 253, 110);
const uint32_t blueColor = strip.Color(35, 0, 255);
const uint32_t purpleColor = strip.Color(193, 36, 225);

const uint32_t colors[5] = {
  redColor,
  yellowColor,
  greenColor,
  blueColor,
  purpleColor,
};

int currentColorIndex = 0;

const int firstBandCount = 7;
const int secondBandCount = 12;
const int thirdBandCount = 17;
const int fourthBandCount = 21;

const int bandCounts[4] = {
  firstBandCount,
  secondBandCount,
  thirdBandCount,
  fourthBandCount,
};

void setup() {
#if defined(__AVR_ATtiny85__) && (F_CPU == 16000000)
  clock_prescale_set(clock_div_1);
#endif

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(50); // Set BRIGHTNESS to about 1/5 (max = 255)
}

void loop() {
  int ledIndex = 0;
  
  for (int i = 0; i < 4; i++) {
    int bandCount = bandCounts[i];
    int colorIndex = i + currentColorIndex;

    if (colorIndex > 4) {
      colorIndex = abs(5 - colorIndex);
    }
    
    uint32_t color = colors[colorIndex];

    for (int l = 0; l < bandCount; l++) {
      strip.setPixelColor(ledIndex + l, color);
    }

    ledIndex += bandCount;
  }

  strip.show();

  currentColorIndex += 1;

  if (currentColorIndex > 4) {
    currentColorIndex = 0;
  }
  
  delay(2500);
}
