#ifndef _MMF_SOURCE_H1V6_H264_H_
#define _MMF_SOURCE_H1V6_H264_H_

#include "basic_types.h"
#include "osdep_service.h"
#include <platform_opts.h>
#include "cmsis.h"
#include "rtl8195bhp_video.h"
#include "hal_encorder.h"
#include "hal_cache.h"
#include "section_config.h"

#include "memory_encoder.h"
#include "hantro_h264_encoder.h"
#include "isp_api.h"

void* h1v6_h264_open(void);
void h1v6_h264_close(void* ctx);
int h1v6_h264_set_param(void* ctx, int cmd, int arg);
int h1v6_h264_handle(void* ctx, void* b);

struct h1v6_h264_rc_parm
{
	unsigned int rcMode;
	unsigned int iQp;		// for fixed QP
	unsigned int pQp;		// for fixed QP
	unsigned int minQp;		// for CBR/VBR
	unsigned int minIQp;	// for CBR/VBR
	unsigned int maxQp;		// for CBR/VBR
};

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/
#define CMD_HANTRO_H264_SET_HEIGHT           0x10
#define CMD_HANTRO_H264_SET_WIDTH            0x11
#define CMD_HANTRO_H264_BITRATE              0x12
#define CMD_HANTRO_H264_FPS                  0x13
#define CMD_HANTRO_H264_GOP                  0x14
#define CMD_HANTRO_H264_RCMODE               0x15
#define CMD_HANTRO_H264_SET_RCPARAM          0x16
#define CMD_HANTRO_H264_GET_RCPARAM          0x17
#define CMD_HANTRO_H264_APPLY                0x18

#define CMD_ISP_STREAMID                     0X20
#define CMD_ISP_HW_SLOT                      0X21
#define CMD_ISP_SW_SLOT                      0X22

#define CMD_H1V6_MEMORY_SIZE                 0X30
#define CMD_H1V6_BLOCK_SIZE                  0X31
#define CMD_H1V6_MAX_FRAME_SIZE              0X32

#define CMD_SNAPSHOT_ENCODE_CB               0x40
#define CMD_HANTRO_H264_CHANGE_PARM_CB       0x41
#endif