#include "BLEHIDDevice.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

BLEHIDDevice BLEHIDDev;

uint8_t ble_hid_report_descriptor[] =
{
  TUD_HID_REPORT_DESC_KEYBOARD( HID_REPORT_ID(REPORT_ID_KEYBOARD) ),
  TUD_HID_REPORT_DESC_CONSUMER( HID_REPORT_ID(REPORT_ID_CONSUMER_CONTROL) ),
  TUD_HID_REPORT_DESC_MOUSE   ( HID_REPORT_ID(REPORT_ID_MOUSE) )
};

void HIDnotifCB (BLECharacteristic* chr, uint8_t connID, uint16_t cccd) {
    (void) connID;
    if (chr->getUUID() == BLEUUID(REPORT_CHAR_UUID)) {
        uint8_t id = chr->getReportRefID();
        uint8_t type = chr->getReportRefType();
        (void) id;
        (void) type;
        if (cccd & GATT_CLIENT_CHAR_CONFIG_NOTIFY) {
            //printf("Notifications enabled on Characteristic %s ID %d Type %d for connection %d \n", chr->getUUID().str(), id, type, connID);
        } else {
            //printf("Notifications disabled on Characteristic %s ID %d Type %d for connection %d \n", chr->getUUID().str(), id, type, connID);
        }
    } else {
        if (cccd & GATT_CLIENT_CHAR_CONFIG_NOTIFY) {
            //printf("Notifications enabled on Characteristic %s for connection %d \n", chr->getUUID().str(), connID);
        } else {
            //printf("Notifications disabled on Characteristic %s for connection %d \n", chr->getUUID().str(), connID);
        }
    }
}

void HIDwriteCB (BLECharacteristic* chr, uint8_t connID) {
    (void) connID;
    if (chr->getUUID() == BLEUUID(REPORT_CHAR_UUID)) {
        uint8_t id = chr->getReportRefID();
        uint8_t type = chr->getReportRefType();
        (void) id;
        (void) type;
        //printf("Data written on Characteristic %s ID %d Type %d for connection %d \n", chr->getUUID().str(), id, type, connID);
    } else {
        //printf("Data written on Characteristic %s for connection %d \n", chr->getUUID().str(), connID);
    }
}

void HIDreadCB (BLECharacteristic* chr, uint8_t connID) {
    (void) connID;
    if (chr->getUUID() == BLEUUID(REPORT_CHAR_UUID)) {
        uint8_t id = chr->getReportRefID();
        uint8_t type = chr->getReportRefType();
        (void) id;
        (void) type;
        //printf("Data read on Characteristic %s ID %d Type %d for connection %d \n", chr->getUUID().str(), id, type, connID);
    } else {
        //printf("Data read from Characteristic %s for connection %d \n", chr->getUUID().str(), connID);
    }
}

BLEHIDDevice::BLEHIDDevice() {

}

BLEHIDDevice::~BLEHIDDevice() {
    delete _pHIDInfoChar;
    delete _pReportMapChar;
    delete _pHIDControlChar;
    delete _pProtocolModeChar;
    delete _pBootKBOutputChar;
    delete _pBootKBInputChar;
    for (int i = 0; i < _numOutputReports; i++) {
        delete _pReportOutputChar[i];
    }
    free(_pReportOutputChar);
    for (int i = 0; i < _numInputReports; i++) {
        delete _pReportInputChar[i];
    }
    free(_pReportInputChar);

    delete _pBattLevelChar;

    delete _pPNPChar;
    delete _pManufacturerChar;
    delete _pModelChar;

    delete _pHIDService;
    delete _pDISService;
    delete _pBASService;

}

