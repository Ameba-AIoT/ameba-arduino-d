#include "BLEHIDGamepad.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

BLEHIDGamepad::BLEHIDGamepad() {
    _pHIDDev = &BLEHIDDev;
}

void BLEHIDGamepad::setReportID(uint8_t reportID) {
    _reportID = reportID;
}

void BLEHIDGamepad::gamepadReport(hid_gamepad_report_t* report) {
    _pHIDDev->inputReport(_reportID, (uint8_t*)report, sizeof(hid_gamepad_report_t));
}

void BLEHIDGamepad::gamepadReport(uint16_t buttons, uint8_t hat, int16_t x, int16_t y, int16_t z, int16_t Rz, int16_t Rx, int16_t Ry) {
    hid_gamepad_report_t report;
    report.buttons = buttons;
    report.hat = hat;
    report.x = x;
    report.y = y;
    report.z = z;
    report.rz = Rz;
    report.rx = Rx;
    report.ry = Ry;
    gamepadReport(&report);
}

void BLEHIDGamepad::buttonPress(uint16_t buttons) {
    // Check if currently pressed buttons already include desired buttons to be pressed
    if ((_buttons | buttons) != _buttons) {
        _buttons = (_buttons | buttons);
        gamepadReport(_buttons, _hat, _x, _y, _z, _Rz, _Rx, _Ry);
    }
}

void BLEHIDGamepad::buttonRelease(uint16_t buttons) {
    // Check if desired buttons to be released are currently pressed
    if ((_buttons & ~buttons) != _buttons) {
        _buttons = (_buttons & ~buttons);
        gamepadReport(_buttons, _hat, _x, _y, _z, _Rz, _Rx, _Ry);
    }
}

void BLEHIDGamepad::buttonReleaseAll(void) {
    _buttons = 0;
    gamepadReport(_buttons, _hat, _x, _y, _z, _Rz, _Rx, _Ry);
}

void BLEHIDGamepad::setHat(uint8_t hat) {
    if (hat <= GAMEPAD_HAT_UP_LEFT) {
        _hat = hat;
        gamepadReport(_buttons, _hat, _x, _y, _z, _Rz, _Rx, _Ry);
    }
}

void BLEHIDGamepad::setAxes(int16_t x, int16_t y, int16_t z, int16_t Rz, int16_t Rx, int16_t Ry) {
    if(x == -32768) { x = -32767; }
    if(y == -32768) { y = -32767; }
    if(z == -32768) { z = -32767; }
    if(Rz == -32768) { Rz = -32767; }
    if(Rx == -32768) { Rx = -32767; }
    if(Ry == -32768) { Ry = -32767; }
    _x = x;
    _y = y;
    _z = z;
    _Rz = Rz;
    _Rx = Rx;
    _Ry = Ry;
    gamepadReport(_buttons, _hat, _x, _y, _z, _Rz, _Rx, _Ry);
}

void BLEHIDGamepad::setLeftStick(int16_t x, int16_t y) {
    if(x == -32768) { x = -32767; }
    if(y == -32768) { y = -32767; }
    _x = x;
    _y = y;
    gamepadReport(_buttons, _hat, _x, _y, _z, _Rz, _Rx, _Ry);
}

void BLEHIDGamepad::setRightStick(int16_t z, int16_t Rz) {
    if(z == -32768) { z = -32767; }
    if(Rz == -32768) { Rz = -32767; }
    _z = z;
    _Rz = Rz;
    gamepadReport(_buttons, _hat, _x, _y, _z, _Rz, _Rx, _Ry);
}

void BLEHIDGamepad::setTriggers(int16_t Rx, int16_t Ry) {
    if(Rx == -32768) { Rx = -32767; }
    if(Ry == -32768) { Ry = -32767; }
    _Rx = Rx;
    _Ry = Ry;
    gamepadReport(_buttons, _hat, _x, _y, _z, _Rz, _Rx, _Ry);
}
