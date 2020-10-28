#ifndef __WIFI_MESH_H
#define __WIFI_MESH_H
/*****************************wifi_mesh.h****************************/
#include <drv_types.h>
#ifdef CONFIG_WIFI_MESH

typedef struct{
	u8 root_node;
	u8 connecting_ap;
	u8 config_by_bt;
	u8 rtw_mesh_enable;
	char mac_0[ETH_ALEN];
	char mac_1[ETH_ALEN];
	char mac_parent_1[ETH_ALEN];
	_sema rtw_mesh_sema;
}Rtw_mesh_t;

typedef enum{
	MESH_DATA_UP = 0,
	MESH_DATA_DOWN = 1,
	MESH_DATA_NO_LINK = 2,
};

struct mesh_rxcache {
	u16 	tid_rxseq[16];
};


//Register mesh rx callback
void rtw_mesh_init();
void rtw_mesh_callback_init(void (*callback)(unsigned char*, uint, ieee80211_frame_info_t*));
void rtw_mesh_deinit();

void rtw_mesh_scan_start_reg_set();
void rtw_mesh_scan_stop_reg_set();
extern void rtw_mesh_pre_connect(u8* bssid);
void rtw_mesh_connected_reg_set(u8 *val, u8 connected_route);

int rtw_mesh_set_channel(u8 channel);
void rtw_mesh_set_connecting_ap(bool val);
u8 *rtw_mesh_get_mac_address(int port);
void rtw_mesh_set_config_by_bt(bool val);
int rtw_mesh_get_sta_state();

//Send mesh data to dst
int rtw_mesh_tx(char* dst, char* data, u16 data_len, u8 up_or_down);

//set beacon format and notify hw to send beacon every 1024 us
int rtw_mesh_beacon_start(char *data, u16 data_len);

void rtw_mesh_beacon_stop();

//if processed by mesh, return TRUE, if processed by LWIP return FALSE
//used by rtw_recv_entry
int rtw_mesh_recv_func(void *rframe);

void rtw_mesh_set_root_node(bool val);
bool rtw_mesh_is_root_node();
#endif
#endif //__WIFI_MESH_H	

