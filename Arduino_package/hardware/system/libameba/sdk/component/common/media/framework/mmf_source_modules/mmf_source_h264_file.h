#ifndef _MMF_SOURCE_H264_FILE_H_
#define _MMF_SOURCE_H264_FILE_H_

#include "sample_h264.h"
#include "memory_encoder.h"
#include "encoder_buffer_handler.h"
#include "timer_api.h"

#define H264F_DBG_SHOW 0
#define H264F_MAX_BUFFER_COUNT 6

struct  h264f_enc_buf_handle{
	struct enc_buf_data enc_data[H264F_MAX_BUFFER_COUNT];
	struct enc_list_handle enc_list;
};

struct  h264f_context
{
	unsigned int fps;
	void *source_addr;
	unsigned char *outputBuffer;
	struct mem_info mem_info_value;
	struct encoder_list_head encoder_lh;
	_sema	encoder_output_sema;
};

void* h264f_mod_open(void);
void h264f_mod_close(void* ctx);
int h264f_mod_set_param(void* ctx, int cmd, int arg);
int h264f_mod_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/
#define CMD_H264F_SET_FPS					0x10
#define CMD_H264F_SET_APPLY					0x11

#define CMD_H264F_MEMORY_SIZE				0X20
#define CMD_H264F_BLOCK_SIZE				0X21
#define CMD_H264F_MAX_FRAME_SIZE			0X22

#endif