#ifndef _BLE_DEVICE_H_
#define _BLE_DEVICE_H_

#include <Arduino.h>
#include "BLEAdvert.h"
#include "BLEConnect.h"
#include "BLEScan.h"
#include "BLEService.h"
#include "BLEClient.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "gap_msg.h"
#include "app_msg.h"
#include "profile_server.h"
#include "profile_client.h"
#include "app_flags.h"
#include "link_mgr.h"

#ifdef __cplusplus
}
#endif

#define BTDEBUG 0

// Config APP LE link number
#define BLE_CENTRAL_APP_MAX_LINKS 3
// Config the discovery table number of gcs_client
#define BLE_CENTRAL_APP_MAX_DISCOV_TABLE_NUM 40

#define BLE_MAX_SERVICE_COUNT 5

class BLEDevice {
    public:
        BLEDevice();
        void init();
        void deinit();
        bool connected(uint8_t connId = 0);
        void setDeviceName(String devName);
        void setDeviceAppearance(uint16_t devAppearance);
        BLEAdvert* configAdvert();
        BLEScan* configScan();
        BLEConnect* configConnection();
        void setScanCallback(void (*scanCB)(T_LE_CB_DATA*));
        void beginCentral(uint8_t connCount = BLE_CENTRAL_APP_MAX_LINKS);
        void beginPeripheral();
        void end();
        void getLocalAddr(uint8_t (&addr)[GAP_BD_ADDR_LEN]);        // Local address is only avaliable after peripheral or central mode is started
        void configServer(uint8_t maxServiceCount);
        void addService(BLEService& newService);
        void configClient();
        BLEClient* addClient(uint8_t connId);

    private:
        static void BLEMainTask(void *p_param);
        void setupGAPBondManager();

        //---------------------------- Default handlers for core BLE functionality ----------------------------//
        static T_APP_RESULT gapCallbackDefault(uint8_t cb_type, void *p_cb_data);
        static void ioMsgHandlerDefault(T_IO_MSG io_msg);
        static void gapMsgHandlerDefault(T_IO_MSG *p_gap_msg);
        static void devStateEvtHandlerPeriphDefault(T_GAP_DEV_STATE new_state, uint16_t cause);
        static void devStateEvtHandlerCentralDefault(T_GAP_DEV_STATE new_state, uint16_t cause);
        static void connStateEvtHandlerPeriphDefault(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause);
        static void connStateEvtHandlerCentralDefault(uint8_t conn_id, T_GAP_CONN_STATE new_state, uint16_t disc_cause);
        static void connMtuInfoEvtHandlerDefault(uint8_t conn_id, uint16_t mtu_size);
        static void connParamUpdateEvtHandlerDefault(uint8_t conn_id, uint8_t status, uint16_t cause);
        static void authenStateEvtHandlerDefault(uint8_t conn_id, uint8_t new_state, uint16_t cause);

        //----------------------------------- Default callbacks for services -----------------------------------//
        static T_APP_RESULT appServiceCallbackDefault(T_SERVER_ID service_id, void *p_data);
        static T_APP_RESULT serviceAttrReadCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                                                        uint16_t offset, uint16_t *p_length, uint8_t **pp_value);
        static T_APP_RESULT serviceAttrWriteCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                                                        T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                                                        P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc);
        static void serviceCccdUpdateCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index, uint16_t ccc_bits);

        //----------------------------------- Default callbacks for clients -----------------------------------//
        static T_APP_RESULT appClientCallbackDefault(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);
        static void clientDiscoverStateCallbackDefault(uint8_t conn_id, T_DISCOVERY_STATE discovery_state);
        static void clientDiscoverResultCallbackDefault(uint8_t conn_id, T_DISCOVERY_RESULT_TYPE result_type, T_DISCOVERY_RESULT_DATA result_data);
        static void clientReadResultCallbackDefault(uint8_t conn_id, uint16_t cause, uint16_t handle, uint16_t value_size, uint8_t *p_value);
        static void clientWriteResultCallbackDefault(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle, uint16_t cause, uint8_t credits);
        static T_APP_RESULT clientNotifyIndicateCallbackDefault(uint8_t conn_id, bool notify, uint16_t handle, uint16_t value_size, uint8_t *p_value);
        static void clientDisconnectCallbackDefault(uint8_t conn_id);

        // Default device name and device appearance
        char _deviceName[GAP_DEVICE_NAME_LEN] = "AMEBA_BLE_DEV";
        uint16_t _appearance = GAP_GATT_APPEARANCE_UNKNOWN;

        static uint8_t _bleState;  // 0 = not running, 1 = peripheral, 2 = central

        // Pointers to other class objects
        static BLEAdvert* _pBLEAdvert;  // Pointer to advertisement object (peripheral mode)
        static BLEScan* _pBLEScan;      // Pointer to scan object (central mode)
        static void (*_pScanCB)(T_LE_CB_DATA*); // Pointer to callback function for processing scan data
        static BLEConnect* _pBLEConn;   // Pointer to connect object (central mode)
        static BLEService* _servicePtrList[BLE_MAX_SERVICE_COUNT];
        static uint8_t _serviceCount;
        static BLEClient* _clientPtrList[BLE_CENTRAL_APP_MAX_LINKS];

        const T_FUN_GATT_SERVICE_CBS _serviceCallbacksDefault = {
            serviceAttrReadCallbackDefault,   // Read callback function pointer
            serviceAttrWriteCallbackDefault,  // Write callback function pointer
            serviceCccdUpdateCallbackDefault  // CCCD update callback function pointer
        };

        const T_FUN_CLIENT_CBS _clientCallbacksDefault = {
            clientDiscoverStateCallbackDefault,   // Discovery State callback function pointer
            clientDiscoverResultCallbackDefault,  // Discovery result callback function pointer
            clientReadResultCallbackDefault,      // Read response callback function pointer
            clientWriteResultCallbackDefault,     // Write result callback function pointer
            clientNotifyIndicateCallbackDefault,  // Notify Indicate callback function pointer
            clientDisconnectCallbackDefault       // Link disconnection callback function pointer
        };

        // task and queue handles
        static void *_appTaskHandle;   //!< main task handle
        static void *_evtQueueHandle;  //!< Event queue handle
        static void *_ioQueueHandle;   //!< IO queue handle

        static T_GAP_DEV_STATE _gapDevState;
        static T_GAP_CONN_STATE _gapConnState;
        static T_APP_LINK _bleCentralAppLinkTable[BLE_CENTRAL_APP_MAX_LINKS];

        // GAP Bond Manager default parameters
        uint8_t  _authPairMode = GAP_PAIRING_MODE_NO_PAIRING;//GAP_PAIRING_MODE_PAIRABLE;
        uint16_t _authFlags = GAP_AUTHEN_BIT_NONE;//GAP_AUTHEN_BIT_BONDING_FLAG;
        uint8_t  _authIoCap = GAP_IO_CAP_NO_INPUT_NO_OUTPUT;
        uint8_t  _authOob = false;
        uint8_t  _authUseFixPasskey = false;
        uint32_t _authFixPasskey = 0;
        uint8_t  _authSecReqEnable = false;
        uint16_t _authSecReqFlags = GAP_AUTHEN_BIT_NONE;//GAP_AUTHEN_BIT_BONDING_FLAG;

        // GAP PHY preference
        static uint8_t all_phys;
        static uint8_t tx_phys;
        static uint8_t rx_phys;
        static T_GAP_PHYS_OPTIONS phy_options;
};

extern BLEDevice BLE;

#endif