void BLEHIDDevice::init() {
// Generate HID required services and characteristics
    _pHIDService = new BLEService(HID_SERVICE_UUID);
    _pDISService = new BLEService(DIS_SERVICE_UUID);
    _pBASService = new BLEService(BATT_SERVICE_UUID);

    _pHIDInfoChar = new BLECharacteristic(HID_INFO_CHAR_UUID);
    _pReportMapChar = new BLECharacteristic(REPORT_MAP_CHAR_UUID);
    _pHIDControlChar = new BLECharacteristic(HID_CONTROL_POINT_CHAR_UUID);
    _pProtocolModeChar = new BLECharacteristic(PROTOCOL_MODE_CHAR_UUID);
    _pBootKBOutputChar = new BLECharacteristic(BOOT_KEYBOARD_OUTPUT_REPORT_CHAR_UUID);
    _pBootKBInputChar = new BLECharacteristic(BOOT_KEYBOARD_INPUT_REPORT_CHAR_UUID);
    _pReportOutputChar = (BLECharacteristic**)malloc(_numOutputReports * sizeof(BLECharacteristic*));
    for (int i = 0; i < _numOutputReports; i++) {
        _pReportOutputChar[i] = new BLECharacteristic(REPORT_CHAR_UUID);
    }
    _pReportInputChar = (BLECharacteristic**)malloc(_numInputReports * sizeof(BLECharacteristic*));
    for (int i = 0; i < _numInputReports; i++) {
        _pReportInputChar[i] = new BLECharacteristic(REPORT_CHAR_UUID);
    }

    _pBattLevelChar = new BLECharacteristic(BATT_LEVEL_CHAR_UUID);

    _pPNPChar = new BLECharacteristic(PNP_CHAR_UUID);
    _pManufacturerChar = new BLECharacteristic(MANUFACTURER_CHAR_UUID);
    _pModelChar = new BLECharacteristic(MODEL_CHAR_UUID);

//Add characteristics to services
    _pHIDService->addCharacteristic(*_pHIDInfoChar);
    _pHIDService->addCharacteristic(*_pReportMapChar);
    _pHIDService->addCharacteristic(*_pHIDControlChar);
    _pHIDService->addCharacteristic(*_pProtocolModeChar);
    _pHIDService->addCharacteristic(*_pBootKBOutputChar);
    _pHIDService->addCharacteristic(*_pBootKBInputChar);
    for (int i = 0; i < _numOutputReports; i++) {
        _pHIDService->addCharacteristic(*(_pReportOutputChar[i]));
    }
    for (int i = 0; i < _numInputReports; i++) {
        _pHIDService->addCharacteristic(*(_pReportInputChar[i]));
    }
    
    _pBASService->addCharacteristic(*_pBattLevelChar);
    _pDISService->addCharacteristic(*_pPNPChar);
    _pDISService->addCharacteristic(*_pManufacturerChar);
    _pDISService->addCharacteristic(*_pModelChar);

//Configure required characteristics
    // HID_INFO_CHAR_UUID                    "2A4A"
    _hidInfo[0] = 0x11;     // bcdHID HID class specification version
    _hidInfo[1] = 0x01;     // bcdHID HID class specification version
    _hidInfo[2] = 0x00;     // bCountryCode
    _hidInfo[3] = 0x02;     // Flags: Bit 0: RemoteWake = 0 | Bit 1: NormallyConnectable = 1
    _pHIDInfoChar->setProperties(GATT_CHAR_PROP_READ);
    _pHIDInfoChar->setPermissions(GATT_PERM_READ_ENCRYPTED_REQ);
    _pHIDInfoChar->setBufferLen(4);
    _pHIDInfoChar->setData(_hidInfo, sizeof(_hidInfo));
    _pHIDInfoChar->setReadCallback(HIDreadCB);

    // REPORT_MAP_CHAR_UUID                    "2A4B"
    //Set HID report map
    setReportMap(ble_hid_report_descriptor, sizeof(ble_hid_report_descriptor));
    _pReportMapChar->setProperties(GATT_CHAR_PROP_READ);
    _pReportMapChar->setPermissions(GATT_PERM_READ_ENCRYPTED_REQ);
    _pReportMapChar->setReadCallback(HIDreadCB);

    // HID_CONTROL_POINT_CHAR_UUID             "2A4C"
    _pHIDControlChar->setProperties(GATT_CHAR_PROP_WRITE_NO_RSP);
    _pHIDControlChar->setPermissions(GATT_PERM_WRITE_ENCRYPTED_REQ);
    _pHIDControlChar->setBufferLen(1);
    _pHIDControlChar->setWriteCallback(HIDwriteCB);

    // PROTOCOL_MODE_CHAR_UUID                 "2A4E"
    _pProtocolModeChar->setProperties(GATT_CHAR_PROP_READ | GATT_CHAR_PROP_WRITE_NO_RSP);
    _pProtocolModeChar->setPermissions(GATT_PERM_READ_ENCRYPTED_REQ | GATT_PERM_WRITE_ENCRYPTED_REQ);
    _pProtocolModeChar->setBufferLen(1);
    _pProtocolModeChar->writeData8(1); // default of 1 for non-boot mode
    _pProtocolModeChar->setWriteCallback(HIDwriteCB);
    _pProtocolModeChar->setReadCallback(HIDreadCB);

    // BOOT_KEYBOARD_OUTPUT_REPORT_CHAR_UUID   "2A32"
    _pBootKBOutputChar->setProperties(GATT_CHAR_PROP_READ | GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_WRITE_NO_RSP);
    _pBootKBOutputChar->setPermissions(GATT_PERM_READ_ENCRYPTED_REQ | GATT_PERM_WRITE_ENCRYPTED_REQ);
    _pBootKBOutputChar->setWriteCallback(HIDwriteCB);
    _pBootKBOutputChar->setReadCallback(HIDreadCB);

    // BOOT_KEYBOARD_INPUT_REPORT_CHAR_UUID    "2A22"
    _pBootKBInputChar->setProperties(GATT_CHAR_PROP_READ | GATT_CHAR_PROP_NOTIFY);
    _pBootKBInputChar->setPermissions(GATT_PERM_READ_ENCRYPTED_REQ);
    _pBootKBInputChar->setNotifyProperty(TRUE);
    //_pProtocolModeChar->setBufferLen(sizeof(hid_keyboard_report_t));
    _pBootKBInputChar->setCCCDCallback(HIDnotifCB);
    _pBootKBInputChar->setReadCallback(HIDreadCB);

    // REPORT_CHAR_UUID                        "2A4D"
    for (int i = 0; i < _numOutputReports; i++) {
        _pReportOutputChar[i]->setProperties(GATT_CHAR_PROP_READ | GATT_CHAR_PROP_WRITE | GATT_CHAR_PROP_WRITE_NO_RSP);
        _pReportOutputChar[i]->setPermissions(GATT_PERM_READ_ENCRYPTED_REQ | GATT_PERM_WRITE_ENCRYPTED_REQ);
        _pReportOutputChar[i]->setReportRefDescriptor((i+1), 0x02);     //Report type 0x02 for output report
        _pReportOutputChar[i]->setWriteCallback(HIDwriteCB);
        _pReportOutputChar[i]->setReadCallback(HIDreadCB);
    }

    // REPORT_CHAR_UUID                        "2A4D"
    for (int i = 0; i < _numInputReports; i++) {
        _pReportInputChar[i]->setProperties(GATT_CHAR_PROP_READ | GATT_CHAR_PROP_NOTIFY);
        _pReportInputChar[i]->setPermissions(GATT_PERM_READ_ENCRYPTED_REQ);
        _pReportInputChar[i]->setNotifyProperty(TRUE);
        _pReportInputChar[i]->setReportRefDescriptor((i+1), 0x01);     //Report type 0x01 for input report
        _pReportInputChar[i]->setCCCDCallback(HIDnotifCB);
        _pReportInputChar[i]->setReadCallback(HIDreadCB);
    }

    // BATT_LEVEL_CHAR_UUID      "2A19"
    _pBattLevelChar->setProperties(GATT_CHAR_PROP_READ | GATT_CHAR_PROP_NOTIFY);
    _pBattLevelChar->setPermissions(GATT_PERM_READ);
    _pBattLevelChar->setNotifyProperty(TRUE);
    _pBattLevelChar->writeData8(100);
    _pBattLevelChar->setCCCDCallback(HIDnotifCB);

    // PNP_CHAR_UUID             "2A50"
    _pPNPChar->setProperties(GATT_CHAR_PROP_READ);
    _pPNPChar->setPermissions(GATT_PERM_READ);
    _pPNPChar->setData(_pnpInfo, sizeof(_pnpInfo));

    // MANUFACTURER_CHAR_UUID    "2A29"
    _pManufacturerChar->setProperties(GATT_CHAR_PROP_READ);
    _pManufacturerChar->setPermissions(GATT_PERM_READ);
    _pManufacturerChar->writeString("Realtek");

    // MODEL_CHAR_UUID           "2A24"
    _pModelChar->setProperties(GATT_CHAR_PROP_READ);
    _pModelChar->setPermissions(GATT_PERM_READ);
    _pModelChar->writeString("Ameba_BLE_HID");

}

