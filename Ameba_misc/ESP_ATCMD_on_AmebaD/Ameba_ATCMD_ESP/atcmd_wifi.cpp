#include "atcmd_core.h"
#include "atcmd_wifi.h"
#include "atcmd_tcpip.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "wifi_conf.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#include "lwip_netconf.h"
#include "inet.h"

extern struct netif xnetif[NET_IF_NUM];

#ifdef __cplusplus
}
#endif

uint8_t cw_dhcp = 3;
uint8_t cw_autoconn = 1;
uint8_t static_ip[4] = {0, 0, 0, 0};
uint8_t static_gw[4] = {0, 0, 0, 0};
uint8_t static_nm[4] = {0, 0, 0, 0};
char cw_hostname[32] = {"ameba"};
TaskHandle_t wifi_scan_task_handle = NULL;
SemaphoreHandle_t wifi_scan_complete_sema = NULL;
rtw_scan_result_t* wifi_scan_result_buffer = NULL;
uint8_t wifi_scan_result_count = 0;
char password[65] = {0};
rtw_network_info_t wifi = {{0,{0}},{0},0,0,0,0};
atcmd_cwstate_t cwstate = CWSTATE_INITIAL;

typedef struct atcmd_cwjap_params{
    uint8_t pci_en;
    uint16_t reconn_int;
    uint16_t reconn_cnt;
    uint8_t listen_int;
    uint8_t scan_mode;
    uint8_t pmf;
} atcmd_cwjap_params_t;
atcmd_cwjap_params_t cwjap_params = {0, 1, 0, 3, 0, 0};

void atcmd_wifi_disconnect_hdl(char* buf, int buf_len, int flags, void* userdata) {
    // Register handler for Connect, Disconnect, Reconnection failed indications
    (void)buf;
    (void)buf_len;
    (void)flags;
    (void)userdata;
    uint8_t autoreconnect_mode;
    /*
    wifi state start at 0
    if connect indicator, state 0 -> 1
    if received IP addr, state 1 -> 2
    if disconnect indicator, and reconnection enabled, state 1/2 -> 3
    if disconnect indicator, and reconnection disabled, state 1/2 -> 4
    if reconnect failed indicator, state 1/2/3 -> 4
    */
    if ((cwstate == CWSTATE_CONNECTED) || (cwstate == CWSTATE_CONNECTEDIP)) {
        wifi_get_autoreconnect(&autoreconnect_mode);
        if (autoreconnect_mode == RTW_AUTORECONNECT_DISABLE) {
            cwstate = CWSTATE_DISCONNECTED;
        } else {
            cwstate = CWSTATE_CONNECTING;
        }
        at_printf("WIFI DISCONNECT\r\n");
    }
}

void atcmd_wifi_connect_hdl(char* buf, int buf_len, int flags, void* userdata) {
    // Register handler for Connect, Disconnect, Reconnection failed indications
    (void)buf;
    (void)buf_len;
    (void)flags;
    (void)userdata;
    uint8_t* ip = NULL;
    /*
    wifi state start at 0
    if connect indicator, state 0 -> 1
    if received IP addr, state 1 -> 2
    if disconnect indicator, and reconnection enabled, state 1/2 -> 3
    if disconnect indicator, and reconnection disabled, state 1/2 -> 4
    if reconnect failed indicator, state 1/2/3 -> 4
    */
    if ((cwstate == CWSTATE_INITIAL) || (cwstate == CWSTATE_CONNECTING) || (cwstate == CWSTATE_DISCONNECTED)) {
        cwstate = CWSTATE_CONNECTED;
        at_printf("WIFI CONNECTED\r\n");
    }
    ip = LwIP_GetIP(&xnetif[0]);
    if ((ip[0] != 0) || (ip[1] != 0) || (ip[2] != 0) || (ip[3] != 0)) {
        cwstate = CWSTATE_CONNECTEDIP;
        at_printf("WIFI GOT IP\r\n");
    }
}

void atcmd_wifi_reconn_fail_hdl(char* buf, int buf_len, int flags, void* userdata) {
    // Register handler for Connect, Disconnect, Reconnection failed indications
    (void)buf;
    (void)buf_len;
    (void)flags;
    (void)userdata;
    /*
    wifi state start at 0
    if connect indicator, state 0 -> 1
    if received IP addr, state 1 -> 2
    if disconnect indicator, and reconnection enabled, state 1/2 -> 3
    if disconnect indicator, and reconnection disabled, state 1/2 -> 4
    if reconnect failed indicator, state 1/2/3 -> 4
    */
    if ((cwstate == CWSTATE_CONNECTING)) {
        cwstate = CWSTATE_DISCONNECTED;
    }
}

uint8_t atcmd_wifi_connect(uint32_t timeout_sec);
void atcmd_wifi_init(void) {
    // initialize semaphore used to indicate newly received ATcommands
    vSemaphoreCreateBinary(wifi_scan_complete_sema);
    xSemaphoreTake(wifi_scan_complete_sema, 1/portTICK_PERIOD_MS);

    LwIP_Init();
    wifi_on(RTW_MODE_STA);
    cwstate = CWSTATE_INITIAL;
    atcmd_wifi_register();

    if (wifi.ssid.len != 0) {
        atcmd_wifi_connect(15);
    }
}

rtw_result_t atcmd_wifi_scan_result_handler(rtw_scan_handler_result_t* scan_result) {
    rtw_scan_result_t* record;

    if (scan_result->scan_complete != RTW_TRUE) {
        record = &scan_result->ap_details;
        record->SSID.val[record->SSID.len] = 0; // Ensure the SSID is null terminated
        memcpy(&(wifi_scan_result_buffer[wifi_scan_result_count]), record, sizeof(rtw_scan_result_t));
        wifi_scan_result_count++;
    } else {
        // Indicate scan complete with semaphore
        xSemaphoreGive(wifi_scan_complete_sema);
    }
    return RTW_SUCCESS;
}

