#ifndef _MODULE_ISP_H
#define _MODULE_ISP_H

#include <stdint.h>
#include <osdep_service.h>
#include "mmf2_module.h"

#include "isp_api.h"
#include "sensor.h"

#define CMD_ISP_SET_PARAMS     	MM_MODULE_CMD(0x00)  // set parameter
#define CMD_ISP_GET_PARAMS     	MM_MODULE_CMD(0x01)  // get parameter
#define CMD_ISP_STREAMID		MM_MODULE_CMD(0x02)
#define CMD_ISP_HW_SLOT			MM_MODULE_CMD(0x03)
#define CMD_ISP_SW_SLOT			MM_MODULE_CMD(0x04)
#define CMD_ISP_FPS				MM_MODULE_CMD(0x05)
#define CMD_ISP_HEIGHT			MM_MODULE_CMD(0x06)
#define CMD_ISP_WIDTH			MM_MODULE_CMD(0x07)
#define CMD_ISP_FORMAT			MM_MODULE_CMD(0x08)
#define CMD_ISP_DUMP_STATE		MM_MODULE_CMD(0x09)
#define CMD_ISP_SET_SELF_BUF            MM_MODULE_CMD(0x0A)
#define CMD_ISP_SET_BOOT_MODE           MM_MODULE_CMD(0x0B)

#define CMD_ISP_APPLY			MM_MODULE_CMD(0x20)  // apply setting
#define CMD_ISP_UPDATE			MM_MODULE_CMD(0x21)  // update new setting
#define CMD_ISP_STREAM_START	MM_MODULE_CMD(0x22)  // start stream
#define CMD_ISP_STREAM_STOP		MM_MODULE_CMD(0x23)  // stop stream


typedef struct isp_state_s{
	uint32_t timer_1;
	uint32_t timer_2;
	uint32_t drop_frame;
	uint32_t isp_frame;
	uint32_t drop_frame_total;
	uint32_t isp_frame_total;
}isp_state_t;

#define MAX_ISP_SELF_BUFF 4

typedef struct isp_param_s{
    uint32_t width;
	uint32_t height;
	uint32_t fps;
	uint32_t slot_num;
	uint32_t buff_num;
	uint32_t stream_id;
	uint32_t format;
        uint32_t self_buf_count;//Use self buffer
        uint32_t self_buf_addr[MAX_ISP_SELF_BUFF];
        uint32_t boot_mode;
		uint32_t bayer_type;
    //...
}isp_params_t;

typedef struct isp_ctx_s{
    void* parent;

	isp_stream_t* stream;
	
	isp_params_t params;
	isp_state_t state;
	
	mm_queue_item_t* hw_slot_item[4];
	
	uint32_t buf_width, buf_height;
}isp_ctx_t;

extern mm_module_t isp_module;

#endif