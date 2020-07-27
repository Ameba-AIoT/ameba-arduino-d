#include "BLEScan.h"
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

uint8_t BLEScan::_scanProcessing = 0;

BLEScan::BLEScan() {
    _scanProcessing = 0;
}

void BLEScan::updateScanParams() {
    le_scan_set_param(GAP_PARAM_SCAN_MODE, sizeof(_scanMode), &_scanMode);
    le_scan_set_param(GAP_PARAM_SCAN_INTERVAL, sizeof(_scanInterval), &_scanInterval);
    le_scan_set_param(GAP_PARAM_SCAN_WINDOW, sizeof(_scanWindow), &_scanWindow);
    le_scan_set_param(GAP_PARAM_SCAN_FILTER_POLICY, sizeof(_scanFilterPolicy), &_scanFilterPolicy);
    le_scan_set_param(GAP_PARAM_SCAN_FILTER_DUPLICATES, sizeof(_scanFilterDuplicate), &_scanFilterDuplicate);
}

void BLEScan::startScan(uint32_t scanDuration_ms) {
    startScan();
    vTaskDelay(scanDuration_ms / portTICK_RATE_MS);
    stopScan();
    vTaskDelay(100 / portTICK_RATE_MS);
}

void BLEScan::startScan() {
    T_GAP_CAUSE cause;
    if (_scanProcessing) {
        printf("Scan is processing, please stop it first\n\r");
    } else {
        _scanProcessing = 1;
        cause = le_scan_start();
        if (cause != GAP_CAUSE_SUCCESS) {
            printf("Scan error\n\r");
            _scanProcessing = 0;
        }
    }
}

void BLEScan::stopScan() {
    if (_scanProcessing) {
        le_scan_stop();
        _scanProcessing = 0;
    } else {
        printf("There is no scan\n\r");
    }
}

void BLEScan::setScanMode(uint8_t scanMode) {
    if (scanMode == GAP_SCAN_MODE_PASSIVE) {
        _scanMode = GAP_SCAN_MODE_PASSIVE;
    } else if (scanMode == GAP_SCAN_MODE_ACTIVE) {
        _scanMode = GAP_SCAN_MODE_ACTIVE;
    }
}

void BLEScan::setScanInterval(uint16_t scanInt_ms) {
    if ((scanInt_ms >= 3) && (scanInt_ms <= 10240)) {
        _scanInterval = (scanInt_ms*1000/625);
    }
}

void BLEScan::setScanWindow(uint16_t scanWindow_ms) {
    if ((scanWindow_ms * 1000 / 625) > _scanInterval) {
        printf("Scan window should be less than or equal to scan interval\r\n");
        return;
    }
    if ((scanWindow_ms >= 3) && (scanWindow_ms <= 10240)) {
        _scanWindow = (scanWindow_ms*1000/625);
    }
}

void BLEScan::setScanDuplicateFilter(bool dupeFilter) {
    if (dupeFilter) {
        _scanFilterDuplicate = GAP_SCAN_FILTER_DUPLICATE_ENABLE;
    } else {
        _scanFilterDuplicate = GAP_SCAN_FILTER_DUPLICATE_DISABLE;
    }
}

bool BLEScan::scanInProgress() {
    if (_scanProcessing) {
        return true;
    }
    return false;
}

