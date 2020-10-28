#include "BLEAdvertData.h"
#include "BLEDevice.h"

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

BLEAdvertData::BLEAdvertData() {
}

void BLEAdvertData::clear() {
    memset(_data, 0, sizeof(_data));
    _dataSize = 0;
    _rssi = 0;
    _flags = 0;
    _serviceCount = 0;
    _devName = String("");
    _txPower = 0;
    _devAppearance = 0;
    _manufacturer = 0;
    _manufacturerDataLength = 0;
}

void BLEAdvertData::addData(const uint8_t* data, uint8_t size) {
    if ((31 - _dataSize) < size) {
        printf("Insufficient space in advertising data packet\r\n");
        return;
    }
    int i;
    for (i = 0; i < size; i++) {
        _data[_dataSize] = data[i];
        _dataSize++;
    }
}

// Valid flags for advertising
// GAP_ADTYPE_FLAGS_LIMITED                            0x01 //!< Discovery Mode: LE Limited Discoverable Mode
// GAP_ADTYPE_FLAGS_GENERAL                            0x02 //!< Discovery Mode: LE General Discoverable Mode
// GAP_ADTYPE_FLAGS_BREDR_NOT_SUPPORTED                0x04 //!< Discovery Mode: BR/EDR Not Supported
// GAP_ADTYPE_FLAGS_SIMULTANEOUS_LE_BREDR_CONTROLLER   0x08 //!< Discovery Mode: Simultaneous LE and BR/EDR Controller Supported
// GAP_ADTYPE_FLAGS_SIMULTANEOUS_LE_BREDR_HOST         0x10 //!< Discovery Mode: Simultaneous LE and BR/EDR Host Supported
uint8_t BLEAdvertData::addFlags(uint8_t flags) {
    uint8_t data[3] = {2, GAP_ADTYPE_FLAGS, flags};
    addData(data, 3);
    return _dataSize;
}

uint8_t BLEAdvertData::addPartialServices(BLEUUID uuid) {
    _serviceList[_serviceCount++] = (uuid);
    switch (uuid.length()) {
        case 2: {
            uint8_t data[4] = {3, GAP_ADTYPE_16BIT_MORE};
            memcpy(&(data[2]), uuid.dataNative(), 2);
            addData(data, 4);
            break;
        }
        case 4: {
            uint8_t data[6] = {5, GAP_ADTYPE_32BIT_MORE};
            memcpy(&(data[2]), uuid.dataNative(), 4);
            addData(data, 6);
            break;
        }
        case 16: {
            uint8_t data[18] = {17, GAP_ADTYPE_128BIT_MORE};
            memcpy(&(data[2]), uuid.dataNative(), 16);
            addData(data, 18);
            break;
        }
        default:
            break;
    }
    return _dataSize;
}

uint8_t BLEAdvertData::addCompleteServices(BLEUUID uuid) {
    _serviceList[_serviceCount++] = (uuid);
    switch (uuid.length()) {
        case 2: {
            uint8_t data[4] = {3, GAP_ADTYPE_16BIT_COMPLETE};
            memcpy(&(data[2]), uuid.dataNative(), 2);
            addData(data, 4);
            break;
        }
        case 4: {
            uint8_t data[6] = {5, GAP_ADTYPE_32BIT_COMPLETE};
            memcpy(&(data[2]), uuid.dataNative(), 4);
            addData(data, 6);
            break;
        }
        case 16: {
            uint8_t data[18] = {17, GAP_ADTYPE_128BIT_COMPLETE};
            memcpy(&(data[2]), uuid.dataNative(), 16);
            addData(data, 18);
            break;
        }
        default:
            break;
    }
    return _dataSize;
}

uint8_t BLEAdvertData::addCompleteServices(uint8_t uuidBitLength) {
        switch (uuidBitLength) {
        case 16: {
            uint8_t data[2] = {1, GAP_ADTYPE_16BIT_COMPLETE};
            addData(data, 2);
            break;
        }
        case 32: {
            uint8_t data[2] = {1, GAP_ADTYPE_32BIT_COMPLETE};
            addData(data, 2);
            break;
        }
        case 128: {
            uint8_t data[2] = {1, GAP_ADTYPE_128BIT_COMPLETE};
            addData(data, 2);
            break;
        }
        default:
            break;
    }
    return _dataSize;
}

// Refer to gap_le_types.h or Bluetooth specifications for full list of appearances
uint8_t BLEAdvertData::addAppearance(uint16_t appearance) {
    _devAppearance = appearance;
    uint8_t appHigh = (uint8_t)((appearance & 0xFF00)>> 8);
    uint8_t appLow = (uint8_t)(appearance & 0x00FF);
    uint8_t data[4] = {3, GAP_ADTYPE_APPEARANCE, appLow, appHigh};
    addData(data, 4);
    return _dataSize;
}

uint8_t BLEAdvertData::addShortName(const char* str) {
    _devName = String(str);
    uint8_t length = _devName.length();
    uint8_t data[(2 + length)] = {(uint8_t)(1 + length), GAP_ADTYPE_LOCAL_NAME_SHORT};
    memcpy(&(data[2]), str, length);
    addData(data, (2 + length));
    return _dataSize;
}

uint8_t BLEAdvertData::addCompleteName(const char* str) {
    _devName = String(str);
    uint8_t length = _devName.length();
    uint8_t data[(2 + length)] = {(uint8_t)(1 + length), GAP_ADTYPE_LOCAL_NAME_COMPLETE};
    memcpy(&(data[2]), str, length);
    addData(data, (2 + length));
    return _dataSize;
}

