#ifndef _MMF_SINK_MP4_H_
#define _MMF_SINK_MP4_H_

#include "FreeRTOS.h"
#include "task.h"
#include <platform/platform_stdlib.h>
#include "basic_types.h"
#include "platform_opts.h"
#include "section_config.h"

#include "ff.h"
#include <fatfs_ext/inc/ff_driver.h>
#include "sdio_host.h"
#include "sdio_combine.h"
#include <disk_if/inc/sdcard.h>

#include "sockets.h"
#include "lwip/netif.h"
#if defined(CONFIG_PLATFORM_8195BHP)
#include "mp4_muxer.h"
#endif
#ifdef  CONFIG_PLATFORM_8195A
#include "mp4_encap.h"
#endif

void mp4_mod_close(void* ctx);
void* mp4_mod_open(void);
int mp4_mod_set_param(void* ctx, int cmd, int arg);
int mp4_mod_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/
#define CMD_MP4_SET_HEIGHT              0x10
#define CMD_MP4_SET_WIDTH               0x11
#define CMD_MP4_SET_FRAMERATE           0x12
#define CMD_MP4_SET_GOP                 0x13
#define CMD_MP4_SET_SAMPLERATE          0x14
#define CMD_MP4_SET_CHANNEL_CNT         0x15
#define CMD_MP4_SET_ST_PERIOD           0x16
#define CMD_MP4_SET_ST_TOTAL            0x17
#define CMD_MP4_SET_ST_TYPE             0x18
#define CMD_MP4_SET_ST_FILENAME         0x19
#define CMD_MP4_SET_ST_START            0x1a
#define CMD_MP4_SET_ST_STOP             0x1b
#define CMD_MP4_SET_START_CB            0x1c
#define CMD_MP4_SET_STOP_CB             0x1d
#define CMD_MP4_GET_STATUS              0x1e

#define CMD_MP4_SET_FATFS_DRV_NUM       0x20
#define CMD_MP4_SET_FATFS_DRV           0x21
#define CMD_MP4_SET_FATFS_SD            0x22
#define CMD_MP4_SET_FATFS_BUF_SIZE      0x23

#endif
