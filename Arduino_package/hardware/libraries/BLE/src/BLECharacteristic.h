#ifndef _BLE_CHARACTERISTIC_H_
#define _BLE_CHARACTERISTIC_H_

#include <Arduino.h>
#include "BLEUUID.h"

#define CHAR_VALUE_MAX_LEN 230

#ifdef __cplusplus
extern "C" {
#endif

#include "gatt.h"
#include "profile_server.h"

#ifdef __cplusplus
}
#endif

// Forward declaration to avoid include loops
class BLEService;

class BLECharacteristic {
    public:
        BLECharacteristic(BLEUUID uuid);
        BLECharacteristic(const char* uuid);
        ~BLECharacteristic();

        //------------- Configure -------------//
        void setUUID(BLEUUID uuid);
        BLEUUID getUUID();
        void setBufferLen(uint16_t max_len);               // Set size of byte array used for containing characteristic data
        uint16_t getBufferLen();
        void setReadProperty(bool value);
        void setWriteProperty(bool value);
        void setNotifyProperty(bool value);
        void setIndicateProperty(bool value);
        void setProperties(uint8_t value);
        uint8_t getProperties();

        //--------- Read Char Value --------//
        String readString();
        uint8_t readData8();
        uint16_t readData16();
        uint32_t readData32();

        //--------- Write Char Value --------//
        bool writeString(String str);
        bool writeString(const char* str);
        bool writeData8(uint8_t num);
        bool writeData16(uint16_t num);
        bool writeData32(uint32_t num);
        bool writeData32(int num);

        //----------- Modify Char -----------//
        bool setData(uint8_t* data, uint16_t datalen);
        uint16_t getData(uint8_t* data, uint16_t datalen);
        uint8_t* getDataBuff();
        uint16_t getDataLen();
        void notify(uint8_t conn_id);          // Send a notification to a client that has enabled notifications
        void indicate(uint8_t conn_id);        // Send a notification requiring ack to a client that has enable indications

        //------------- Descriptors -------------//
        void setUserDescriptor(const char* description);               // Descriptor UUID 0x2901
        void setFormatDescriptor(uint8_t format, uint8_t exponent, uint16_t unit, uint16_t description);             // Descriptor UUID 0x2904

        //------------- Callbacks -------------//
        void setReadCallback(void (*fCallback) (BLECharacteristic* chr, uint8_t conn_id));              // Called when client reads value
        void setWriteCallback(void (*fCallback) (BLECharacteristic* chr, uint8_t conn_id));             // Called when client writes to value
        void setCCCDCallback(void (*fCallback) (BLECharacteristic* chr, uint8_t conn_id, uint16_t ccc_bits));              // Called when client modifies CCCD



    private:
        uint8_t getHandleIndex();

        uint8_t getAttrCount();
        uint8_t generateCharacteristicAttrTable(T_ATTRIB_APPL* attr_tbl, uint8_t index);
        uint8_t generateAttrCharacteristicDeclaration(T_ATTRIB_APPL* attr_tbl, uint8_t index);
        uint8_t generateAttrDescriptorDeclaration(T_ATTRIB_APPL* attr_tbl, uint8_t index);

        T_APP_RESULT charAttrReadCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                                                uint16_t offset, uint16_t *p_length, uint8_t **pp_value);
        T_APP_RESULT charAttrWriteCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                                                T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                                                P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc);
        void charCccdUpdateCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index, uint16_t ccc_bits);


        friend class BLEDevice;
        friend class BLEService;

        BLEService* _pService = nullptr;
        BLEUUID _uuid;
        uint8_t _handle_index = 0xff;
        uint8_t _char_properties = 0x00;        // default = no properties
        uint32_t _char_attr_permissions = GATT_PERM_NONE;

        uint8_t* _data_buf = nullptr;
        uint16_t _data_buf_len = 20;     // Default buffer size of 20 bytes = default MTU 23 - 3 headers
        uint16_t _data_len = 0;         // Length of last written data

        void (*_pReadCB)(BLECharacteristic* chr, uint8_t conn_id) = nullptr;
        void (*_pWriteCB)(BLECharacteristic* chr, uint8_t conn_id) = nullptr;
        void (*_pCccdCB)(BLECharacteristic* chr, uint8_t conn_id, uint16_t ccc_bits) = nullptr;

        uint8_t _includeCCCDescriptor = 0;
        uint8_t _includeUserDescriptor = 0;
        uint8_t _includeFormatDescriptor = 0;

        char* _userDesc = nullptr;
        uint8_t _userDescSize = 0;

        uint8_t _fDescFormat = 0;       // Default = rfu
        uint8_t _fDescExponent = 0;     // Default = zero
        uint16_t _fDescUnit = 0x2700;   // Default = unitless
        uint16_t _fDescDesc = 0;        // Default = unknown description

};

#endif
