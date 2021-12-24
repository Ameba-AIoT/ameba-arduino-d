#ifndef _BLE_SECURITY_H_
#define _BLE_SECURITY_H_

#include <Arduino.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "gap.h"

#ifdef __cplusplus
}
#endif

class BLESecurity {
    public:
        void setPairable(bool pairMode);
        void setAuthFlags(uint16_t authFlags);
        void setIOCapability(uint8_t ioCap);
        void setSecReqEnable(bool secReq);
        void setSecReqFlags(uint16_t secReqFlags);
        void setStaticPin(uint32_t pin);

        void setPasskeyDisplayCallback(void (*fCallback) (uint8_t conn_id, uint32_t passkey));
        void setPasskeyInputCallback(uint32_t (*fCallback) (uint8_t conn_id));
        void setNumericComparisonCallback(bool (*fCallback) (uint8_t conn_id, uint32_t passkey));

    private:
        BLESecurity();

        friend class BLEDevice;

        void setupGAPBondManager(void);

        // GAP Bond Manager default parameters
        uint8_t  _authPairMode = GAP_PAIRING_MODE_NO_PAIRING;
        uint16_t _authFlags = GAP_AUTHEN_BIT_NONE;
        uint8_t  _authIoCap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
        uint8_t  _authOob = false;
        uint8_t  _authUseFixPasskey = false;
        uint32_t _authFixPasskey = 0;
        uint8_t  _authSecReqEnable = false;
        uint16_t _authSecReqFlags = GAP_AUTHEN_BIT_NONE;

        // Bonding verification callback functions
        static void (*_pKeyDisplayCB)(uint8_t conn_id, uint32_t passkey);
        static uint32_t (*_pKeyInputCB)(uint8_t conn_id);
        static bool (*_pNumCompareCB)(uint8_t conn_id, uint32_t passkey);
};

#endif
