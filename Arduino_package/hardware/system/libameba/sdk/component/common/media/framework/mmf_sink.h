#ifndef _STREAM_H
#define _STREAM_H
#include "cmsis_os.h"
#include "mmf_common.h"
#include "mmf_source_modules/mmf_source_list.h"	// for sink modules to find src

struct _media_sink_module{
	void* (*create)(void);
	void  (*destroy)(void*);
	int   (*set_param)(void*, int, int);
	int   (*handle)(void*,void*);	// data handle, input output will cast to exch_buf_t
};

typedef struct _media_sink_context{
	xQueueHandle		input_qid;
	//xQueueHandle		output_qid;
	int					state;
	xTaskHandle			hdl_task;	// queue service task
	
	void*				drv_priv;		// private data for module
	msink_module_t *	sink;	
}msink_context;

void			mmf_sink_close(msink_context* ctx);
msink_context*	mmf_sink_open(msink_module_t *sink);
int				mmf_sink_ctrl(msink_context* ctx, int cmd, int arg);

// must be here
#include "mmf_sink_modules/mmf_sink_list.h"
#endif
