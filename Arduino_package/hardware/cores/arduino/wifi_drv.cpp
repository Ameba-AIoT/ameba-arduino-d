#include "wifi_drv.h"

// arduino includes
#include "wl_definitions.h"
#include "wl_types.h"
#include "wiring.h"

#ifdef __cplusplus
extern "C" {

// RTK includes
#include "main.h"
#include "wifi_conf.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#include "lwip_netconf.h"
#include "lwip/err.h"
#include "lwip/api.h"
#include <dhcp/dhcps.h>

extern struct netif xnetif[NET_IF_NUM];
}
#endif

// Array of data to cache the information related to the networks discovered
uint8_t     WiFiDrv::_networkCount = 0;
char        WiFiDrv::_networkSsid[][WL_SSID_MAX_LENGTH] = {{"1"}, {"2"}, {"3"}, {"4"}, {"5"}};
int32_t     WiFiDrv::_networkRssi[WL_NETWORKS_LIST_MAXNUM] = {0};
uint32_t    WiFiDrv::_networkEncr[WL_NETWORKS_LIST_MAXNUM] = {0};

static bool init_wlan = false;
//static int wifi_mode = NULL;
static int wifi_mode = 0;

static rtw_network_info_t wifi;
static rtw_ap_info_t ap;
static unsigned char password[65] = {0};

rtw_wifi_setting_t WiFiDrv::wifi_setting;

IPAddress WiFiDrv::_arduinoIpAddr = IPAddress(192, 168, 1, 80);
IPAddress WiFiDrv::_arduinoGwAddr = IPAddress(192, 168, 1, 1);
IPAddress WiFiDrv::_arduinoNetmaskAddr = IPAddress(255, 255, 255, 0);
IPAddress WiFiDrv::_arduinoApIpAddr = IPAddress(192, 168, 1, 1);
IPAddress WiFiDrv::_arduinoApGwAddr = IPAddress(192, 168, 1, 1);
IPAddress WiFiDrv::_arduinoApNetmaskAddr = IPAddress(255, 255, 255, 0);
IPAddress WiFiDrv::_arduinoDns1;
IPAddress WiFiDrv::_arduinoDns2;
bool WiFiDrv::_useStaticIp = false;

static void init_wifi_struct(void)
{
    memset(wifi.ssid.val, 0, sizeof(wifi.ssid.val));
    memset(wifi.bssid.octet, 0, ETH_ALEN);
    memset(password, 0, sizeof(password));
    wifi.ssid.len = 0;
    wifi.password = NULL;
    wifi.password_len = 0;
    wifi.key_id = -1;
    memset(ap.ssid.val, 0, sizeof(ap.ssid.val));
    ap.ssid.len = 0;
    ap.password = NULL;
    ap.password_len = 0;
    ap.channel = 1;
}

void WiFiDrv::wifiDriverInit()
{
    struct netif * pnetif = &xnetif[0];

    if (init_wlan == false) {
        init_wlan = true;
        LwIP_Init();
        wifi_on(RTW_MODE_STA);
        wifi_mode = RTW_MODE_STA;
    } else if (init_wlan == true) {
        if (wifi_mode != RTW_MODE_STA) {
            dhcps_deinit();
            wifi_off();
            vTaskDelay(20);
            wifi_on(RTW_MODE_STA);
            dhcps_init(pnetif);
            wifi_mode = RTW_MODE_STA;
        }
    }
}