void atcmd_wifi_print_scan_result(rtw_scan_result_t* record) {
    // Encryption type AES = CCMP
    uint8_t enc_type = 0;
    uint8_t cip_type = 7;
    uint8_t wps_enabled = 0;
    rtw_security_t sec = record->security;
//    encryption method
//    0: OPEN
//    1: WEP
//    2: WPA_PSK
//    3: WPA2_PSK
//    4: WPA_WPA2_PSK
//    5: WPA2_ENTERPRISE
//    6: WPA3_PSK
//    7: WPA2_WPA3_PSK
    if (sec == RTW_SECURITY_OPEN) {
        enc_type = 0;
    }
    if (sec & (WEP_ENABLED)) {
        enc_type = 1;
    }
    if ((sec & WPA_SECURITY) && (sec & WPA2_SECURITY)) {
        enc_type = 4;
    } else if (sec & (WPA2_SECURITY)) {
        enc_type = 3;
    } else if (sec & (WPA_SECURITY)) {
        enc_type = 2;
    }
    if ((sec & WPA2_SECURITY) && (sec & ENTERPRISE_ENABLED)) {
        enc_type = 5;
    }
    if ((sec & WPA2_SECURITY) && (sec & WPA3_SECURITY)) {
        enc_type = 7;
    } else if (sec & (WPA3_SECURITY)) {
        enc_type = 6;
    }
//    cipher type.
//    0: None
//    1: WEP40
//    2: WEP104
//    3: TKIP
//    4: CCMP
//    5: TKIP and CCMP
//    6: AES-CMAC-128
//    7: Unknown
    if (sec == RTW_SECURITY_OPEN) {
        cip_type = 0;
    }
    if (sec & (SHARED_ENABLED)) {
        // Assume WEP104, scan result does not discriminate between 40 and 104
        cip_type = 2;
    }
    if ((sec & TKIP_ENABLED) && (sec & AES_ENABLED)) {
        cip_type = 5;
    } else if (sec & (AES_ENABLED)) {
        cip_type = 4;
    } else if (sec & (TKIP_ENABLED)) {
        cip_type = 3;
    }
    // WPS
    if (record->wps_type != RTW_WPS_TYPE_NONE) {
        wps_enabled = 1;
    }
    at_printf("+CWLAP:(%d,\"%s\",%d,\"" MAC_FMT "\",%d,-1,-1,%d,%d,%d,%d)\r\n", 
    enc_type, record->SSID.val, record->signal_strength, MAC_ARG(record->BSSID.octet), record->channel, cip_type, cip_type, 7, wps_enabled);
}

uint8_t hex2int(char hex1, char hex2) {
    uint8_t val = 0;

    // transform hex character to the 4bit equivalent number, using the ascii table indexes
    if (hex1 >= '0' && hex1 <= '9') {
        hex1 = hex1 - '0';
    } else if (hex1 >= 'a' && hex1 <='f') {
        hex1 = hex1 - 'a' + 10;
    } else if (hex1 >= 'A' && hex1 <='F') {
        hex1 = hex1 - 'A' + 10;
    }
    if (hex2 >= '0' && hex2 <= '9') {
        hex2 = hex2 - '0';
    } else if (hex2 >= 'a' && hex2 <='f') {
        hex2 = hex2 - 'a' + 10;
    } else if (hex2 >= 'A' && hex2 <='F') {
        hex2 = hex2 - 'A' + 10;
    }

    val = (hex1 << 4) | (hex2 & 0x0F);
    return val;
}

static int _find_ap_from_scan_buf(char* buf, int buflen, char* target_ssid, void* user_data) {
    rtw_wifi_setting_t* pwifi = (rtw_wifi_setting_t*)user_data;
    int plen = 0;
    
    while(plen < buflen){
        u8 len, ssid_len, security_mode;
        char *mac, *ssid;
        int rssi;

        // len offset = 0
        len = (int)*(buf + plen);
        // check end
        if (len == 0) break;
        // mac
        mac = buf + plen + 1;
        (void)mac;
        // ssid offset = 14
        ssid_len = len - 1 - 6 - 4 - 1 - 1 - 1;
        ssid = buf + plen + 1 + 6 + 4 + 1 + 1 + 1;
        if ( (ssid_len == strlen(target_ssid))  && (!memcmp(ssid, target_ssid, ssid_len)) ) {
            strncpy((char*)pwifi->ssid, target_ssid, 33);
            // rssi offset = 7
            rssi = *(int*)(buf + plen + 1 + 6);
            (void)rssi;
            // security_mode offset = 11
            security_mode = (u8)*(buf + plen + 1 + 6 + 4);
            // channel offset = 13
            pwifi->channel = *(buf + plen + 13);
            if (security_mode == RTW_ENCODE_ALG_NONE)
                pwifi->security_type = RTW_SECURITY_OPEN;
            else if (security_mode == RTW_ENCODE_ALG_WEP)
                pwifi->security_type = RTW_SECURITY_WEP_PSK;
            else if (security_mode == RTW_ENCODE_ALG_CCMP)
                pwifi->security_type = RTW_SECURITY_WPA2_AES_PSK;

            if (RTW_ENCODE_ALG_WEP != security_mode)
                break;
        }
        plen += len;
    }
    return 0;
}

