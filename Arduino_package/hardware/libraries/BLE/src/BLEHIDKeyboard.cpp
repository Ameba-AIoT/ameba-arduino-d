#include "BLEHIDKeyboard.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

uint8_t const hid_ascii_to_keycode[128][2] =  { HID_ASCII_TO_KEYCODE };

BLEHIDKeyboard::BLEHIDKeyboard() {
    _pHIDDev = &BLEHIDDev;
    memset(&report, 0, 8);
}

void BLEHIDKeyboard::setReportID(uint8_t reportIDKeyboard, uint8_t reportIDConsumer) {
    _reportIDKeyboard = reportIDKeyboard;
    _reportIDConsumer = reportIDConsumer;
}

void BLEHIDKeyboard::consumerReport(uint16_t usage_code) {
    _pHIDDev->inputReport(_reportIDConsumer, (uint8_t*)(&usage_code), sizeof(usage_code));
}

void BLEHIDKeyboard::keyboardReport() {
    _pHIDDev->inputReport(_reportIDKeyboard, (uint8_t*)(&report), sizeof(hid_keyboard_report_t));
}

void BLEHIDKeyboard::keyboardReport(uint8_t modifiers, uint8_t keycode[6]) {
    report.modifier = modifiers;
    memcpy(report.keycode, keycode, 6);
    keyboardReport();
}

void BLEHIDKeyboard::consumerPress(uint16_t usage_code) {
    consumerReport(usage_code);
}

void BLEHIDKeyboard::consumerRelease(void) {
    consumerReport(0);
}

void BLEHIDKeyboard::keyPress(uint16_t key) {
    if (key == 0x00) return;
    // Distinguish between modifier keys and regular keys
    if ((key >= HID_KEY_CONTROL_LEFT) && (key <= HID_KEY_GUI_RIGHT)) {
        report.modifier |= (1 << (key & 0x0F));
    } else {
        uint8_t i = 0;
        // Add to the key report only if it's not already present and if there is an empty slot.
        if ((report.keycode[0] != key) && (report.keycode[1] != key) && (report.keycode[2] != key) && 
            (report.keycode[3] != key) && (report.keycode[4] != key) && (report.keycode[5] != key)) {

            for (i=0; i<6; i++) {
                if (report.keycode[i] == 0x00) {
                    report.keycode[i] = key;
                    break;
                }
            }
        }
    }
    keyboardReport();
}

void BLEHIDKeyboard::keyRelease(uint16_t key) {
    if (key == 0x00) return;
    // Distinguish between modifier keys and regular keys
    if ((key >= HID_KEY_CONTROL_LEFT) && (key <= HID_KEY_GUI_RIGHT)) {
        report.modifier &= ~(1 << (key & 0x0F));
    } else {
        uint8_t i = 0;
        // Check all report slots and clear if it exists.
        for (i=0; i<6; i++) {
            if (report.keycode[i] == key) {
                report.keycode[i] = 0x00;
            }
        }
    }
    keyboardReport();
}

void BLEHIDKeyboard::keyReleaseAll(void) {
    memset(&report, 0, 8);
    keyboardReport();
}

void BLEHIDKeyboard::keyCharPress(char ch) {
    // Check for valid ASCII value
    if (ch <= 0x7F) {
        memset(&report, 0, 8);
        // Apply shift modifier if required
        if (hid_ascii_to_keycode[(uint8_t)ch][0] == 1) {
            report.modifier = KEYBOARD_MODIFIER_LEFTSHIFT;
        }
        report.keycode[0] = hid_ascii_to_keycode[(uint8_t)ch][1];
        keyboardReport();
    }
}

void BLEHIDKeyboard::keySequence(const char* str, uint16_t delayTime) {
    char current, next;
    while (*str != '\0') {
        current = *str;
        next = *(str++);

        keyCharPress(current);
        delay(delayTime);
        if ((next == current) || (next == '\0')) {
            keyReleaseAll();
            delay(delayTime);
        }
    }
}

void BLEHIDKeyboard::keySequence(String str, uint16_t delayTime) {
    keySequence(str.c_str(), delayTime);
}