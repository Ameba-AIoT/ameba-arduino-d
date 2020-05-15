#ifndef _MODULE_AAC_H
#define _MODULE_AAC_H

#include <stdint.h>
#include "mmf2_module.h"

#include "faac.h"

#define CMD_AAC_SET_PARAMS     		MM_MODULE_CMD(0x00)  // set parameter
#define CMD_AAC_GET_PARAMS     		MM_MODULE_CMD(0x01)  // get parameter
#define CMD_AAC_SAMPLERATE 			MM_MODULE_CMD(0x02)
#define CMD_AAC_CHANNEL				MM_MODULE_CMD(0x03)
#define CMD_AAC_BITLENGTH			MM_MODULE_CMD(0x04)
#define CMD_AAC_MEMORY_SIZE			MM_MODULE_CMD(0x07)
#define CMD_AAC_BLOCK_SIZE			MM_MODULE_CMD(0x08)
#define CMD_AAC_MAX_FRAME_SIZE		MM_MODULE_CMD(0x09)
#define CMD_AAC_INIT_MEM_POOL		MM_MODULE_CMD(0x0a)

#define CMD_AAC_APPLY				MM_MODULE_CMD(0x20)  // for hardware module

typedef struct aac_param_s{
    uint32_t sample_rate;	// 8000
	uint32_t channel;		// 1
	uint32_t bit_length;	// 16
	uint32_t mpeg_version;	// 16
	
	uint32_t mem_total_size;
	uint32_t mem_block_size;
	uint32_t mem_frame_size;	
	
	int samples_input;
	int max_bytes_output;
    //...
}aac_params_t;

typedef struct aac_ctx_s{
    void* parent;
    
	faacEncHandle faac_enc;
	
	void* mem_pool;
	aac_params_t params;
	
	uint8_t *cache;
	uint32_t cache_idx;
}aac_ctx_t;

extern mm_module_t aac_module;

#endif
