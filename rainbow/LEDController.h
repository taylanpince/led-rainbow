// LED controller
// Works with Adafruit Neopixel library and stores all config

#include <Adafruit_NeoPixel.h>
#include <RTClib.h>


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
    int total_leds;

    unsigned long lastUpdateTime = 0;
    int UPDATE_INTERVAL = 2500;

    const int MAX_BRIGHTNESS = 200;
    const int BRIGHNESS_SPAN = 60 * 30;

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

    void turnOff() {
        strip->fill(strip->Color(0, 0, 0), 0, total_leds);
    }

    void updateStripColors() {
        int ledIndex = 0;

        for (int i = 0; i < 4; i++) {
            int bandCount = bandCounts[i];
            int colorIndex = (i + currentColorIndex) % 5;
            uint32_t color = strip->Color(colors[colorIndex][0], colors[colorIndex][1], colors[colorIndex][2]);

            strip->fill(color, ledIndex, bandCount);

            ledIndex += bandCount;
        }

        if (animationMode == AnimationMode::on) {
            currentColorIndex = (currentColorIndex >= 4) ? 0 : currentColorIndex + 1;
        }
    }

    int calculateBrightness(DateTime now) {
        // 1. Calculate midnight today
        DateTime midnight = DateTime(now.year(), now.month(), now.day());

        // 2. Calculate wakeUpTime and sleepTime projected to today/tomorrow
        DateTime wakeUpDate = midnight + TimeSpan(0, wakeUpTime[0], wakeUpTime[1], wakeUpTime[2]);
        DateTime sleepDate = midnight + TimeSpan(0, sleepTime[0], sleepTime[1], sleepTime[2]);

        // 3. Figure out if we are between two times, if not, turn off
        if (now < wakeUpDate || now >= sleepDate) {
            return 0;
        }

        // 4. Calculate time span from sleepTime and to wakeUpTime
        TimeSpan spanSinceWakeUp = now - wakeUpDate;
        TimeSpan spanTilSleep = sleepDate - now;

        // 5. Calculate brightness based on time spans
        if (spanSinceWakeUp.totalseconds() > 0 && spanSinceWakeUp.totalseconds() <= BRIGHNESS_SPAN) {
            return map(spanSinceWakeUp.totalseconds(), 0, BRIGHNESS_SPAN, 0, MAX_BRIGHTNESS);
        } else if (spanTilSleep.totalseconds() > 0 && spanTilSleep.totalseconds() <= BRIGHNESS_SPAN) {
            return map(spanTilSleep.totalseconds(), 0, BRIGHNESS_SPAN, 0, MAX_BRIGHTNESS);
        } else {
            return MAX_BRIGHTNESS;
        }
    }

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

    uint8_t wakeUpTime[3] = {8, 0, 0};
    uint8_t sleepTime[3] = {20, 0, 0};

    LEDController(Adafruit_NeoPixel &strip, int total_leds) : strip(&strip), total_leds(total_leds) {};
    
    void init() {
        strip->begin();
        strip->show();
    };

    void update(DateTime now) {
        if (millis() - lastUpdateTime < UPDATE_INTERVAL) {
            return;
        }

        lastUpdateTime = millis();

        switch (currentMode) {
            case DeviceMode::alwaysOff:
                turnOff();
                break;
            case DeviceMode::alwaysOn:
                updateStripColors();
                break;
            case DeviceMode::automatic:
                int brightness = calculateBrightness(now);

                if (brightness > 0) {
                    updateStripColors();

                    strip->setBrightness(brightness);
                } else {
                    turnOff();
                }
                break;
            default:
                break;
        }

        strip->show();
    };
};
