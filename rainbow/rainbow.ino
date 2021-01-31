#include "BLEController.h"


const int LED_PIN = 2;
const int LED_COUNT = 57;

Adafruit_NeoPixel strip(LED_COUNT, LED_PIN, NEO_GRB + NEO_KHZ800);
LEDController led(strip, LED_COUNT);
BLEController ble = BLEController(led);
RTC_DS3231 rtc;

void setup() {
    ble.init();
    led.init();
  
    if (!rtc.begin()) {
        abort();
    }

    // If datetime adjustment is needed:
    //rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
}

void loop() {
    ble.process();
    led.update(rtc.now());

    delay(250);
}
