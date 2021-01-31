// LED controller
// Works with Adafruit Neopixel library and stores all config

#include <Adafruit_NeoPixel.h>


enum DeviceMode {
    automatic = 100,
    alwaysOn = 101,
    alwaysOff = 102,
};

enum AnimationMode {
    on = 100,
    off = 101,
};

class LEDController {
private:
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

public:
    Adafruit_NeoPixel *strip;

    DeviceMode currentMode = DeviceMode::automatic;
    AnimationMode animationMode = AnimationMode::on;

    uint32_t colors[5][3] = {
        {217, 74, 98},
        {241, 196, 18},
        {0, 253, 110},
        {35, 0, 255},
        {193, 36, 225},
    };

    LEDController(Adafruit_NeoPixel &strip) : strip(&strip) {};
    
    void init() {
        strip->begin();
        strip->show();
    };

    void update() {
//  uint8_t hr = now.hour();
//  uint8_t mnt = now.minute();
//  if (hr >= 20 || hr < 8) {
//    strip.fill(strip.Color(0, 0, 0), 0, LED_COUNT);
//    strip.show();
//  } else {
//    int ledIndex = 0;
//
//    for (int i = 0; i < 4; i++) {
//      int bandCount = bandCounts[i];
//      int colorIndex = (i + currentColorIndex) % 5;
//      uint32_t color = strip.Color(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);
//    
//      strip.fill(color, ledIndex, bandCount);
//    
//      ledIndex += bandCount;
//    }
//
//    if (hr == 8) {
//      strip.setBrightness(map(now.minute(), 0, 60, 0, MAX_BRIGHTNESS));
//    } else if (hr == 19) {
//      strip.setBrightness(map(now.minute(), 0, 60, MAX_BRIGHTNESS, 0));
//    } else {
//      strip.setBrightness(MAX_BRIGHTNESS);
//    }
//
//    strip.show();
//
//    currentColorIndex = (currentColorIndex >= 4) ? 0 : currentColorIndex + 1;
//  }
    };
};