int8_t WiFiDrv::wifiSetNetwork(char* ssid, uint8_t ssid_len)
{
    int ret;
    uint8_t dhcp_result;

    memset(wifi.bssid.octet, 0, ETH_ALEN);
    memcpy(wifi.ssid.val, ssid, ssid_len);
    wifi.ssid.len = ssid_len;

    wifi.security_type = RTW_SECURITY_OPEN;
    wifi.password = NULL;
    wifi.password_len = 0;
    wifi.key_id = 0;

    ret = wifi_connect((char*)wifi.ssid.val, wifi.security_type, (char*)wifi.password, wifi.ssid.len, wifi.password_len, wifi.key_id, NULL);

    if (ret == RTW_SUCCESS) {
        init_wifi_struct();

        if (_useStaticIp) {
            struct ip_addr ipaddr;
            struct ip_addr netmask;
            struct ip_addr gw;
            struct netif * pnetif = &xnetif[0];
            IP4_ADDR(&ipaddr, _arduinoIpAddr[0], _arduinoIpAddr[1], _arduinoIpAddr[2], _arduinoIpAddr[3]);
            IP4_ADDR(&netmask, _arduinoNetmaskAddr[0], _arduinoNetmaskAddr[1], _arduinoNetmaskAddr[2], _arduinoNetmaskAddr[3]);
            IP4_ADDR(&gw, _arduinoGwAddr[0], _arduinoGwAddr[1], _arduinoGwAddr[2], _arduinoGwAddr[3]);
            netif_set_addr(pnetif, &ipaddr, &netmask,&gw);
            return WL_SUCCESS;
        } else {
            dhcp_result = LwIP_DHCP(0, DHCP_START);
            if (dhcp_result == DHCP_ADDRESS_ASSIGNED) {
                return WL_SUCCESS;
            } else {
                wifi_disconnect();
                return WL_FAILURE;
            }
        }
    } else {
        init_wifi_struct();
        return WL_FAILURE;
    }
}

int8_t WiFiDrv::wifiSetPassphrase(char* ssid, uint8_t ssid_len, const char *passphrase, const uint8_t len)
{
    int ret;
    uint8_t dhcp_result;

    memset(wifi.bssid.octet, 0, ETH_ALEN);
    memcpy(wifi.ssid.val, ssid, ssid_len);
    wifi.ssid.len = ssid_len;

    wifi.security_type = RTW_SECURITY_WPA2_AES_PSK;

    memset(password, 0, sizeof(password));

    memcpy(password, passphrase, len);
    wifi.password = password;
    wifi.password_len = len;
    wifi.key_id = 0;

    ret = wifi_connect((char*)wifi.ssid.val, wifi.security_type, (char*)wifi.password, wifi.ssid.len, wifi.password_len, wifi.key_id, NULL);

    if (ret == RTW_SUCCESS) {
        init_wifi_struct();

        if (_useStaticIp) {
            struct ip_addr ipaddr;
            struct ip_addr netmask;
            struct ip_addr gw;
            struct netif * pnetif = &xnetif[0];
            IP4_ADDR(&ipaddr, _arduinoIpAddr[0], _arduinoIpAddr[1], _arduinoIpAddr[2], _arduinoIpAddr[3]);
            IP4_ADDR(&netmask, _arduinoNetmaskAddr[0], _arduinoNetmaskAddr[1], _arduinoNetmaskAddr[2], _arduinoNetmaskAddr[3]);
            IP4_ADDR(&gw, _arduinoGwAddr[0], _arduinoGwAddr[1], _arduinoGwAddr[2], _arduinoGwAddr[3]);
            netif_set_addr(pnetif, &ipaddr, &netmask,&gw);
            return WL_SUCCESS;
        } else {
            dhcp_result = LwIP_DHCP(0, DHCP_START);
            if (dhcp_result == DHCP_ADDRESS_ASSIGNED) {
                return WL_SUCCESS;
            } else {
                wifi_disconnect();
                return WL_FAILURE;
            }
        }
    } else {
        init_wifi_struct();
        return WL_FAILURE;
    }
}

