#ifndef _BLE_BEACON_H_
#define _BLE_BEACON_H_

#include "BLEUUID.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "gap_le_types.h"

#ifdef __cplusplus
}
#endif


class iBeacon {
    public:
        iBeacon();
        uint16_t    getManufacturerId();
        void        getUUID(uint8_t* uuid); // provide a pointer to a 16 element uint8_t array 
        uint16_t    getMajor();
        uint16_t    getMinor();
        int8_t      getRSSI();
        void        setManufacturerId(uint16_t id);
        void        setUUID(uint8_t* uuid); // provide a pointer to a 16 element uint8_t array 
        void        setUUID(const char* uuid);
        void        setMajor(uint16_t major);
        void        setMinor(uint16_t minor);
        void        setRSSI(int8_t rssi);
        uint8_t*    getAdvData();
        uint8_t*    getScanRsp();
        const uint8_t     advDataSize = 30;
        const uint8_t     scanRspSize = 27;

    private:
        struct {
            uint8_t length = 0x02;
            uint8_t type = GAP_ADTYPE_FLAGS;
            uint8_t flags = GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED;
            struct {
                uint8_t  length = 0x1A;
                uint8_t  type = GAP_ADTYPE_MANUFACTURER_SPECIFIC;
                uint16_t manufacturerId = 0x004C;   // Company: Apple
                uint8_t  subType = 0x02;            // Type: iBeacon
                uint8_t  subTypeLength = 0x15;      // iBeacon data length 0x15 (21) = UUID (16) + major (2) + minor (2) + RSSI (1)
                uint8_t  UUID[16] = {0};
                uint16_t major = 0x00;
                uint16_t minor = 0x00;
                int8_t   RSSI = 0x00;               // RSSI at 1 meter distance, used for distance approximation
            } __attribute__((packed)) _iBeaconScanRsp;
        } __attribute__((packed)) _iBeaconAdvData;
};

class altBeacon {
    public:
        altBeacon();
        uint16_t    getManufacturerId();
        void        getUUID(uint8_t* uuid); // provide a pointer to a 16 element uint8_t array 
        uint16_t    getMajor();
        uint16_t    getMinor();
        int8_t      getRSSI();
        uint8_t     getRSVD();
        void        setManufacturerId(uint16_t id);
        void        setUUID(uint8_t* uuid); // provide a pointer to a 16 element uint8_t array
        void        setUUID(const char* uuid);
        void        setMajor(uint16_t major);
        void        setMinor(uint16_t minor);
        void        setRSSI(int8_t rssi);
        void        setRSVD(uint8_t rsvd);
        uint8_t*    getAdvData();
        uint8_t*    getScanRsp();
        const uint8_t     advDataSize = 31;
        const uint8_t     scanRspSize = 28;

    private:
        struct {
        uint8_t length = 0x02;
        uint8_t type = GAP_ADTYPE_FLAGS;
        uint8_t flags = GAP_ADTYPE_FLAGS_GENERAL | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED;
            struct {
                uint8_t  length = 0x1B;
                uint8_t  type = GAP_ADTYPE_MANUFACTURER_SPECIFIC;
                uint16_t manufacturerId = 0x005D;   // Company: Realtek
                uint16_t subType = 0xACBE;          // Type: AltBeacon (0xBEAC)
                uint8_t  UUID[16] = {0};
                uint16_t major = 0x00;
                uint16_t minor = 0x00;
                int8_t   RSSI = 0x00;               // RSSI at 1 meter distance, used for distance approximation
                uint8_t  RSVD = 0x00;
            } __attribute__((packed)) _altBeaconScanRsp;
        } __attribute__((packed)) _altBeaconAdvData;
};

#endif
