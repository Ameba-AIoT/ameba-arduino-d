#ifndef _MODULE_HTTPFS_H
#define _MODULE_HTTPFS_H

#include <stdint.h>
#include <osdep_service.h>
#include "fatfs_sdcard_api.h"

#define CMD_HTTPFS_SET_PARAMS     	MM_MODULE_CMD(0x00)  // set parameter
#define CMD_HTTPFS_GET_PARAMS     	MM_MODULE_CMD(0x01)  // get parameter
#define CMD_HTTPFS_APPLY		MM_MODULE_CMD(0x20)  // for hardware module
#define CMD_HTTPFS_STREAM_STOP		MM_MODULE_CMD(0x23)  // stop stream
#define CMD_HTTPFS_SET_RESPONSE_CB      MM_MODULE_CMD(0x14)

typedef struct httpfs_param_s{
	char fileext[4]; 
	char filedir[32];
	char request_string[128];
	uint32_t fatfs_buf_size;
}httpfs_params_t;

typedef struct httpfs_ctx_s{
    	void* parent;
	TaskHandle_t task;
	_sema up_sema;
	
	httpfs_params_t params;
	fatfs_sd_params_t fatfs_params;
	int (*cb_response)(void);
}httpfs_ctx_t;

extern mm_module_t httpfs_module;
#endif