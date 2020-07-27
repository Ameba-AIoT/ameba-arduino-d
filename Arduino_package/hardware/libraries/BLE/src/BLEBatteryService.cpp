#include "BLEDevice.h"
#include "BLEBatteryService.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "bas.h"

#ifdef __cplusplus
}
#endif

T_SERVER_ID BLEBatteryService::_basSrvId = 0;
uint8_t BLEBatteryService::_connId = 0;
uint8_t BLEBatteryService::_battLevel = 0;
uint8_t BLEBatteryService::_notificationEnable = 0;
void (*BLEBatteryService::_pBasReadCB)(void) = nullptr;
void (*BLEBatteryService::_pBasNotificationCB)(uint8_t) = nullptr;

BLEBatteryService::BLEBatteryService() {
}

void BLEBatteryService::addService() {
    _basSrvId = bas_add_service((void *)serviceCallbackDefault);
}

// takes in a callback function that accepts no parameters
void BLEBatteryService::setReadCallback(void (*fCallback)(void)) {
    _pBasReadCB = fCallback;
}

// takes in a callback function that accepts a uint8_t | 1 -> notification enable | 2 -> notification disable
void BLEBatteryService::setNotificationCallback(void (*fCallback)(uint8_t)) {
    _pBasNotificationCB = fCallback;
}

bool BLEBatteryService::notifyEnabled() {
    if (_notificationEnable) {
        return true;
    } else {
        return false;
    }
}

void BLEBatteryService::setLevel(uint8_t level) {
    _battLevel = level;
    bas_set_parameter(BAS_PARAM_BATTERY_LEVEL, 1, &_battLevel);
}

void BLEBatteryService::sendNotification(uint8_t level) {
    _battLevel = level;
    bas_set_parameter(BAS_PARAM_BATTERY_LEVEL, 1, &_battLevel);
    if (_notificationEnable) {
        bas_battery_level_value_notify(_connId, _basSrvId, _battLevel);
    } else {
        printf("Notification not enabled\r\n");
    }
}

T_APP_RESULT BLEBatteryService::serviceCallbackDefault(T_SERVER_ID service_id, void *p_data) {
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    if (service_id == _basSrvId) {
        T_BAS_CALLBACK_DATA *p_bas_cb_data = (T_BAS_CALLBACK_DATA *)p_data;
        switch (p_bas_cb_data->msg_type) {
            case SERVICE_CALLBACK_TYPE_INDIFICATION_NOTIFICATION: {
                switch (p_bas_cb_data->msg_data.notification_indification_index) {
                    case BAS_NOTIFY_BATTERY_LEVEL_ENABLE: {
                        if (BTDEBUG) printf("BAS_NOTIFY_BATTERY_LEVEL_ENABLE\r\n");
                        _notificationEnable = 1;
                        _connId = p_bas_cb_data->conn_id;
                        break;
                    }
                    case BAS_NOTIFY_BATTERY_LEVEL_DISABLE: {
                        if (BTDEBUG) printf("BAS_NOTIFY_BATTERY_LEVEL_DISABLE\r\n");
                        _notificationEnable = 0;
                        _connId = 0;
                        break;
                    }
                    default:
                        break;
                }
                if (_pBasNotificationCB != nullptr) {
                    _pBasNotificationCB(p_bas_cb_data->msg_data.notification_indification_index);
                }
                break;
            }

            case SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE: {
                if (p_bas_cb_data->msg_data.read_value_index == BAS_READ_BATTERY_LEVEL) {
                    if (_pBasReadCB != nullptr) {
                        _pBasReadCB();
                    } else {
                        if (BTDEBUG) printf("BAS_READ_BATTERY_LEVEL: battery_level %d\r\n", _battLevel);
                        bas_set_parameter(BAS_PARAM_BATTERY_LEVEL, 1, &_battLevel);
                    }
                }
                break;
            }

            default:
                break;
        }
    }
    return app_result;
}
