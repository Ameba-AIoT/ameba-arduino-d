#ifndef _BLE_DEVICE_H_
#define _BLE_DEVICE_H_

#include <Arduino.h>
#include "BLEAdvert.h"
#include "BLEScan.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "gap_msg.h"
#include "app_msg.h"
#include "profile_server.h"
#include "profile_client.h"
#include "ble_scatternet_link_mgr.h"

#ifdef __cplusplus
}
#endif

#define BTDEBUG 0

// Config APP LE link number
#define BLE_CENTRAL_APP_MAX_LINKS  3
// Config the discovery table number of gcs_client
#define BLE_CENTRAL_APP_MAX_DISCOV_TABLE_NUM 40

class BLEDevice {
    public:
        BLEDevice();
        void init();
        void deinit();
        bool connected();
        void setDeviceName(String devName);
        void setDeviceAppearance(uint16_t devAppearance);
        BLEAdvert* configAdvert();
        BLEScan* configScan();
        void setScanCallback(void (*scanCB)(T_LE_CB_DATA*));
        void beginCentral(uint8_t connCount);
        void beginPeripheral();
        void end();
        void configServer(uint8_t serviceCount);
        void configClient(uint8_t clientCount);

    private:
        static void BLEMainTask(void *p_param);
        void setupGAPBondManager();
        
        // Default message handlers for both peripheral and central modes
        static T_APP_RESULT gapCallbackDefault(uint8_t cb_type, void *p_cb_data);
        static T_APP_RESULT gapCallbackCentral(uint8_t cb_type, void *p_cb_data);
        static void ioMsgHandlerDefault(T_IO_MSG io_msg);
        static void gapMsgHandlerDefault(T_IO_MSG *p_gap_msg);
        static void devStateEvtHandlerPeriphDefault(T_GAP_DEV_STATE new_state, uint16_t cause);
        static void devStateEvtHandlerCentralDefault(T_GAP_DEV_STATE new_state, uint16_t cause);
        static void connStateEvtHandlerPeriphDefault(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause);
        static void connStateEvtHandlerCentralDefault(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause);
        static void connMtuInfoEvtHandlerDefault(uint8_t conn_id, uint16_t mtu_size);
        static void connParamUpdateEvtHandlerDefault(uint8_t conn_id, uint8_t status, uint16_t cause);
        static void authenStateEvtHandlerDefault(uint8_t conn_id, uint8_t new_state, uint16_t cause);
        static T_APP_RESULT appProfileCallbackDefault(T_SERVER_ID service_id, void *p_data);
        static T_APP_RESULT appClientCallbackDefault(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);

        // Default device name and device appearance
        char _deviceName[GAP_DEVICE_NAME_LEN] = "AMEBA_BLE_DEV";
        uint16_t _appearance = GAP_GATT_APPEARANCE_UNKNOWN;

        static BLEAdvert* _pBLEAdvert;  // Pointer to advertisement object (peripheral mode)
        static BLEScan* _pBLEScan;      // Pointer to scan object (central mode)
        static void (*_pScanCB)(T_LE_CB_DATA*); // Pointer to callback function for processing scan data
        //static BLEConnect* _pBLEConn;   // Pointer to connect object (central mode)
        static uint8_t _bleState;  // 0 = not running, 1 = peripheral, 2 = central

        // task and queue handles
        static void *_appTaskHandle;   //!< main task handle
        static void *_evtQueueHandle;  //!< Event queue handle
        static void *_ioQueueHandle;   //!< IO queue handle

        static T_GAP_DEV_STATE _gapDevState;
        static T_GAP_CONN_STATE _gapConnState;
        static T_APP_LINK _bleCentralAppLinkTable[BLE_CENTRAL_APP_MAX_LINKS];

        // GAP Bond Manager default parameters
        uint8_t  _authPairMode = GAP_PAIRING_MODE_PAIRABLE;
        uint16_t _authFlags = GAP_AUTHEN_BIT_BONDING_FLAG;
        uint8_t  _authIoCap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
        uint8_t  _authOob = false;
        uint8_t  _authUseFixPasskey = false;
        uint32_t _authFixPasskey = 0;
        uint8_t  _authSecReqEnable = false;
        uint16_t _authSecReqFlags = GAP_AUTHEN_BIT_BONDING_FLAG;
};

extern BLEDevice BLE;

#endif
