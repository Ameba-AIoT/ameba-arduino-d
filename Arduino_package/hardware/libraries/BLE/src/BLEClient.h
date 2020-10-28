#ifndef _BLE_CLIENT_H_
#define _BLE_CLIENT_H_

#include <Arduino.h>
#include "BLEUUID.h"
#include "BLERemoteService.h"

#define MAX_REMOTE_SERVICE_COUNT 10

#ifdef __cplusplus
extern "C" {
#endif

#include "profile_client.h"

#ifdef __cplusplus
}
#endif

typedef enum {
    DISC_IDLE,
    DISC_START,
    DISC_DONE,
    DISC_FAILED
} T_CLIENT_DISC_STATE;

class BLEClient {
    public:
        bool connected();
        void discoverServices();
        bool discoveryDone();
        void printServices();

        BLERemoteService* getService(const char* uuid);
        BLERemoteService* getService(BLEUUID uuid);
        uint8_t getConnId();
        T_CLIENT_ID getClientId();

        void setDisconnectCallback(void (*fCallback) (BLEClient* client));

    private:
        BLEClient();
        ~BLEClient();

        void clientDiscoverStateCallbackDefault(uint8_t conn_id, T_DISCOVERY_STATE discovery_state);
        void clientDiscoverResultCallbackDefault(uint8_t conn_id, T_DISCOVERY_RESULT_TYPE result_type, T_DISCOVERY_RESULT_DATA result_data);
        void clientReadResultCallbackDefault(uint8_t conn_id, uint16_t cause, uint16_t handle, uint16_t value_size, uint8_t *p_value);
        void clientWriteResultCallbackDefault(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle, uint16_t cause, uint8_t credits);
        T_APP_RESULT clientNotifyIndicateCallbackDefault(uint8_t conn_id, bool notify, uint16_t handle, uint16_t value_size, uint8_t *p_value);
        void clientDisconnectCallbackDefault(uint8_t conn_id);

        friend class BLEDevice;
        friend class BLERemoteService;

        uint8_t _connId = 0xff;
        T_CLIENT_ID _clientId = CLIENT_PROFILE_GENERAL_ID;
        T_CLIENT_DISC_STATE _serviceDiscState = DISC_IDLE;

        BLERemoteService* _servicePtrList[MAX_REMOTE_SERVICE_COUNT] = {0};
        uint8_t _serviceCount = 0;

        void (*_pDiscCB)(BLEClient*) = nullptr;
};

#endif

