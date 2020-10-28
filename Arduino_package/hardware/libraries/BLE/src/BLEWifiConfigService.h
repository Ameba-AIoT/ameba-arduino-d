#ifndef _BLE_BATTSERVICE_H_
#define _BLE_BATTSERVICE_H_

#include <Arduino.h>
#include "BLEAdvertData.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "gap.h"
#include "profile_server.h"

#ifdef __cplusplus
}
#endif

class BLEWifiConfigService {
    public:
        BLEWifiConfigService();
        void begin();
        void end();
        void addService();
        BLEAdvertData advData();    // Wifi Config service advertisement data requires the local BT address, and should be called only after peripheral mode is started

    private:
        static T_APP_RESULT serviceCallbackDefault(T_SERVER_ID service_id, void *p_data);
        static T_SERVER_ID _configSrvId;
        BLEAdvertData _advData;
};

#endif
