#ifndef _BLE_ADVERTDATA_H_
#define _BLE_ADVERTDATA_H_

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

class BLEAdvertData {
    public:
        BLEAdvertData();
        void clear(void);
        void addData(const uint8_t* data, uint8_t size);      // Advertising data types not included in this class can be added manually using addData
        uint8_t addFlags(uint8_t flags = (GAP_ADTYPE_FLAGS_LIMITED | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED));
        uint8_t addPartialServices(BLEUUID uuid);
        uint8_t addCompleteServices(BLEUUID uuid);
        uint8_t addCompleteServices(uint8_t uuidBitLength);         // Add a blank entry to indicate that there are no services with a certain UUID length
        uint8_t addAppearance(uint16_t appearance);
        uint8_t addShortName(const char* str);
        uint8_t addCompleteName(const char* str);

        // Methods used for processing received advertisments
        void parseScanInfo(T_LE_CB_DATA *p_data);

        bool hasFlags();
        bool hasUUID();
        bool hasName();
        bool hasManufacturer();

        T_GAP_ADV_EVT_TYPE      getAdvType();
        T_GAP_REMOTE_ADDR_TYPE  getAddrType();
        BLEAddr     getAddr();
        int8_t      getRSSI();
        uint8_t     getFlags();
        uint8_t     getServiceCount();
        BLEUUID*    getServiceList();
        String      getName();
        int8_t      getTxPower();
        uint16_t    getAppearance();
        uint16_t    getManufacturer();
        uint8_t     getManufacturerDataLength();
        uint8_t*    getManufacturerData();

    private:
        friend class BLEAdvert;
        uint8_t _data[31] ={0};     // array for storing formatted advertising data for receiving and sending
        uint8_t _dataSize = 0;

        // Variables used for processing received advertisments
        T_GAP_ADV_EVT_TYPE _advType;
        T_GAP_REMOTE_ADDR_TYPE _addrType;
        BLEAddr     _address;
        int8_t      _rssi = 0;
        uint8_t     _flags = 0;
        BLEUUID     _serviceList[7];        // A 31byte advert can only fit a maximum of 7 service UUIDs of 16bit length
        uint8_t     _serviceCount = 0;
        String      _devName;
        int8_t      _txPower = 0;           // -127dBm to +127dBm range
        uint16_t    _devAppearance = 0;
        uint16_t    _manufacturer = 0;
        uint8_t     _manufacturerData[27] = {0};
        uint8_t     _manufacturerDataLength = 0;
};

#endif
