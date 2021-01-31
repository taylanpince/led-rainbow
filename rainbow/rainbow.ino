#include "BLEController.h"

//#include "RTClib.h"

//RTC_DS3231 rtc;

const int LED_PIN = 2;
const int LED_COUNT = 57;
const int MAX_BRIGHTNESS = 200;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
LEDController led(strip);
BLEController ble = BLEController(led);

void setup() {
  ble.init();
  led.init();
  
//  if (!rtc.begin()) {
//    abort();
//  }

  // If datetime adjustment is needed:
  //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));

}

void loop() {
    ble.process();

//  DateTime now = rtc.now();

    led.update();
}
