#ifndef _BLE_ADVERT_H_
#define _BLE_ADVERT_H_

#include <Arduino.h>
#include "gap.h"
#include "gap_le_types.h"
#include "gatt.h"

class BLEAdvertData {
    public:

    private:
        uint8_t _advDataDefault[31] = {
                                        /* Flags */
                                        0x02,             /* length */
                                        GAP_ADTYPE_FLAGS, /* type="Flags" */
                                        GAP_ADTYPE_FLAGS_LIMITED | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
                                        };
        uint8_t _scanRspDataDefault[31] = {
                                        0x03,                             /* length */
                                        GAP_ADTYPE_APPEARANCE,            /* type="Appearance" */
                                        LO_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
                                        HI_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
                                        };
        uint8_t _advDataSize = 3;
        uint8_t _scanRspDataSize = 0;
    };

class BLEAdvert {
    public:
        void updateAdvertParams();
        void startAdv();
        void stopAdv();

        void setAdvType(uint8_t advType);
        void setMinInterval(uint16_t minInt_ms);
        void setMaxInterval(uint16_t maxInt_ms);
        void setAdvData();
        void setScanRspData();
        void setAdvData(uint8_t* pdata, uint8_t size);
        void setScanRspData(uint8_t* pdata, uint8_t size);

    private:
        BLEAdvert();
        friend class BLEDevice;
        // Advertising parameters
        uint8_t* _pAdvData = nullptr;
        uint8_t  _pAdvDataSize = 0;
        uint8_t* _pScanRspData = nullptr;
        uint8_t  _pScanRspDataSize = 0;
        uint8_t  _slaveInitMtuReq = false;

        uint8_t  _advEvtType = GAP_ADTYPE_ADV_NONCONN_IND;
        uint8_t  _advDirectType = GAP_REMOTE_ADDR_LE_PUBLIC;
        uint8_t  _advDirectAddr[GAP_BD_ADDR_LEN] = {0};
        uint8_t  _advChannMap = GAP_ADVCHAN_ALL;
        uint8_t  _advFilterPolicy = GAP_ADV_FILTER_ANY;
        uint16_t _advIntMin = 320;   // Minimum advertising interval for undirected and low duty cycle directed advertising. Value range: 0x0020 - 0x4000 (20ms - 10240ms)(0.625ms/step)
        uint16_t _advIntMax = 480;  // Maximum advertising interval for undirected and low duty cycle directed advertising. Value range: 0x0020 - 0x4000 (20ms - 10240ms)(0.625ms/step)

        uint8_t _advDataDefault[18] = {
                                        /* Flags */
                                        0x02,             /* length */
                                        GAP_ADTYPE_FLAGS, /* type="Flags" */
                                        GAP_ADTYPE_FLAGS_LIMITED | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED,
                                        /* Local name */
                                        0x0E,             /* length */
                                        GAP_ADTYPE_LOCAL_NAME_COMPLETE,
                                        'A', 'M', 'E', 'B', 'A', '_', 'B', 'L', 'E', '_', 'D', 'E', 'V',
                                        };
        uint8_t _scanRspDataDefault[4] = {
                                        0x03,                             /* length */
                                        GAP_ADTYPE_APPEARANCE,            /* type="Appearance" */
                                        LO_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
                                        HI_WORD(GAP_GATT_APPEARANCE_UNKNOWN),
                                        };
};

#endif
