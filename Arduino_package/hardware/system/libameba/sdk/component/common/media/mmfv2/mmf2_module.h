#ifndef MMF2_MODULE_H
#define MMF2_MODULE_H

#include <stdint.h>
#include <FreeRTOS.h>
#include <semphr.h>

#include <diag.h>
#if defined(CONFIG_PLATFORM_8721D)
#ifndef rt_printf
#define rt_printf printf
#endif
#define mm_printf rt_printf
#else
#define mm_printf(...) do{ __disable_irq(); rt_printf(__VA_ARGS__); __enable_irq();}while(0)
#endif

// MM : media module
//-----------------------------------------------------------------------------

#define MM_TYPE_NONE            0x00    // not create output queue if output type is none

#define MM_MASK_SRC             0x10
#define MM_TYPE_VSRC            0x10    // ISP or file
#define MM_TYPE_ASRC            0x11    // I2S or file or RTP

#define MM_MASK_DSP             0x20
#define MM_TYPE_VDSP            0x20    // H264 ENC or JPEG ENC
#define MM_TYPE_ADSP            0x21    // AAC ENC or AAC DEC

#define MM_MASK_SINK            0x40
#define MM_TYPE_VSINK           0x41    // RTSP or file
#define MM_TYPE_ASINK           0x42    // RTSP or I2S or file
#define MM_TYPE_AVSINK          0x43    // RTSP or file


#define MM_CMD_INIT_QUEUE_ITEMS     0x00  // init static queue item 
#define MM_CMD_SET_QUEUE_LEN		0x01  // set one queue's length
#define MM_CMD_SET_QUEUE_NUM		0x02  // set number of queue (multiple queue)
#define MM_CMD_SELECT_QUEUE			0x03  // select queue from queue0 ~ queue3
#define MM_CMD_CLEAR_QUEUE_ITEMS     0x04  // clear queue item 


#define MM_CMD_MODULE_BASE			0x80
#define MM_MODULE_CMD(x)			(MM_CMD_MODULE_BASE+(x))

#define MM_STAT_INIT				0x00
#define MM_STAT_READY				0x01
#define MM_STAT_ERROR				0x10
#define MM_STAT_ERR_MALLOC			0x11
#define MM_STAT_ERR_QUEUE			0x12
#define MM_STAT_ERR_NEWITEM			0x13

typedef struct mm_module_s{
    void* (*create)(void*);
	void* (*destroy)(void*);
	int   (*control)(void*, int, int);
	int   (*handle)(void*, void*, void*);   // void* ctx, void* input, void *output , return ( <0 error code, >0 success with output length , =0 success)
    
    void* (*new_item)(void*);          // malloc
    void* (*del_item)(void*, void*);        // free 
    void* (*rsz_item)(void*, void*, int);   // resize (only shrink)

    uint32_t        output_type;
    uint32_t        module_type;
	char* name;
}mm_module_t;

typedef struct mm_conveyor_s{
	xQueueHandle    output_ready;
	xQueueHandle    output_recycle; 
	int32_t item_num;	
}mm_conveyor_t;

typedef struct mm_contex_s{
	union{
		struct{
			xQueueHandle    output_ready;
			xQueueHandle    output_recycle; 
			int32_t item_num;
		};
		mm_conveyor_t port[4];
	};
	
    mm_module_t*    module;

	// private data structure for created instance
    void* priv;
    
	// module state
	uint32_t state;
	int32_t queue_num;			// number of queue	
	int32_t curr_queue;
}mm_context_t;

#define MMQI_FLAG_STATIC        0x00
#define MMQI_FLAG_DYNAMIC       0x01
#define MMQI_FLAG_READY         0x10
#define MMQI_FLAG_USED          0x20

typedef struct mm_queue_item_s{
    uint32_t flag;
    uint32_t data_addr;    // store data address
	
	uint32_t timestamp;
	uint32_t hw_timestamp;
	uint32_t type;
	uint32_t size;
	uint32_t index;			// for RTSP
	uint32_t in_idx;		// input index
}mm_queue_item_t;


extern int mm_module_ctrl(mm_context_t *ctx, int cmd, int arg);
extern mm_context_t* mm_module_open(mm_module_t *mm );
extern mm_context_t* mm_module_close(mm_context_t *ctx);

#endif