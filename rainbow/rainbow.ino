#include <Adafruit_NeoPixel.h>
#ifdef __AVR__
 #include <avr/power.h> // Required for 16 MHz Adafruit Trinket
#endif

#include "RTClib.h"

RTC_DS3231 rtc;

unsigned int year_old = 0;
DateTime dst_start, dst_end;

#define UPLOAD_OFFSET 5

#define POLLING_DELAY 500

const int LED_PIN = 1;
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

//  if (rtc.lostPower()) {
//    // Set the time to the compile time + offset
//    DateTime compile_time = DateTime(F(__DATE__), F(__TIME__)) + TimeSpan(UPLOAD_OFFSET);

//    // Checking if compile time is in DST
//    uint16_t year = compile_time.year();
//  
//    // DST starts on the second Sunday of March, 2AM
//    // Get beginning of second week and then offset to Sunday
//    DateTime dst_start = DateTime(year, 3, 8, 2, 0, 0);
//    dst_start = dst_start + TimeSpan((7-dst_start.dayOfTheWeek()) % 7, 0, 0, 0);
//  
//    // DST ends on the first Sunday of November, 2AM
//    // Get first day of month and then offset to Sunday
//    DateTime dst_end = DateTime(year, 11, 1, 2, 0, 0);
//    dst_end = dst_end + TimeSpan((7-dst_end.dayOfTheWeek()) % 7, 0, 0, 0);
//  
//    // If compile time is between DST start and end, then subtract 1 hour to get standard time
//    compile_time = compile_time >= dst_start && compile_time < dst_end ? (compile_time - TimeSpan(0,1,0,0)) : compile_time;
//    
//    rtc.adjust(compile_time);
//  }

  strip.begin();           // INITIALIZE NeoPixel strip object (REQUIRED)
  strip.show();            // Turn OFF all pixels ASAP
  strip.setBrightness(100);
}

void loop() {
  DateTime now = rtc.now();
  byte hr = now.hour();
  
  // Calculate new DST cutoffs if the year changes
//  if(now.year() != year_old) {
//    // DST starts on the second Sunday of March, 2AM
//    dst_start = DateTime(now.year(), 3, 8, 2, 0, 0);
//    dst_start = dst_start + TimeSpan((7-dst_start.dayOfTheWeek()) % 7, 0, 0, 0);
//  
//    // DST ends on the first Sunday of November, 1AM (standard time)
//    dst_end = DateTime(now.year(), 11, 1, 1, 0, 0);
//    dst_end = dst_end + TimeSpan((7-dst_end.dayOfTheWeek()) % 7, 0, 0, 0); 
//    
//    year_old = now.year();
//  }
  
  // If current time is between the DST cutoffs, add 1 to the hour digit
//  hr = (now >= dst_start && now < dst_end) ? (hr + 1) : hr;
  
  int ledIndex = 0;
  int brightness = (hr >= 20 || hr < 8) ? 0 : 250;
  
  for (int i = 0; i < 4; i++) {
    int bandCount = bandCounts[i];
    int colorIndex = (i + currentColorIndex) % 5;
    uint32_t color = strip.Color(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2], brightness);

    strip.fill(color, ledIndex, bandCount);

    ledIndex += bandCount;
  }

  strip.show();

  currentColorIndex += 1;

  if (currentColorIndex > 4) {
    currentColorIndex = 0;
  }
  
  delay(2500);
}
