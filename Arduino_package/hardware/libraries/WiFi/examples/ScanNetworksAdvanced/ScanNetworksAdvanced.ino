/*
 Advanced dual channel WiFi Scan example for Ameba 

 This example scans for available Wifi networks using the build in functionality.
 Every 3 seconds, it scans again. 
 It doesn't actually connect to any network.

 based on the work of https://gist.github.com/EstebanFuentealba/3da9ccecefa7e1b44d84e7cfaad2f35f
 */

#include <WiFi.h>
#include <wifi_conf.h>

#define SCAN_INTERVAL 3000

static uint8_t _networkCount;
static char _networkSsid[WL_NETWORKS_LIST_MAXNUM][WL_SSID_MAX_LENGTH];
static int32_t _networkRssi[WL_NETWORKS_LIST_MAXNUM];
static uint32_t _networkEncr[WL_NETWORKS_LIST_MAXNUM];
static uint8_t _networkChannel[WL_NETWORKS_LIST_MAXNUM];
static uint8_t _networkBand[WL_NETWORKS_LIST_MAXNUM];
static char _networkMac[WL_NETWORKS_LIST_MAXNUM][18];

static rtw_result_t wifidrv_scan_result_handler(rtw_scan_handler_result_t *malloced_scan_result) {
  rtw_scan_result_t *record;

  if (malloced_scan_result->scan_complete != RTW_TRUE) {
    record = &malloced_scan_result->ap_details;
    record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */

    if (_networkCount < WL_NETWORKS_LIST_MAXNUM) {
      strcpy(_networkSsid[_networkCount], (char *)record->SSID.val);
      _networkRssi[_networkCount] = record->signal_strength;
      _networkEncr[_networkCount] = record->security;
      _networkChannel[_networkCount] = record->channel;
      _networkBand[_networkCount] = record->band;
      sprintf(_networkMac[_networkCount], "%02X:%02X:%02X:%02X:%02X:%02X",
              record->BSSID.octet[0], record->BSSID.octet[1], record->BSSID.octet[2],
              record->BSSID.octet[3], record->BSSID.octet[4], record->BSSID.octet[5]);
      _networkCount++;
    }
  }
  return RTW_SUCCESS;
}

void printEncryptionTypeEx(uint32_t thisType) {
  switch (thisType) {
    case RTW_SECURITY_OPEN:
      Serial.print("Open");
      break;
    case RTW_SECURITY_WEP_PSK:
      Serial.print("WEP");
      break;
    case RTW_SECURITY_WPA_TKIP_PSK:
      Serial.print("WPA TKIP");
      break;
    case RTW_SECURITY_WPA_AES_PSK:
      Serial.print("WPA AES");
      break;
    case RTW_SECURITY_WPA2_AES_PSK:
      Serial.print("WPA2 AES");
      break;
    case RTW_SECURITY_WPA2_TKIP_PSK:
      Serial.print("WPA2 TKIP");
      break;
    case RTW_SECURITY_WPA2_MIXED_PSK:
      Serial.print("WPA2 Mixed");
      break;
    case RTW_SECURITY_WPA_WPA2_MIXED_PSK:
      Serial.print("WPA/WPA2 AES");
      break;
    case RTW_SECURITY_WPA3_AES_PSK:
      Serial.print("WPA3 AES");
      break;
    case RTW_SECURITY_WPA2_WPA3_MIXED:
      Serial.print("WPA2/WPA3");
  }
}

static int8_t scanNetworks() {
  _networkCount = 0;
  if (wifi_scan_networks(wifidrv_scan_result_handler, NULL) != RTW_SUCCESS) {
    return WL_FAILURE;
  }
  return _networkCount;
}


void setup() {
  // Initialize serial and wait for port to open:
  Serial.begin(115200);
  while (!Serial) {
    ;  // wait for serial port to connect. Needed for native USB port only
  }
  // Initialize the onboard WiFi:
  WiFi.status();
}

void loop() {

  // scan for existing networks:
  Serial.println("Scanning available networks...");
  int n = scanNetworks();
  if (n == 0) {
    Serial.println("No networks found");
  } else {
    for (int network = 1; network < n; network++) {
      Serial.print(_networkSsid[network]);
      Serial.print("\tSignal: ");
      Serial.print(_networkRssi[network]);
      Serial.print(" dBm");
      Serial.print("\tEncryptionRaw: ");
      printEncryptionTypeEx(_networkEncr[network]);
      Serial.print("\tBand: ");
      // Serial.print(_networkBand[network]);
      if (_networkChannel[network] < 14) {
        Serial.print("2.4 Ghz");
      } else {
        Serial.print("5 Ghz");
      }
      Serial.print("\tChannel: ");
      Serial.print(_networkChannel[network]);
      Serial.print("\tMac: ");
      Serial.print(_networkMac[network]);
      Serial.println("");
    }
  }

  Serial.println("");
  delay(SCAN_INTERVAL);
}
