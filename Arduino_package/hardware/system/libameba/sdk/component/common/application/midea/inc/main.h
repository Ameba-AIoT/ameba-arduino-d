#ifndef MAIN_H
#define MAIN_H

#include <autoconf.h>

#ifndef CONFIG_WLAN
#define CONFIG_WLAN	1
#endif

/* Header file declaration*/
void wlan_network();
void user_wifi_beacon_hdl( char* buf, int buf_len, int flags, void* userdata);


/* Interactive Mode */
#define SERIAL_DEBUG_RX 1

#ifndef WLAN0_NAME
#define WLAN0_NAME		"wlan0"
#endif
#ifndef WLAN1_NAME
#define WLAN1_NAME		"wlan1"
#endif

#endif
