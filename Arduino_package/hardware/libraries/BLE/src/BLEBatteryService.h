#ifndef _BLE_BATTSERVICE_H_
#define _BLE_BATTSERVICE_H_

#ifdef __cplusplus
extern "C" {
#endif

#include "bas.h"
#include "gap.h"
#include "profile_server.h"

#ifdef __cplusplus
}
#endif

class BLEBatteryService {
    public:
        BLEBatteryService();
        void addService();
        void setReadCallback(void (*fCallback)(void));
        void setNotificationCallback(void (*fCallback)(uint8_t));
        bool notifyEnabled();
        void setLevel(uint8_t level);
        void sendNotification(uint8_t level);

    private:
        static T_APP_RESULT serviceCallbackDefault(T_SERVER_ID service_id, void *p_data);

        static T_SERVER_ID _basSrvId;
        static uint8_t _notificationEnable;
        static uint8_t _connId;
        static uint8_t _battLevel;
        static void (*_pBasReadCB)(void);
        static void (*_pBasNotificationCB)(uint8_t);
};

#endif