static int _get_ap_security_mode(char* ssid, rtw_security_t* security_mode, uint8_t* channel) {
    rtw_wifi_setting_t wifi;
    u32 scan_buflen = 1000;

    memset(&wifi, 0, sizeof(wifi));

    if (wifi_scan_networks_with_ssid(_find_ap_from_scan_buf, (void*)&wifi, scan_buflen, ssid, strlen(ssid)) != RTW_SUCCESS) {
        if (AT_DEBUG) printf("[_get_ap_security_mode] Wifi scan failed!\n");
        return 0;
    }

    if (strcmp((char*)wifi.ssid, ssid) == 0) {
        *security_mode = wifi.security_type;
        *channel = wifi.channel;
        return 1;
    }
    return 0;
}

uint8_t atcmd_wifi_connect(uint32_t timeout_sec) {
    uint8_t connect_channel = 0;
    uint8_t dhcp_result;
    int ret = 0;
    uint32_t starttime = xTaskGetTickCount();
    uint32_t timeoutms = timeout_sec * 1000;

    // Perform wifi scan to get exact security type
    if (wifi.security_type == (rtw_security_t)RTW_SECURITY_UNKNOWN) {
        while ((xTaskGetTickCount() - starttime) < timeoutms) {
            if ((ret = _get_ap_security_mode((char*)wifi.ssid.val, &wifi.security_type, &connect_channel))) {
                break;
            }
        }
        if (ret == 0) {
            if (AT_DEBUG) printf("[atcmd_wifi_connect] Failed to get AP security mode and channel\r\n");
            at_printf("+CWJAP:3\r\n");
            return ATCMD_ERROR;
        }
    }

    // Connect to wifi
//    uint8_t pscan_config = PSCAN_ENABLE;
//    wifi_set_pscan_chan(&connect_channel, &pscan_config, 1);
//    at_printf("WIFI partial scan channel %d\r\n", connect_channel);
    wifi_unreg_event_handler(WIFI_EVENT_CONNECT, atcmd_wifi_connect_hdl);
    wifi_unreg_event_handler(WIFI_EVENT_RECONNECTION_FAIL, atcmd_wifi_reconn_fail_hdl);

    while ((xTaskGetTickCount() - starttime) < timeoutms) {
        if ((wifi.bssid.octet[0] != 0) || (wifi.bssid.octet[1] != 0) || (wifi.bssid.octet[2] != 0) || (wifi.bssid.octet[3] != 0) || (wifi.bssid.octet[4] != 0) || (wifi.bssid.octet[5] != 0)) {
            ret = wifi_connect_bssid(wifi.bssid.octet, (char*)wifi.ssid.val, wifi.security_type, (char*)wifi.password,
                            ETH_ALEN, wifi.ssid.len, wifi.password_len, wifi.key_id, NULL);
        } else {
            ret = wifi_connect((char*)wifi.ssid.val, wifi.security_type, (char*)wifi.password, wifi.ssid.len,
                            wifi.password_len, wifi.key_id, NULL);
        }
        if (ret == RTW_SUCCESS) {
            break;
        }
    }
    if (ret == RTW_SUCCESS) {
        cwstate = CWSTATE_CONNECTED;
        at_printf("WIFI CONNECTED\r\n");
        netif_set_hostname(&xnetif[0], cw_hostname);
        if (cw_dhcp & 0x01) {   // Use DHCP
            dhcp_result = LwIP_DHCP(0, DHCP_START);
            if (dhcp_result != DHCP_ADDRESS_ASSIGNED) {
                wifi_disconnect();
                cwstate = CWSTATE_DISCONNECTED;
                at_printf("+CWJAP:4\r\n");
                at_printf("WIFI DISCONNECT\r\n");
                return ATCMD_ERROR;
            }
        } else {                // Use Static IP
            if (((static_ip[0] == 0) && (static_ip[1] == 0) && (static_ip[2] == 0) && (static_ip[3] == 0)) || 
                ((static_gw[0] == 0) && (static_gw[1] == 0) && (static_gw[2] == 0) && (static_gw[3] == 0))) {
                wifi_disconnect();
                cwstate = CWSTATE_DISCONNECTED;
                at_printf("+CWJAP:0\r\n");
                at_printf("WIFI DISCONNECT\r\n");
                return ATCMD_ERROR;
            }
            struct ip_addr ipaddr;
            struct ip_addr netmask;
            struct ip_addr gateway;
            IP4_ADDR(ip_2_ip4(&ipaddr), static_ip[0], static_ip[1], static_ip[2], static_ip[3]);
            IP4_ADDR(ip_2_ip4(&netmask), static_nm[0], static_nm[1] , static_nm[2], static_nm[3]);
            IP4_ADDR(ip_2_ip4(&gateway), static_gw[0], static_gw[1], static_gw[2], static_gw[3]);
            netif_set_addr(&xnetif[0], ip_2_ip4(&ipaddr), ip_2_ip4(&netmask),ip_2_ip4(&gateway));
        }
        cwstate = CWSTATE_CONNECTEDIP;
        at_printf("WIFI GOT IP\r\n");
        wifi_reg_event_handler(WIFI_EVENT_DISCONNECT, atcmd_wifi_disconnect_hdl, NULL);
        // Configure reconnection
        if (cwjap_params.reconn_int == 0) {
            wifi_config_autoreconnect(RTW_AUTORECONNECT_DISABLE, 0, 0);
        } else if (cwjap_params.reconn_cnt == 0) {
            wifi_config_autoreconnect(RTW_AUTORECONNECT_INFINITE, 2, cwjap_params.reconn_int);
            wifi_reg_event_handler(WIFI_EVENT_CONNECT, atcmd_wifi_connect_hdl, NULL);
            wifi_reg_event_handler(WIFI_EVENT_RECONNECTION_FAIL, atcmd_wifi_reconn_fail_hdl, NULL);
        } else {
            wifi_config_autoreconnect(RTW_AUTORECONNECT_FINITE, cwjap_params.reconn_cnt, cwjap_params.reconn_int);
            wifi_reg_event_handler(WIFI_EVENT_CONNECT, atcmd_wifi_connect_hdl, NULL);
            wifi_reg_event_handler(WIFI_EVENT_RECONNECTION_FAIL, atcmd_wifi_reconn_fail_hdl, NULL);
        }
    } else {
        ret = wifi_get_last_error();
        switch (ret) {
            case 0: {   // No Error
                if ((xTaskGetTickCount() - starttime) >= timeoutms) {
                    // Connection timeout
                    at_printf("+CWJAP:1\r\n");
                }
                break;
            }
            case 1: {   // No Network
                at_printf("+CWJAP:3\r\n");
                break;
            }
            case 2: {   // Connect Fail
                at_printf("+CWJAP:4\r\n");
                break;
            }
            case 3: {   // Wrong Password
                at_printf("+CWJAP:2\r\n");
                break;
            }
            case 4: {   // Handshake Timeout
                at_printf("+CWJAP:2\r\n");
                break;
            }
            case 5: {   // DHCP Fail
                at_printf("+CWJAP:4\r\n");
                break;
            }
            default: {  // Unknown
                if ((xTaskGetTickCount() - starttime) >= timeoutms) {
                    // Connection timeout
                    at_printf("+CWJAP:1\r\n");
                } else {
                    at_printf("+CWJAP:5\r\n");
                }
                break;
            }
        }
        return ATCMD_ERROR;
    }
    return ATCMD_OK;
}

