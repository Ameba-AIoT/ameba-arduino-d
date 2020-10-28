#include "BLECharacteristic.h"
#include "BLEService.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "gatt.h"
#include "profile_server.h"

#ifdef __cplusplus
}
#endif

BLECharacteristic::BLECharacteristic(BLEUUID uuid) {
    setUUID(uuid);
    _data_buf = (uint8_t*)malloc(_data_buf_len*sizeof(uint8_t));
}

BLECharacteristic::BLECharacteristic(const char* uuid) {
    setUUID(BLEUUID(uuid));
    _data_buf = (uint8_t*)malloc(_data_buf_len*sizeof(uint8_t));
}

BLECharacteristic::~BLECharacteristic() {
    free(_userDesc);
    free(_data_buf);
}

//------------- Configure -------------//

void BLECharacteristic::setUUID(BLEUUID uuid) {
    if ((uuid.length() == 2) || (uuid.length() == 16)) {
        _uuid = uuid;
    } else {
        printf("Characteristic error: Only 16bit & 128bit UUIDs are supported for characteristics \n");
    }

}

BLEUUID BLECharacteristic::getUUID() {
    return _uuid;
}

void BLECharacteristic::setBufferLen(uint16_t max_len) {
    if (max_len > CHAR_VALUE_MAX_LEN) {
        printf ("Characteristic %s error: requested buffer size too large, maximum of %d \n", _uuid.str(), CHAR_VALUE_MAX_LEN);
    } else {
        _data_buf = (uint8_t*) realloc(_data_buf, max_len*sizeof(uint8_t));
        if (_data_buf == NULL) {
            printf("Characteristic %s error: Not enough memory to set buffer length \n", _uuid.str());
            _data_buf_len = 0;
        } else {
            _data_buf_len = max_len;
        }
    }
}

uint16_t BLECharacteristic::getBufferLen() {
    return _data_buf_len;
}

void BLECharacteristic::setReadProperty(bool value) {
    if (value) {
        setProperties(getProperties() | GATT_CHAR_PROP_READ);
        _char_attr_permissions = (_char_attr_permissions | GATT_PERM_READ);
    } else {
        setProperties(getProperties() & ~GATT_CHAR_PROP_READ);
        _char_attr_permissions = (_char_attr_permissions & ~GATT_PERM_READ);
    }
}

void BLECharacteristic::setWriteProperty(bool value) {
    if (value) {
        setProperties(getProperties() | GATT_CHAR_PROP_WRITE);
        _char_attr_permissions = (_char_attr_permissions | GATT_PERM_WRITE);
    } else {
        setProperties(getProperties() & ~GATT_CHAR_PROP_WRITE);
        _char_attr_permissions = (_char_attr_permissions & ~GATT_PERM_WRITE);
    }
}

void BLECharacteristic::setNotifyProperty(bool value) {
    if (value) {
        setProperties(getProperties() | GATT_CHAR_PROP_NOTIFY);
        _includeCCCDescriptor = 1;
    } else {
        setProperties(getProperties() & ~GATT_CHAR_PROP_NOTIFY);
        if (getProperties() & GATT_CHAR_PROP_INDICATE) {    // Check if indicate still needs to use CCCD
            _includeCCCDescriptor = 1;
        } else {
            _includeCCCDescriptor = 0;
        }
    }
}

void BLECharacteristic::setIndicateProperty(bool value) {
    if (value) {
        setProperties(getProperties() | GATT_CHAR_PROP_INDICATE);
        _includeCCCDescriptor = 1;
    } else {
        setProperties(getProperties() & ~GATT_CHAR_PROP_INDICATE);
        if (getProperties() & GATT_CHAR_PROP_NOTIFY) {      // Check if notify still needs to use CCCD
            _includeCCCDescriptor = 1;
        } else {
            _includeCCCDescriptor = 0;
        }
    }
}

void BLECharacteristic::setProperties(uint8_t value) {
    _char_properties = value;
}

uint8_t BLECharacteristic::getProperties() {
    return _char_properties;
}

//--------- Read Char Value --------//

String BLECharacteristic::readString() {
    char datastring[_data_buf_len + 1] = {0};
    getData((uint8_t*)datastring, sizeof(datastring));
    return String(datastring);
}

