#ifndef _BLE_CONN_H_
#define _BLE_CONN_H_

#include <Arduino.h>
#include "BLEAdvertData.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "gap.h"
#include "gap_conn_le.h"
#include "gap_le_types.h"

#ifdef __cplusplus
}
#endif

class BLEConnect {
    public:
        bool connect(char* btAddr, T_GAP_REMOTE_ADDR_TYPE destAddrType, uint16_t scanTimeout);
        bool connect(uint8_t (&btAddr)[6], T_GAP_REMOTE_ADDR_TYPE destAddrType, uint16_t scanTimeout);
        bool connect(BLEAdvertData targetDevice, uint16_t scanTimeout);
        bool connect(BLEAddr destAddr, T_GAP_REMOTE_ADDR_TYPE destAddrType, uint16_t scanTimeout);
        bool disconnect(uint8_t connId = 0);

        void setScanInterval(uint16_t scanInt_ms);
        void setScanWindow(uint16_t scanWindow_ms);
        
        void setConnInterval(uint16_t min_ms, uint16_t max_ms);
        void setConnLatency(uint16_t latency);
        void setConnTimeout(uint16_t timeout_ms);
        void updateConnParams(uint8_t conn_id);

        bool getConnInfo(uint8_t connId, T_GAP_CONN_INFO *pConnInfo);
        bool getConnAddr(uint8_t connId, uint8_t* addr, uint8_t* addrType);
        int8_t getConnId(char* btAddr, uint8_t addrType);
        int8_t getConnId(uint8_t* addr, uint8_t addrType);
        int8_t getConnId(BLEAdvertData targetDevice);

    private:
        BLEConnect();
        friend class BLEDevice;
        T_GAP_LOCAL_ADDR_TYPE _localAddrType = GAP_LOCAL_ADDR_LE_PUBLIC;
        T_GAP_LE_CONN_REQ_PARAM _connReqParamDefault = {0x40, 0x30, 80, 80, 0, 500, 158, 158};
        T_GAP_LE_CONN_REQ_PARAM _connReqParam;
};

#endif