//---------------------------- Commands for wifi ATCMD functionality ----------------------------//

uint8_t q_AT_CWMODE(void *arg) {
    // Query the Wi-Fi mode of ESP devices.
    // AT+CWMODE?
    (void)arg;
    at_printf("+CWMODE:%d\r\n", 1);
    return ATCMD_OK;
}

uint8_t s_AT_CWMODE(void *arg) {
    // Set the Wi-Fi mode of ESP devices.
    // AT+CWMODE=<mode>[,<auto_connect>]
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint8_t mode = 0;           // Arg 1
    uint8_t auto_connect = 0;   // Arg 2

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (argc > 3) {
        return ATCMD_ERROR;
    }

    mode = atoi(argv[1]);
    if (mode > 3) {
        return ATCMD_ERROR;
    }

    if (argv[2] != NULL) {
        auto_connect = atoi(argv[2]);
        if (auto_connect > 1) {
            return ATCMD_ERROR;
        }
    }

    return ATCMD_OK;
}

uint8_t q_AT_CWSTATE(void *arg) {
    // Query Wi-Fi state and Wi-Fi information
    (void)arg;
    at_printf("+CWSTATE:%d,\"%s\"\r\n", cwstate, (char *)wifi.ssid.val);
    return ATCMD_OK;
}

uint8_t q_AT_CWJAP(void *arg) {
    // Query currently connected AP information
    (void)arg;
    char ssid[33];
    uint8_t bssid[ETH_ALEN] = {0};
    int channel = 0;
    int rssi = 0;

    // No information if not connected
    if (wext_get_ssid(WLAN0_NAME, (unsigned char *)ssid) < 0) {
        // not connected yet
        return ATCMD_OK;
    }
    wifi_get_ap_bssid((unsigned char *)bssid);
    wifi_get_channel(&channel);
    wifi_get_rssi(&rssi);

    // +CWJAP:<ssid>,<bssid>,<channel>,<rssi>,<pci_en>,<reconn_interval>,<listen_interval>,<scan_mode>,<pmf>
    at_printf("+CWJAP:\"%s\",\"%2x:%2x:%2x:%2x:%2x:%2x\",%d,%d,%d,%d,%d,%d,%d\r\n",
            ssid, bssid[0], bssid[1], bssid[2], bssid[3], bssid[4], bssid[5], channel, rssi,
            cwjap_params.pci_en, cwjap_params.reconn_int, cwjap_params.listen_int, cwjap_params.scan_mode, cwjap_params.pmf);

    return ATCMD_OK;
}

