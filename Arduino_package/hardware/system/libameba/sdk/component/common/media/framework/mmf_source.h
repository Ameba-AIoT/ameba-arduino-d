#ifndef _MEDIA_H
#define _MEDIA_H
#include "cmsis_os.h"
#include "mmf_common.h"
#include "mmf_sink_modules/mmf_sink_list.h"	// for source modules to set dest_sink

struct _media_source_module{
	void* (*create)(void);
	void  (*destroy)(void*);
	int   (*set_param)(void*, int, int);
	int   (*handle)(void*, void*);	// data handle, input output will cast to exch_buf_t
};

typedef struct _media_source_output_queue{
	uint8_t				index;
	msink_module_t *	sink;			// destination sink
	xQueueHandle		output_qid;
	uint8_t				sink_state;		// SINK_STOP or SINK_RUN	
}msrc_output_queue_t;

typedef struct _media_source_context{
	xQueueHandle			input_qid;
	msrc_output_queue_t*	output_qid_list;
	//xQueueHandle output_qid;
	int						state;
	xTaskHandle				hdl_task;	// queue service task
	
	void*					drv_priv;
	msrc_module_t *			source;
	
	uint8_t					stream_on;
	uint32_t				free_exch_buf_cnt;
	_sema					stream_off_done_sema;
	uint8_t					sink_cnt;
	uint8_t					max_share_cnt;
	uint32_t				share_cache_size;// if 0 -> don't use cache, use referece count
	uint32_t				exch_buf_cnt;
	exch_buf_t*				p_exbuf;
}msrc_context;

void			mmf_source_close(msrc_context* ctx);
msrc_context*	mmf_source_open(msrc_module_t *source, uint8_t sink_cnt, uint8_t max_share_cnt, uint32_t cache_size);
int				mmf_source_ctrl(msrc_context* ctx, int cmd, int arg);
void			mmf_source_sink_state_change(msrc_context* ctx, msink_module_t* sink, uint8_t state);
char			mmf_source_all_sink_off(msrc_context* ctx);
void			mmf_source_reset_exbuf_sending_list(exch_buf_t *exbuf);
void			mmf_source_add_exbuf_sending_list(exch_buf_t *exbuf, msink_module_t *sink);
void			mmf_source_add_exbuf_sending_list_all(exch_buf_t *exbuf);

// must be here
#include "mmf_source_modules/mmf_source_list.h"
#endif
