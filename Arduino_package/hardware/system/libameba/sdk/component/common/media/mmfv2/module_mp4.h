#ifndef _MODULE_MP4_H
#define _MODULE_MP4_H

#include <stdint.h>
#include "mmf2_module.h"

#include "fatfs_sdcard_api.h"
#include "mp4_muxer.h"

#define CMD_MP4_SET_PARAMS				MM_MODULE_CMD(0x00)
#define CMD_MP4_GET_PARAMS				MM_MODULE_CMD(0x01)
#define CMD_MP4_SET_HEIGHT				MM_MODULE_CMD(0x02)
#define CMD_MP4_SET_WIDTH				MM_MODULE_CMD(0x03)
#define CMD_MP4_SET_FPS					MM_MODULE_CMD(0x04)
#define CMD_MP4_SET_GOP					MM_MODULE_CMD(0x05)
#define CMD_MP4_SET_SAMPLERATE			MM_MODULE_CMD(0x06)
#define CMD_MP4_SET_CHANNEL				MM_MODULE_CMD(0x07)
#define CMD_MP4_SET_RECORD_LENGTH		MM_MODULE_CMD(0x08)
#define CMD_MP4_GET_RECORD_LENGTH		MM_MODULE_CMD(0x09)
#define CMD_MP4_SET_RECORD_TYPE			MM_MODULE_CMD(0x0a)
#define CMD_MP4_GET_RECORD_TYPE			MM_MODULE_CMD(0x0b)
#define CMD_MP4_SET_RECORD_FILE_NAME	MM_MODULE_CMD(0x0c)
#define CMD_MP4_GET_RECORD_FILE_NAME	MM_MODULE_CMD(0x0d)
#define CMD_MP4_SET_FATFS_BUF_SIZE		MM_MODULE_CMD(0x0e)
#define CMD_MP4_GET_FATFS_PARAMS		MM_MODULE_CMD(0x0f)
#define CMD_MP4_START					MM_MODULE_CMD(0x10)
#define CMD_MP4_STOP					MM_MODULE_CMD(0x11)
#define CMD_MP4_GET_STATUS				MM_MODULE_CMD(0x12)
#define CMD_MP4_RESET_FILE_INDEX		MM_MODULE_CMD(0x13)
#define CMD_MP4_SET_STOP_CB		MM_MODULE_CMD(0x14) //Every record loop will enter the callback
#define CMD_MP4_SET_END_CB		MM_MODULE_CMD(0x15) //Only final loop will ebter the callback
#define CMD_MP4_LOOP_MODE				MM_MODULE_CMD(0x16)
#define CMD_MP4_SET_ERROR_CB            MM_MODULE_CMD(0x17)
typedef struct mp4_param_s{
	uint32_t width;
	uint32_t height;
	uint32_t fps;
	uint32_t gop;
	
	uint32_t sample_rate;
	uint32_t channel;
	
	uint32_t record_length;
	uint32_t record_type;
	uint32_t record_file_num;
	char record_file_name[32];
	uint32_t fatfs_buf_size;
}mp4_params_t;

typedef struct mp4_ctx_s{
	void* parent;
	
	pmp4_context mp4_muxer;
	
	mp4_params_t params;
	fatfs_sd_params_t fatfs_params;
}mp4_ctx_t;

extern mm_module_t mp4_module;

#endif