int8_t WiFiDrv::wifiSetKey(char* ssid, uint8_t ssid_len, uint8_t key_idx, const void *key, const uint8_t len)
{
    int ret;
    uint8_t dhcp_result;

    memset(wifi.bssid.octet, 0, ETH_ALEN);
    memcpy(wifi.ssid.val, ssid, ssid_len);
    wifi.ssid.len = ssid_len;

    wifi.security_type = RTW_SECURITY_WEP_PSK;
    memset(password, 0, sizeof(password));

#if 0
    const unsigned char* k = (const unsigned char *)key;
    // convert hex sring to hex value
    for (int i = 0,int idx = 0; i < len; i++) {
        if ((k[i] >= '0') && (k[i] <= '9')) {
            password[idx] += (k[i] - '0');
        } else if ((k[i] >= 'a') && (k[i] <= 'f')) {
            password[idx] += (k[i] - 'a' + 10);
        } else if ((k[i] >= 'A') && (k[i] <= 'F')) {
            password[idx] += (k[i] - 'A' + 10);
        }

        if (i % 2 == 0) {
            password[idx] *= 16;
        } else {
            idx++;
        }
    }

    wifi.password = password;
    wifi.password_len = len/2;
    wifi.key_id = key_idx;
#else
    memcpy(password, key, len);

    wifi.password = password;
    wifi.password_len = len;
    wifi.key_id = key_idx;
#endif

    ret = wifi_connect((char*)wifi.ssid.val, wifi.security_type, (char*)wifi.password, wifi.ssid.len, wifi.password_len, wifi.key_id, NULL);

    if (ret == RTW_SUCCESS) {
        init_wifi_struct();

        if (_useStaticIp) {
            struct ip_addr ipaddr;
            struct ip_addr netmask;
            struct ip_addr gw;
            struct netif * pnetif = &xnetif[0];
            IP4_ADDR(&ipaddr, _arduinoIpAddr[0], _arduinoIpAddr[1], _arduinoIpAddr[2], _arduinoIpAddr[3]);
            IP4_ADDR(&netmask, _arduinoNetmaskAddr[0], _arduinoNetmaskAddr[1], _arduinoNetmaskAddr[2], _arduinoNetmaskAddr[3]);
            IP4_ADDR(&gw, _arduinoGwAddr[0], _arduinoGwAddr[1], _arduinoGwAddr[2], _arduinoGwAddr[3]);
            netif_set_addr(pnetif, &ipaddr, &netmask,&gw);
            return WL_SUCCESS;
        } else {
            dhcp_result = LwIP_DHCP(0, DHCP_START);
            if (dhcp_result == DHCP_ADDRESS_ASSIGNED) {
                return WL_SUCCESS;
            } else {
                wifi_disconnect();
                return WL_FAILURE;
            }
        }
    } else {
        init_wifi_struct();
        return WL_FAILURE;
    }
}

int8_t WiFiDrv::apSetNetwork(char* ssid, uint8_t ssid_len)
{
    int ret = WL_SUCCESS;

    ap.ssid.len = ssid_len;

    if (ap.ssid.len > 32) {
        printf("Error: SSID length can't exceed 32\n\r");
        ret = WL_FAILURE;
    }
    strcpy((char *)ap.ssid.val, (char*)ssid);
    return ret;
}

int8_t WiFiDrv::apSetPassphrase(const char *passphrase, uint8_t len)
{
    int ret = WL_SUCCESS;
    strcpy((char *)password, (char*)passphrase);
    ap.password = password;
    ap.password_len = len;
    if (ap.password_len < 8) {
        printf("Error: Password length can't less than 8\n\r");
        ret = WL_FAILURE;
    }
    return ret;
}

int8_t WiFiDrv::apSetChannel(const char *channel)
{
    int ret = WL_SUCCESS;
    ap.channel = (unsigned char) atoi((const char *)channel);
    return ret;
}

