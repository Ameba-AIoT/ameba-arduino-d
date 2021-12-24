#include "BLEHIDMouse.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

BLEHIDMouse::BLEHIDMouse() {
    _pHIDDev = &BLEHIDDev;
}

void BLEHIDMouse::setReportID(uint8_t reportID) {
    _reportID = reportID;
}

void BLEHIDMouse::mouseReport(hid_mouse_report_t* report) {
    _pHIDDev->inputReport(_reportID, (uint8_t*)report, sizeof(hid_mouse_report_t));
}

void BLEHIDMouse::mouseReport(uint8_t buttons, int8_t x, int8_t y, int8_t scroll) {
    _buttons = buttons;
    hid_mouse_report_t report;
    report.buttons = _buttons;
    report.x = x;
    report.y = y;
    report.wheel = scroll;
    mouseReport(&report);
}

void BLEHIDMouse::mousePress(uint8_t buttons) {
    // Check if currently pressed buttons already include desired buttons to be pressed
    if ((_buttons | buttons) != _buttons) {
        _buttons = (_buttons | buttons);
        mouseReport(_buttons, 0, 0, 0);
    }
}

void BLEHIDMouse::mouseRelease(uint8_t buttons) {
    // Check if desired buttons to be released are currently pressed
    if ((_buttons & ~buttons) != _buttons) {
        _buttons = (_buttons & ~buttons);
        mouseReport(_buttons, 0, 0, 0);
    }
}

void BLEHIDMouse::mouseReleaseAll(void) {
    mouseReport(0, 0, 0, 0);
}

void BLEHIDMouse::mouseMove(int8_t x, int8_t y) {
    mouseReport(_buttons, x, y, 0);
}

void BLEHIDMouse::mouseScroll(int8_t scroll) {
    mouseReport(_buttons, 0, 0, scroll);
}