uint8_t BLECharacteristic::readData8() {
    uint8_t num = 0;
    getData(&num, sizeof(num));
    return num;
}

uint16_t BLECharacteristic::readData16() {
    uint16_t num = 0;
    getData((uint8_t*) &num, sizeof(num));
    return num;
}

uint32_t BLECharacteristic::readData32() {
    uint32_t num = 0;
    getData((uint8_t*) &num, sizeof(num));
    return num;
}

//--------- Write Char Value --------//

bool BLECharacteristic::writeString(String str) {
    return writeString(str.c_str());
}

bool BLECharacteristic::writeString(const char* str) {
    return setData((uint8_t*) str, strlen(str));
}

bool BLECharacteristic::writeData8(uint8_t num) {
    return setData((uint8_t*) &num, sizeof(num));
}

bool BLECharacteristic::writeData16(uint16_t num) {
    return setData((uint8_t*) &num, sizeof(num));
}

bool BLECharacteristic::writeData32(uint32_t num) {
    return setData((uint8_t*) &num, sizeof(num));
}

bool BLECharacteristic::writeData32(int num) {
    return setData((uint8_t*) &num, sizeof(num));
}

//--------- Modify Char Value ---------//

bool BLECharacteristic::setData(uint8_t* data, uint16_t datalen) {
    if (datalen > _data_buf_len) {
        printf("Characteristic %s error: setData size too large, data buffer size set to %d \n", _uuid.str(), _data_buf_len);
        return false;
    }
    memset(_data_buf, 0, _data_buf_len);
    memcpy(_data_buf, data, datalen);
    _data_len = datalen;
    return true;
}

uint16_t BLECharacteristic::getData(uint8_t* data, uint16_t datalen) {
    if (datalen > _data_buf_len) {
        memcpy(data, _data_buf, _data_buf_len);
        return _data_buf_len;
    }
    memcpy(data, _data_buf, datalen);
    return datalen;
}

uint8_t* BLECharacteristic::getDataBuff() {
    return _data_buf;
}

uint16_t BLECharacteristic::getDataLen() {
    return _data_len;
}

void BLECharacteristic::notify(uint8_t conn_id) {
    if (!(getProperties() & GATT_CHAR_PROP_NOTIFY)) {
        printf("Characteristic %s error: Notification property not enabled \n", _uuid.str());
        return;
    }
    server_send_data(conn_id, _pService->getServiceID(), _handle_index, _data_buf, _data_len, GATT_PDU_TYPE_NOTIFICATION);
}

void BLECharacteristic::indicate(uint8_t conn_id) {
    if (!(getProperties() & GATT_CHAR_PROP_INDICATE)) {
        printf("Characteristic %s error: Indication property not enabled \n", _uuid.str());
        return;
    }
    server_send_data(conn_id, _pService->getServiceID(), _handle_index, _data_buf, _data_len, GATT_PDU_TYPE_INDICATION);
}

//------------- Descriptors -------------//

void BLECharacteristic::setUserDescriptor(const char* description) {
    _includeUserDescriptor = 1;
    _userDescSize = strlen(description);
    free(_userDesc);
    _userDesc = (char*)malloc((_userDescSize+1)*sizeof(char));
    memset(_userDesc, 0, ((_userDescSize+1)*sizeof(char)));
    strcpy(_userDesc, description);
}

void BLECharacteristic::setFormatDescriptor(uint8_t format, uint8_t exponent, uint16_t unit, uint16_t description) {
    _includeFormatDescriptor = 1;
    _fDescFormat = format;
    _fDescExponent = exponent;
    _fDescUnit = unit;
    _fDescDesc = description;
}

//------------- Callbacks -------------//

void BLECharacteristic::setReadCallback(void (*fCallback) (BLECharacteristic* chr, uint8_t conn_id)) {
    _pReadCB = fCallback;
}

void BLECharacteristic::setWriteCallback(void (*fCallback) (BLECharacteristic* chr, uint8_t conn_id)) {
    _pWriteCB = fCallback;
}

void BLECharacteristic::setCCCDCallback(void (*fCallback) (BLECharacteristic* chr, uint8_t conn_id, uint16_t ccc_bits)) {
    _pCccdCB = fCallback;
}

