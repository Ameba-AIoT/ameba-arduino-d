#ifndef _WIFI_P2P_SUPPLICANT_H_
#define _WIFI_P2P_SUPPLICANT_H_

#include <p2p/p2p.h>

enum p2p_group_removal_reason {
	P2P_GROUP_REMOVAL_UNKNOWN,
	P2P_GROUP_REMOVAL_SILENT,
	P2P_GROUP_REMOVAL_FORMATION_FAILED,
	P2P_GROUP_REMOVAL_REQUESTED,
	P2P_GROUP_REMOVAL_IDLE_TIMEOUT,
	P2P_GROUP_REMOVAL_UNAVAILABLE,
	P2P_GROUP_REMOVAL_GO_ENDING_SESSION
};

/**
 * struct wpa_scan_res - Scan result for an BSS/IBSS
 * @flags: information flags about the BSS/IBSS (WPA_SCAN_*)
 * @bssid: BSSID
 * @freq: frequency of the channel in MHz (e.g., 2412 = channel 1)
 * @beacon_int: beacon interval in TUs (host byte order)
 * @caps: capability information field in host byte order
 * @qual: signal quality
 * @noise: noise level
 * @level: signal level
 * @tsf: Timestamp
 * @age: Age of the information in milliseconds (i.e., how many milliseconds
 * ago the last Beacon or Probe Response frame was received)
 * @ie_len: length of the following IE field in octets
 * @beacon_ie_len: length of the following Beacon IE field in octets
 *
 * This structure is used as a generic format for scan results from the
 * driver. Each driver interface implementation is responsible for converting
 * the driver or OS specific scan results into this format.
 *
 * If the driver does not support reporting all IEs, the IE data structure is
 * constructed of the IEs that are available. This field will also need to
 * include SSID in IE format. All drivers are encouraged to be extended to
 * report all IEs to make it easier to support future additions.
 */
struct wpa_scan_res {
	unsigned int flags;
	u8 bssid[ETH_ALEN];
	int freq;
	u16 beacon_int;
	u16 caps;
	int qual;
	int noise;
	int level;
	u64 tsf;
	unsigned int age;
	size_t ie_len;
	size_t beacon_ie_len;
	u8 *ie;
	/*
	 * Followed by ie_len octets of IEs from Probe Response frame (or if
	 * the driver does not indicate source of IEs, these may also be from
	 * Beacon frame). After the first set of IEs, another set of IEs may
	 * follow (with beacon_ie_len octets of data) if the driver provides
	 * both IE sets.
	 */
};

/** P2P device scanned (only basic information) */
#define P2P_EVENT_DEVICE_SCANNED "P2P-DEVICE-SCANNED "

/** P2P device found */
#define P2P_EVENT_DEVICE_FOUND "P2P-DEVICE-FOUND "

/** P2P device lost */
#define P2P_EVENT_DEVICE_LOST "P2P-DEVICE-LOST "

/** P2P find stopped */
#define P2P_EVENT_FIND_STOPPED "P2P-FIND-STOPPED "

/** A P2P device requested GO negotiation, but we were not ready to start the
 * negotiation */
#define P2P_EVENT_GO_NEG_REQUEST "P2P-GO-NEG-REQUEST "
#define P2P_EVENT_GO_NEG_SUCCESS "P2P-GO-NEG-SUCCESS "
#define P2P_EVENT_GO_NEG_FAILURE "P2P-GO-NEG-FAILURE "
#define P2P_EVENT_GROUP_FORMATION_SUCCESS "P2P-GROUP-FORMATION-SUCCESS "
#define P2P_EVENT_GROUP_FORMATION_FAILURE "P2P-GROUP-FORMATION-FAILURE "

/* P2P Group started or removed */
#define P2P_EVENT_GROUP_STARTED "P2P-GROUP-STARTED "
#define P2P_EVENT_GROUP_REMOVED "P2P-GROUP-REMOVED "

#define AP_STA_CONNECTED "AP-STA-CONNECTED "
#define AP_STA_DISCONNECTED "AP-STA-DISCONNECTED "

int wifi_p2p_rx_mgnt(u8 *data, u16 len, u8 channel);
int wifi_p2p_connect(const u8 *peer_addr,
		     const char *pin, enum p2p_wps_method wps_method,
		     int persistent_group, int auto_join, int join, int auth,
		     int go_intent, int freq, int persistent_id, int pd,
		     int ht40);
void wifi_p2p_scan_res_handler(void);
int wifi_p2p_start_wps(void *res);
void wifi_p2p_set_state(u8 p2p_state);
int wifi_p2p_group_notify_assoc(u8 *buf, u16 buf_len);
int wifi_p2p_group_notify_disassoc(u8 *addr);

int wifi_cmd_p2p_listen(unsigned int timeout);
void wifi_cmd_p2p_find(void);
void wifi_cmd_p2p_peers(void);
void wifi_cmd_p2p_info(void);
int wifi_cmd_p2p_connect(u8 *dest, enum p2p_wps_method config_method, char *pin);
void wifi_cmd_p2p_disconnect(void);

int wifi_p2p_init(u8 *dev_addr, int go_intent, u8 listen_ch, u8 op_ch);
void wifi_p2p_deinit(void);
int is_wifi_p2p_initialized();
void wifi_p2p_set_dev_name(const char *dev_name);
void wifi_p2p_set_manufacturer(const char *manufacturer);
void wifi_p2p_set_model_name(const char *model_name);
void wifi_p2p_set_model_number(const char *model_number);
void wifi_p2p_set_serial_number(const char *serial_number);
void wifi_p2p_set_pri_dev_type(const u8 *pri_dev_type);
void wifi_p2p_set_ssid(const char *ssid_in);
void wifi_p2p_set_config_methods(u16 config_methods);
void wifi_p2p_init_auto_go_params(void *res, u8 *passphrase,u8 channel);
int wifi_p2p_start_auto_go(void *res);

#endif //_WIFI_P2P_SUPPLICANT_H_

