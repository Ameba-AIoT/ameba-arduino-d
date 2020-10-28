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

void wifi_btcoex_set_bt_on(void);
void wifi_btcoex_set_bt_off(void);

#ifdef __cplusplus
}
#endif

BLEDevice BLE;

uint8_t BLEDevice::_bleState = 0;
BLEAdvert* BLEDevice::_pBLEAdvert = nullptr;
BLEScan* BLEDevice::_pBLEScan = nullptr;
void (*BLEDevice::_pScanCB)(T_LE_CB_DATA*) = nullptr;
BLEConnect* BLEDevice::_pBLEConn = nullptr;
BLEService* BLEDevice::_servicePtrList[BLE_MAX_SERVICE_COUNT] = {};
uint8_t BLEDevice::_serviceCount = 0;
BLEClient* BLEDevice::_clientPtrList[BLE_CENTRAL_APP_MAX_LINKS] = {};
void *BLEDevice::_appTaskHandle = NULL;   //!< main task handle
void *BLEDevice::_evtQueueHandle = NULL;  //!< Event queue handle
void *BLEDevice::_ioQueueHandle = NULL;   //!< IO queue handle
T_GAP_DEV_STATE BLEDevice::_gapDevState = {0, 0, 0, 0, 0};
T_GAP_CONN_STATE BLEDevice::_gapConnState = GAP_CONN_STATE_DISCONNECTED;
T_APP_LINK BLEDevice::_bleCentralAppLinkTable[BLE_CENTRAL_APP_MAX_LINKS];
uint8_t BLEDevice::all_phys = GAP_PHYS_PREFER_2M_BIT;
uint8_t BLEDevice::tx_phys = GAP_PHYS_PREFER_2M_BIT;
uint8_t BLEDevice::rx_phys = GAP_PHYS_PREFER_2M_BIT;
T_GAP_PHYS_OPTIONS BLEDevice::phy_options = GAP_PHYS_OPTIONS_CODED_PREFER_NO;

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
        //gap_config_max_le_paired_device(BLE_CENTRAL_APP_MAX_LINKS);
        le_gap_init(connCount);
    } else {
        printf("Recommended max link count exceeded\r\n");
    }

    // Update GAP PHY preferences
    le_set_gap_param(GAP_PARAM_DEFAULT_PHYS_PREFER, sizeof(all_phys), &all_phys);
    le_set_gap_param(GAP_PARAM_DEFAULT_TX_PHYS_PREFER, sizeof(tx_phys), &tx_phys);
    le_set_gap_param(GAP_PARAM_DEFAULT_RX_PHYS_PREFER, sizeof(rx_phys), &rx_phys);

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
    uint8_t  slave_init_mtu_req = true;//false;

    gap_config_max_le_link_num(1);
    //gap_config_max_le_paired_device(BLE_CENTRAL_APP_MAX_LINKS);
    le_gap_init(1);

    // Update GAP PHY preferences
    le_set_gap_param(GAP_PARAM_DEFAULT_PHYS_PREFER, sizeof(all_phys), &all_phys);
    le_set_gap_param(GAP_PARAM_DEFAULT_TX_PHYS_PREFER, sizeof(tx_phys), &tx_phys);
    le_set_gap_param(GAP_PARAM_DEFAULT_RX_PHYS_PREFER, sizeof(rx_phys), &rx_phys);

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
    uint8_t connId;
    for (connId = 0; connId < BLE_CENTRAL_APP_MAX_LINKS; connId++) {
        if (_bleCentralAppLinkTable[connId].conn_state == GAP_CONN_STATE_CONNECTED) {
            le_disconnect(connId);
        }
    }

    // delete all existing clients and services
    for (connId = 0; connId < BLE_CENTRAL_APP_MAX_LINKS; connId++) {
        if (_clientPtrList[connId] != nullptr) {
            delete _clientPtrList[connId];
        }
    }
    for (connId = 0; connId < BLE_MAX_SERVICE_COUNT; connId++) {
        _servicePtrList[connId] = nullptr;
    }
    _serviceCount = 0;

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

    // app task deinit
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
    if (_pBLEConn != nullptr) {
        delete _pBLEConn;
        _pBLEConn = nullptr;
    }
}

void BLEDevice::configServer(uint8_t maxServiceCount) {
    if (maxServiceCount <= BLE_MAX_SERVICE_COUNT) {
        server_init(maxServiceCount);
        // register default service callback
        server_register_app_cb(appServiceCallbackDefault);
    } else {
        printf("Too many services \r\n");
    }
}

void BLEDevice::configClient() {
        client_init(BLE_CENTRAL_APP_MAX_LINKS);
        // register default client callback
        client_register_general_client_cb(appClientCallbackDefault);
}

void BLEDevice::addService(BLEService& newService) {
    if (_serviceCount < (BLE_MAX_SERVICE_COUNT)) {
        T_SERVER_ID service_id;
        if (false == server_add_service(&service_id, (uint8_t *)newService.generateServiceAttrTable(), newService._total_attr_count * sizeof(T_ATTRIB_APPL), _serviceCallbacksDefault)) {
            printf("server_add_service %s failed\n", newService.getUUID().str());
        } else {
            _servicePtrList[_serviceCount++] = &newService;
            newService.setServiceID(service_id);
        }
    } else {
        printf("Maximum number of services reached \n");
    }
}

BLEClient* BLEDevice::addClient(uint8_t connId) {
    BLEClient* newClient = nullptr;
    if (connId >= BLE_CENTRAL_APP_MAX_LINKS) {
        printf("Invalid connection ID %d \n", connId);
        return newClient;
    }
    if (!connected(connId)) {
        printf("No device connected at conn ID %d \n", connId);
        return newClient;
    }

    if (_clientPtrList[connId] != nullptr) {
        return _clientPtrList[connId];
    }
    T_CLIENT_ID client_id;
    newClient = new BLEClient();
    if (newClient == nullptr) {
        printf("Create new client failed for conn ID %d \n", connId);
        return newClient;
    }
    if (false == client_register_spec_client_cb(&client_id, &_clientCallbacksDefault)) {
        printf("Register_client failed for conn ID %d \n", connId);
        delete newClient;
        newClient = nullptr;
    } else {
        _clientPtrList[connId] = newClient;
        newClient->_clientId = client_id;
        newClient->_connId = connId;
    }

    return newClient;
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
    //int ret = gap_set_pairable_mode();
    //if(ret == GAP_CAUSE_SUCCESS)
        //printf("\n\rSet pairable mode success!\r\n");
    //else
        //printf("\n\rSet pairable mode fail!\r\n");
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
