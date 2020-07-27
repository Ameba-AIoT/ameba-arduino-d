#include "BLEAddr.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

static uint8_t ctoi(char c) {
    if((c >= 'A') && (c <= 'F')) {
        return (c - 'A' + 0x0A);
    }
    if((c >= 'a') && (c <= 'f')) {
        return (c - 'a' + 0x0A);
    }
    if((c >= '0') && (c <= '9')) {
        return (c - '0' + 0x00);
    }
    return 0xFF;
}

BLEAddr::BLEAddr() {
}

BLEAddr::BLEAddr(uint8_t (&addr)[6]) {
    _btAddr[0] = addr[0];
    _btAddr[1] = addr[1];
    _btAddr[2] = addr[2];
    _btAddr[3] = addr[3];
    _btAddr[4] = addr[4];
    _btAddr[5] = addr[5];
}

BLEAddr::BLEAddr(const char* str) {
    uint8_t idx = 5;
    uint32_t n = 0;
    uint8_t num = 0;
    uint8_t str_len = strlen(str);

    if (str_len < 12)
        return;
     while (n < str_len) {
        if (str[n] == ':') {
            n++;
        } else {
            if ((num = ctoi(str[n++])) == 0xFF) {
                return;
            }
            _btAddr[idx] = num << 4;
            if ((num = ctoi(str[n++])) == 0xFF) {
                return;
            }
            _btAddr[idx] |= num;
            
            idx--;
        }
    }
}

const char* BLEAddr::str() {
    sprintf(_str, "%2X:%2X:%2X:%2X:%2X:%2X", _btAddr[5], _btAddr[4], _btAddr[3], _btAddr[2], _btAddr[1], _btAddr[0]);
    return _str;
}

uint8_t* BLEAddr::data() {
    return _btAddr;
}

bool BLEAddr::operator ==(const BLEAddr &addr) {
    return(memcmp(_btAddr, addr._btAddr, 6) == 0);
}
