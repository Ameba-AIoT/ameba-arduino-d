#ifndef _MODULE_RTP_H
#define _MODULE_RTP_H

#include <stdint.h>
#include "rtsp/rtp_api.h"

#define AUDIO_BUF_SIZE  		(1500) //160 BYTE DATA + 12 BYTE RTP HEADER
#define DEFAULT_RTP_PORT        16384

#define MAX_SELECT_SOCKET       8


#define CMD_RTP_SET_PARAMS     	MM_MODULE_CMD(0x00)  // set parameter
#define CMD_RTP_GET_PARAMS     	MM_MODULE_CMD(0x01)  // get parameter
#define CMD_RTP_STREAMING		MM_MODULE_CMD(0x02)  

#define CMD_RTP_APPLY			MM_MODULE_CMD(0x20)  // for hardware module

//static long listen_time_s = 0;
//static long listen_time_us = 20000; //20ms
#define LISTEN_TIME_S	0
#define LISTEN_TIME_US	500

typedef struct rtp_param_s{
	uint32_t valid_pt;		// set 0xFFFFFFFF ==> allow RTP_PT_PCMU/RTP_PT_PCMA/RTP_PT_DYN_BASE (type is an unsigned 8 bit value)
	uint32_t port;
	uint32_t frame_size;
	uint32_t cache_depth;
}rtp_params_t;

typedef struct rtp_ctx_s{
    void* parent;
	struct connect_context connect_ctx;
	TaskHandle_t task;
	_sema up_sema;
	
	rtp_params_t params;
	// flag
	int rtp_shutdown;
	int socket_closed;
	
	// cache handler
	xQueueHandle    cache_ready;
    xQueueHandle    cache_recycle;
	TaskHandle_t 	cache_handler;
}rtp_ctx_t;

extern mm_module_t rtp_module;
#endif