void BLEAdvertData::parseScanInfo(T_LE_CB_DATA *p_data) {
    T_LE_SCAN_INFO *scan_info = p_data->p_le_scan_info;
    clear();

    _advType = (scan_info->adv_type);
    _addrType = (scan_info->remote_addr_type);
    _address = (BLEAddr(scan_info->bd_addr));
    _rssi = (scan_info->rssi);
    memcpy(_data, scan_info->data, scan_info->data_len);
    _dataSize = scan_info->data_len;

    uint8_t buffer[32];
    uint8_t pos = 0;

    while (pos < scan_info->data_len) {
        // Length of the AD structure.
        uint8_t length = scan_info->data[pos++];    // length of following data field = length of advert data field + 1 (adtype)
        uint8_t type;

        if ((length > 0x01) && ((pos + length) <= 31)) {
            // Copy the AD Data to buffer.
            memcpy(buffer, scan_info->data + pos + 1, length - 1);
            // AD Type, one octet.
            type = scan_info->data[pos];

            //if (BTDEBUG) printf("parseScanInfo: AD Structure Info: AD type 0x%x, AD Data Length %d\r\n", type, (length - 1));

            switch (type) {
                case GAP_ADTYPE_FLAGS: {
                    // (0x01) -- LE Limited Discoverable Mode
                    // (0x02) -- LE General Discoverable Mode
                    // (0x04) -- BR/EDR Not Supported
                    // (0x08) -- Simultaneous LE and BR/EDR to Same Device Capable (Controller)
                    // (0x10) -- Simultaneous LE and BR/EDR to Same Device Capable (Host)
                    _flags = (scan_info->data[(pos + 1)]);
                    break;
                }

                case GAP_ADTYPE_16BIT_MORE:
                case GAP_ADTYPE_16BIT_COMPLETE: {
                    uint8_t *p_uuid = buffer;
                    uint8_t i = length - 1;

                    while (i >= 2) {
                        _serviceList[_serviceCount++] = (BLEUUID(p_uuid, 2));
                        p_uuid += 2;
                        i -= 2;
                    }
                    break;
                }

                case GAP_ADTYPE_32BIT_MORE:
                case GAP_ADTYPE_32BIT_COMPLETE: {
                    uint8_t *p_uuid = buffer;
                    uint8_t i = length - 1;

                    while (i >= 4) {
                        _serviceList[_serviceCount++] = (BLEUUID(p_uuid, 4));
                        p_uuid += 4;
                        i -= 4;
                    }
                    break;
                }

                case GAP_ADTYPE_128BIT_MORE:
                case GAP_ADTYPE_128BIT_COMPLETE: {
                    uint8_t *p_uuid = buffer;
                    _serviceList[_serviceCount++] = (BLEUUID(p_uuid, 16));
                    break;
                }

                case GAP_ADTYPE_LOCAL_NAME_SHORT:
                case GAP_ADTYPE_LOCAL_NAME_COMPLETE: {
                    buffer[length - 1] = '\0';
                    _devName = (String((char*)buffer));
                    break;
                }

                case GAP_ADTYPE_POWER_LEVEL: {
                    _txPower = (scan_info->data[(pos + 1)]);
                    break;
                }

                case GAP_ADTYPE_APPEARANCE: {
                    _devAppearance = (((uint16_t)buffer[1] << 8)|(buffer[0]));
                    break;
                }

                case GAP_ADTYPE_MANUFACTURER_SPECIFIC: {
                    uint8_t data_len = length - 3;      // adtype (-1), manufacturerID (-2)
                    _manufacturer = (((uint16_t)buffer[1] << 8)|(buffer[0]));
                    memcpy(_manufacturerData, (buffer + 2), data_len);
                    _manufacturerDataLength = data_len;
                    break;
                }

                default: {
                    uint8_t i = 0;
                    for (i = 0; i < (length - 1); i++) {
                        //if (BTDEBUG) printf("parseScanInfo: Unhandled Data = 0x%x\r\n", scan_info->data[(pos + i)]);
                    }
                    break;
                }
            }
        }
        pos += length;
    }
}

bool BLEAdvertData::hasFlags() {
    return (_flags > 0);
}

bool BLEAdvertData::hasUUID() {
    return (_serviceCount > 0);
}

bool BLEAdvertData::hasName() {
    return ((_devName.length()) > 0);
}

bool BLEAdvertData::hasManufacturer() {
    return (_manufacturerDataLength > 0);
}

T_GAP_ADV_EVT_TYPE BLEAdvertData::getAdvType() {
    return _advType;
}

T_GAP_REMOTE_ADDR_TYPE BLEAdvertData::getAddrType() {
    return _addrType;
}

BLEAddr BLEAdvertData::getAddr() {
    return _address;
}

int8_t BLEAdvertData::getRSSI() {
    return _rssi;
}

uint8_t BLEAdvertData::getFlags() {
    return _flags;
}

uint8_t BLEAdvertData::getServiceCount() {
    return _serviceCount;
}

BLEUUID* BLEAdvertData::getServiceList() {
    return _serviceList;
}

String BLEAdvertData::getName() {
    return _devName;
}

int8_t BLEAdvertData::getTxPower() {
    return _txPower;
}

uint16_t BLEAdvertData::getAppearance() {
    return _devAppearance;
}

uint16_t BLEAdvertData::getManufacturer() {
    return _manufacturer;
}

uint8_t BLEAdvertData::getManufacturerDataLength() {
    return _manufacturerDataLength;
}

uint8_t* BLEAdvertData::getManufacturerData() {
    return _manufacturerData;
}
