#include "BLEClient.h"
#include "BLEDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "profile_client.h"

#ifdef __cplusplus
}
#endif

BLEClient::BLEClient() {
}

BLEClient::~BLEClient() {
    // Clean up and delete previously created services
    // Should have been deleted by disconnect callback first, once more just in case
    uint8_t i;
    for (i = 0; i < _serviceCount; i++) {
        delete _servicePtrList[i];
        _servicePtrList[i] = nullptr;
    }
}

bool BLEClient::connected() {
    return BLE.connected(_connId);
}

void BLEClient::discoverServices() {
    if (_serviceDiscState != DISC_IDLE) {
        printf("Conn ID %d client error: Services already discovered or failed previously \n", _connId);
        return;
    }
    if (!connected())  {
        printf("Conn ID %d client error: not connected \n", _connId);
        return;
    }
    if (client_all_primary_srv_discovery(_connId, _clientId) == GAP_CAUSE_SUCCESS) {
        _serviceDiscState = DISC_START;
    } else {
        printf("Conn ID %d client error: Service discovery request failed \n", _connId);
        return;
    }
}

bool BLEClient::discoveryDone() {
    return (_serviceDiscState == DISC_DONE);
}

void BLEClient::printServices() {
    if (_serviceDiscState != DISC_DONE) {
        printf("Conn ID %d client error: Discover services first \n", _connId);
        return;
    } else if (_serviceCount == 0) {
        printf("Conn ID %d client error: No services found to print \n", _connId);
        return;
    }
    printf("Device at conn ID %d has services: \n", _connId);
    BLERemoteService* pserv;
    BLERemoteCharacteristic* pchar;
    BLERemoteDescriptor* pdesc;
    for (uint8_t i = 0; i < _serviceCount; i++) {
        pserv = _servicePtrList[i];
        printf("Service UUID: %s\n", pserv->_uuid.str());
        printf("  Start handle: 0x%x, End handle: 0x%x\n", pserv->_handleStart, pserv->_handleEnd);
        for (uint8_t j = 0; j < (pserv->_characteristicCount); j++) {
            pchar = pserv->_characteristicPtrList[j];
            printf("\tCharacteristic UUID: %s, Properties: 0x%x\n", pchar->_uuid.str(), pchar->_properties);
            printf("\t  Declaration handle: 0x%x, Value handle: 0x%x\n", pchar->_declarationHandle, pchar->_valueHandle);
            for (uint8_t k = 0; k < (pchar->_descriptorCount); k++) {
                pdesc = pchar->_descriptorPtrList[k];
                printf("\t\tDescriptor UUID: %s\n", pdesc->_uuid.str());
                printf("\t\t  Declaration handle: 0x%x\n", pdesc->_declarationHandle);
            }
        }
    }
}

BLERemoteService* BLEClient::getService(const char* uuid) {
    return getService(BLEUUID(uuid));
}

BLERemoteService* BLEClient::getService(BLEUUID uuid) {
    if (_serviceDiscState != DISC_DONE) {
        printf("Conn ID %d client error: Discover services first \n", _connId);
        return nullptr;
    } else if (_serviceCount == 0) {
        printf("Conn ID %d client error: No services found \n", _connId);
        return nullptr;
    }
    for (uint8_t i = 0; i < _serviceCount; i++) {
        if (_servicePtrList[i]->_uuid == uuid) {
            return _servicePtrList[i];
        }
    }
    // Service not found, return nullptr
    return nullptr; 
}

uint8_t BLEClient::getConnId() {
    return _connId;
}

T_CLIENT_ID BLEClient::getClientId() {
    return _clientId;
}

void BLEClient::setDisconnectCallback(void (*fCallback) (BLEClient* client)) {
    _pDiscCB = fCallback;
}