uint8_t s_AT_CWJAP(void *arg) {
    // Connect to a targeted AP
    // AT+CWJAP=[<ssid>],[<pwd>][,<bssid>][,<pci_en>][,<reconn_interval>][,<listen_interval>][,<scan_mode>][,<jap_timeout>][,<pmf>]
    (void)arg;
    // listen_interval, scan_mode, PMF parameters ignored
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    char* ssid = NULL;          // Arg 1
    char* pass = NULL;          // Arg 2
    char* mac = NULL;           // Arg 3
    uint8_t pci_en = 0;         // Arg 4
    uint16_t reconn_int = 1;    // Arg 5
    uint8_t listen_int = 3;     // Arg 6
    uint8_t scan_mode = 0;      // Arg 7
    uint16_t jap_timeout = 15;  // Arg 8
    uint8_t pmf = 0;            // Arg 9

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if ((argc < 3) || (argc > 10)) {
        return ATCMD_ERROR;
    }

    if (AT_DEBUG) {
        for (uint8_t i = 1; i < argc; i++) {
            if (argv[i] != NULL) {
                printf("[s_AT_CWJAP] Arg %d: %s\r\n", i, argv[i]);
            }
        }
    }

    //SSID
    if (argv[1] != NULL) {
        ssid = argv[1];
        int ssid_len = strlen(ssid);
        if (ssid_len == 0) {
            // No SSID error
            return ATCMD_ERROR;
        }
        strncpy((char *)wifi.ssid.val, ssid, sizeof(wifi.ssid.val));
        wifi.ssid.len = ssid_len;
    }else{
        // No SSID error
        return ATCMD_ERROR;
    }
    wifi.security_type = RTW_SECURITY_OPEN;

    //PASSWORD
    if (argv[2] != NULL) {
        pass = argv[2];
        int pwd_len = strlen(pass);
        if (pwd_len > 64 || (pwd_len < 8 && pwd_len != 5)) {
            // password format error
            return ATCMD_ERROR;
        }
        strncpy(password, pass, sizeof(password));
        wifi.password = (unsigned char*)password;
        wifi.password_len = pwd_len;
        wifi.security_type = RTW_SECURITY_UNKNOWN;
    }

    //BSSID
    if (argv[3] != NULL) {
        mac = argv[3];
        if (strlen(mac) != 17) {
            // BSSID format error
            return ATCMD_ERROR;
        }
        uint8_t i,j;
        for (i = 0, j = 0; i < ETH_ALEN; i++, j += 3) {
            wifi.bssid.octet[i] = hex2int(mac[j], mac[j+1]);
        }
    } else {
        memset(wifi.bssid.octet, 0, 6 * sizeof(unsigned char));
    }

    if (argv[4] != NULL) {
        pci_en = atoi(argv[4]);
        if (pci_en > 1) {
            return ATCMD_ERROR;
        }
        cwjap_params.pci_en = pci_en;
    } else {
        cwjap_params.pci_en = 0;
    }
    if (argv[5] != NULL) {
        reconn_int = atoi(argv[5]);
        if (reconn_int > 7200) {
            return ATCMD_ERROR;
        }
        cwjap_params.reconn_int = reconn_int;
    } else {
        cwjap_params.reconn_int = 1;
    }
    if (argv[6] != NULL) {
        listen_int = atoi(argv[6]);
        if ((listen_int > 100) || (listen_int == 0)) {
            return ATCMD_ERROR;
        }
        cwjap_params.listen_int = listen_int;
    } else {
        cwjap_params.listen_int = 3;
    }
    if (argv[7] != NULL) {
        scan_mode = atoi(argv[7]);
        if (scan_mode > 1) {
            return ATCMD_ERROR;
        }
        cwjap_params.scan_mode = scan_mode;
    } else {
        cwjap_params.scan_mode = 0;
    }
    if (argv[8] != NULL) {
        jap_timeout = atoi(argv[8]);
        if (jap_timeout > 3600) {
            return ATCMD_ERROR;
        }
    }
    if (argv[9] != NULL) {
        pmf = atoi(argv[9]);
        if (pmf > 3) {
            return ATCMD_ERROR;
        }
        cwjap_params.pmf = pmf;
    } else {
        cwjap_params.pmf = 1;
    }

    return atcmd_wifi_connect(jap_timeout);
}

uint8_t e_AT_CWJAP(void *arg) {
    // Connect to previously targeted AP
    (void)arg;
    // Check for existing saved SSID, error if none
    if (wifi.ssid.len == 0) {
        return ATCMD_ERROR;
    }

    return atcmd_wifi_connect(15);
}

uint8_t q_AT_CWRECONNCFG(void *arg) {
    // Query autoconnection setting.
    (void)arg;
    at_printf("+CWRECONNCFG:%d,%d\r\n", cwjap_params.reconn_int, cwjap_params.reconn_cnt);
    return ATCMD_OK;
}

uint8_t s_AT_CWRECONNCFG(void *arg) {
    // Automatically Connect to an AP When Powered on.
    // AT+CWAUTOCONN=<enable>
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint16_t interval = 0;          // Arg 1
    uint16_t count = 0;             // Arg 2

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (argc != 3) {
        return ATCMD_ERROR;
    }

    interval = atoi(argv[1]);
    if (interval > 7200) {
        return ATCMD_ERROR;
    }

    count = atoi(argv[2]);
    if (count > 1000) {
        return ATCMD_ERROR;
    }

    cwjap_params.reconn_int = interval;
    cwjap_params.reconn_cnt = count;

    // Configure reconnection
    if (cwjap_params.reconn_int == 0) {
        wifi_config_autoreconnect(RTW_AUTORECONNECT_DISABLE, 0, 0);
    } else if (cwjap_params.reconn_cnt == 0) {
        wifi_config_autoreconnect(RTW_AUTORECONNECT_INFINITE, 2, cwjap_params.reconn_int);
    } else {
        wifi_config_autoreconnect(RTW_AUTORECONNECT_FINITE, cwjap_params.reconn_cnt, cwjap_params.reconn_int);
    }

    return ATCMD_OK;
}