void BLEHIDDevice::setNumOutputReport(uint8_t numOutputReports) {
    if (_pReportOutputChar == NULL) {
        // number of input reports can only be set before init is called
        _numOutputReports = numOutputReports;
    }
}

void BLEHIDDevice::setNumInputReport(uint8_t numInputReports) {
    if (_pReportInputChar == NULL) {
        // number of input reports can only be set before init is called
        _numInputReports = numInputReports;
    }
}

void BLEHIDDevice::setReportMap(uint8_t* report_map, uint16_t len) {
    _pReportMap = report_map;
    _pReportMapChar->setBufferLen(len);
    _pReportMapChar->setData(report_map, len);
}

void BLEHIDDevice::inputReport(uint8_t reportID, uint8_t* data, uint16_t len, uint8_t conn_id) {
    if ((reportID-1) >= _numInputReports) {
        return;
    }
    _pReportInputChar[reportID-1]->setData(data, len);
    _pReportInputChar[reportID-1]->notify(conn_id);
}

void BLEHIDDevice::setOutputReportCallback(uint8_t reportID, void (*fCallback) (BLECharacteristic* chr, uint8_t conn_id)) {
    _pReportOutputChar[reportID-1]->setWriteCallback(fCallback);
}

void BLEHIDDevice::bootKeyboardReport(uint8_t* data, uint16_t len, uint8_t conn_id) {
    _pBootKBInputChar->setData(data, len);
    _pBootKBInputChar->notify(conn_id);
}

