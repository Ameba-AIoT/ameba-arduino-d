#include "BLEBeacon.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "gap_le_types.h"
#include "platform_stdlib_rtl8721d.h"

#ifdef __cplusplus
}
#endif

#define U16_ENDIAN_SWAP(x) (((x & 0xFF00)>>8) + ((x & 0x00FF)<<8))

iBeacon::iBeacon() {
}

uint16_t iBeacon::getMajor() {
    return U16_ENDIAN_SWAP(_iBeaconAdvData._iBeaconScanRsp.major);
}

uint16_t iBeacon::getMinor() {
    return U16_ENDIAN_SWAP(_iBeaconAdvData._iBeaconScanRsp.minor);
}

uint16_t iBeacon::getManufacturerId() {
    return _iBeaconAdvData._iBeaconScanRsp.manufacturerId;
}

void iBeacon::getUUID(uint8_t* uuid) {
    memcpy(uuid, _iBeaconAdvData._iBeaconScanRsp.UUID, sizeof(_iBeaconAdvData._iBeaconScanRsp.UUID));
}

int8_t iBeacon::getRSSI() {
    return _iBeaconAdvData._iBeaconScanRsp.RSSI;
}

void iBeacon::setMajor(uint16_t major) {
    _iBeaconAdvData._iBeaconScanRsp.major = U16_ENDIAN_SWAP(major);
}

void iBeacon::setMinor(uint16_t minor) {
    _iBeaconAdvData._iBeaconScanRsp.minor = U16_ENDIAN_SWAP(minor);
}

void iBeacon::setManufacturerId(uint16_t id) {
    _iBeaconAdvData._iBeaconScanRsp.manufacturerId = id;
}

void iBeacon::setUUID(uint8_t* uuid) {
    memcpy(_iBeaconAdvData._iBeaconScanRsp.UUID, uuid, sizeof(_iBeaconAdvData._iBeaconScanRsp.UUID));
}

void iBeacon::setUUID(const char* uuid) {
    BLEUUID newuuid (uuid);
    memcpy(_iBeaconAdvData._iBeaconScanRsp.UUID, newuuid.data(), newuuid.length());
}

void iBeacon::setRSSI(int8_t rssi) {
    _iBeaconAdvData._iBeaconScanRsp.RSSI = rssi;
}

uint8_t* iBeacon::getAdvData() {
    return ((uint8_t*)&_iBeaconAdvData);
}

uint8_t* iBeacon::getScanRsp() {
    return ((uint8_t*)&(_iBeaconAdvData._iBeaconScanRsp));
}

altBeacon::altBeacon() {
}

uint16_t altBeacon::getMajor() {
    return U16_ENDIAN_SWAP(_altBeaconAdvData._altBeaconScanRsp.major);
}

uint16_t altBeacon::getMinor() {
    return U16_ENDIAN_SWAP(_altBeaconAdvData._altBeaconScanRsp.minor);
}

uint16_t altBeacon::getManufacturerId() {
    return _altBeaconAdvData._altBeaconScanRsp.manufacturerId;
}

void altBeacon::getUUID(uint8_t* uuid) {
    memcpy(uuid, _altBeaconAdvData._altBeaconScanRsp.UUID, sizeof(_altBeaconAdvData._altBeaconScanRsp.UUID));
}

int8_t altBeacon::getRSSI() {
    return _altBeaconAdvData._altBeaconScanRsp.RSSI;
}

uint8_t altBeacon::getRSVD() {
    return _altBeaconAdvData._altBeaconScanRsp.RSVD;
}

void altBeacon::setMajor(uint16_t major) {
    _altBeaconAdvData._altBeaconScanRsp.major = U16_ENDIAN_SWAP(major);
}

void altBeacon::setMinor(uint16_t minor) {
    _altBeaconAdvData._altBeaconScanRsp.minor = U16_ENDIAN_SWAP(minor);
}

void altBeacon::setManufacturerId(uint16_t id) {
    _altBeaconAdvData._altBeaconScanRsp.manufacturerId = id;
}

void altBeacon::setUUID(uint8_t* uuid) {
    memcpy(_altBeaconAdvData._altBeaconScanRsp.UUID, uuid, sizeof(_altBeaconAdvData._altBeaconScanRsp.UUID));
}

void altBeacon::setUUID(const char* uuid) {
    BLEUUID newuuid (uuid);
    memcpy(_altBeaconAdvData._altBeaconScanRsp.UUID, newuuid.data(), newuuid.length());
}

void altBeacon::setRSSI(int8_t rssi) {
    _altBeaconAdvData._altBeaconScanRsp.RSSI = rssi;
}

void altBeacon::setRSVD(uint8_t rsvd) {
    _altBeaconAdvData._altBeaconScanRsp.RSVD = rsvd;
}

uint8_t* altBeacon::getAdvData() {
    return ((uint8_t*)&_altBeaconAdvData);
}

uint8_t* altBeacon::getScanRsp() {
    return ((uint8_t*)&(_altBeaconAdvData._altBeaconScanRsp));
}