uint8_t s_AT_CWLAP(void *arg) {
    // Scan for APs with specific parameters
    (void)arg;
    // scan_type, scan_time_min, scan_time_max parameters are ignored
    uint8_t argc = 0;
    char *argv[ATCMD_MAX_ARG_CNT] = {0};
    char* ssid = NULL;      // Arg 1
    char* mac = NULL;       // Arg 2
    uint8_t channel = 0;    // Arg 3
//    uint8_t scan_type = 0;  // Arg 4
//    uint16_t scan_min = 0;  // Arg 5
//    uint16_t scan_max = 0;  // Arg 6

    if (!arg) {
        return ATCMD_ERROR;
    }
    if ((argc = atcmd_parse_params((char*)arg, argv)) > 7) {
        return ATCMD_ERROR;
    }

    if (AT_DEBUG) {
        for (uint8_t i = 1; i < argc; i++) {
            if (argv[i] != NULL) {
                printf("[s_AT_CWLAP] Arg %d: %s\r\n", i, argv[i]);
            }
        }
    }

    ssid = argv[1];
    mac = argv[2];
    if (argv[3] != NULL) {
        channel = atoi(argv[3]);
    }

    // Configure for single channel scanning
    if (channel) {
        uint8_t pscan = PSCAN_ENABLE;
        if (wifi_set_pscan_chan(&channel, &pscan, 1) < 0) {
            return ATCMD_ERROR;
        }
    }
    // Allocate sufficient memory space to store all results
    wifi_scan_result_count = 0;
    if (wifi_scan_result_buffer != NULL) {
        free(wifi_scan_result_buffer);
    }
    wifi_scan_result_buffer = (rtw_scan_result_t*)malloc(WIFI_SCAN_RESULT_MAX_COUNT*sizeof(rtw_scan_result_t));
    if (wifi_scan_result_buffer == NULL) {
        // Failed to allocate memory for scan results
        return ATCMD_ERROR;
    }
    if (wifi_scan_networks(atcmd_wifi_scan_result_handler, NULL ) != RTW_SUCCESS) {
        // Failed to start wifi scan
        free(wifi_scan_result_buffer);
        return ATCMD_ERROR;
    }
    // Wait for semaphore indicating scan complete
    if (xSemaphoreTake(wifi_scan_complete_sema, 20000/portTICK_PERIOD_MS) != pdTRUE) {
        free(wifi_scan_result_buffer);
        return ATCMD_ERROR;
    }
    // Iterate through result list, filter results by SSID and MAC
    for (uint8_t i = 0; i < wifi_scan_result_count; i++) {
        if (ssid && (ssid[0] != '\0')) {
            if (strcmp(ssid, (char*)(wifi_scan_result_buffer[i].SSID.val))) {
                continue;
            }
        }
        if (mac && (mac[0] != '\0')) {
            char mac_addr[18] = {0};
            unsigned char* addr = wifi_scan_result_buffer[i].BSSID.octet;
            snprintf(mac_addr, 18, "%2x:%2x:%2x:%2x:%2x:%2x", addr[0], addr[1], addr[2], addr[3], addr[4], addr[5]);
            if (_stricmp(mac, mac_addr)) {
                continue;
            }
        }
        atcmd_wifi_print_scan_result(&(wifi_scan_result_buffer[i]));
    }
    free(wifi_scan_result_buffer);
    return ATCMD_OK;
}

uint8_t e_AT_CWLAP(void *arg) {
    // List all APs
    (void)arg;
    // Allocate sufficient memory space to store all results
    wifi_scan_result_count = 0;
    if (wifi_scan_result_buffer != NULL) {
        free(wifi_scan_result_buffer);
    }
    wifi_scan_result_buffer = (rtw_scan_result_t*)malloc(WIFI_SCAN_RESULT_MAX_COUNT*sizeof(rtw_scan_result_t));
    if (wifi_scan_result_buffer == NULL) {
        // Failed to allocate memory for scan results
        return ATCMD_ERROR;
    }
    if (wifi_scan_networks(atcmd_wifi_scan_result_handler, NULL ) != RTW_SUCCESS) {
        // Failed to start wifi scan
        free(wifi_scan_result_buffer);
        return ATCMD_ERROR;
    }
    // Wait for semaphore indicating scan complete
    if (xSemaphoreTake(wifi_scan_complete_sema, 20000/portTICK_PERIOD_MS) != pdTRUE) {
        free(wifi_scan_result_buffer);
        return ATCMD_ERROR;
    }
    // Iterate through result list and print
    for (uint8_t i = 0; i < wifi_scan_result_count; i++) {
        atcmd_wifi_print_scan_result(&(wifi_scan_result_buffer[i]));
    }
    free(wifi_scan_result_buffer);
    return ATCMD_OK;
}

uint8_t e_AT_CWQAP(void *arg) {
    // Disconnect from AP
    (void)arg;
    char ssid[33];
    uint8_t timeout = 20;
    int ret = RTW_SUCCESS;

    if (wext_get_ssid(WLAN0_NAME, (unsigned char *)ssid) < 0) {
        // not connected yet
        return ATCMD_OK;
    }
    wifi_unreg_event_handler(WIFI_EVENT_DISCONNECT, atcmd_wifi_disconnect_hdl);
    if ((ret = wifi_disconnect()) < 0) {
        return ATCMD_ERROR;
    }
    while (1) {
        if (wext_get_ssid(WLAN0_NAME, (unsigned char *) ssid) < 0) {
            break;
        }

        if (timeout == 0) {
            return ATCMD_ERROR;
            break;
        }

        vTaskDelay(1 * configTICK_RATE_HZ);
        timeout --;
    }
    LwIP_ReleaseIP(WLAN0_IDX);
    cwstate = CWSTATE_DISCONNECTED;
    at_printf("WIFI DISCONNECT\r\n");
    return ATCMD_OK;
}

uint8_t q_AT_CWDHCP(void *arg) {
    // Query DHCP mode.
    // AT+CWDHCP?
    (void)arg;
    at_printf("+CWDHCP:%d", cw_dhcp);
    return ATCMD_OK;
}

