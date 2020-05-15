#ifndef _MODULE_RTSP2_H
#define _MODULE_RTSP2_H

#include "rtsp/rtsp_api.h"
#include "mmf2_module.h"

#define TIME_SYNC_UNDEF         0
#define TIME_SYNC_EN            1
#define TIME_SYNC_DIS           2


#define RTSP2_NON_BLOCK_TYPE     0
#define RTSP2_BLOCK_TYPE         1

#define STREAM_FLOW_ID_BASE 0

#define CMD_RTSP2_SET_PARAMS			MM_MODULE_CMD(0x00)
#define CMD_RTSP2_GET_PARAMS			MM_MODULE_CMD(0x01)
#define CMD_RTSP2_SET_STREAMMING		MM_MODULE_CMD(0x02)
#define CMD_RTSP2_SELECT_STREAM 		MM_MODULE_CMD(0x03)
#define CMD_RTSP2_SET_FRAMERATE			MM_MODULE_CMD(0x04)
#define CMD_RTSP2_SET_BITRATE			MM_MODULE_CMD(0x05)
#define CMD_RTSP2_SET_SAMPLERATE		MM_MODULE_CMD(0x06)
#define CMD_RTSP2_SET_CHANNEL			MM_MODULE_CMD(0x07)
#define CMD_RTSP2_SET_SPS				MM_MODULE_CMD(0x08)
#define CMD_RTSP2_SET_PPS				MM_MODULE_CMD(0x09)
#define CMD_RTSP2_SET_LEVEL				MM_MODULE_CMD(0x0a)
#define CMD_RTSP2_SET_CODEC				MM_MODULE_CMD(0x0b)
#define CMD_RTSP2_SET_FLAG				MM_MODULE_CMD(0x0c)
#define CMD_RTSP2_SET_APPLY				MM_MODULE_CMD(0x0d)
#define CMD_RTSP2_USE_RTP_TICK_INC		MM_MODULE_CMD(0x0e)
#define CMD_RTSP2_SET_PORT				MM_MODULE_CMD(0x0f)                 


#define CMD_RTSP2_SET_START_CB			MM_MODULE_CMD(0x10)
#define CMD_RTSP2_SET_STOP_CB			MM_MODULE_CMD(0x11)
#define CMD_RTSP2_SET_PAUSE_CB			MM_MODULE_CMD(0x12)
#define CMD_RTSP2_SET_CUSTOM_CB			MM_MODULE_CMD(0x13)

#define CMD_RTSP2_SET_DROP_TIME                 MM_MODULE_CMD(0x14)
#define CMD_RTSP2_SET_BLOCK_TYPE                MM_MODULE_CMD(0x15)
#define CMD_RTSP2_SET_SYNC_MODE                 MM_MODULE_CMD(0x16)

typedef struct rtsp2_params_s{
	uint32_t type;
	union{
		struct rtsp_video_param_s{
			uint32_t codec_id;
			uint32_t fps;
			uint32_t bps;
			uint32_t ts_flag;
			char* sps;
			char* pps;
			char* lv;
		}v;
		struct rtsp_audio_param_s{
			uint32_t codec_id;
			uint32_t channel;
			uint32_t samplerate;
		}a;
	}u;
}rtsp2_params_t;

typedef struct rtsp2_ctx_s{
	void* parent;
	
	struct rtsp_context *rtsp;
	
	rtsp2_params_t params[2];
        uint32_t block_type;
}rtsp2_ctx_t;



extern mm_module_t rtsp2_module;

#endif