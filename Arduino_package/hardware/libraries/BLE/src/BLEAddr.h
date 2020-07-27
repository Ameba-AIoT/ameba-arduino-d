#ifndef _BLE_ADDR_H_
#define _BLE_ADDR_H_

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

class BLEAddr {
    public:
        BLEAddr();
        BLEAddr(uint8_t (&addr)[6]);
        BLEAddr(const char * str);      // Build a BLEAddr object from an address string. Use of colons (:) to seperate address bytes in the string is acceptable.
        const char* str();
        uint8_t* data();
        bool operator ==(const BLEAddr &addr);

    private:
        friend class BLEDevice;
        uint8_t _btAddr[6] = {0};   // BT address is stored MSB at _btAddr[5]
        char _str[20] = {0};
};

#endif
