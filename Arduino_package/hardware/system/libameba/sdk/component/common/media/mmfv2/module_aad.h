#ifndef _MODULE_AAD_H
#define _MODULE_AAD_H

#include <stdint.h>

#include "aacdec.h"

#define CMD_AAD_SET_PARAMS     		MM_MODULE_CMD(0x00)  // set parameter
#define CMD_AAD_GET_PARAMS     		MM_MODULE_CMD(0x01)  // get parameter
#define CMD_AAD_SAMPLERATE 			MM_MODULE_CMD(0x02)
#define CMD_AAD_CHANNEL				MM_MODULE_CMD(0x03)
#define CMD_AAD_STREAM_TYPE			MM_MODULE_CMD(0x04)


#define CMD_AAD_APPLY				MM_MODULE_CMD(0x20)  // for hardware module

#define TYPE_ADTS		0
#define TYPE_RTP_RAW	1
#define TYPE_TS			2

typedef struct aad_param_s{
	uint32_t type;
    uint32_t sample_rate;	// 8000
	uint32_t channel;		// 1
}aad_params_t;

typedef void (*aad_parser_t)(void*, void*, int);

typedef struct aad_ctx_s{
    void* parent;
    
	HAACDecoder aad;
	
	aad_params_t params;
	
	uint16_t data_cache_len;
	uint16_t data_cache_size;
	uint8_t *data_cache;
	uint8_t *decode_buf;
	
	aad_parser_t parser;
}aad_ctx_t;

extern mm_module_t aad_module;

#endif