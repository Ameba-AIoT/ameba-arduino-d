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
    _report.buttons = buttons;
    _report.hat = hat;
    _report.x = x;
    _report.y = y;
    _report.z = z;
    _report.rz = Rz;
    _report.rx = Rx;
    _report.ry = Ry;
    gamepadReport(&_report);
}

void BLEHIDGamepad::buttonPress(uint16_t buttons) {
    // Check if currently pressed buttons already include desired buttons to be pressed
    if ((_report.buttons | buttons) != _report.buttons) {
        _report.buttons |= buttons;
        gamepadReport(&_report);
    }
}

void BLEHIDGamepad::buttonRelease(uint16_t buttons) {
    // Check if desired buttons to be released are currently pressed
    if ((_report.buttons & ~buttons) != _report.buttons) {
        _report.buttons &= ~buttons;
        gamepadReport(&_report);
    }
}

void BLEHIDGamepad::buttonReleaseAll(void) {
    _report.buttons = 0;
    gamepadReport(&_report);
}

void BLEHIDGamepad::setHat(uint8_t hat) {
    if (hat <= GAMEPAD_HAT_UP_LEFT) {
        _report.hat = hat;
        gamepadReport(&_report);
    }
}

void BLEHIDGamepad::setAxes(int16_t x, int16_t y, int16_t z, int16_t Rz, int16_t Rx, int16_t Ry) {
    if(x == -32768) { x = -32767; }
    if(y == -32768) { y = -32767; }
    if(z == -32768) { z = -32767; }
    if(Rz == -32768) { Rz = -32767; }
    if(Rx == -32768) { Rx = -32767; }
    if(Ry == -32768) { Ry = -32767; }
    _report.x = x;
    _report.y = y;
    _report.z = z;
    _report.rz = Rz;
    _report.rx = Rx;
    _report.ry = Ry;
    gamepadReport(&_report);
}

void BLEHIDGamepad::setLeftStick(int16_t x, int16_t y) {
    if(x == -32768) { x = -32767; }
    if(y == -32768) { y = -32767; }
    _report.x = x;
    _report.y = y;
    gamepadReport(&_report);
}

void BLEHIDGamepad::setRightStick(int16_t z, int16_t Rz) {
    if(z == -32768) { z = -32767; }
    if(Rz == -32768) { Rz = -32767; }
    _report.z = z;
    _report.rz = Rz;
    gamepadReport(&_report);
}

void BLEHIDGamepad::setTriggers(int16_t Rx, int16_t Ry) {
    if(Rx == -32768) { Rx = -32767; }
    if(Ry == -32768) { Ry = -32767; }
    _report.rx = Rx;
    _report.ry = Ry;
    gamepadReport(&_report);
}
