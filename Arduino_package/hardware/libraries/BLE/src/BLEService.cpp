#include "BLEService.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "gatt.h"

#ifdef __cplusplus
}
#endif

BLEService::BLEService(BLEUUID uuid) {
    setUUID(uuid);
}

BLEService::BLEService(const char* uuid) {
    setUUID(BLEUUID(uuid));
}

BLEService::~BLEService() {
    // Call delete for each characteristic pointer
    // Alternative: do not allow creation of new characteristics with method
    free(_service_attr_tbl);
}

void BLEService::setUUID(BLEUUID uuid) {
    if ((uuid.length() == 2) || (uuid.length() == 16)) {
        _uuid = uuid;
    } else {
        printf("Only 16bit & 128bit UUIDs are supported for services\n");
    }
}

BLEUUID BLEService::getUUID() {
    return _uuid;
}

void BLEService::addCharacteristic(BLECharacteristic& newChar) {
    if (_characteristicCount < (MAX_NUM_CHARS)) {
        _characteristicPtrList[_characteristicCount] = &newChar;
        _characteristicCount += 1;
        newChar._pService = this;
    } else {
        printf("Maximum number of characteristics per service reached \n");
    }
}

BLECharacteristic* BLEService::getCharacteristic(uint8_t charIndex) {
    if (charIndex < _characteristicCount) {
        return (_characteristicPtrList[charIndex]);
    } else {
        return NULL;
    }
}

void BLEService::setServiceID(T_SERVER_ID service_id) {
    _service_id = service_id;
}

T_SERVER_ID BLEService::getServiceID() {
    return _service_id;
}

// Calculate total number of attributes required by service and characteristics
uint8_t BLEService::getAttrCount() {
    _total_attr_count = 1;
    uint8_t i;
    for (i = 0; i < _characteristicCount; i++) {
        _total_attr_count += _characteristicPtrList[i]->getAttrCount();
    }
    return _total_attr_count;
}

T_ATTRIB_APPL* BLEService::generateServiceAttrTable() {
    // Calculate total number of attributes required by service and characteristics, allocate required memory to contain all attributes
    getAttrCount();
    free(_service_attr_tbl);
    _service_attr_tbl = (T_ATTRIB_APPL*)malloc(_total_attr_count * sizeof(T_ATTRIB_APPL));
    memset(_service_attr_tbl, 0, _total_attr_count * sizeof(T_ATTRIB_APPL));

    // Generate first attribute for service
    uint8_t _attr_count = 0;    // Pass in as handle?
    _attr_count += generateAttrServiceDeclaration(&(_service_attr_tbl[0]), _attr_count);

    // Pass in index to subsequent empty elements for characteristics to fill in attributes
    uint8_t i;
    for (i = 0; i < _characteristicCount; i++) {
        _attr_count += _characteristicPtrList[i]->generateCharacteristicAttrTable(&(_service_attr_tbl[0]), _attr_count);
    }
    return _service_attr_tbl;
}

// Generate basic default service declaration attribute
uint8_t BLEService::generateAttrServiceDeclaration(T_ATTRIB_APPL* attr_tbl, uint8_t index) {
    if (_uuid.length() == 2) {
        attr_tbl[index].flags = (ATTRIB_FLAG_VALUE_INCL | ATTRIB_FLAG_LE);
        attr_tbl[index].type_value[0] = LO_WORD(GATT_UUID_PRIMARY_SERVICE);
        attr_tbl[index].type_value[1] = HI_WORD(GATT_UUID_PRIMARY_SERVICE);
        attr_tbl[index].type_value[2] = ((_uuid.dataNative())[0]);     // figure out how to add 128bit uuid, required to keep UUID_PRIMARY_SERVICE?
        attr_tbl[index].type_value[3] = ((_uuid.dataNative())[1]);
        attr_tbl[index].value_len = UUID_16BIT_SIZE;
        attr_tbl[index].p_value_context = NULL;
    } else if (_uuid.length() == 16) {
        attr_tbl[index].flags = (ATTRIB_FLAG_VOID | ATTRIB_FLAG_LE);
        attr_tbl[index].type_value[0] = LO_WORD(GATT_UUID_PRIMARY_SERVICE);
        attr_tbl[index].type_value[1] = HI_WORD(GATT_UUID_PRIMARY_SERVICE);
        attr_tbl[index].value_len = UUID_128BIT_SIZE;
        attr_tbl[index].p_value_context = (void*) _uuid.dataNative();
    }
    attr_tbl[index].permissions = GATT_PERM_READ;

    // Return number of attributes added
    return 1;
}