void BLEScan::printScanInfo(T_LE_CB_DATA *p_data) {
    char adv_type[20];
    char remote_addr_type[10];
    T_LE_SCAN_INFO *scan_info = p_data->p_le_scan_info;

    sprintf(adv_type,
            "%s",
            (scan_info->adv_type ==GAP_ADV_EVT_TYPE_UNDIRECTED)? "CON_UNDIRECT":
            (scan_info->adv_type ==GAP_ADV_EVT_TYPE_DIRECTED)? "CON_DIRECT":
            (scan_info->adv_type ==GAP_ADV_EVT_TYPE_SCANNABLE)? "SCAN_UNDIRECT":
            (scan_info->adv_type ==GAP_ADV_EVT_TYPE_NON_CONNECTABLE)? "NON_CONNECTABLE":
            (scan_info->adv_type ==GAP_ADV_EVT_TYPE_SCAN_RSP)? "SCAN_RSP":"unknown");

    sprintf(remote_addr_type,
            "%s",
            (scan_info->remote_addr_type == GAP_REMOTE_ADDR_LE_PUBLIC)? "public":
            (scan_info->remote_addr_type == GAP_REMOTE_ADDR_LE_RANDOM)? "random":"unknown");

    printf("ADVType\t\t\t| AddrType\t| BT_Addr\t\t| rssi\n\r");
    printf("%s\t\t| %s\t| %02x:%02x:%02x:%02x:%02x:%02x\t| %d\n\r",
            adv_type,
            remote_addr_type,
            (scan_info->bd_addr)[5],
            (scan_info->bd_addr)[4],
            (scan_info->bd_addr)[3],
            (scan_info->bd_addr)[2],
            (scan_info->bd_addr)[1],
            (scan_info->bd_addr)[0],
            scan_info->rssi);

    uint8_t buffer[32];
    uint8_t pos = 0;

    while (pos < scan_info->data_len) {
        // Length of the AD structure.
        uint8_t length = scan_info->data[pos++];
        uint8_t type;

        if ((length > 0x01) && ((pos + length) <= 31)) {
            // Copy the AD Data to buffer.
            memcpy(buffer, scan_info->data + pos + 1, length - 1);
            // AD Type, one octet.
            type = scan_info->data[pos];

            if (BTDEBUG) {
                printf("printScanInfo: AD Structure Info: AD type 0x%x, AD Data Length %d\r\n", type, (length - 1));
            }

            switch (type) {
                case GAP_ADTYPE_FLAGS: {
                    // (0x01) -- LE Limited Discoverable Mode
                    // (0x02) -- LE General Discoverable Mode
                    // (0x04) -- BR/EDR Not Supported
                    // (0x08) -- Simultaneous LE and BR/EDR to Same Device Capable (Controller)
                    // (0x10) -- Simultaneous LE and BR/EDR to Same Device Capable (Host)
                    uint8_t flags = scan_info->data[(pos + 1)];
                    printf("GAP_ADTYPE_FLAGS: 0x%x\n\r", flags);
                    break;
                }

                case GAP_ADTYPE_16BIT_MORE:
                case GAP_ADTYPE_16BIT_COMPLETE: {
                    uint16_t *p_uuid = (uint16_t *)(buffer);
                    uint8_t i = length - 1;

                    while (i >= 2) {
                        printf("GAP_ADTYPE_16BIT_XXX: 0x%x\n\r", *p_uuid);
                        p_uuid ++;
                        i -= 2;
                    }
                    break;
                }

                case GAP_ADTYPE_32BIT_MORE:
                case GAP_ADTYPE_32BIT_COMPLETE: {
                    uint32_t *p_uuid = (uint32_t *)(buffer);
                    uint8_t    i     = length - 1;

                    while (i >= 4) {
                        printf("GAP_ADTYPE_32BIT_XXX: 0x%x\n\r", (unsigned int)*p_uuid);
                        p_uuid ++;
                        i -= 4;
                    }
                    break;
                }

                case GAP_ADTYPE_128BIT_MORE:
                case GAP_ADTYPE_128BIT_COMPLETE: {
                    uint32_t *p_uuid = (uint32_t *)(buffer);
                    printf("GAP_ADTYPE_128BIT_XXX: 0x%8x%8x%8x%8x\n\r", (unsigned int)p_uuid[3], (unsigned int)p_uuid[2], (unsigned int)p_uuid[1], (unsigned int)p_uuid[0]);
                    break;
                }

                case GAP_ADTYPE_LOCAL_NAME_SHORT:
                case GAP_ADTYPE_LOCAL_NAME_COMPLETE: {
                    buffer[length - 1] = '\0';
                    printf("GAP_ADTYPE_LOCAL_NAME_XXX: %s\n\r", buffer);
                    break;
                }

                case GAP_ADTYPE_POWER_LEVEL: {
                    printf("GAP_ADTYPE_POWER_LEVEL: 0x%x\n\r", scan_info->data[(pos + 1)]);
                    break;
                }

                case GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE: {
                    uint16_t *p_min = (uint16_t *)(buffer);
                    uint16_t *p_max = p_min + 1;
                    printf("GAP_ADTYPE_SLAVE_CONN_INTERVAL_RANGE: 0x%x - 0x%x\n\r", *p_min, *p_max);
                    break;
                }

                case GAP_ADTYPE_SERVICE_DATA: {
                    uint16_t *p_uuid = (uint16_t *)(buffer);
                    uint8_t data_len = length - 3;
                    printf("GAP_ADTYPE_SERVICE_DATA: UUID 0x%x, len %d\n\r", *p_uuid, data_len);
                    break;
                }

                case GAP_ADTYPE_APPEARANCE: {
                    uint16_t *p_appearance = (uint16_t *)(buffer);
                    printf("GAP_ADTYPE_APPEARANCE: %d\n\r", *p_appearance);
                    break;
                }

                case GAP_ADTYPE_MANUFACTURER_SPECIFIC: {
                    uint8_t data_len = length - 3;
                    uint16_t *p_company_id = (uint16_t *)(buffer);
                    printf("GAP_ADTYPE_MANUFACTURER_SPECIFIC: company_id 0x%x, len %d\n\r", *p_company_id, data_len);
                    break;
                }

                default: {
                    uint8_t i = 0;
                    for (i = 0; i < (length - 1); i++) {
                        printf("AD Data: Unhandled Data = 0x%x\n\r", scan_info->data[(pos + i)]);
                    }
                    break;
                }
            }
        }
        pos += length;
    }
}