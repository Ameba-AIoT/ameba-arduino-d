#ifndef _MODULE_H264_H
#define _MODULE_H264_H

#include "h264_encoder.h"
#include "mmf2_module.h"

#define CMD_H264_SET_PARAMS			MM_MODULE_CMD(0x00)
#define CMD_H264_GET_PARAMS			MM_MODULE_CMD(0x01)
#define CMD_H264_SET_HEIGHT			MM_MODULE_CMD(0x02)
#define CMD_H264_SET_WIDTH			MM_MODULE_CMD(0x03)
#define CMD_H264_BITRATE			MM_MODULE_CMD(0x04)
#define CMD_H264_FPS				MM_MODULE_CMD(0x05)
#define CMD_H264_GOP				MM_MODULE_CMD(0x06)
#define CMD_H264_MEMORY_SIZE		MM_MODULE_CMD(0x07)
#define CMD_H264_BLOCK_SIZE			MM_MODULE_CMD(0x08)
#define CMD_H264_MAX_FRAME_SIZE		MM_MODULE_CMD(0x09)
#define CMD_H264_RCMODE				MM_MODULE_CMD(0x0a)
#define CMD_H264_SET_RCPARAM		MM_MODULE_CMD(0x0b)
#define CMD_H264_GET_RCPARAM		MM_MODULE_CMD(0x0c)
#define CMD_H264_INIT_MEM_POOL		MM_MODULE_CMD(0x0d)
#define CMD_H264_FORCE_IFRAME		MM_MODULE_CMD(0x0e)
#define CMD_H264_SET_RCADVPARAM		MM_MODULE_CMD(0x10)
#define CMD_H264_GET_RCADVPARAM		MM_MODULE_CMD(0x11)
#define CMD_H264_SET_ROIPARM		MM_MODULE_CMD(0x12)
#define CMD_H264_SET_ROI		MM_MODULE_CMD(0x13)
#define CMD_H264_SET_QPCHROMA_OFFSET    MM_MODULE_CMD(0x14)
#define CMD_H264_SET_FORCE_DROP_FRAME   MM_MODULE_CMD(0x15)

#define CMD_SNAPSHOT_ENCODE_CB		MM_MODULE_CMD(0x30)

#define CMD_H264_APPLY				MM_MODULE_CMD(0x20)
#define CMD_H264_UPDATE				MM_MODULE_CMD(0x21)
#define CMD_H264_PAUSE				MM_MODULE_CMD(0x22)

typedef struct h264_ctx_s{
	void* parent;
 	
	struct h264_context* h264_cont;
	
	void* mem_pool;
}h264_ctx_t;

typedef struct h264_params_s{
	uint32_t width;
	uint32_t height;
	uint32_t bps;
	uint32_t fps;
	uint32_t gop;
	uint32_t rc_mode;
	uint32_t auto_qp;			// 1 : enable auto QP, discard qpMin, qpMax
	uint32_t rst_rcerr;			// 1 : reset rc error when rate change
        uint32_t rotation;                      // 0 : Rotate 0 1:Rotate 90R 2: Rorate 90L
	uint32_t mem_total_size;
	uint32_t mem_block_size;
	uint32_t mem_frame_size;
}h264_params_t;

typedef struct h264_rc_parm_s
{
	unsigned int rcMode;
	unsigned int iQp;		// for fixed QP
	unsigned int pQp;		// for fixed QP
	unsigned int minQp;		// for CBR/VBR
	unsigned int minIQp;	// for CBR/VBR
	unsigned int maxQp;		// for CBR/VBR
}h264_rc_parm_t;

typedef struct h264_rc_adv_parm_s
{
  	unsigned int rc_adv_enable;
  	unsigned int maxBps;		// for VBR
	unsigned int minBps;		// for VBR
	
	int intraQpDelta;
	int mbQpAdjustment;
	unsigned int mbQpAutoBoost;
}h264_rc_adv_parm_t;

typedef struct h264_roi_parm_s
{
	unsigned int enable;
	unsigned int left;		// for fixed QP
	unsigned int right;		// for fixed QP
	unsigned int top;		// for CBR/VBR
	unsigned int bottom;	// for CBR/VBR
}h264_roi_parm_t;

extern mm_module_t h264_module;


#endif