void BLEClient::clientDiscoverStateCallbackDefault(uint8_t conn_id, T_DISCOVERY_STATE discovery_state) {
    if (conn_id != _connId) {
        printf("Conn ID %d client error: Conn ID mismatch in discover state callback \n", _connId);
        return;
    }
    // Perform further discovery steps after each stage is complete
    if (_serviceDiscState == DISC_START) {
        switch (discovery_state) {
            //case DISC_STATE_IDLE:
                //break;
            //case DISC_STATE_SRV:
                //break;
            case DISC_STATE_SRV_DONE:
                if (_serviceCount > 0) {
                    uint16_t start_handle = 0x0001;
                    uint16_t end_handle = 0xffff;
                    if (client_all_char_discovery(_connId, _clientId, start_handle, end_handle) != GAP_CAUSE_SUCCESS) {
                        printf("Conn ID %d client error: all_char_discovery failed \n", _connId);
                    }
                }
                break;
            //case DISC_STATE_RELATION:
                //break;
            //case DISC_STATE_RELATION_DONE:
                //break;
            //case DISC_STATE_CHAR:
                //break;
            case DISC_STATE_CHAR_DONE:
                if (_serviceCount > 0) {
                    uint16_t start_handle = 0x0001;
                    uint16_t end_handle = 0xffff;
                    if (client_all_char_descriptor_discovery(_connId, _clientId, start_handle, end_handle) != GAP_CAUSE_SUCCESS) {
                        printf("Conn ID %d client error: all_char_descriptor_discovery failed \n", _connId);
                    }
                }
                break;
            //case DISC_STATE_CHAR_UUID16_DONE:
                //break;
            //case DISC_STATE_CHAR_UUID128_DONE:
                //break;
            //case DISC_STATE_CHAR_DESCRIPTOR:
                //break;
            case DISC_STATE_CHAR_DESCRIPTOR_DONE:
                _serviceDiscState = DISC_DONE;
                break;
            case DISC_STATE_FAILED:
                printf("Conn ID %d client error: Discovery failed \n", _connId);
                _serviceDiscState = DISC_FAILED;
                break;
            default:
                printf("Conn ID %d client error: Unhandled discovery state %d \n", _connId, discovery_state);
                break;
        }
    }
}

