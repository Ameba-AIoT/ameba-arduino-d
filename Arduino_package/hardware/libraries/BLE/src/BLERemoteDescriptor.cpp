#include "BLERemoteDescriptor.h"
#include "BLEClient.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "profile_client.h"

#ifdef __cplusplus
}
#endif

//------------- Configure -------------//

BLEUUID BLERemoteDescriptor::getUUID() {
    return _uuid;
}

void BLERemoteDescriptor::setBufferLen(uint16_t max_len) {
    if (max_len > DESC_VALUE_MAX_LEN) {
        printf ("Descriptor %s error: requested buffer size too large, maximum of %d \n", _uuid.str(), DESC_VALUE_MAX_LEN);
    } else {
        _data_buf = (uint8_t*) realloc(_data_buf, max_len*sizeof(uint8_t));
        if (_data_buf == NULL) {
            printf("Descriptor %s error: Not enough memory to set buffer length \n", _uuid.str());
            _data_buf_len = 0;
        } else {
            _data_buf_len = max_len;
        }
    }
}

uint16_t BLERemoteDescriptor::getBufferLen() {
    return _data_buf_len;
}

//--------- Read Char Value --------//

String BLERemoteDescriptor::readString() {
    char datastring[_data_buf_len + 1] = {0};
    getData((uint8_t*)datastring, sizeof(datastring));
    return String(datastring);
}

uint8_t BLERemoteDescriptor::readData8() {
    uint8_t num = 0;
    getData(&num, sizeof(num));
    return num;
}

uint16_t BLERemoteDescriptor::readData16() {
    uint16_t num = 0;
    getData((uint8_t*) &num, sizeof(num));
    return num;
}

uint32_t BLERemoteDescriptor::readData32() {
    uint32_t num = 0;
    getData((uint8_t*) &num, sizeof(num));
    return num;
}

//--------- Write Char Value --------//

bool BLERemoteDescriptor::writeString(String str) {
    return writeString(str.c_str());
}

bool BLERemoteDescriptor::writeString(const char* str) {
    return setData((uint8_t*) str, strlen(str));
}

bool BLERemoteDescriptor::writeData8(uint8_t num) {
    return setData((uint8_t*) &num, sizeof(num));
}

bool BLERemoteDescriptor::writeData16(uint16_t num) {
    return setData((uint8_t*) &num, sizeof(num));
}

bool BLERemoteDescriptor::writeData32(uint32_t num) {
    return setData((uint8_t*) &num, sizeof(num));
}

bool BLERemoteDescriptor::writeData32(int num) {
    return setData((uint8_t*) &num, sizeof(num));
}

//------------ Modify Char ------------//

bool BLERemoteDescriptor::setData(uint8_t* data, uint16_t datalen) {
    // Check if device is still connected
    if (!_pClient->connected()) {
        printf("Descriptor %s error: client not connected \n", _uuid.str());
        return false;
    }
    // Attempt to write
    if (client_attr_write(_pClient->getConnId(), _pClient->getClientId(),  GATT_WRITE_TYPE_REQ, _declarationHandle, datalen, data) == GAP_CAUSE_SUCCESS) {
        // Check for write callback semaphore indicating data write successful
        if (xSemaphoreTake(_writeSemaphore, CB_WAIT_TIMEOUT/portTICK_PERIOD_MS) != pdTRUE) {
            printf("Descriptor %s error: set data timeout \n", _uuid.str());
            return false;
        }
    } else {
        printf("Descriptor %s error: client_attr_write failed \n", _uuid.str());
        return false;
    }
    return true;
}

uint16_t BLERemoteDescriptor::getData(uint8_t* data, uint16_t datalen) {
    // Check if device is still connected
    if (!_pClient->connected()) {
        printf("Descriptor %s error: client not connected \n", _uuid.str());
        return 0;
    }
    // Attempt to read
    if (client_attr_read(_pClient->getConnId(), _pClient->getClientId(), _declarationHandle) == GAP_CAUSE_SUCCESS) {
        // Check for read callback semaphore indicating new data received
        if (xSemaphoreTake(_readSemaphore, CB_WAIT_TIMEOUT/portTICK_PERIOD_MS) != pdTRUE) {
            printf("Descriptor %s error: get data timeout \n", _uuid.str());
        } else {
            // Copy new data, up to the smaller data buffer size
            if (datalen > _data_buf_len) {
                memcpy(data, _data_buf, _data_buf_len);
                return _data_buf_len;
            } else {
                memcpy(data, _data_buf, datalen);
                return datalen;
            }
        }
    } else {
        printf("Descriptor %s error: client_attr_read failed \n", _uuid.str());
    }
    return 0;
}

//---------- Private Methods ----------//

BLERemoteDescriptor::BLERemoteDescriptor(BLEUUID uuid) {
    _uuid = uuid;
    _data_buf = (uint8_t*)malloc(_data_buf_len*sizeof(uint8_t));
    _readSemaphore = xSemaphoreCreateBinary();
    _writeSemaphore = xSemaphoreCreateBinary();
}

BLERemoteDescriptor::~BLERemoteDescriptor() {
    vSemaphoreDelete(_readSemaphore);
    vSemaphoreDelete(_writeSemaphore);
}

void BLERemoteDescriptor::clientReadResultCallbackDefault(uint8_t conn_id, uint16_t cause, uint16_t handle, uint16_t value_size, uint8_t *p_value) {
    if (handle == _declarationHandle) {
        if (cause == GAP_SUCCESS) {
            if (value_size > _data_buf_len) {
                printf("Descriptor %s error: Buffer size insufficient for data size of %d bytes \n", _uuid.str(), _data_buf_len);
            } else {
                memset(_data_buf, 0, _data_buf_len);
                memcpy(_data_buf, p_value, value_size);
                _data_len = value_size;
            }
            xSemaphoreGive(_readSemaphore);
        }
    } else {
        printf("Descriptor %s error: Handle %d mismatch read result callback \n", _uuid.str(), handle);
    }
    (void)conn_id;
}

void BLERemoteDescriptor::clientWriteResultCallbackDefault(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle, uint16_t cause, uint8_t credits) {
    if (handle == _declarationHandle) {
        if (cause == GAP_SUCCESS) {
            xSemaphoreGive(_writeSemaphore);
        }
    } else {
        printf("Descriptor %s error: Handle %d mismatch write result callback \n", _uuid.str(), handle);
    }
    (void)conn_id;
    (void)type;
    (void)credits;
}

