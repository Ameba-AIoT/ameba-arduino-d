#include "BLEDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "mpu_wrappers.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "wifi_drv.h"
#include "trace_app.h"
#include "bte.h"
#include "bt_types.h"
#include "gap.h"
#include "gap_adv.h"
#include "gap_bond_le.h"
#include "gap_callback_le.h"
#include "gap_config.h"
#include "gap_conn_le.h"
#include "gap_le.h"
#include "gap_le_types.h"
#include "gap_msg.h"
#include "gap_privacy.h"
#include "gap_scan.h"
#include "gap_storage_le.h"
#include "os_task.h"
#include "os_msg.h"
#include "app_msg.h"
#include "rtk_coex.h"
#include "profile_server.h"
#include "profile_client.h"

#ifdef __cplusplus
}
#endif

BLEDevice BLE;

BLEAdvert* BLEDevice::_pBLEAdvert = nullptr;
BLEScan* BLEDevice::_pBLEScan = nullptr;
void (*BLEDevice::_pScanCB)(T_LE_CB_DATA*) = nullptr;
BLEConnect* BLEDevice::_pBLEConn = nullptr;
uint8_t BLEDevice::_bleState = 0;
void *BLEDevice::_appTaskHandle = NULL;   //!< main task handle
void *BLEDevice::_evtQueueHandle = NULL;  //!< Event queue handle
void *BLEDevice::_ioQueueHandle = NULL;   //!< IO queue handle
T_GAP_DEV_STATE BLEDevice::_gapDevState = {0, 0, 0, 0, 0};
T_GAP_CONN_STATE BLEDevice::_gapConnState = GAP_CONN_STATE_DISCONNECTED;
T_APP_LINK BLEDevice::_bleCentralAppLinkTable[BLE_CENTRAL_APP_MAX_LINKS];

BLEDevice::BLEDevice() {
}

// allocate memory and low level resources for BT
// call this method before doing anything BT related
void BLEDevice::init() {
    T_GAP_DEV_STATE new_state;
    if (!(wifi_is_up(RTW_STA_INTERFACE) || wifi_is_up(RTW_AP_INTERFACE))) {
        wiFiDrv.wifiDriverInit();
    }
    while (!(wifi_is_up(RTW_STA_INTERFACE) || wifi_is_up(RTW_AP_INTERFACE))) {
        vTaskDelay(1000 / portTICK_RATE_MS);
        printf("WiFi not up\r\n");
    }

    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    if (new_state.gap_init_state == GAP_INIT_STATE_STACK_READY) {
        printf("BT Stack already on\r\n");
    } else {
        //bt_trace_init();
        bte_init();
    }
}

// free up memory and low level resources used by BT
void BLEDevice::deinit() {
    T_GAP_DEV_STATE new_state;
    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    if (new_state.gap_init_state != GAP_INIT_STATE_STACK_READY) {
        printf("BT Stack is not running\r\n");
    } else {
        BLEDevice::end();
        bte_deinit();
        //bt_trace_uninit();
        memset(&_gapDevState, 0, sizeof(T_GAP_DEV_STATE));
        printf("BT Stack deinitalized\r\n");
    }
}

// Peripheral should use connId = 0
bool BLEDevice::connected(uint8_t connId) {
    T_GAP_CONN_INFO connInfo;
    if (configConnection()->getConnInfo(connId, &connInfo)) {
        if (connInfo.conn_state == GAP_CONN_STATE_CONNECTED) {
            return true;
        } else {
            return false;
        }
    }
    return false;
}

// set BLE GAP device name
// other devices will see this name after connection is established
// note: this is seperate from the device name sent in advertising packets
// note: this name and advertised name can be different, but ideally should be set as indentical
void BLEDevice::setDeviceName(String devName) {
    // Set the Device Name in GAP, which will be visible after a connection is established
    if (devName.length() > GAP_DEVICE_NAME_LEN) {
        printf("Device name too long, maximum of %d chars\r\n", (GAP_DEVICE_NAME_LEN-1));
    }
    strcpy(_deviceName, devName.c_str());
}

// set BLE GAP device appearance
// default appearance is GAP_GATT_APPEARANCE_UNKNOWN
// refer to Bluetooth specifications for full list of possible appearances
void BLEDevice::setDeviceAppearance(uint16_t devAppearance) {
    _appearance = devAppearance;
}

BLEAdvert* BLEDevice::configAdvert() {
    if (_pBLEAdvert == nullptr) {
        _pBLEAdvert = new BLEAdvert();
    }
    return (_pBLEAdvert);
}

BLEScan* BLEDevice::configScan() {
    if (_pBLEScan == nullptr) {
        _pBLEScan = new BLEScan();
    }
    return (_pBLEScan);
}

