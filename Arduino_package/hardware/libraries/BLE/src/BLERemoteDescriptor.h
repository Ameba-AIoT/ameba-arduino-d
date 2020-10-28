#ifndef _BLE_REMOTEDESCRIPTOR_H_
#define _BLE_REMOTEDESCRIPTOR_H_

#include <Arduino.h>
#include "BLEUUID.h"

#define DESC_VALUE_MAX_LEN 230
#define CB_WAIT_TIMEOUT 5000

#ifdef __cplusplus
extern "C" {
#endif

#include "profile_client.h"

#ifdef __cplusplus
}
#endif

// Forward declaration to avoid include loops
class BLEClient;

class BLERemoteDescriptor {
    public:
        //------------- Configure -------------//
        BLEUUID getUUID();
        void setBufferLen(uint16_t max_len);               // Set size of byte array used for containing characteristic data
        uint16_t getBufferLen();

        //---------- Read Desc Value ----------//
        String readString();
        uint8_t readData8();
        uint16_t readData16();
        uint32_t readData32();

        //---------- Write Desc Value ---------//
        bool writeString(String str);
        bool writeString(const char* str);
        bool writeData8(uint8_t num);
        bool writeData16(uint16_t num);
        bool writeData32(uint32_t num);
        bool writeData32(int num);

        //------------ Modify Desc ------------//
        bool setData(uint8_t* data, uint16_t datalen);
        uint16_t getData(uint8_t* data, uint16_t datalen);


    private:
        BLERemoteDescriptor(BLEUUID uuid);
        ~BLERemoteDescriptor();

        void clientReadResultCallbackDefault(uint8_t conn_id, uint16_t cause, uint16_t handle, uint16_t value_size, uint8_t *p_value);
        void clientWriteResultCallbackDefault(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle, uint16_t cause, uint8_t credits);

        friend class BLEClient;
        friend class BLERemoteCharacteristic;

        BLEClient* _pClient = nullptr;
        BLEUUID _uuid;
        uint16_t _declarationHandle = 0;

        uint8_t* _data_buf = nullptr;
        uint16_t _data_buf_len = 20;     // Default buffer size of 20 bytes = default MTU 23 - 3 headers
        uint16_t _data_len = 0;         // Length of last written data

        SemaphoreHandle_t _readSemaphore;
        SemaphoreHandle_t _writeSemaphore;
};

#endif