int8_t WiFiDrv::apActivate(uint8_t hidden_ssid)
{
#if CONFIG_LWIP_LAYER
    struct ip_addr ipaddr;
    struct ip_addr netmask;
    struct ip_addr gw;
    struct netif * pnetif = &xnetif[0];
#endif
    int timeout = 20;
    int ret = WL_SUCCESS;
    if (ap.ssid.val[0] == 0) {
        printf("Error: SSID can't be empty\n\r");
        ret = WL_FAILURE;
        goto exit;
    }
    if (ap.password == NULL) {
        ap.security_type = RTW_SECURITY_OPEN;
    } else{
          ap.security_type = RTW_SECURITY_WPA2_AES_PSK;
    }

#if CONFIG_LWIP_LAYER
    dhcps_deinit();
    IP4_ADDR(&ipaddr, _arduinoApIpAddr[0], _arduinoApIpAddr[1], _arduinoApIpAddr[2], _arduinoApIpAddr[3]);
    IP4_ADDR(&netmask, _arduinoApNetmaskAddr[0], _arduinoApNetmaskAddr[1], _arduinoApNetmaskAddr[2], _arduinoApNetmaskAddr[3]);
    IP4_ADDR(&gw, _arduinoApGwAddr[0], _arduinoApGwAddr[1], _arduinoApGwAddr[2], _arduinoApGwAddr[3]);
    netif_set_addr(pnetif, &ipaddr, &netmask,&gw);
#endif
    wifi_off();
    vTaskDelay(20);
    if (wifi_on(RTW_MODE_AP) < 0){
        printf("\n\rERROR: Wifi on failed!");
        ret = WL_FAILURE;
        goto exit;
    }
    printf("\n\rStarting AP ...");

    if (hidden_ssid == 1) {
        ret = wifi_start_ap_with_hidden_ssid((char*)ap.ssid.val, ap.security_type, (char*)ap.password, ap.ssid.len, ap.password_len, ap.channel);
    } else {
        ret = wifi_start_ap((char*)ap.ssid.val, ap.security_type, (char*)ap.password, ap.ssid.len, ap.password_len, ap.channel);
    }

    if(ret < 0) {
        printf("\n\rERROR: Operation failed!");
        ret = WL_FAILURE;
        goto exit;
    }

    while (1) {
        char essid[33];

        if (wext_get_ssid(WLAN0_NAME, ((unsigned char *)essid)) > 0) {
            if (strcmp(((const char *)essid), ((const char *)ap.ssid.val)) == 0) {
                printf("\n\r%s started\n", ap.ssid.val);
                ret = WL_SUCCESS;
                break;
            }
        }

        if (timeout == 0) {
            printf("\n\rERROR: Start AP timeout!");
            ret = WL_FAILURE;
            break;
        }

        vTaskDelay(1 * configTICK_RATE_HZ);
        timeout --;
    }
#if CONFIG_LWIP_LAYER
    //LwIP_UseStaticIP(pnetif);
    dhcps_init(pnetif);
#endif

exit:
    init_wifi_struct( );
    if (ret == WL_SUCCESS) {
        wifi_mode = RTW_MODE_AP;
    }
    return ret;
}

int8_t WiFiDrv::disconnect()
{
    wifi_disconnect();
    return WL_DISCONNECTED;
}

uint8_t WiFiDrv::getConnectionStatus()
{
    wifiDriverInit();

    if (wifi_is_connected_to_ap() == 0) {
        return WL_CONNECTED;
    } else {
        return WL_DISCONNECTED;
    }
}

uint8_t* WiFiDrv::getMacAddress()
{
    return LwIP_GetMAC(&xnetif[0]);
}

void WiFiDrv::getIpAddress(IPAddress& ip)
{
    ip = LwIP_GetIP(&xnetif[0]);
}

void WiFiDrv::getSubnetMask(IPAddress& mask)
{
    mask = LwIP_GetMASK(&xnetif[0]);
}

void WiFiDrv::getGatewayIP(IPAddress& ip)
{
    ip = LwIP_GetGW(&xnetif[0]);
}

char* WiFiDrv::getCurrentSSID()
{
    wifi_get_setting(WLAN0_NAME, &wifi_setting);
    return (char *)(wifi_setting.ssid);
}

uint8_t* WiFiDrv::getCurrentBSSID()
{
    // zzw
    //uint8_t bssid[ETH_ALEN];
    uint8_t* bssid = new uint8_t[ETH_ALEN];
    wext_get_bssid(WLAN0_NAME, bssid);
    return bssid;
}

int32_t WiFiDrv::getCurrentRSSI()
{
    int rssi = 0;
    wifi_get_rssi(&rssi);
    return rssi;
}

uint8_t WiFiDrv::getCurrentEncryptionType()
{
    wifi_get_setting(WLAN0_NAME, &wifi_setting);
    return (wifi_setting.security_type);
}

rtw_result_t WiFiDrv::wifidrv_scan_result_handler( rtw_scan_handler_result_t* malloced_scan_result )
{
    rtw_scan_result_t* record;

    if (malloced_scan_result->scan_complete != RTW_TRUE) {
        record = &malloced_scan_result->ap_details;
        record->SSID.val[record->SSID.len] = 0; /* Ensure the SSID is null terminated */

        if (_networkCount < WL_NETWORKS_LIST_MAXNUM) {
            strcpy( _networkSsid[_networkCount], (char *)record->SSID.val);
            _networkRssi[_networkCount] = record->signal_strength;
            _networkEncr[_networkCount] = record->security;
            _networkCount++;
        }
    }

    return RTW_SUCCESS;
}

