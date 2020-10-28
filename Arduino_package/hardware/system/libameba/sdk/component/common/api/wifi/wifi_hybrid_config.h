#ifndef WIFI_HYBRID_CONFIG_H
#define WIFI_HYBRID_CONFIG_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "udp.h"
#include <sockets.h>
#if CONFIG_LWIP_LAYER
#include <lwip_netconf.h>
#endif
#include <osdep_service.h>
#include "platform_stdlib.h"
#include "wifi_constants.h"
#include "wifi_structures.h"
#if CONFIG_AIRKISS
/* airkiss 2.0 */
#include "airkiss.h"
#endif

#define WHC_DBG_FULL 2
#define WHC_DBG_IMPORTANT 1
#define WHC_DBG_NOTHING 0
/* if too much log, change this */
#define WHC_DBG_LEVEL WHC_DBG_IMPORTANT
#define WHC_TRACE(LEVEL, fmt, ...) \
do {	\
    if (LEVEL <= WHC_DBG_LEVEL) \
        printf(fmt , ##__VA_ARGS__); \
} while (0)	

enum airkiss_fix_reason {
	WHC_FIX_WITH_AIRKISS,
	WHC_FIX_WITH_SIMPLE_CONFIG,
 };

enum wifi_info_reason {
	WHC_FINISH_WITH_AIRKISS,
	WHC_FINISH_WITH_SIMPLE_CONFIG,

};
	
////////////////////////////// copied //////////////////////////////////////////
struct rtk_test_sc {
	int			pattern_type;
	unsigned char		smac[6];		/* the source mac of the profile packet, it maybe the Phone MAC */
	unsigned char		dmac[6];		/* the destination mac, it is the DUT wlan MAC for send ack packet */
	unsigned char		ssid[33];
	unsigned char		password[65];	
	unsigned int		ip_addr;	
	unsigned char		sync_pkt[9][6];		/* parser syn start */
	unsigned char		profile_pkt[256][6];
	unsigned int		profile_pkt_len;
	unsigned char 		plain_buf[256];
	unsigned int 		plain_len;
	unsigned char 		key_buf[32];		/* kek */
	unsigned int 		key_len;
	unsigned char 		crypt_buf[256];
	unsigned int 		crypt_len;
	int 			pattern_index; 		/* pattern start index */
	struct pattern_ops 	*pattern[64];		/* pattern array */
	int 			max_pattern_num; 	/* total register pattern num */
	unsigned char        	pin[65];
	unsigned char        	default_pin[65];
	unsigned char 		have_pin;
	unsigned short		device_type;
	unsigned char		device_name[64];
};

////////////////////////////////////////////////////////////////////////

#if CONFIG_AIRKISS
struct whc_airkiss {

	airkiss_context_t context;
	airkiss_config_t config;
	airkiss_result_t result;

	bool finished;
	u8 encrypt;

};
#endif

/* whc for short */
struct wifi_hybrid_config {

	/* trigger variable, set it at last*/
	bool wifi_info_ready;
	bool need_switch_channel;
	int wifi_info_ready_reason;

	/* this variable can be changed by external model*/

	int channel_switch_interval_ms;
	int timeout_val_ms;
	u32 start_time;
	u8 channel;/* after located certain AP, we make it fixed*/

	u8 sa[6];
	u8 da[6];
	u8 bssid[6];
	u8 ssid[32];
	int ssid_len;
	u8* password;
	int password_len;

	//wifi_hybrid_config state machine
	int cur_state;

	#if 0
	/* vendor protocol */
	struct rtk_sc *sc;
	int sync_bitmap;
	int profile_packet_count;
	bool need_check_sync_packet;
	bool need_check_profile_packet;
	#endif

#if CONFIG_AIRKISS
	struct whc_airkiss airkiss;
#endif
};

#if 1

struct rtk_sc {
	int			pattern_type;
	unsigned char		smac[6];		/* the source mac of the profile packet, it maybe the Phone MAC */
	unsigned char		dmac[6];		/* the destination mac, it is the DUT wlan MAC for send ack packet */
	unsigned char		ssid[33];
	unsigned char		password[65];	
	unsigned int		ip_addr;	
	unsigned char		sync_pkt[9][6];		/* parser syn start */
	unsigned char		profile_pkt[256][6];
	unsigned int		profile_pkt_len;
	unsigned char 		plain_buf[256];
	unsigned int 		plain_len;
	unsigned char 		key_buf[32];		/* kek */
	unsigned int 		key_len;
	unsigned char 		crypt_buf[256];
	unsigned int 		crypt_len;
	int 			pattern_index; 		/* pattern start index */
	struct pattern_ops 	*pattern[64];		/* pattern array */
	int 			max_pattern_num; 	/* total register pattern num */
	unsigned char        	pin[65];
	unsigned char        	default_pin[65];
	unsigned char 		have_pin;
	unsigned short		device_type;
	unsigned char		device_name[64];
};

#endif


int whc_vendor_init(struct wifi_hybrid_config *config);
void whc_vendor_deinit();
void whc_vendor_promisc_callback(unsigned char *buf, unsigned int len, void* userdata,
					struct wifi_hybrid_config *config);

/* simple config external API */
extern void remove_filter();
extern void deinit_test_data();
extern void filter_add_enable();
extern int init_test_data(char *custom_pin_code);
extern int SC_connect_to_AP(void);
extern int simple_config_result;
extern int is_promisc_callback_unlock;
extern struct rtk_test_sc *backup_sc_ctx;
extern unsigned char g_bssid[6];
extern unsigned char g_ssid[33];
extern int g_ssid_len;
extern int rtk_start_parse_packet(unsigned char *da, unsigned char *sa, int len,
				  	void * user_data, struct rtk_sc *backup_sc_ctx);

extern int wifi_set_pscan_chan(u8 * channel_list,u8 * pscan_config, u8 length);
extern int promisc_get_fixed_channel(void * fixed_bssid, u8 *ssid, int * ssid_length);
extern int wifi_set_channel(int channel);
extern int wifi_set_promisc(rtw_rcr_level_t enabled, void (*callback)(unsigned char*, unsigned int, void*), unsigned char len_used);
extern int wifi_connect(char *ssid,rtw_security_t security_type, char *password,int ssid_len,
		int password_len, int key_id, void *semaphore);
extern void whc_deinit(struct wifi_hybrid_config *);
extern void whc_entry(rtw_network_info_t *);

#endif