void BLEClient::clientDiscoverResultCallbackDefault(uint8_t conn_id, T_DISCOVERY_RESULT_TYPE result_type, T_DISCOVERY_RESULT_DATA result_data) {
    if (conn_id != _connId) {
        printf("Conn ID %d client error: Conn ID mismatch in discover result callback \n", _connId);
        return;
    }
    // Create corresponding services and clients as discovered
    if (_serviceDiscState == DISC_START) {
        uint16_t handle_start, handle_end, handle_decl, handle_value, properties;
        BLEUUID uuid;
        switch (result_type) {
            case DISC_RESULT_ALL_SRV_UUID16:
            case DISC_RESULT_ALL_SRV_UUID128:
                if (result_type == DISC_RESULT_ALL_SRV_UUID16) {
                    handle_start = result_data.p_srv_uuid16_disc_data->att_handle;
                    handle_end = result_data.p_srv_uuid16_disc_data->end_group_handle;
                    uuid = BLEUUID((uint8_t*)(&(result_data.p_srv_uuid16_disc_data->uuid16)), 2);
                } else if (result_type == DISC_RESULT_ALL_SRV_UUID128) {
                    handle_start = result_data.p_srv_uuid128_disc_data->att_handle;
                    handle_end = result_data.p_srv_uuid128_disc_data->end_group_handle;
                    uuid = BLEUUID(result_data.p_srv_uuid128_disc_data->uuid128, 16);
                }
                if (_serviceCount >= MAX_REMOTE_SERVICE_COUNT) {
                    printf("Conn ID %d client error: Maximum number of remote services per client reached \n", _connId);
                } else {
                    // Create new service corresponding to discovered data, add to list
                    _servicePtrList[_serviceCount] = new BLERemoteService(uuid);
                    _servicePtrList[_serviceCount]->_handleStart = handle_start;
                    _servicePtrList[_serviceCount]->_handleEnd = handle_end;
                    _servicePtrList[_serviceCount]->_pClient = this;
                    _serviceCount += 1;
                }
                break;

            //case DISC_RESULT_SRV_DATA:
                //break;

            case DISC_RESULT_CHAR_UUID16:
            case DISC_RESULT_CHAR_UUID128:
                if (result_type == DISC_RESULT_CHAR_UUID16) {
                    handle_decl = result_data.p_char_uuid16_disc_data->decl_handle;
                    handle_value = result_data.p_char_uuid16_disc_data->value_handle;
                    properties = result_data.p_char_uuid16_disc_data->properties;
                    uuid = BLEUUID((uint8_t*)(&(result_data.p_char_uuid16_disc_data->uuid16)), 2);
                } else if (result_type == DISC_RESULT_CHAR_UUID128) {
                    handle_decl = result_data.p_char_uuid128_disc_data->decl_handle;
                    handle_value = result_data.p_char_uuid128_disc_data->value_handle;
                    properties = result_data.p_char_uuid128_disc_data->properties;
                    uuid = BLEUUID(result_data.p_char_uuid128_disc_data->uuid128, 16);
                }
                for (uint8_t i = 0; i < _serviceCount; i++) {
                    // Find corresponding service for characteristic, create and add
                    if ((handle_decl > _servicePtrList[i]->_handleStart) && (handle_decl < _servicePtrList[i]->_handleEnd)) {
                        BLERemoteCharacteristic* newChar = new BLERemoteCharacteristic(uuid);
                        newChar->_pClient = this;
                        newChar->_declarationHandle = handle_decl;
                        newChar->_valueHandle = handle_value;
                        newChar->_properties = properties;
                        // Try to add characteristic to service, delete if service full
                        // Core 5.2 spec: Characteristic definitions appear on the server within a service definition in an order based on Attribute Handle
                        // API may not return results in order of declaration handle, service needs to sort based on handle
                        if (!(_servicePtrList[i]->addCharacteristic(newChar))) {
                            delete newChar;
                        }
                        break;
                    }
                }
                break;

            case DISC_RESULT_CHAR_DESC_UUID16:
            case DISC_RESULT_CHAR_DESC_UUID128:
                if (result_type == DISC_RESULT_CHAR_DESC_UUID16) {
                    handle_decl = result_data.p_char_desc_uuid16_disc_data->handle;
                    uuid = BLEUUID((uint8_t*)(&(result_data.p_char_desc_uuid16_disc_data->uuid16)), 2);
                } else if (result_type == DISC_RESULT_CHAR_DESC_UUID128) {
                    handle_decl = result_data.p_char_desc_uuid128_disc_data->handle;
                    uuid = BLEUUID(result_data.p_char_desc_uuid128_disc_data->uuid128, 16);
                }
                // Descriptor discovery returns duplicate entries for previously found services and characteristics
                // Find corresponding service for descriptor
                for (uint8_t i = 0; i < _serviceCount; i++) {
                    // Filter away duplicate service handles
                    if (handle_decl == _servicePtrList[i]->_handleStart) {
                        break;
                    }
                    if ((handle_decl > _servicePtrList[i]->_handleStart) && (handle_decl <= _servicePtrList[i]->_handleEnd)) {
                        BLERemoteService* pserv = _servicePtrList[i];
                        BLERemoteCharacteristic* pchar = nullptr;
                        uint8_t charCount = pserv->_characteristicCount;
                        // Find corresponding characteristic for descriptor
                        // Core 5.2 spec: The Characteristic Value declaration shall exist immediately following the characteristic declaration.
                        // Core 5.2 spec: Any optional characteristic descriptor declarations are placed after the Characteristic Value declaration.
                        if (charCount == 0) {
                            printf("Conn ID %d client error: Service %s has no characteristics to add descriptor to \n", _connId, pserv->_uuid.str());
                            break;
                        } else {
                            pchar = pserv->_characteristicPtrList[charCount - 1];
                            // owner char handle < descriptor handle < next char handle, requires service has sorted list of characteristics
                            for (uint8_t k = 0; k < (charCount - 1); k++) {
                                if ((handle_decl >= (pserv->_characteristicPtrList[k]->_declarationHandle)) && (handle_decl < (pserv->_characteristicPtrList[k+1]->_declarationHandle))) {
                                    pchar = pserv->_characteristicPtrList[k];
                                    break;
                                }
                            }
                        }
                        if ((handle_decl != pchar->_declarationHandle) && (handle_decl != pchar->_valueHandle)) {
                            // Try to add descriptor to characteristic, delete if full
                            BLERemoteDescriptor* newDesc = new BLERemoteDescriptor(uuid);
                            newDesc->_pClient = this;
                            newDesc->_declarationHandle = handle_decl;
                            if (!(pchar->addDescriptor(newDesc))) {
                                delete newDesc;
                            }
                        }
                        break;
                    }
                }
                break;

            //case DISC_RESULT_RELATION_UUID16:
                //break;
            //case DISC_RESULT_RELATION_UUID128:
                //break;
            //case DISC_RESULT_BY_UUID16_CHAR:
                //break;
            //case DISC_RESULT_BY_UUID128_CHAR:
                //break;
            default:
                printf("Conn ID %d client error: Unhandled discovery result %d \n", _connId, result_type);
                break;
        }
    }
}

