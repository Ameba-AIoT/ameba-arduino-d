#include "BLEDevice.h"
#include "BLEBatteryClient.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "bas_client.h"

#ifdef __cplusplus
}
#endif

T_SERVER_ID BLEBatteryClient::_basClientId = 0;
bool BLEBatteryClient::_basServiceAvaliable[BLE_CENTRAL_APP_MAX_LINKS] = {0};
bool BLEBatteryClient::_basNotificationEnabled[BLE_CENTRAL_APP_MAX_LINKS] = {0};
uint8_t BLEBatteryClient::_battLevel[BLE_CENTRAL_APP_MAX_LINKS] = {0};
void (*BLEBatteryClient::_pReadCB)(uint8_t, uint8_t) = nullptr;
void (*BLEBatteryClient::_pNotificationCB)(uint8_t, uint8_t) = nullptr;
void (*BLEBatteryClient::_pDiscoveryCB)(uint8_t, bool) = nullptr;

BLEBatteryClient::BLEBatteryClient() {
}

void BLEBatteryClient::addClient(uint8_t max_link_num) {
    if (max_link_num <= BLE_CENTRAL_APP_MAX_LINKS) {
        _basClientId = bas_add_client(clientCallbackDefault, max_link_num);
    } else {
        printf("Recommended max link count exceeded\r\n");
    }
}

bool BLEBatteryClient::discoverServices(uint8_t conn_id) {
    return (bas_start_discovery(conn_id));
}

bool BLEBatteryClient::readLevel(uint8_t conn_id) {
    if (_basServiceAvaliable[conn_id]) {
        return (bas_read_battery_level(conn_id));
    } else {
        printf("Battery service not avaliable on connection %d, discover services first.\r\n", conn_id);
    }
    return 0;
}

bool BLEBatteryClient::readNotificationState(uint8_t conn_id) {
    if (_basServiceAvaliable[conn_id]) {
        return (_basNotificationEnabled[conn_id]);
    } else {
        printf("Battery service not avaliable on connection %d, discover services first.\r\n", conn_id);
    }
    return false;
}

bool BLEBatteryClient::setNotificationState(uint8_t conn_id, bool notifyEnable) {
    if (_basServiceAvaliable[conn_id]) {
        if (bas_set_notify(conn_id, notifyEnable)) {
            vTaskDelay(100 / portTICK_RATE_MS);
            bas_read_notify(conn_id);
            return true;
        }
    } else {
        printf("Battery service not avaliable on connection %d, discover services first.\r\n", conn_id);
    }
    return false;
}

void BLEBatteryClient::setReadCallback(void (*fCallback)(uint8_t, uint8_t)) {
    _pReadCB = fCallback;
}

void BLEBatteryClient::setNotificationCallback(void (*fCallback)(uint8_t, uint8_t)) {
    _pNotificationCB = fCallback;
}

void BLEBatteryClient::setDiscoveryCallback(void (*fCallback)(uint8_t, bool)) {
    _pDiscoveryCB = fCallback;
}

T_APP_RESULT BLEBatteryClient::clientCallbackDefault(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data) {
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    if (client_id == _basClientId) {
        T_BAS_CLIENT_CB_DATA *p_bas_cb_data = (T_BAS_CLIENT_CB_DATA *)p_data;
        switch (p_bas_cb_data->cb_type) {
            
            case BAS_CLIENT_CB_TYPE_DISC_STATE: {
                switch (p_bas_cb_data->cb_content.disc_state) {
                    case DISC_BAS_FAILED: {
                        if (BTDEBUG) printf("Battery service not found on connection %d device\r\n", conn_id);
                        _basServiceAvaliable[conn_id] = 0;
                        break;
                    }
                    case DISC_BAS_DONE: {
                        if (BTDEBUG) printf("Battery service found on connection %d device\r\n", conn_id);
                        _basServiceAvaliable[conn_id] = 1;
                        break;
                    }
                    default:
                        break;
                }
                if (_pDiscoveryCB != nullptr) {
                    _pDiscoveryCB(conn_id, _basServiceAvaliable[conn_id]);
                }
                break;
            }
            
            case BAS_CLIENT_CB_TYPE_READ_RESULT: {
                switch (p_bas_cb_data->cb_content.read_result.type) {
                    case BAS_READ_NOTIFY: {
                        if (BTDEBUG) printf("Read result connection %d device notification state %d\r\n", conn_id, p_bas_cb_data->cb_content.read_result.data.notify);
                        _basNotificationEnabled[conn_id] = p_bas_cb_data->cb_content.read_result.data.notify;
                        break;
                    }
                    case BAS_READ_BATTERY_LEVEL: {
                        if (BTDEBUG) printf("Read result connection %d device battery level %d\r\n", conn_id, p_bas_cb_data->cb_content.read_result.data.battery_level);
                        _battLevel[conn_id] = p_bas_cb_data->cb_content.read_result.data.battery_level;
                        if (_pReadCB != nullptr) {
                            _pReadCB(conn_id, _battLevel[conn_id]);
                        }
                        break;
                    }
                }
                break;
            }
            
            case BAS_CLIENT_CB_TYPE_WRITE_RESULT: {
                switch (p_bas_cb_data->cb_content.write_result.type) {
                    case BAS_WRITE_NOTIFY_ENABLE: {
                        if (BTDEBUG) printf("Write result connection %d device notification enabled\r\n", conn_id);
                        _basNotificationEnabled[conn_id] = 1;
                        break;
                    }
                    case BAS_WRITE_NOTIFY_DISABLE: {
                        if (BTDEBUG) printf("Write result connection %d device notification disabled\r\n", conn_id);
                        _basNotificationEnabled[conn_id] = 0;
                        break;
                    }
                }
                break;
            }
            
            case BAS_CLIENT_CB_TYPE_NOTIF_IND_RESULT: {
                if (BTDEBUG) printf("Received notification connection %d device battery level %d\r\n", conn_id, p_bas_cb_data->cb_content.notify_data.battery_level);
                _battLevel[conn_id] = p_bas_cb_data->cb_content.notify_data.battery_level;
                if (_pNotificationCB != nullptr) {
                    _pNotificationCB(conn_id, _battLevel[conn_id]);
                }
                break;
            }
            
            default:
                if (BTDEBUG) printf("Unhandled callback type\r\n");
                break;
        }
    }
    return app_result;
}
