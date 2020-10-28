#ifndef _MODULE_DUP_H
#define _MODULE_DUP_H

#include <stdint.h>

#define CMD_DUP_SET_PARAMS     	MM_MODULE_CMD(0x00)  // set parameter
#define CMD_DUP_GET_PARAMS     	MM_MODULE_CMD(0x01)  // get parameter
#define CMD_DUP_CODECID	 		MM_MODULE_CMD(0x02)
#define CMD_DUP_LENGTH			MM_MODULE_CMD(0x03)
#define CMD_DUP_CODECMODE		MM_MODULE_CMD(0x04)


#define CMD_DUP_APPLY			MM_MODULE_CMD(0x20)  // for hardware module


typedef struct dup_param_s{
    uint32_t codec_id;		// AV_CODEC_ID_PCM_RAW
	uint32_t buf_len;		// output buffer length
}dup_params_t;

typedef struct dup_ctx_s{
    void* parent;
    
	dup_params_t params;
}dup_ctx_t;

extern mm_module_t dup_module;

#endif