#ifndef _MMF_SOURCE_AAC_ENCODE_H_
#define _MMF_SOURCE_AAC_ENCODE_H_

#include "aac_util.h"
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "aac_util.h"
#include <math.h>
#include "psych.h"
#include "faac.h"

#include "sample_aacenc.h"
#include "faac_api.h"

#include "memory_encoder.h"
#include "encoder_buffer_handler.h"

#define AAC_MAX_BUFFER_COUNT 6
#define AAC_DISABLE 0
#define AAC_ENABLE  1

#define AAC_DEBUG_SHOW 0

struct aac_enc_buf_handle{
	struct enc_buf_data enc_data[AAC_MAX_BUFFER_COUNT];
	struct enc_list_handle enc_list;
};

struct aacen_context
{
	faacEncHandle hEncoder;
	int bit_length;
	int sample_rate;
	int channel;
	int samplesInput;
	int maxBytesOutput;
	void *source_addr;
	unsigned char *outputBuffer;
	struct mem_info mem_info_value;
	struct encoder_list_head encoder_lh;
	_sema	encoder_output_sema;
};

void* aac_encode_mod_open(void);
void aacf_encode_close(void* ctx);
int aacf_encode_mod_set_param(void* ctx, int cmd, int arg);
int aac_encode_mod_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/
#define CMD_AACEN_SET_BIT_LENGTH			0x10
#define CMD_AACEN_SET_SAMPLE_RATE			0x11
#define CMD_AACEN_SET_APPLY					0x12
#define CMD_AACEN_SET_CHANEEL				0X13

#define CMD_AACEN_MEMORY_SIZE				0X30
#define CMD_AACEN_BLOCK_SIZE				0X31
#define CMD_AACEN_MAX_FRAME_SIZE			0X32


#endif