BLEConnect* BLEDevice::configConnection() {
    if(_pBLEConn == nullptr) {
        _pBLEConn = new BLEConnect();
    }
    return (_pBLEConn);
}

void BLEDevice::setScanCallback(void (*scanCB)(T_LE_CB_DATA*)) {
    _pScanCB = scanCB;
}

// starts the BLE stack to operate as a central device
// note: central devices should not advertise to other devices
void BLEDevice::beginCentral(uint8_t connCount) {
    T_GAP_DEV_STATE new_state;
    if (_bleState != 0) {
        printf("BLE already running, unable to start central\r\n");
        return;
    } else {
        _bleState = 2;
    }

    if (connCount <= BLE_CENTRAL_APP_MAX_LINKS) {
        gap_config_max_le_link_num(connCount);
        le_gap_init(connCount);
    } else {
        printf("Recommended max link count exceeded\r\n");
    }

    uint8_t  phys_prefer = GAP_PHYS_PREFER_ALL;
    uint8_t  tx_phys_prefer = GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT | GAP_PHYS_PREFER_CODED_BIT;
    uint8_t  rx_phys_prefer = GAP_PHYS_PREFER_1M_BIT | GAP_PHYS_PREFER_2M_BIT | GAP_PHYS_PREFER_CODED_BIT;
    le_set_gap_param(GAP_PARAM_DEFAULT_PHYS_PREFER, sizeof(phys_prefer), &phys_prefer);
    le_set_gap_param(GAP_PARAM_DEFAULT_TX_PHYS_PREFER, sizeof(tx_phys_prefer), &tx_phys_prefer);
    le_set_gap_param(GAP_PARAM_DEFAULT_RX_PHYS_PREFER, sizeof(rx_phys_prefer), &rx_phys_prefer);

    // update device parameters
    le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, _deviceName);
    le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(_appearance), &_appearance);

    // update scan parameters
    configScan()->updateScanParams();
    if (BTDEBUG) printf("Scan update\r\n");

    setupGAPBondManager();

    // register callback to handle app GAP message
    le_register_app_cb(gapCallbackDefault);
    if (BTDEBUG) printf("GAP cb reg\r\n");

    // register clients and callbacks
    client_register_general_client_cb(appClientCallbackDefault);
    //ble_central_gcs_client_id = gcs_add_client(ble_central_gcs_client_callback, BLE_CENTRAL_APP_MAX_LINKS, BLE_CENTRAL_APP_MAX_DISCOV_TABLE_NUM);

    // start BLE main task to handle IO and GAP msg
    os_task_create(&_appTaskHandle, "BLE_Central_Task", BLEMainTask, 0, 256*6, 1);
    if (BTDEBUG) printf("Task create\r\n");

    bt_coex_init();
    if (BTDEBUG) printf("Coex init\r\n");

    /*Wait BT init complete*/
    do {
        vTaskDelay(100 / portTICK_RATE_MS);
        le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    }while(new_state.gap_init_state != GAP_INIT_STATE_STACK_READY);

    /*Start BT WIFI coexistence*/
    wifi_btcoex_set_bt_on();
    if (BTDEBUG) printf("Coex on\r\n");

}

// starts the BLE stack to operate as a peripheral device
// note: peripheral devices should not scan for other devices
void BLEDevice::beginPeripheral() {
    T_GAP_DEV_STATE new_state;
    if (_bleState != 0) {
        printf("BLE already running, unable to start peripheral\r\n");
        return;
    } else {
        _bleState = 1;
    }
    uint8_t  slave_init_mtu_req = false;

    gap_config_max_le_link_num(1);
    le_gap_init(1);

    // update device parameters
    le_set_gap_param(GAP_PARAM_DEVICE_NAME, GAP_DEVICE_NAME_LEN, _deviceName);
    le_set_gap_param(GAP_PARAM_APPEARANCE, sizeof(_appearance), &_appearance);
    le_set_gap_param(GAP_PARAM_SLAVE_INIT_GATT_MTU_REQ, sizeof(slave_init_mtu_req), &slave_init_mtu_req);

    // update advertising parameters
    configAdvert()->updateAdvertParams();
    if (BTDEBUG) printf("Adv update\r\n");

    setupGAPBondManager();

    // register callback to handle app GAP message
    le_register_app_cb(gapCallbackDefault);
    if (BTDEBUG) printf("GAP cb reg\r\n");

    // register services and callbacks
    server_register_app_cb(appProfileCallbackDefault);

    // start BLE main task to handle IO and GAP msg
    os_task_create(&_appTaskHandle, "BLE_Peripheral_Task", BLEMainTask, 0, 256*4, 1);
    if (BTDEBUG) printf("Task create\r\n");

    bt_coex_init();
    if (BTDEBUG) printf("Coex init\r\n");

    // Wait BT init complete
    do {
        vTaskDelay(100 / portTICK_RATE_MS);
        le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    }while(new_state.gap_adv_state != GAP_ADV_STATE_ADVERTISING); // Waiting for GAP_ADV_STATE seems more reliable than GAP_INIT_STATE

    // Start BT WIFI coexistence
    wifi_btcoex_set_bt_on();
    if (BTDEBUG) printf("Coex on\r\n");
}

