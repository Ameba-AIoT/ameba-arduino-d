#ifndef _EXCHBUF_H
#define _EXCHBUF_H
#include "cmsis_os.h"
#include "osdep_service.h"
#include "mmf_dbg.h"

/*service task state*/
#define S_STOP 					0x00
#define S_RUN 					0x01
#define S_FROZEN 				0X02

/*exchange buffer state*/
#define STAT_INIT       0
#define STAT_USED       1
#define STAT_READY      2
#define STAT_RESERVED   3

/*output sink state*/
#define SINK_STOP				0x00
#define SINK_RUN				0x01

typedef struct _media_source_module msrc_module_t;
typedef struct _media_sink_module msink_module_t;

typedef struct _share_cache{
	_mutex				mutex;
	uint8_t* 			data;			// command arg
}share_cache_t;

typedef struct _exch_buf_output_list{
	uint8_t				index;
	msink_module_t *	sink;			// destination sink
	xQueueHandle		output_qid;
	uint8_t				send;
}output_list_t;

typedef struct _exch_buf{
	//_list 				node;			// linking node
	uint32_t			type;			// CMD, or DATA
	uint32_t			cmd;			// command 
	uint32_t			arg;			// command arg
	
	uint8_t* 			data;			//
	uint32_t			index;
	uint32_t			len;
	uint32_t 			timestamp;		// 0: not set
	uint32_t 			hw_timestamp;	// ISP hw timestamp
	uint32_t			codec_fmt;		// FMT_V_xx or FMT_A_xx
	
	uint32_t			state;		
	void* 				priv;			// private use

	xQueueHandle		source_qid;
	output_list_t*		output_sink_list;
	uint8_t				sink_cnt;
	uint32_t			dst_count;
	uint32_t*			ref_count;
	_mutex				ref_count_lock;
	uint32_t			share_cache_size;
	uint32_t			max_share_cnt;
	share_cache_t*		cache_list;
}exch_buf_t;

/*CMDs necessary for MMF*/
#define CMD_SET_STREAMMING			0x00
#define CMD_SET_INPUT_QUEUE			0x01
#define CMD_SET_OUTPUT_QUEUE		0x02
#define CMD_SET_TASK_ON				0x03
#define CMD_SET_TASK_FROZEN			0x04
#define CMD_SET_TASK_OFF			0x05

/* CMDs optional for MMF*/
#define CMD_SET_CB_START			0x0a
#define CMD_SET_CB_STOP				0x0b
#define CMD_SET_CB_PAUSE			0x0c
#define CMD_SET_CB_CUSTOMCMD		0x0d

/* currently supported media format type */
#define FMT_V_MJPG			0x00
#define FMT_V_H264			0x01
#define FMT_V_MP4V_ES		0x02
#define FMT_A_PCMU			0x10
#define FMT_A_PCMA			0x11
#define FMT_A_MP4A_LATM		0x12
#define FMT_A_AAC_RAW       0x13
#define FMT_AV_UNKNOWN		0xFF

/* common error code for MMF */
#define EAGAIN          11
#define	EFAULT          14
#define	EINVAL          22
#define ENOSR			63

#endif
