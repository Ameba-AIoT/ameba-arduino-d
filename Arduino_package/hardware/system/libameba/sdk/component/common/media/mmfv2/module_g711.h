#ifndef _MODULE_G711_H
#define _MODULE_G711_H

#include <stdint.h>

#define CMD_G711_SET_PARAMS     	MM_MODULE_CMD(0x00)  // set parameter
#define CMD_G711_GET_PARAMS     	MM_MODULE_CMD(0x01)  // get parameter
#define CMD_G711_CODECID	 		MM_MODULE_CMD(0x02)
#define CMD_G711_LENGTH				MM_MODULE_CMD(0x03)
#define CMD_G711_CODECMODE			MM_MODULE_CMD(0x04)


#define CMD_G711_APPLY				MM_MODULE_CMD(0x20)  // for hardware module

#define G711_ENCODE 0
#define G711_DECODE 1

typedef struct g711_param_s{
    uint32_t codec_id;		// AV_CODEC_ID_PCMA or AV_CODEC_ID_PCMU
	uint32_t buf_len;		// output buffer length
	uint32_t mode;			// decode or encode
}g711_params_t;

typedef struct g711_ctx_s{
    void* parent;
    
	g711_params_t params;
}g711_ctx_t;

extern mm_module_t g711_module;

#endif
