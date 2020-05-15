#ifndef _MODULE_ARRAY_H
#define _MODULE_ARRAY_H

#include <FreeRTOS.h>
#include <Freertos_service.h>
#include <task.h>
#include <stdint.h>
#include "timer_api.h"

#define DEFAULT_G711_LEN			160
#define DEFAULT_AAC_LEN				2048

#define ARRAY_MODE_ONCE		0
#define ARRAY_MODE_LOOP		1

#define CMD_ARRAY_SET_PARAMS		MM_MODULE_CMD(0x00)  // set parameter
#define CMD_ARRAY_GET_PARAMS		MM_MODULE_CMD(0x01)  // get parameter
#define CMD_ARRAY_SET_ARRAY			MM_MODULE_CMD(0x02)
#define CMD_ARRAY_SET_MODE			MM_MODULE_CMD(0x03)
#define CMD_ARRAY_GET_STATE			MM_MODULE_CMD(0x04)
#define CMD_ARRAY_STREAMING			MM_MODULE_CMD(0x05)


#define CMD_ARRAY_APPLY				MM_MODULE_CMD(0x20)  // for hardware module

typedef struct array_param_s{
	uint32_t type;
	uint32_t codec_id;
	uint8_t mode;
	union{
		struct array_video_param_s{
			uint32_t fps;
			uint8_t h264_nal_size;
		}v;
		struct array_audio_param_s{
			uint32_t channel;
			uint32_t samplerate;
			uint32_t sample_bit_length;
			uint32_t frame_size;
		}a;
	}u;
}array_params_t;

typedef struct array_s{
	uint32_t data_addr;
	uint32_t data_len;
	uint32_t data_offset;
}array_t;

typedef struct array_ctx_s{
	void* parent;
	TaskHandle_t task;
	_sema up_sema;
	gtimer_t frame_timer;
	uint32_t frame_timer_period; // us
	
	array_params_t params;
	array_t array;
	// flag
	int stop;
}array_ctx_t;

extern mm_module_t array_module;
#endif