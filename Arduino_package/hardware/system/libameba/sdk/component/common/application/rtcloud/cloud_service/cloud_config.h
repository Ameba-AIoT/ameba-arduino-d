#ifndef __CLOUDCONFIG_H
#define __CLOUDCONFIG_H

#define CONFIG_PORT		1234

#include "wifi_constants.h"

typedef struct _CLOUD_CONFIG {
	uint32_t	boot_mode;
	uint8_t 	ssid[32];
	uint8_t		ssid_len;
	rtw_security_t		security_type;
	uint8_t		password[32];
	uint8_t		password_len;
	float		sensor_threshold;
} CLOUD_CONFIG;

typedef struct _CLOUD_INFO {
	uint8_t		cloud_id[32];
	uint8_t		cloud_secrete[32];
} CLOUD_INFO;

void start_config_listener(char *id, char *secrete, char *type);
void apply_sta_config_hook_callback(void (*apply_sta_config_callback)(char *ssid, char *password, rtw_security_t security_type));

#ifdef __CLOUDCONFIG_C
void reboot_config_mode(void);
#else
extern void reboot_config_mode(void);
#endif
#endif /* __CLOUDCONFIG_H */