void BLEHIDDevice::setHidInfo(uint16_t bcd, uint8_t country, uint8_t flags) {
    memcpy(_hidInfo, &bcd, 2);     // bcdHID HID class specification version
    _hidInfo[2] = country;     // bCountryCode
    _hidInfo[3] = flags;     // Flags: Bit 0: RemoteWake = 0 | Bit 1: NormallyConnectable = 1
    _pHIDInfoChar->setData(_hidInfo, sizeof(_hidInfo));
}

void BLEHIDDevice::setBattLevel(uint8_t level) {
    _pBattLevelChar->writeData8(level);
}

void BLEHIDDevice::setPNPInfo(uint8_t sig, uint16_t vid, uint16_t pid, uint16_t version) {
    _pnpInfo[0] = sig;
    _pnpInfo[1] = (uint8_t) (vid >> 8);
    _pnpInfo[2] = (uint8_t) (vid);
    _pnpInfo[3] = (uint8_t) (pid >> 8);
    _pnpInfo[4] = (uint8_t) (pid);
    _pnpInfo[5] = (uint8_t) (version >> 8);
    _pnpInfo[6] = (uint8_t) (version);
    _pPNPChar->setData(_pnpInfo, sizeof(_pnpInfo));
}

void BLEHIDDevice::setManufacturerString(const char* manufacturer) {
    _pManufacturerChar->writeString(manufacturer);
}

void BLEHIDDevice::setModelString(const char* model) {
    _pModelChar->writeString(model);
}

BLEService& BLEHIDDevice::hidService() {
    return *_pHIDService;
}

BLEService& BLEHIDDevice::devInfoService() {
    return *_pDISService;
}

BLEService& BLEHIDDevice::battService() {
    return *_pBASService;
}

