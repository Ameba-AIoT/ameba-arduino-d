#ifndef _USB_HIDMOUSE_H_
#define _USB_HIDMOUSE_H_

#include <Arduino.h>
#include "USBHIDDevice.h"
#include "HID.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif


class USBHIDMouse {
    public:
        USBHIDMouse();

        void setReportID(uint8_t reportID);
        void mouseReport(hid_mouse_report_t* report);
        void mouseReport(uint8_t buttons, int8_t x, int8_t y, int8_t scroll = 0);

        void mousePress(uint8_t buttons = MOUSE_BUTTON_LEFT);
        void mouseRelease(uint8_t buttons = MOUSE_BUTTON_LEFT);
        void mouseReleaseAll(void);

        void mouseMove(int8_t x, int8_t y);
        void mouseScroll(int8_t scroll);

    private:
        USBHIDDevice* _pHIDDev = NULL;
        uint8_t _buttons = 0;
        uint8_t _reportID = REPORT_ID_MOUSE;
};

#endif