// stops the BLE stack operating as a peripheral or central device
void BLEDevice::end() {
    T_GAP_DEV_STATE new_state;
    if (_bleState == 0) {
        printf("BLE not running, nothing to end\r\n");
        return;
    } else {
        _bleState = 0;
    }

    // disconnect current devices
    // app task deinit
    // check advertising state and stop advertising
    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    if (new_state.gap_adv_state != GAP_ADV_STATE_IDLE) {
        le_adv_stop();
    }

    // check scan state and stop scan
    le_get_gap_param(GAP_PARAM_DEV_STATE , &new_state);
    if (new_state.gap_scan_state != GAP_SCAN_STATE_SCANNING) {
        le_scan_stop();
    }

    if (_ioQueueHandle) {
        os_msg_queue_delete(_ioQueueHandle);
    }
    if (_evtQueueHandle) {
        os_msg_queue_delete(_evtQueueHandle);
    }
    if (_appTaskHandle) {
        os_task_delete(_appTaskHandle);
    }
    _ioQueueHandle = NULL;
    _evtQueueHandle = NULL;
    _appTaskHandle = NULL;

    wifi_btcoex_set_bt_off();

    if (_pBLEAdvert != nullptr) {
        delete _pBLEAdvert;
        _pBLEAdvert = nullptr;
    }
    if (_pBLEScan != nullptr) {
        delete _pBLEScan;
        _pBLEScan = nullptr;
    }
}

void BLEDevice::configServer(uint8_t serviceCount) {
    if (serviceCount <= 5) {
        server_init(serviceCount);
    } else {
        printf("Too many services \r\n");
    }
}

void BLEDevice::configClient(uint8_t clientCount) {
    if (clientCount <= 5) {
        client_init(clientCount);
    } else {
        printf("Too many clients \r\n");
    }
}

void BLEDevice::getLocalAddr(uint8_t (&addr)[GAP_BD_ADDR_LEN]) {
    uint8_t  btaddr[GAP_BD_ADDR_LEN] = {0};
    gap_get_param(GAP_PARAM_BD_ADDR, btaddr);
    memcpy(addr, btaddr, sizeof(btaddr));
}

void BLEDevice::setupGAPBondManager() {
    // Setup the GAP Bond Manager
    gap_set_param(GAP_PARAM_BOND_PAIRING_MODE, sizeof(_authPairMode), &_authPairMode);
    gap_set_param(GAP_PARAM_BOND_AUTHEN_REQUIREMENTS_FLAGS, sizeof(_authFlags), &_authFlags);
    gap_set_param(GAP_PARAM_BOND_IO_CAPABILITIES, sizeof(_authIoCap), &_authIoCap);
    gap_set_param(GAP_PARAM_BOND_OOB_ENABLED, sizeof(_authOob), &_authOob);
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY, sizeof(_authFixPasskey), &_authFixPasskey);
    le_bond_set_param(GAP_PARAM_BOND_FIXED_PASSKEY_ENABLE, sizeof(_authUseFixPasskey), &_authUseFixPasskey);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_ENABLE, sizeof(_authSecReqEnable), &_authSecReqEnable);
    le_bond_set_param(GAP_PARAM_BOND_SEC_REQ_REQUIREMENT, sizeof(_authSecReqFlags), &_authSecReqFlags);
}

void BLEDevice::BLEMainTask(void *p_param) {
    (void)p_param;
    uint8_t event;
    os_msg_queue_create(&_ioQueueHandle, 0x20, sizeof(T_IO_MSG));
    os_msg_queue_create(&_evtQueueHandle, 0x40, sizeof(uint8_t));

    gap_start_bt_stack(_evtQueueHandle, _ioQueueHandle, 0x20);
    if (BTDEBUG) printf("BT stack start\r\n");

    while (true) {
        if (os_msg_recv(_evtQueueHandle, &event, 0xFFFFFFFF) == true) {
            if (event == EVENT_IO_TO_APP) {
                T_IO_MSG io_msg;
                if (os_msg_recv(_ioQueueHandle, &io_msg, 0) == true) {
                    ioMsgHandlerDefault(io_msg);
                }
            } else {
                gap_handle_msg(event);
            }
        }
    }
}
