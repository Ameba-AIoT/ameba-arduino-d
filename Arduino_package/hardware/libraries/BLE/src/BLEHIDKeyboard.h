#ifndef _BLE_HIDKEYBOARD_H_
#define _BLE_HIDKEYBOARD_H_

#include <Arduino.h>
#include "BLEHIDDevice.h"
#include "HID.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif


class BLEHIDKeyboard {
    public:
        BLEHIDKeyboard();

        void setReportID(uint8_t reportIDKeyboard, uint8_t reportIDConsumer);
        void consumerReport(uint16_t usage_code);
        void keyboardReport();
        void keyboardReport(uint8_t modifiers, uint8_t keycode[6]);

        void consumerPress(uint16_t usage_code);
        void consumerRelease(void);

        void keyPress(uint16_t key);
        void keyRelease(uint16_t key);
        void keyReleaseAll(void);
        void keyCharPress(char ch);
        void keySequence(const char* str, uint16_t delayTime = 5);
        void keySequence(String str, uint16_t delayTime = 5);

    private:
        BLEHIDDevice* _pHIDDev = NULL;
        hid_keyboard_report_t report;
        uint8_t _reportIDKeyboard = REPORT_ID_KEYBOARD;
        uint8_t _reportIDConsumer = REPORT_ID_CONSUMER_CONTROL;
};

#endif