//---------- Private Methods ----------//

uint8_t BLECharacteristic::getHandleIndex() {
    if (_handle_index == 0) {
        printf ("Characteristic %s error: handle index unallocated \n", _uuid.str());
    }
    return _handle_index;
}

uint8_t BLECharacteristic::getAttrCount() {
    uint8_t _char_attr_count = 2;
    if (_includeCCCDescriptor) _char_attr_count += 1;
    if (_includeUserDescriptor) _char_attr_count += 1;
    if (_includeFormatDescriptor) _char_attr_count += 1;
    return _char_attr_count;
}

uint8_t BLECharacteristic::generateCharacteristicAttrTable(T_ATTRIB_APPL* attr_tbl, uint8_t index) {
    uint8_t _attr_count = 0;
    _attr_count += generateAttrCharacteristicDeclaration(attr_tbl, index);
    _handle_index = index + 1;

    _attr_count += generateAttrDescriptorDeclaration(attr_tbl, _attr_count + index);

    return _attr_count;
}

// Generate characteristic declaration attribute and characteristic value attribute
uint8_t BLECharacteristic::generateAttrCharacteristicDeclaration(T_ATTRIB_APPL* attr_tbl, uint8_t index) {
    attr_tbl[index].flags = (ATTRIB_FLAG_VALUE_INCL);
    attr_tbl[index].type_value[0] = LO_WORD(GATT_UUID_CHARACTERISTIC);
    attr_tbl[index].type_value[1] = HI_WORD(GATT_UUID_CHARACTERISTIC);
    attr_tbl[index].type_value[2] = _char_properties;
    attr_tbl[index].value_len = 1;
    attr_tbl[index].p_value_context = NULL;
    attr_tbl[index].permissions = GATT_PERM_READ;

    if (_uuid.length() == 2) {
        attr_tbl[index+1].flags = (ATTRIB_FLAG_VALUE_APPL);
        memcpy(attr_tbl[index+1].type_value, _uuid.dataNative(), 2);
    } else if (_uuid.length() == 16) {
        attr_tbl[index+1].flags = (ATTRIB_FLAG_VALUE_APPL | ATTRIB_FLAG_UUID_128BIT);
        memcpy(attr_tbl[index+1].type_value, _uuid.dataNative(), 16);
    }
    attr_tbl[index+1].value_len = 0;
    attr_tbl[index+1].p_value_context = NULL;
    attr_tbl[index+1].permissions = _char_attr_permissions;

    // Return number of attributes added
    return 2;
}

// Generate required characteristic descriptor attributes
uint8_t BLECharacteristic::generateAttrDescriptorDeclaration(T_ATTRIB_APPL* attr_tbl, uint8_t index) {
    uint8_t _desc_attr_count = 0;

    if (_includeCCCDescriptor) {
        _desc_attr_count += 1;
        attr_tbl[index].flags = (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_CCCD_APPL);
        attr_tbl[index].type_value[0] = LO_WORD(GATT_UUID_CHAR_CLIENT_CONFIG);
        attr_tbl[index].type_value[1] = HI_WORD(GATT_UUID_CHAR_CLIENT_CONFIG);
        attr_tbl[index].type_value[2] = LO_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT);
        attr_tbl[index].type_value[3] = HI_WORD(GATT_CLIENT_CHAR_CONFIG_DEFAULT);
        attr_tbl[index].value_len = 2;
        attr_tbl[index].p_value_context = NULL;
        attr_tbl[index].permissions = (GATT_PERM_READ | GATT_PERM_WRITE);
        index += 1;
    }
    if (_includeUserDescriptor) {
        _desc_attr_count += 1;
        attr_tbl[index].flags = (ATTRIB_FLAG_VOID | ATTRIB_FLAG_ASCII_Z);
        attr_tbl[index].type_value[0] = LO_WORD(GATT_UUID_CHAR_USER_DESCR);
        attr_tbl[index].type_value[1] = HI_WORD(GATT_UUID_CHAR_USER_DESCR);
        attr_tbl[index].value_len = (_userDescSize);
        attr_tbl[index].p_value_context = ((void *)_userDesc);
        attr_tbl[index].permissions = GATT_PERM_READ;
        index += 1;
    }
    if (_includeFormatDescriptor) {
        _desc_attr_count += 1;
        attr_tbl[index].flags = (ATTRIB_FLAG_VALUE_INCL);
        attr_tbl[index].type_value[0] = LO_WORD(GATT_UUID_CHAR_FORMAT);
        attr_tbl[index].type_value[1] = HI_WORD(GATT_UUID_CHAR_FORMAT);
        attr_tbl[index].type_value[2] = _fDescFormat;           // https://www.bluetooth.com/specifications/assigned-numbers/format-types/
        attr_tbl[index].type_value[3] = _fDescExponent;
        attr_tbl[index].type_value[4] = LO_WORD(_fDescUnit);    // https://www.bluetooth.com/specifications/assigned-numbers/units/
        attr_tbl[index].type_value[5] = HI_WORD(_fDescUnit);
        attr_tbl[index].type_value[6] = 0x01;                   // 0x01 = Bluetooth SIG namespace
        attr_tbl[index].type_value[7] = LO_WORD(_fDescDesc);    // https://www.bluetooth.com/specifications/assigned-numbers/gatt-namespace-descriptors/
        attr_tbl[index].type_value[8] = HI_WORD(_fDescDesc);
        attr_tbl[index].value_len = 7;
        attr_tbl[index].p_value_context = NULL;
        attr_tbl[index].permissions = GATT_PERM_READ;
        index += 1;
    }
    return _desc_attr_count;
}

