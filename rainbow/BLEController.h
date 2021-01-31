// BLE controller
// Works with BLE Mini library to control access protocol

#include "ble_mini.h"
#include "LEDController.h"


enum CommandCode {
    readSettings = 99,
    mode = 100,
    wakeUpTime = 101,
    sleepTime = 102,
    colorOne = 103,
    colorTwo = 104,
    colorThree = 105,
    colorFour = 106,
    colorFive = 107,
    animationMode = 108,
    time = 109,
    date = 110,
};

class BLEController {
private:
    void write(CommandCode code, uint8_t arg1 = 0, uint8_t arg2 = 0, uint8_t arg3 = 0) {
        BLEMini_write(code);
        BLEMini_write(arg1);
        BLEMini_write(arg2);
        BLEMini_write(arg3);
        delay(5);
    }

public:
    LEDController *led;

    BLEController(LEDController &led) : led(&led) {};

    void init() {
        BLEMini_begin(57600);
    };

    void process() {
        bool writeSettings = false;

        while (BLEMini_available() == 4) {
            int data0 = BLEMini_read();
            int data1 = BLEMini_read();
            int data2 = BLEMini_read();
            int data3 = BLEMini_read();

            switch (data0) {
                case CommandCode::readSettings:
                    writeSettings = true;
                    break;
                case CommandCode::mode:
                    break;
                case CommandCode::wakeUpTime:
                    break;
                case CommandCode::sleepTime:
                    break;
                case CommandCode::colorOne:
                    break;
                case CommandCode::colorTwo:
                    break;
                case CommandCode::colorThree:
                    break;
                case CommandCode::colorFour:
                    break;
                case CommandCode::colorFive:
                    break;
                case CommandCode::animationMode:
                    break;
                case CommandCode::time:
                    break;
                case CommandCode::date:
                    break;
                default:
                    break;
            }
        }

        if (writeSettings) {
            write(CommandCode::mode, led->currentMode);
            write(CommandCode::wakeUpTime, 8, 0, 0);
            write(CommandCode::sleepTime, 20, 0, 0);
            write(CommandCode::colorOne, led->colors[0][0], led->colors[0][1], led->colors[0][2]);
            write(CommandCode::colorTwo, led->colors[1][0], led->colors[1][1], led->colors[1][2]);
            write(CommandCode::colorThree, led->colors[2][0], led->colors[2][1], led->colors[2][2]);
            write(CommandCode::colorFour, led->colors[3][0], led->colors[3][1], led->colors[3][2]);
            write(CommandCode::colorFive, led->colors[4][0], led->colors[4][1], led->colors[4][2]);
            write(CommandCode::animationMode, led->animationMode);
        }
    };
};