int8_t WiFiDrv::startScanNetworks()
{
    _networkCount = 0;
    if (wifi_scan_networks(wifidrv_scan_result_handler, NULL ) != RTW_SUCCESS) {
        return WL_FAILURE;
    }

    return WL_SUCCESS;
}

uint8_t WiFiDrv::getScanNetworks()
{
    return _networkCount;
}

char* WiFiDrv::getSSIDNetoworks(uint8_t networkItem)
{
    if (networkItem >= WL_NETWORKS_LIST_MAXNUM) {
        return NULL;
    }

    return _networkSsid[networkItem];
}

uint8_t WiFiDrv::getEncTypeNetowrks(uint8_t networkItem)
{
    if (networkItem >= WL_NETWORKS_LIST_MAXNUM) {
        //return NULL;
        return 0;
    }

    uint8_t encType = 0;

    if (_networkEncr[networkItem] == RTW_SECURITY_OPEN) {
        encType = ENC_TYPE_NONE;
    } else if ((_networkEncr[networkItem] & AES_ENABLED) || (_networkEncr[networkItem] == RTW_SECURITY_WPA_WPA2_MIXED)) {
        encType = ENC_TYPE_CCMP;
    } else if (_networkEncr[networkItem] & TKIP_ENABLED) {
        encType = ENC_TYPE_TKIP;
    } else if (_networkEncr[networkItem] == RTW_SECURITY_WEP_PSK) {
        encType = ENC_TYPE_WEP;
    }

    return encType;
}

uint32_t WiFiDrv::getEncTypeNetowrksEx(uint8_t networkItem)
{
    //return ((networkItem >= WL_NETWORKS_LIST_MAXNUM) ? NULL : _networkEncr[networkItem]);
    return ((networkItem >= WL_NETWORKS_LIST_MAXNUM) ? 0 : _networkEncr[networkItem]);
}

int32_t WiFiDrv::getRSSINetoworks(uint8_t networkItem)
{
    if (networkItem >= WL_NETWORKS_LIST_MAXNUM) {
        //return NULL;
        return 0;
    }

    return _networkRssi[networkItem];
}

char* WiFiDrv::getFwVersion()
{
    // The version is for compatible to arduino example code
    return "1.0.0";
}

int WiFiDrv::getHostByName(const char* aHostname, IPAddress& aResult)
{
    ip_addr_t ip_addr;
    err_t err;
    err = netconn_gethostbyname(aHostname, &ip_addr);

    if (err != ERR_OK) {
        return WL_FAILURE;
    } else {
        aResult = ip_addr.addr;
        return WL_SUCCESS;
    }
}

int WiFiDrv::disablePowerSave()
{
    return wifi_disable_powersave();
}

void WiFiDrv::config(uint8_t validParams, IPAddress local_ip, IPAddress gateway, IPAddress subnet) {

    if (validParams == 1) {
        _arduinoIpAddr = local_ip;
        _arduinoApIpAddr = local_ip;
    } else if (validParams == 2) {
        _arduinoIpAddr = local_ip;
        _arduinoGwAddr = gateway;
        _arduinoApIpAddr = local_ip;
        _arduinoApGwAddr = gateway;
    } else if (validParams == 3) {
        _arduinoIpAddr = local_ip;
        _arduinoGwAddr = gateway;
        _arduinoNetmaskAddr = subnet;
        _arduinoApIpAddr = local_ip;
        _arduinoApGwAddr = gateway;
        _arduinoApNetmaskAddr = subnet;
    } else {
        return;
    }

    _useStaticIp = true;
}

void WiFiDrv::setDNS(uint8_t validParams, IPAddress dns_server1, IPAddress dns_server2) {
    struct ip_addr dns;

    if (validParams == 1) {
        _arduinoDns1 = dns_server1;
    } else if (validParams == 2) {
        _arduinoDns1 = dns_server1;
        _arduinoDns2 = dns_server2;
    } else {
        return;
    }
    IP4_ADDR(&dns, _arduinoDns1[0], _arduinoDns1[1], _arduinoDns1[2], _arduinoDns1[3]);
    LwIP_SetDNS(&dns);
}

