#ifndef ATCMD_WIFI_H
#define ATCMD_WIFI_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#define WIFI_SCAN_RESULT_MAX_COUNT 64
#define DEVICE_DEFAULT_HOSTNAME ("ameba")

//wifi state start at 0
//if connect indicator, state 0 -> 1
//if received IP addr, state 1 -> 2
//if disconnect indicator, and reconnection enabled, state 1/2 -> 3
//if disconnect indicator, and reconnection disabled, state 1/2 -> 4
//if reconnect failed indicator, state 1/2/3 -> 4
typedef enum {
    CWSTATE_INITIAL = 0,        // station has not started any Wi-Fi connection
    CWSTATE_CONNECTED = 1,      // station has connected to an AP, but not an IPv4 address yet
    CWSTATE_CONNECTEDIP = 2,    // station has connected to an AP, and received an IPv4 address
    CWSTATE_CONNECTING = 3,     // station is in Wi-Fi connecting or reconnecting state
    CWSTATE_DISCONNECTED = 4,   // station is in Wi-Fi disconnected state
} atcmd_cwstate_t;

void atcmd_wifi_init(void);
void atcmd_wifi_register(void);

#endif
