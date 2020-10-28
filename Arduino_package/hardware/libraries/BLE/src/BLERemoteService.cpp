#include "BLERemoteService.h"

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

BLERemoteService::BLERemoteService(BLEUUID uuid) {
    _uuid = uuid;
}

BLERemoteService::~BLERemoteService() {
    uint8_t i;
    for (i = 0; i < _characteristicCount; i++) {
        delete _characteristicPtrList[i];
        _characteristicPtrList[i] = nullptr;
    }
}

BLEUUID BLERemoteService::getUUID() {
    return _uuid;
}

BLERemoteCharacteristic* BLERemoteService::getCharacteristic(const char* uuid) {
    return getCharacteristic(BLEUUID(uuid));
}

BLERemoteCharacteristic* BLERemoteService::getCharacteristic(BLEUUID uuid) {
    if (_characteristicCount == 0) {
        printf("Service %s error: No characteristics found \n", _uuid.str());
        return nullptr;
    }
    uint8_t i;
    for (i = 0; i < _characteristicCount; i++) {
        if (_characteristicPtrList[i]->getUUID() == uuid) {
            return _characteristicPtrList[i];
        }
    }
    return nullptr; // Target service not found, return nullptr
}

bool BLERemoteService::addCharacteristic(BLERemoteCharacteristic* newChar) {
    if (_characteristicCount >= MAX_NUM_CHARS) {
        printf("Service %s error: Maximum number of characteristics per service reached \n", _uuid.str());
        return false;
    }
    if (_characteristicCount == 0) {
        _characteristicPtrList[_characteristicCount] = newChar;
        _characteristicCount += 1;
    } else {
        BLERemoteCharacteristic* tempChar;
        // Add new characteristic into list, sorted based on ascending declaration handle
        for (uint8_t i = 0; i < _characteristicCount; i++) {
            if ((newChar->_declarationHandle) < (_characteristicPtrList[i]->_declarationHandle)) {
                tempChar = _characteristicPtrList[i];
                _characteristicPtrList[i] = newChar;
                newChar = tempChar;
            }
        }
        _characteristicPtrList[_characteristicCount] = newChar;
        _characteristicCount += 1;
    }
    return true;
}

void BLERemoteService::clientReadResultCallbackDefault(uint8_t conn_id, uint16_t cause, uint16_t handle, uint16_t value_size, uint8_t *p_value) {
    if ((handle < _handleStart) || (handle > _handleEnd)) {
        printf("Service %s error: Handle %d mismatch read result callback \n", _uuid.str(), handle);
        return;
    }
    // Check handle and pass into corresponding characteristic
    if (_characteristicCount == 1) {
        _characteristicPtrList[0]->clientReadResultCallbackDefault(conn_id, cause, handle, value_size, p_value);
    } else {
        // Check if result handle falls between any characteristic handles, otherwise assign to last characteristic with largest handle
        BLERemoteCharacteristic* tempChar = _characteristicPtrList[_characteristicCount-1];
        for (uint8_t i = 0; i < _characteristicCount-1; i++) {
            if ((handle >= _characteristicPtrList[i]->_declarationHandle) && (handle < _characteristicPtrList[i+1]->_declarationHandle)) {
                tempChar = _characteristicPtrList[i];
            }
        }
        tempChar->clientReadResultCallbackDefault(conn_id, cause, handle, value_size, p_value);
    }
}

void BLERemoteService::clientWriteResultCallbackDefault(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle, uint16_t cause, uint8_t credits) {
    if ((handle < _handleStart) || (handle > _handleEnd)) {
        printf("Service %s error: Handle %d mismatch in write result callback \n", _uuid.str(), handle);
        return;
    }
    // Check handle and pass into corresponding characteristic
    if (_characteristicCount == 1) {
        _characteristicPtrList[0]->clientWriteResultCallbackDefault(conn_id, type, handle, cause, credits);
    } else {
        // Check if result handle falls between any characteristic handles, otherwise assign to last characteristic with largest handle
        BLERemoteCharacteristic* tempChar = _characteristicPtrList[_characteristicCount-1];
        for (uint8_t i = 0; i < _characteristicCount-1; i++) {
            if ((handle >= _characteristicPtrList[i]->_declarationHandle) && (handle < _characteristicPtrList[i+1]->_declarationHandle)) {
                tempChar = _characteristicPtrList[i];
            }
        }
        tempChar->clientWriteResultCallbackDefault(conn_id, type, handle, cause, credits);
    }
}

T_APP_RESULT BLERemoteService::clientNotifyIndicateCallbackDefault(uint8_t conn_id, bool notify, uint16_t handle, uint16_t value_size, uint8_t *p_value) {
    T_APP_RESULT app_result = APP_RESULT_APP_ERR;
    if ((handle < _handleStart) || (handle > _handleEnd)) {
        printf("Service %s error: Handle %d mismatch in notify/indicate callback \n", _uuid.str(), handle);
        return app_result;
    }
    // Check handle and pass into corresponding characteristic
    if (_characteristicCount == 1) {
        app_result = _characteristicPtrList[0]->clientNotifyIndicateCallbackDefault(conn_id, notify, handle, value_size, p_value);
    } else {
        // Check if result handle falls between any characteristic handles, otherwise assign to last characteristic with largest handle
        BLERemoteCharacteristic* tempChar = _characteristicPtrList[_characteristicCount-1];
        for (uint8_t i = 0; i < _characteristicCount-1; i++) {
            if ((handle >= _characteristicPtrList[i]->_declarationHandle) && (handle < _characteristicPtrList[i+1]->_declarationHandle)) {
                tempChar = _characteristicPtrList[i];
            }
        }
        app_result = tempChar->clientNotifyIndicateCallbackDefault(conn_id, notify, handle, value_size, p_value);
    }
    return app_result;
}

