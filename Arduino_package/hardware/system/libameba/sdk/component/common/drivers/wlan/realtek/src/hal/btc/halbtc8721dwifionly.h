#ifndef __INC_HAL8721DWIFIONLYHWCFG_H
#define __INC_HAL8721DWIFIONLYHWCFG_H

VOID
ex_hal8721d_wifi_only_hw_config(
	IN PWIFI_ONLY_CFG  pwifionlycfg
	);
VOID
ex_hal8721d_wifi_only_scannotify(
	IN PWIFI_ONLY_CFG  pwifionlycfg,
	IN u1Byte  is_5g
	);
VOID
ex_hal8721d_wifi_only_switchbandnotify(
	IN PWIFI_ONLY_CFG  pwifionlycfg,
	IN u1Byte  is_5g
	);
VOID
ex_hal8721d_wifi_only_connectnotify(
	IN PWIFI_ONLY_CFG  pwifionlycfg,
	IN u1Byte  is_5g
	);
VOID
hal8721d_wifi_only_switch_antenna(IN PWIFI_ONLY_CFG  pwifionlycfg,
	IN u1Byte  is_5g
	);
#endif