uint8_t s_AT_CWDHCP(void *arg) {
    // Enable/Disable DHCP.
    // AT+CWDHCP=<operate>,<mode>
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint8_t operate = 0;          // Arg 1
    uint8_t mode = 0;          // Arg 2

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (argc != 3) {
        return ATCMD_ERROR;
    }

    operate = atoi(argv[1]);
    if (operate > 1) {
        return ATCMD_ERROR;
    }

    mode = atoi(argv[2]);
    if ((mode > 3) || (mode == 0)) {
        return ATCMD_ERROR;
    }

    // Station mode DHCP configuration
    if (mode & 0x01) {
        if ((cwstate == CWSTATE_CONNECTEDIP) || (cwstate == CWSTATE_CONNECTED)) {
            if (cw_dhcp & 0x01) {       // Already using DHCP
                if (operate == 0) {     // Change to static IP
                    LwIP_ReleaseIP(WLAN0_IDX);
                    struct ip_addr ipaddr;
                    struct ip_addr netmask;
                    struct ip_addr gateway;
                    IP4_ADDR(ip_2_ip4(&ipaddr), static_ip[0], static_ip[1], static_ip[2], static_ip[3]);
                    IP4_ADDR(ip_2_ip4(&netmask), static_nm[0], static_nm[1] , static_nm[2], static_nm[3]);
                    IP4_ADDR(ip_2_ip4(&gateway), static_gw[0], static_gw[1], static_gw[2], static_gw[3]);
                    netif_set_addr(&xnetif[0], ip_2_ip4(&ipaddr), ip_2_ip4(&netmask),ip_2_ip4(&gateway));
                    if (((static_ip[0] != 0) || (static_ip[1] != 0) || (static_ip[2] != 0) || (static_ip[3] != 0)) && 
                        ((static_gw[0] != 0) || (static_gw[1] != 0) || (static_gw[2] != 0) || (static_gw[3] != 0))) {
                        at_printf("WIFI GOT IP\r\n");
                    }
                }
            } else {            // Already using static IP
                if (operate) {  // Change to use DHCP
                    LwIP_ReleaseIP(WLAN0_IDX);
                    uint8_t dhcp_result;
                    dhcp_result = LwIP_DHCP(0, DHCP_START);
                    if (dhcp_result == DHCP_ADDRESS_ASSIGNED) {
                        at_printf("WIFI GOT IP\r\n");
                    }
                } else {
                    struct ip_addr ipaddr;
                    struct ip_addr netmask;
                    struct ip_addr gateway;
                    IP4_ADDR(ip_2_ip4(&ipaddr), static_ip[0], static_ip[1], static_ip[2], static_ip[3]);
                    IP4_ADDR(ip_2_ip4(&netmask), static_nm[0], static_nm[1] , static_nm[2], static_nm[3]);
                    IP4_ADDR(ip_2_ip4(&gateway), static_gw[0], static_gw[1], static_gw[2], static_gw[3]);
                    netif_set_addr(&xnetif[0], ip_2_ip4(&ipaddr), ip_2_ip4(&netmask),ip_2_ip4(&gateway));
                    if (((static_ip[0] != 0) || (static_ip[1] != 0) || (static_ip[2] != 0) || (static_ip[3] != 0)) && 
                        ((static_gw[0] != 0) || (static_gw[1] != 0) || (static_gw[2] != 0) || (static_gw[3] != 0))) {
                        at_printf("WIFI GOT IP\r\n");
                    }
                }
            }
        }
        cw_dhcp &= ~(0x01);
        cw_dhcp |= (operate & 0x01);
    }

    // SoftAP mode DHCP configuration
    if (mode & 0x02) {
        cw_dhcp &= ~(0x02);
        cw_dhcp |= ((operate << 1) & 0x02);
    }

    return ATCMD_OK;
}

uint8_t q_AT_CWAUTOCONN(void *arg) {
    // Query autoconnection setting.
    (void)arg;
    at_printf("+CWAUTOCONN:%d", 0);
    return ATCMD_OK;
}

uint8_t s_AT_CWAUTOCONN(void *arg) {
    // Automatically Connect to an AP When Powered on.
    // AT+CWAUTOCONN=<enable>
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint8_t mode = 0;          // Arg 1

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (argc != 2) {
        return ATCMD_ERROR;
    }

    mode = atoi(argv[1]);
    if (mode > 1) {
        return ATCMD_ERROR;
    }

    return ATCMD_OK;
}

uint8_t q_AT_CIPSTAMAC(void *arg) {
    // Query the MAC address of the Station.
    // AT+CIPSTAMAC?
    (void)arg;
    char mac[32] = {0};

    wifi_get_mac_address(mac);
    at_printf("+CIPSTAMAC:\"%s\"\r\n", mac);
    return ATCMD_OK;
}

uint8_t q_AT_CIPSTA(void *arg) {
    // Query the IP address of the Station.
    // AT+CIPSTA?
    (void)arg;
    char ip[15] = {"0.0.0.0"};
    char gw[15] = {"0.0.0.0"};
    char nm[15] = {"0.0.0.0"};

    if (cw_dhcp & 0x01) {
//        // Station DHCP is on
        if (cwstate == CWSTATE_CONNECTEDIP) {
            uint8_t* ip_addr = LwIP_GetIP(&xnetif[0]);
            uint8_t* gw_addr = LwIP_GetGW(&xnetif[0]);
            uint8_t* netmask = LwIP_GetMASK(&xnetif[0]);
            sprintf(ip, "%d.%d.%d.%d", ip_addr[0], ip_addr[1], ip_addr[2], ip_addr[3]);
            sprintf(gw, "%d.%d.%d.%d", gw_addr[0], gw_addr[1], gw_addr[2], gw_addr[3]);
            sprintf(nm, "%d.%d.%d.%d", netmask[0], netmask[1], netmask[2], netmask[3]);
        }
    } else {
            sprintf(ip, "%d.%d.%d.%d", static_ip[0], static_ip[1], static_ip[2], static_ip[3]);
            sprintf(gw, "%d.%d.%d.%d", static_gw[0], static_gw[1], static_gw[2], static_gw[3]);
            sprintf(nm, "%d.%d.%d.%d", static_nm[0], static_nm[1], static_nm[2], static_nm[3]);
    }
    at_printf("+CIPSTA:ip:\"%s\"\r\n", ip);
    at_printf("+CIPSTA:gateway:\"%s\"\r\n", gw);
    at_printf("+CIPSTA:netmask:\"%s\"\r\n", nm);
    return ATCMD_OK;
}

