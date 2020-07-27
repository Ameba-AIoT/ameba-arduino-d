#ifndef _BLE_BATTCLIENT_H_
#define _BLE_BATTCLIENT_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "gap.h"
#include "bas_client.h"
#include "profile_client.h"

#ifdef __cplusplus
}
#endif

class BLEBatteryClient {
    public:
        BLEBatteryClient();
        void addClient(uint8_t max_link_num);
        bool discoverServices(uint8_t conn_id);
        bool readLevel(uint8_t conn_id);
        bool readNotificationState(uint8_t conn_id);
        bool setNotificationState(uint8_t conn_id, bool notifyEnable);
        void setReadCallback(void (*fCallback)(uint8_t, uint8_t));
        void setNotificationCallback(void (*fCallback)(uint8_t, uint8_t));
        void setDiscoveryCallback(void (*fCallback)(uint8_t, bool));

    private:
        static T_APP_RESULT clientCallbackDefault(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);

        static T_CLIENT_ID _basClientId;
        static bool _basServiceAvaliable[BLE_CENTRAL_APP_MAX_LINKS];
        static bool _basNotificationEnabled[BLE_CENTRAL_APP_MAX_LINKS];
        static uint8_t _battLevel[BLE_CENTRAL_APP_MAX_LINKS];
        static void (*_pReadCB)(uint8_t, uint8_t);
        static void (*_pNotificationCB)(uint8_t, uint8_t);
        static void (*_pDiscoveryCB)(uint8_t, bool);

};

#endif
