#ifndef _MODULE_JPEG_H
#define _MODULE_JPEG_H

#include "jpeg_encoder.h"
#include "mmf2_module.h"

#define CMD_JPEG_SET_PARAMS			MM_MODULE_CMD(0x00)
#define CMD_JPEG_GET_PARAMS			MM_MODULE_CMD(0x01)
#define CMD_JPEG_SET_HEIGHT			MM_MODULE_CMD(0x02)
#define CMD_JPEG_SET_WIDTH			MM_MODULE_CMD(0x03)
#define CMD_JPEG_LEVEL				MM_MODULE_CMD(0x04)
#define CMD_JPEG_FPS				MM_MODULE_CMD(0x05)
#define CMD_JPEG_HEADER_TYPE		MM_MODULE_CMD(0x06)
#define CMD_JPEG_MEMORY_SIZE		MM_MODULE_CMD(0x07)
#define CMD_JPEG_BLOCK_SIZE			MM_MODULE_CMD(0x08)
#define CMD_JPEG_MAX_FRAME_SIZE		MM_MODULE_CMD(0x09)
#define CMD_JPEG_INIT_MEM_POOL		MM_MODULE_CMD(0x0a)
#define CMD_JPEG_GET_ENCODER		MM_MODULE_CMD(0x0b)

#define CMD_SNAPSHOT_CB				MM_MODULE_CMD(0x10)
#define CMD_JPEG_CHANGE_PARM_CB		MM_MODULE_CMD(0x11)
#define CMD_JPEG_UPDATE                 MM_MODULE_CMD(0x12)        

#define CMD_JPEG_APPLY				MM_MODULE_CMD(0x20)

typedef struct jpeg_ctx_s{
	void* parent;
 	
	struct jpeg_context* jpeg_cont;
	
	void* mem_pool;
}jpeg_ctx_t;

typedef struct jpeg_params_s{
	uint32_t width;
	uint32_t height;
	uint32_t level;
	uint32_t fps;
	uint32_t mem_total_size;
	uint32_t mem_block_size;
	uint32_t mem_frame_size;
}jpeg_params_t;

extern mm_module_t jpeg_module;


#endif //_MODULE_JPEG_H