uint8_t s_AT_CIPSTA(void *arg) {
    // Set the IPv4 address of the station.
    // AT+CIPSTA=<"ip">[,<"gateway">,<"netmask">]
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint32_t ipaddr = 0;

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if ((argc < 2) || (argc > 4)) {
        return ATCMD_ERROR;
    }

    if (argv[1] != NULL) {
        ipaddr = inet_addr(argv[1]);
        if (ipaddr == 0) {
            return ATCMD_ERROR;
        }
        static_ip[0] = (uint8_t)(ipaddr & 0xff);
        static_ip[1] = (uint8_t)((ipaddr >> 8) & 0xff);
        static_ip[2] = (uint8_t)((ipaddr >> 16) & 0xff);
        static_ip[3] = (uint8_t)((ipaddr >> 24) & 0xff);
    }

    if (argv[2] != NULL) {
        ipaddr = inet_addr(argv[2]);
        if (ipaddr == 0) {
            return ATCMD_ERROR;
        }
        static_gw[0] = (uint8_t)(ipaddr & 0xff);
        static_gw[1] = (uint8_t)((ipaddr >> 8) & 0xff);
        static_gw[2] = (uint8_t)((ipaddr >> 16) & 0xff);
        static_gw[3] = (uint8_t)((ipaddr >> 24) & 0xff);
    } else {
        static_gw[0] = static_ip[0];
        static_gw[1] = static_ip[1];
        static_gw[2] = static_ip[2];
        static_gw[3] = 1;
    }

    if (argv[3] != NULL) {
        ipaddr = inet_addr(argv[3]);
        static_nm[0] = (uint8_t)(ipaddr & 0xff);
        static_nm[1] = (uint8_t)((ipaddr >> 8) & 0xff);
        static_nm[2] = (uint8_t)((ipaddr >> 16) & 0xff);
        static_nm[3] = (uint8_t)((ipaddr >> 24) & 0xff);
    } else {
        static_nm[0] = 255;
        static_nm[1] = 255;
        static_nm[2] = 255;
        static_nm[3] = 0;
    }

    cw_dhcp &= ~(0x01);     // Change to using static IP
    struct ip_addr ip_add;
    struct ip_addr netmask;
    struct ip_addr gateway;
    IP4_ADDR(ip_2_ip4(&ip_add), static_ip[0], static_ip[1], static_ip[2], static_ip[3]);
    IP4_ADDR(ip_2_ip4(&netmask), static_nm[0], static_nm[1] , static_nm[2], static_nm[3]);
    IP4_ADDR(ip_2_ip4(&gateway), static_gw[0], static_gw[1], static_gw[2], static_gw[3]);
    netif_set_addr(&xnetif[0], ip_2_ip4(&ip_add), ip_2_ip4(&netmask),ip_2_ip4(&gateway));
    if ((cwstate == CWSTATE_CONNECTEDIP) || (cwstate == CWSTATE_CONNECTED)) {
        at_printf("WIFI GOT IP\r\n");
    }

    return ATCMD_OK;
}

uint8_t q_AT_CWHOSTNAME(void *arg) {
    // Query the host name of ESP Station.
    // AT+CWHOSTNAME?
    (void)arg;
    at_printf("+CWHOSTNAME:%s\r\n", cw_hostname);
    return ATCMD_OK;
}

uint8_t s_AT_CWHOSTNAME(void *arg) {
    // Automatically Connect to an AP When Powered on.
    // AT+CWAUTOCONN=<enable>
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    char* hostname = NULL;          // Arg 1

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (argc != 2) {
        return ATCMD_ERROR;
    }

    // Check for station mode

    if (argv[1] != NULL) {
        hostname = argv[1];
        int len = strlen(hostname);
        if (len > 32) {
            return ATCMD_ERROR;
        }
        strncpy(cw_hostname, hostname, sizeof(cw_hostname));
    }
    return ATCMD_OK;
}

atcmd_command_t atcmd_wifi_commands[] = {
      {"AT+CWMODE",      NULL,   q_AT_CWMODE,      s_AT_CWMODE,      NULL,           {NULL, NULL}},
      {"AT+CWSTATE",     NULL,   q_AT_CWSTATE,     NULL,             NULL,           {NULL, NULL}},
      {"AT+CWJAP",       NULL,   q_AT_CWJAP,       s_AT_CWJAP,       e_AT_CWJAP,     {NULL, NULL}},
      {"AT+CWRECONNCFG", NULL,   q_AT_CWRECONNCFG, s_AT_CWRECONNCFG, NULL,           {NULL, NULL}},
      {"AT+CWLAP",       NULL,   NULL,             s_AT_CWLAP,       e_AT_CWLAP,     {NULL, NULL}},
      {"AT+CWQAP",       NULL,   NULL,             NULL,             e_AT_CWQAP,     {NULL, NULL}},
      {"AT+CWDHCP",      NULL,   q_AT_CWDHCP,      s_AT_CWDHCP,      NULL,           {NULL, NULL}},
      {"AT+CWAUTOCONN",  NULL,   q_AT_CWAUTOCONN,  s_AT_CWAUTOCONN,  NULL,           {NULL, NULL}},
      {"AT+CIPSTAMAC",   NULL,   q_AT_CIPSTAMAC,   NULL,             NULL,           {NULL, NULL}},
      {"AT+CIPSTA",      NULL,   q_AT_CIPSTA,      s_AT_CIPSTA,      NULL,           {NULL, NULL}},
      {"AT+CWHOSTNAME",  NULL,   q_AT_CWHOSTNAME,  s_AT_CWHOSTNAME,  NULL,           {NULL, NULL}},
};

void atcmd_wifi_register(void) {
    atcmd_add_cmdtable(atcmd_wifi_commands, sizeof(atcmd_wifi_commands)/sizeof(atcmd_wifi_commands[0]));
}