void BLEService::printAttr() {
    uint8_t i;
    printf("Service attribute table for service %s with %d number of attributes:\n", _uuid.str(), _total_attr_count);
    for (i = 0; i < _total_attr_count; i++) {
        printf("Attribute num %d:\n", i);
        printf("Flags: \t %4x \n", _service_attr_tbl[i].flags);
        printf("Type Value: \t %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x %2x\n",
            _service_attr_tbl[i].type_value[0],
            _service_attr_tbl[i].type_value[1],
            _service_attr_tbl[i].type_value[2],
            _service_attr_tbl[i].type_value[3],
            _service_attr_tbl[i].type_value[4],
            _service_attr_tbl[i].type_value[5],
            _service_attr_tbl[i].type_value[6],
            _service_attr_tbl[i].type_value[7],
            _service_attr_tbl[i].type_value[8],
            _service_attr_tbl[i].type_value[9],
            _service_attr_tbl[i].type_value[10],
            _service_attr_tbl[i].type_value[11],
            _service_attr_tbl[i].type_value[12],
            _service_attr_tbl[i].type_value[13],
            _service_attr_tbl[i].type_value[14],
            _service_attr_tbl[i].type_value[15]);
        printf("Value Length: \t %4x \n", _service_attr_tbl[i].value_len);
        if (_service_attr_tbl[i].p_value_context != NULL) {
            printf("Context Value Pointer as string: %s \n", (char*)(_service_attr_tbl[i].p_value_context));
        } else {
            printf("Context Value Pointer: NULL \n");
        }
        printf("Permissions: \t %4x %4x \n", (uint16_t)((_service_attr_tbl[i].permissions & 0xFFFF0000) >> 16),
                                             (uint16_t)(_service_attr_tbl[i].permissions & 0xFFFF));
        printf("--------------------------------------------------------\n");
    }
}

T_APP_RESULT BLEService::serviceAttrReadCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                                                uint16_t offset, uint16_t *p_length, uint8_t **pp_value) {

    T_APP_RESULT cause  = APP_RESULT_ATTR_NOT_FOUND;
    if (service_id != _service_id) {
        printf("Service ID mismatch for service %s in read callback\n", getUUID().str());
        return cause;
    }
    uint8_t i;
    for (i = 0; i < _characteristicCount; i++) {
        if ((_characteristicPtrList[i]->getHandleIndex()) == attrib_index) {
            cause = _characteristicPtrList[i]->charAttrReadCallbackDefault(conn_id, service_id, attrib_index, offset, p_length, pp_value);
            break;
        }
    }
    return cause;
}

T_APP_RESULT BLEService::serviceAttrWriteCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index,
                                                                T_WRITE_TYPE write_type, uint16_t length, uint8_t *p_value,
                                                                P_FUN_WRITE_IND_POST_PROC *p_write_ind_post_proc) {

    T_APP_RESULT cause  = APP_RESULT_ATTR_NOT_FOUND;
    if (service_id != _service_id) {
        printf("Service ID mismatch for service %s in write callback\n", getUUID().str());
        return cause;
    }
    uint8_t i;
    for (i = 0; i < _characteristicCount; i++) {
        if ((_characteristicPtrList[i]->getHandleIndex()) == attrib_index) {
            cause = _characteristicPtrList[i]->charAttrWriteCallbackDefault(conn_id, service_id, attrib_index, write_type, length, p_value, p_write_ind_post_proc);
            break;
        }
    }
    return cause;
}

void BLEService::serviceCccdUpdateCallbackDefault(uint8_t conn_id, T_SERVER_ID service_id, uint16_t attrib_index, uint16_t ccc_bits) {
    if (service_id != _service_id) {
        printf("Service ID mismatch for service %s in CCCD callback\n", getUUID().str());
    }
    uint8_t i;
    for (i = 0; i < _characteristicCount; i++) {
        if ((_characteristicPtrList[i]->getHandleIndex()) == (attrib_index - 1)) {
            _characteristicPtrList[i]->charCccdUpdateCallbackDefault(conn_id, service_id, attrib_index, ccc_bits);
            break;
        }
    }
}

