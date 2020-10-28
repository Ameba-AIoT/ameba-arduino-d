#include "BLEDevice.h"
#include "BLEWifiConfigService.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "gap.h"
#include "bt_config_service.h"
#include "bt_config_wifi.h"

#ifdef __cplusplus
}
#endif

T_SERVER_ID BLEWifiConfigService::_configSrvId = 0;

BLEWifiConfigService::BLEWifiConfigService() {
}

void BLEWifiConfigService::begin() {
    BC_cmd_task_init();
}

void BLEWifiConfigService::end() {
    BC_cmd_task_deinit();
}

void BLEWifiConfigService::addService() {
    _configSrvId = bt_config_service_add_service((void *)serviceCallbackDefault);
}

BLEAdvertData BLEWifiConfigService::advData() {
    char device_name[GAP_DEVICE_NAME_LEN] = {0};
    uint8_t btaddr[GAP_BD_ADDR_LEN] = {0};
    BLE.getLocalAddr(btaddr);
    sprintf(device_name, "Ameba_%2X%2X%2X",btaddr[2],btaddr[1],btaddr[0]);

    _advData.addFlags(GAP_ADTYPE_FLAGS_LIMITED | GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED);
    _advData.addCompleteServices(BLEUUID("FF01"));
    _advData.addCompleteName((char *)device_name);

    return _advData;
}

T_APP_RESULT BLEWifiConfigService::serviceCallbackDefault(T_SERVER_ID service_id, void *p_data) {
    T_APP_RESULT app_result = APP_RESULT_SUCCESS;
    if (service_id == _configSrvId) {
        TBTCONFIG_CALLBACK_DATA *p_simp_cb_data = (TBTCONFIG_CALLBACK_DATA *)p_data;
        switch (p_simp_cb_data->msg_type) {

            case SERVICE_CALLBACK_TYPE_READ_CHAR_VALUE: {
                uint8_t *read_buf = NULL;
                uint32_t read_buf_len = 0;
                // Otherwise, use BC_handle_read_request to get read response from BT Config
                BC_handle_read_request(&read_buf, &read_buf_len, p_simp_cb_data->msg_data.read_offset);
                if(read_buf != NULL) {
                    bt_config_service_set_parameter(BTCONFIG_SERVICE_PARAM_V1_READ_CHAR_VAL, read_buf_len, read_buf);
                }
                if (BTDEBUG) printf("BTCONFIG read char value\r\n");
                break;
            }

        case SERVICE_CALLBACK_TYPE_WRITE_CHAR_VALUE: {
            switch (p_simp_cb_data->msg_data.write.opcode) {
                case BTCONFIG_WRITE_V1: {
                    // Otherwise, use BC_send_cmd to send data (BT Config command) to BT Config
                    BC_send_cmd( p_simp_cb_data->msg_data.write.p_value,  p_simp_cb_data->msg_data.write.len);
                    if (BTDEBUG) printf("BTCONFIG write char value\r\n");
                    break;
                }
                default:
                    break;
            }
            break;
        }

        default:
            break;
        }
    }
    return app_result;
}
