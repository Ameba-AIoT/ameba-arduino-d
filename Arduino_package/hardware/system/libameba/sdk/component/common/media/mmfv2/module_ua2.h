#ifndef _MODULE_UA2_H
#define _MODULE_UA2_H


#include "sip/re/inc/re.h"
#include "sip/re/inc/baresip.h"
#include "sip/ua_api.h"
#include "mmf2_module.h"

#define TIME_SYNC_UNDEF         0
#define TIME_SYNC_EN            1
#define TIME_SYNC_DIS           2

#define STREAM_FLOW_ID_BASE 0

#define CMD_UA2_SET_PARAMS			MM_MODULE_CMD(0x00)
#define CMD_UA2_GET_PARAMS			MM_MODULE_CMD(0x01)
#define CMD_UA2_SET_STREAMMING		MM_MODULE_CMD(0x02)
#define CMD_UA2_SELECT_STREAM 		MM_MODULE_CMD(0x03)
#define CMD_UA2_SET_FRAMERATE			MM_MODULE_CMD(0x04)
#define CMD_UA2_SET_BITRATE			MM_MODULE_CMD(0x05)
#define CMD_UA2_SET_SAMPLERATE		MM_MODULE_CMD(0x06)
#define CMD_UA2_SET_CHANNEL			MM_MODULE_CMD(0x07)
#define CMD_UA2_SET_SPS				MM_MODULE_CMD(0x08)
#define CMD_UA2_SET_PPS				MM_MODULE_CMD(0x09)
#define CMD_UA2_SET_LEVEL				MM_MODULE_CMD(0x0a)
#define CMD_UA2_SET_CODEC				MM_MODULE_CMD(0x0b)
#define CMD_UA2_SET_FLAG				MM_MODULE_CMD(0x0c)
#define CMD_UA2_SET_APPLY				MM_MODULE_CMD(0x0d)
#define CMD_UA2_USE_RTP_TICK_INC		MM_MODULE_CMD(0x0e)

#define CMD_UA2_SET_REGISTER				MM_MODULE_CMD(0x0f)
#define CMD_UA2_SET_FROM_URI				MM_MODULE_CMD(0x10)
#define CMD_UA2_SET_TO_URI				MM_MODULE_CMD(0x11)

typedef struct ua2_params_s{
	uint32_t type;
	union{
		struct ua_video_param_s{
			uint32_t codec_id;
			uint32_t fps;
			uint32_t bps;
			uint32_t ts_flag;
			char* sps;
			char* pps;
			char* lv;
		}v;
		struct ua_audio_param_s{
			uint32_t codec_id;
			uint32_t channel;
			uint32_t samplerate;
		}a;
	}u;
}ua2_params_t;

typedef struct ua2_ctx_s{
	void* parent;
	
	struct ua_context *ua;
	
	ua2_params_t params[2];
}ua2_ctx_t;



extern mm_module_t ua2_module;

#endif