T_APP_RESULT BLECharacteristic::charAttrReadCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                                        uint16_t offset, uint16_t *p_length, uint8_t **pp_value) {

    T_APP_RESULT cause  = APP_RESULT_ATTR_NOT_FOUND;
    if (attrib_index != _handle_index) {
        printf("Characteristic %s error: Char ID mismatch in read callback: actual %d received %d\n", _uuid.str(), _handle_index, attrib_index);
        return cause;
    }
    // Avoid unused variable warning
    (void)service_id;
    (void)offset;

    // Call registered callback function
    if (_pReadCB != nullptr) {
        _pReadCB(this, conn_id);
    }

    // Update value
    *pp_value = _data_buf;
    *p_length = _data_len;
    cause = APP_RESULT_SUCCESS;

    return cause;
}

T_APP_RESULT BLECharacteristic::charAttrWriteCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                                        T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                                        P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc) {

    T_APP_RESULT cause  = APP_RESULT_ATTR_NOT_FOUND;
    if (attrib_index != _handle_index) {
        printf("Characteristic %s error: Char ID mismatch in write callback: actual %d received %d\n", _uuid.str(), _handle_index, attrib_index);
        return cause;
    }
    // Avoid unused variable warning
    (void)service_id;
    (void)write_type;
    (void)p_write_ind_post_proc;

    p_write_ind_post_proc = NULL; // Write confirmation post process not used

    // Save updated value
    if (length > _data_buf_len) {
        printf("Characteristic %s error: Insufficient buffer allocated for write value, required: %d \n", _uuid.str(), length);
        cause = APP_RESULT_APP_ERR;
        return cause;
    } else {
        memset(_data_buf, 0, _data_buf_len);
        memcpy(_data_buf, p_value, length);
        _data_len = length;
        cause = APP_RESULT_SUCCESS;
    }

    // Call registered callback function
    if (_pWriteCB != nullptr) {
        _pWriteCB(this, conn_id);
    }

    return cause;
}

void BLECharacteristic::charCccdUpdateCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index, uint16_t ccc_bits) {
    if (attrib_index != (_handle_index + 1)) {    // +1 for Cccd attr index
        printf("Characteristic %s error: Char ID mismatch in CCCD callback: actual %d received %d\n", _uuid.str(), _handle_index, attrib_index);
    }
    // Avoid unused variable warning
    (void)service_id;

    // Need to manage connected device notificationi status? use BLEDevice app link table?

    // Call registered callback function
    // Compare cccbits with GATT_CLIENT_CHAR_CONFIG_NOTIFY or GATT_CLIENT_CHAR_CONFIG_INDICATE to determine new CCCD status
    if (_pCccdCB != nullptr) {
        _pCccdCB(this, conn_id, ccc_bits);
    }
}

