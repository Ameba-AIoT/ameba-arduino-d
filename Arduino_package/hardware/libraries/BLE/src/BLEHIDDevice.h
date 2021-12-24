#ifndef _BLE_HIDDEVICE_H_
#define _BLE_HIDDEVICE_H_

#include <Arduino.h>
#include "BLEService.h"
#include "BLECharacteristic.h"
#include "HID.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#define HID_SERVICE_UUID                        "1812"
#define HID_INFO_CHAR_UUID                      "2A4A"
#define REPORT_MAP_CHAR_UUID                    "2A4B"
#define HID_CONTROL_POINT_CHAR_UUID             "2A4C"
#define REPORT_CHAR_UUID                        "2A4D"
#define PROTOCOL_MODE_CHAR_UUID                 "2A4E"
#define BOOT_KEYBOARD_INPUT_REPORT_CHAR_UUID    "2A22"
#define BOOT_KEYBOARD_OUTPUT_REPORT_CHAR_UUID   "2A32"

#define DIS_SERVICE_UUID          "180A"
#define PNP_CHAR_UUID             "2A50"
#define MANUFACTURER_CHAR_UUID    "2A29"
#define MODEL_CHAR_UUID           "2A24"

#define BATT_SERVICE_UUID         "180F"
#define BATT_LEVEL_CHAR_UUID      "2A19"

#define NUM_INPUT_REPORT_DEFAULT 3
// Report ID should start with 1
#define REPORT_ID_KEYBOARD          1
#define REPORT_ID_CONSUMER_CONTROL  2
#define REPORT_ID_MOUSE             3

class BLEHIDDevice {
    public:
        BLEHIDDevice();
        ~BLEHIDDevice();

        void init();

        void setNumOutputReport(uint8_t numOutputReports);
        void setNumInputReport(uint8_t numInputReports);
        void setReportMap(uint8_t* report_map, uint16_t len);

        void inputReport(uint8_t reportID, uint8_t* data, uint16_t len, uint8_t conn_id = 0);
        void setOutputReportCallback(uint8_t reportID, void (*fCallback) (BLECharacteristic* chr, uint8_t conn_id));
        void bootKeyboardReport(uint8_t* data, uint16_t len, uint8_t conn_id = 0);

        void setHidInfo(uint16_t bcd, uint8_t country, uint8_t flags);
        void setBattLevel(uint8_t level);
        void setPNPInfo(uint8_t sig, uint16_t vid, uint16_t pid, uint16_t version);
        void setManufacturerString(const char* manufacturer);
        void setModelString(const char* model);

        BLEService& hidService();
        BLEService& devInfoService();
        BLEService& battService();

    private:
        BLEService* _pHIDService = nullptr;
        BLEService* _pDISService = nullptr;
        BLEService* _pBASService = nullptr;

        BLECharacteristic* _pHIDInfoChar = nullptr;
        BLECharacteristic* _pReportMapChar = nullptr;
        BLECharacteristic* _pHIDControlChar = nullptr;
        BLECharacteristic* _pProtocolModeChar = nullptr;
        BLECharacteristic* _pBootKBInputChar = nullptr;
        BLECharacteristic* _pBootKBOutputChar = nullptr;
        BLECharacteristic** _pReportOutputChar = NULL;
        BLECharacteristic** _pReportInputChar = NULL;       // Pointer to array of input report characteristics

        BLECharacteristic* _pPNPChar = nullptr;
        BLECharacteristic* _pManufacturerChar = nullptr;
        BLECharacteristic* _pModelChar = nullptr;

        BLECharacteristic* _pBattLevelChar = nullptr;

        uint8_t _hidInfo[4] = {};
        uint8_t _pnpInfo[7] = {0x01, 0x5D, 0x00, 0, 0, 0, 0}; // SIG, VID, VID, PID, PID, Ver, Ver
        uint8_t* _pReportMap = nullptr;
        uint8_t _numInputReports = NUM_INPUT_REPORT_DEFAULT;
        uint8_t _numOutputReports = 1;

};

extern BLEHIDDevice BLEHIDDev;

#endif
