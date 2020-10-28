#include "BLEConnect.h"
#include "BLEDevice.h"
#include "BLEAddr.h"
#include "BLEAdvertData.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "mpu_wrappers.h"

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

#ifdef __cplusplus
}
#endif

BLEConnect::BLEConnect() {
    _connReqParam.scan_interval = 0x40;
    _connReqParam.scan_window = 0x30;
    _connReqParam.conn_interval_min = 12;
    _connReqParam.conn_interval_max = 12;
    _connReqParam.conn_latency = 0;
    _connReqParam.supv_tout = 500;
    _connReqParam.ce_len_min = 2 * (_connReqParam.conn_interval_min - 1);
    _connReqParam.ce_len_max = 2 * (_connReqParam.conn_interval_max - 1);
}

bool BLEConnect::connect(char* btAddr, T_GAP_REMOTE_ADDR_TYPE destAddrType, uint16_t scanTimeout) {
    BLEAddr destAddr(btAddr);
    return connect(destAddr, destAddrType, scanTimeout);
}

bool BLEConnect::connect(uint8_t (&btAddr)[6], T_GAP_REMOTE_ADDR_TYPE destAddrType, uint16_t scanTimeout) {
    BLEAddr destAddr(btAddr);
    return connect(destAddr, destAddrType, scanTimeout);
}

bool BLEConnect::connect(BLEAdvertData targetDevice, uint16_t scanTimeout) {
    BLEAddr destAddr = targetDevice.getAddr();
    T_GAP_REMOTE_ADDR_TYPE destAddrType = targetDevice.getAddrType();
    return connect(destAddr, destAddrType, scanTimeout);
}

bool BLEConnect::connect(BLEAddr destAddr, T_GAP_REMOTE_ADDR_TYPE destAddrType, uint16_t scanTimeout) {
    le_set_conn_param(GAP_CONN_PARAM_1M, &_connReqParamDefault);
    // init connection on default 1M PHYS
    T_GAP_CAUSE result = le_connect(GAP_CONN_PARAM_1M, destAddr.data(), destAddrType, _localAddrType, scanTimeout);

    if (result == GAP_CAUSE_SUCCESS) {
        if (BTDEBUG) printf("Connect successful to %s\r\n", destAddr.str());
        return true;
    } else {
        if (BTDEBUG) printf("Connect failed\r\n");
        return false;
    }
}

bool BLEConnect::disconnect(uint8_t connId) {
    T_GAP_CAUSE result = le_disconnect(connId);

    if (result == GAP_CAUSE_SUCCESS) {
        return true;
    } else {
        return false;
    }
}

void BLEConnect::setScanInterval(uint16_t scanInt_ms) {
    if((scanInt_ms >= 3) && (scanInt_ms <= 10240)) {
        _connReqParam.scan_interval = (scanInt_ms*1000/625);
    }
}

void BLEConnect::setScanWindow(uint16_t scanWindow_ms) {
    if ((scanWindow_ms*1000/625) > _connReqParam.scan_interval) {
        printf("Scan window should be less than or equal to scan interval\r\n");
        return;
    }
    if((scanWindow_ms >= 3) && (scanWindow_ms <= 10240)) {
        _connReqParam.scan_window = (scanWindow_ms*1000/625);
    }
}

void BLEConnect::setConnInterval(uint16_t min_ms, uint16_t max_ms) {
    if ((min_ms < 8) || (min_ms > 4000) || (max_ms < 8) || (max_ms > 4000)) {
        printf("Error: valid conn interval values range from 8ms to 4000ms \n");
        return;
    }
    if (min_ms > max_ms) {
        printf("Error: ConnInterval min_ms must be less than or equal to max_ms \n");
        return;
    }
    _connReqParam.conn_interval_min = (uint16_t)(min_ms/1.25);
    _connReqParam.conn_interval_max = (uint16_t)(max_ms/1.25);
    _connReqParam.ce_len_min = 2 * (_connReqParam.conn_interval_min - 1);
    _connReqParam.ce_len_max = 2 * (_connReqParam.conn_interval_max - 1);
}

void BLEConnect::setConnLatency(uint16_t latency) {
    if (latency > 0x01F3) {
        printf("Error: valid conn latency values range from 0 to 499 \n");
        return;
    }
    _connReqParam.conn_latency = latency;
}

void BLEConnect::setConnTimeout(uint16_t timeout_ms) {
    if ((timeout_ms < 100) || (timeout_ms > 32000)) {
        printf("Error: valid conn interval values range from 100ms to 32000ms \n");
        return;
    }
    _connReqParam.supv_tout = timeout_ms/10;
}

void BLEConnect::updateConnParams(uint8_t conn_id) {
    le_update_conn_param(conn_id,
                         _connReqParam.conn_interval_min,
                         _connReqParam.conn_interval_max,
                         _connReqParam.conn_latency,
                         _connReqParam.supv_tout,
                         _connReqParam.ce_len_min,
                         _connReqParam.ce_len_max);
}

bool BLEConnect::getConnInfo(uint8_t connId, T_GAP_CONN_INFO *pConnInfo) {
    return le_get_conn_info(connId, pConnInfo);
}

bool BLEConnect::getConnAddr(uint8_t connId, uint8_t* addr, uint8_t* addrType) {
    return le_get_conn_addr(connId, addr, addrType);
}

int8_t BLEConnect::getConnId(char* btAddr, uint8_t addrType) {
    uint8_t connId = 0;
    BLEAddr destAddr(btAddr);

    if (le_get_conn_id(destAddr.data(), addrType, &connId)) {
        return connId;
    } else {
        return -1;
    }
}

int8_t BLEConnect::getConnId(uint8_t* btAddr, uint8_t addrType) {
    uint8_t connId = 0;

    if (le_get_conn_id(btAddr, addrType, &connId)) {
        return connId;
    } else {
        return -1;
    }
}

int8_t BLEConnect::getConnId(BLEAdvertData targetDevice) {
    uint8_t connId = 0;
    uint8_t* btAddr = targetDevice.getAddr().data();
    uint8_t addrType = targetDevice.getAddrType();

    if (le_get_conn_id(btAddr, addrType, &connId)) {
        return connId;
    } else {
        return -1;
    }
}
