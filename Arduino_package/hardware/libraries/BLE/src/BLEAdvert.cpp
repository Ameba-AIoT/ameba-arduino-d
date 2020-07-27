#include "BLEAdvert.h"

#ifdef __cplusplus
extern "C" {
#endif

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

BLEAdvert::BLEAdvert() {
}

void BLEAdvert::setAdvType(uint8_t advType) {
    if (advType <= GAP_ADTYPE_ADV_LDC_DIRECT_IND) {
        _advEvtType = advType;
    }
}

void BLEAdvert::setMinInterval(uint16_t minInt_ms) {
    if ((minInt_ms >= 20) && (minInt_ms <= 10240)) {
        _advIntMin = (minInt_ms*1000/625);
    }
}

void BLEAdvert::setMaxInterval(uint16_t maxInt_ms) {
    if ((maxInt_ms >= 20) && (maxInt_ms <= 10240)) {
        _advIntMax = (maxInt_ms*1000/625);
    }
}

void BLEAdvert::setAdvData(BLEAdvertData adData) {
    memcpy(_advData, adData._data, adData._dataSize);
    _advDataSize = adData._dataSize;
}

void BLEAdvert::setScanRspData(BLEAdvertData adData) {
    memcpy(_scanRspData, adData._data, adData._dataSize);
    _scanRspDataSize = adData._dataSize;
}

void BLEAdvert::setAdvData(uint8_t* pdata, uint8_t size) {
    if (size <= 31) {
        memcpy(_advData, pdata, size);
        _advDataSize = size;
    }
}

void BLEAdvert::setScanRspData(uint8_t* pdata, uint8_t size) {
    if (size <= 31) {
        memcpy(_scanRspData, pdata, size);
        _scanRspDataSize = size;
    }
}

void BLEAdvert::updateAdvertParams() {
    le_adv_set_param(GAP_PARAM_ADV_EVENT_TYPE, sizeof(_advEvtType), &(_advEvtType));
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR_TYPE, sizeof(_advDirectType), &(_advDirectType));
    le_adv_set_param(GAP_PARAM_ADV_DIRECT_ADDR, sizeof(_advDirectAddr), (_advDirectAddr));
    le_adv_set_param(GAP_PARAM_ADV_CHANNEL_MAP, sizeof(_advChannMap), &(_advChannMap));
    le_adv_set_param(GAP_PARAM_ADV_FILTER_POLICY, sizeof(_advFilterPolicy), &(_advFilterPolicy));
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MIN, sizeof(_advIntMin), &(_advIntMin));
    le_adv_set_param(GAP_PARAM_ADV_INTERVAL_MAX, sizeof(_advIntMax), &(_advIntMax));
    le_adv_set_param(GAP_PARAM_ADV_DATA, _advDataSize, _advData);
    le_adv_set_param(GAP_PARAM_SCAN_RSP_DATA, _scanRspDataSize, _scanRspData);

    le_set_gap_param(GAP_PARAM_SLAVE_INIT_GATT_MTU_REQ, sizeof(_slaveInitMtuReq), &_slaveInitMtuReq);
}

// direct methods to start and stop advertising
// you should not be using these methods directly to start the BLE device stack, without first registering the callbacks and handlers. 
// BLEDevice::beginPeripheral() will start advertising by default
// possible uses for these are to stop advertising to update advertising parameters and restart advertising
void BLEAdvert::startAdv() {
    le_adv_start();
}

void BLEAdvert::stopAdv() {
    le_adv_stop();
}