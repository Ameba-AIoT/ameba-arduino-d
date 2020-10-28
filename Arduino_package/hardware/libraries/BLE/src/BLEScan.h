#ifndef _BLE_SCAN_H_
#define _BLE_SCAN_H_

#include <Arduino.h>
#include "BLEAddr.h"
#include "BLEUUID.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "gap_le_types.h"
#include "gap_callback_le.h"

#ifdef __cplusplus
}
#endif

class BLEScan {
    public:
        void updateScanParams();
        void startScan(uint32_t scanDuration_ms);       // Scan for scanDuration_ms and stop
        void startScan();
        void stopScan();

        void setScanMode(uint8_t scanMode);
        void setScanInterval(uint16_t scanInt_ms);
        void setScanWindow(uint16_t scanWindow_ms);
        void setScanDuplicateFilter(bool dupeFilter);
        bool scanInProgress();
        void printScanInfo(T_LE_CB_DATA *p_data);                       // Print out scan found device

    private:
        BLEScan();
        friend class BLEDevice;
        uint8_t  _scanMode = GAP_SCAN_MODE_ACTIVE;  // Active Mode will request for scan response packet
        uint16_t _scanInterval = 0x40;              // Duration to wait between starting a scan. Value range: 0x0004 - 0x4000 (2.5ms - 10240ms)(0.625ms/step).
        uint16_t _scanWindow = 0x30;                // Duration to continuously scan for after starting. Value range: 0x0004 - 0x4000 (2.5ms - 10240ms)(0.625ms/step).
        uint8_t  _scanFilterPolicy = GAP_SCAN_FILTER_ANY;
        uint8_t  _scanFilterDuplicate = GAP_SCAN_FILTER_DUPLICATE_ENABLE;
        static uint8_t _scanProcessing;
};

#endif
