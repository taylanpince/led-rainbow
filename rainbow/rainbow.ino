#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif


#include "RTClib.h"

RTC_DS3231 rtc;

char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};

const int LED_PIN = 2;
const int LED_COUNT = 57;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);

const uint32_t colors[5][3] = {
  {217, 74, 98},
  {241, 196, 18},
  {0, 253, 110},
  {35, 0, 255},
  {193, 36, 225},
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

  Serial.begin(57600);
  
  if (!rtc.begin()) {
    Serial.println("RTC not working, abort");
    abort();
  }

  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  
  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
}

void loop() {
  DateTime now = rtc.now();
  uint8_t hr = now.hour();

  Serial.print(now.year(), DEC);
  Serial.print('/');
  Serial.print(now.month(), DEC);
  Serial.print('/');
  Serial.print(now.day(), DEC);
  Serial.print(" (");
  Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
  Serial.print(") ");
  Serial.print(now.hour(), DEC);
  Serial.print(':');
  Serial.print(now.minute(), DEC);
  Serial.print(':');
  Serial.print(now.second(), DEC);
  Serial.println();
  
  if (hr >= 20 || hr < 8) {
    strip.fill(strip.Color(0, 0, 0), 0, LED_COUNT);
    strip.show();
  } else {
    int ledIndex = 0;

    for (int i = 0; i < 4; i++) {
      int bandCount = bandCounts[i];
      int colorIndex = (i + currentColorIndex) % 5;
      uint32_t color = strip.Color(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
    
      strip.fill(color, ledIndex, bandCount);
    
      ledIndex += bandCount;
    }

    if (hr == 8) {
      strip.setBrightness(map(now.minute(), 0, 60, 0, 100));
    } else if (hr == 19) {
      strip.setBrightness(map(now.minute(), 0, 60, 100, 0));
    } else {
      strip.setBrightness(100);
    }

    strip.show();

    currentColorIndex = (currentColorIndex >= 4) ? 0 : currentColorIndex + 1;
  }
  
  delay(2500);
}
