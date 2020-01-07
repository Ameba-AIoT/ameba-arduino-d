#ifndef _MMF_SINK_RTSP_H_
#define _MMF_SINK_RTSP_H_

#include "rtsp/rtsp_api.h"
#include "sockets.h"
#include "lwip/netif.h"

void rtsp_mod_close(void* ctx);
void* rtsp_mod_open(void);
int rtsp_mod_set_param(void* ctx, int cmd, int arg);
int rtsp_mod_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/
#define CMD_RSTP_SET_FRAMERATE          0x10
#define CMD_RSTP_SET_BITRATE            0x11
#define CMD_RSTP_SET_SAMPLERATE         0x12
#define CMD_RSTP_SET_CHANNEL            0x13
#define CMD_RSTP_SET_SPS                0x14
#define CMD_RSTP_SET_PPS                0x15
#define CMD_RSTP_SET_LEVEL              0x16
#define CMD_RSTP_SET_APPLY              0x17
#define CMD_RSTP_SET_CODEC              0x18
#define CMD_RSTP_SET_START_CB           0x19
#define CMD_RSTP_SET_STOP_CB            0x1a
#define CMD_RSTP_SET_PAUSE_CB           0x1b
#define CMD_RSTP_SET_CUSTOM_CB          0x1c

#endif
