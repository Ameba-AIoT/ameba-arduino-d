#ifndef _MMF_SINK_RTSP2_H_
#define _MMF_SINK_RTSP2_H_

#include "rtsp/rtsp_api.h"
#include "sockets.h"
#include "lwip/netif.h"

void rtsp2_mod_close(void* ctx);
void* rtsp2_mod_open(void);
int rtsp2_mod_set_param(void* ctx, int cmd, int arg);
int rtsp2_mod_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/
#define CMD_RSTP2_SELECT_CHANNEL         0x10
#define CMD_RSTP2_SET_FRAMERATE          0x11
#define CMD_RSTP2_SET_BITRATE            0x12
#define CMD_RSTP2_SET_SAMPLERATE         0x13
#define CMD_RSTP2_SET_CHANNEL            0x14
#define CMD_RSTP2_SET_SPS                0x15
#define CMD_RSTP2_SET_PPS                0x16
#define CMD_RSTP2_SET_LEVEL              0x17
#define CMD_RSTP2_SET_APPLY              0x18
#define CMD_RSTP2_SET_CODEC              0x19
#define CMD_RSTP2_SET_FLAG               0x1a
#define CMD_RSTP2_SET_START_CB           0x1b
#define CMD_RSTP2_SET_STOP_CB            0x1c
#define CMD_RSTP2_SET_PAUSE_CB           0x1d
#define CMD_RSTP2_SET_CUSTOM_CB          0x1e

#define TIME_SYNC_EN            0
#define TIME_SYNC_DIS           1

#endif