void BLEClient::clientReadResultCallbackDefault(uint8_t conn_id, uint16_t cause, uint16_t handle, uint16_t value_size, uint8_t *p_value) {
    if (conn_id != _connId) {
        printf("Conn ID %d client error: Conn ID mismatch read result callback \n", _connId);
        return;
    }
    // Check handle and pass into corresponding service
    for (uint8_t i = 0; i < _serviceCount; i++) {
        if ((handle > _servicePtrList[i]->_handleStart) && (handle <= _servicePtrList[i]->_handleEnd)) {
            _servicePtrList[i]->clientReadResultCallbackDefault(conn_id, cause, handle, value_size, p_value);
            break;
        }
    }
}

void BLEClient::clientWriteResultCallbackDefault(uint8_t conn_id, T_GATT_WRITE_TYPE type, uint16_t handle, uint16_t cause, uint8_t credits) {
    if (conn_id != _connId) {
        printf("Conn ID %d client error: Conn ID mismatch in write result callback \n", _connId);
        return;
    }
    // Check handle and pass into corresponding service
    for (uint8_t i = 0; i < _serviceCount; i++) {
        if ((handle > _servicePtrList[i]->_handleStart) && (handle <= _servicePtrList[i]->_handleEnd)) {
            _servicePtrList[i]->clientWriteResultCallbackDefault(conn_id, type, handle, cause, credits);
            break;
        }
    }
}

T_APP_RESULT BLEClient::clientNotifyIndicateCallbackDefault(uint8_t conn_id, bool notify, uint16_t handle, uint16_t value_size, uint8_t *p_value) {
    T_APP_RESULT app_result = APP_RESULT_APP_ERR;
    if (conn_id != _connId) {
        printf("Conn ID %d client error: Conn ID mismatch in notify/indicate callback \n", _connId);
        return app_result;
    }
    // Check handle and pass into corresponding service
    for (uint8_t i = 0; i < _serviceCount; i++) {
        if ((handle > _servicePtrList[i]->_handleStart) && (handle <= _servicePtrList[i]->_handleEnd)) {
            app_result = _servicePtrList[i]->clientNotifyIndicateCallbackDefault(conn_id, notify, handle, value_size, p_value);
            break;
        }
    }
    return app_result;
}

void BLEClient::clientDisconnectCallbackDefault(uint8_t conn_id) {
    if (conn_id != _connId) {
        printf("Conn ID %d client error: Conn ID mismatch in disconnect callback \n", _connId);
        return;
    }
    // Alert application about disconnection
    if (_pDiscCB != nullptr) {
        _pDiscCB(this);
    }
    // Delete previously created services
    uint8_t i;
    for (i = 0; i < _serviceCount; i++) {
        delete _servicePtrList[i];
        _servicePtrList[i] = nullptr;
    }
}

