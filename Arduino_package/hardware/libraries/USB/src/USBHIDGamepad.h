#ifndef _USB_HIDGAMEPAD_H_
#define _USB_HIDGAMEPAD_H_

#include <Arduino.h>
#include "USBHIDDevice.h"
#include "HID.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif


class USBHIDGamepad {
    public:
        USBHIDGamepad();

        void setReportID(uint8_t reportID);
        void gamepadReport(hid_gamepad_report_t* report);
        void gamepadReport(uint16_t buttons, uint8_t hat, int16_t x, int16_t y, int16_t z, int16_t Rz, int16_t Rx, int16_t Ry);

        void buttonPress(uint16_t buttons);
        void buttonRelease(uint16_t buttons);
        void buttonReleaseAll(void);

        void setHat(uint8_t hat);

        void setAxes(int16_t x = 0, int16_t y = 0, int16_t z = 0, int16_t Rz = 0, int16_t Rx = 0, int16_t Ry = 0);
        void setLeftStick(int16_t x = 0, int16_t y = 0);
        void setRightStick(int16_t z = 0, int16_t Rz = 0);
        void setTriggers(int16_t Rx = 0, int16_t Ry = 0);


    private:
        USBHIDDevice* _pHIDDev = NULL;
        hid_gamepad_report_t _report = {0, 0, 0, 0, 0, 0, 0, 0};
        uint8_t _reportID = 1;
};


#endif
