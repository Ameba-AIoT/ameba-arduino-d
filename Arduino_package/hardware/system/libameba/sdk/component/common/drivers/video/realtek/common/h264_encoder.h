#ifndef H264_ENCODER
#define H264_ENCODER
#include "memory_encoder.h"
#include "encoder_buffer_handler.h"
#include "isp_api.h"

#define RC_MODE_H264CBR 1
#define RC_MODE_H264VBR 2
#define RC_MODE_H264ABR	3
#define RC_MODE_H264FIXQP 4
#define RC_MODE_H264RATEADP	5	// bit rate adaptive, original mode

#define H264_ROTATE_0   0
#define H264_ROTATE_90R 1
#define H264_ROTATE_90L 2

#define H264_FRAME_SKIP 2
#define H264_DROP_FRAME_THRESHOLD 1500

struct roi_param{
        int enable;
        int left;
        int right;
        int bottom;
        int top;
};

struct h264_parameter {
	int inputtype;			//YUV420 SEMI-PLANAR
	int width;
	int height;
	int ratenum;
        unsigned int force_drop_frame;//The frame rate is follow by the h264 setting
        //unsigned int drop_frame_time;
        unsigned int target_interval;
        unsigned int target_timestamp;
        unsigned int target_diff;
	unsigned int bps;		// for CBR/ABR
	unsigned int gopLen;
	unsigned int rcMode;
	unsigned int iQp;		// for fixed QP
	unsigned int pQp;		// for fixed QP
	unsigned int minQp;		// for CBR/VBR/ABR
	unsigned int minIQp;	// for CBR/VBR/ABR
	unsigned int maxQp;		// for CBR/VBR/ABR

	unsigned int rc_adv_enable;
	
	unsigned int maxBps;		// for VBR
	unsigned int minBps;		// for VBR
	
	int intraQpDelta;
	int mbQpAdjustment;
	unsigned int mbQpAutoBoost;
	
	int adaptQpEn;			// for Adaptive QP
	int rcErrorRst;			// for RC error reset when rate change
        struct roi_param roi;
        unsigned int rotation;
        int qp_chroma_offset;
};

struct h264_change_config{
	int bps;
	int bps_config;
        int forcei_config;
        int roi_config;
};

#define SPS_PPS_SIZE 64

struct h264_context {
	void* encoder;
	struct h264_parameter h264_parm;
	long index;
	int start;
        int isp_timestamp;
	uint32_t source_addr;
	uint32_t y_addr;
	uint32_t uv_addr;
	uint32_t dest_addr;
	uint32_t dest_len;
	uint32_t dest_actual_len;
	//uint32_t stab_addr;
	uint8_t  sps_pps_buf[SPS_PPS_SIZE];
	uint32_t sps_pps_len;
	struct isp_info isp_info_value;
	struct mem_info mem_info_value;
	struct encoder_list_head encoder_lh;
	int (*snapshot_encode_cb)(uint32_t,uint32_t);
	void (*change_parm_cb)(void*);
	struct h264_change_config h264_ch_config;
        int pause;
};
void *h264_open();
int h264_encode(void *ctx);
int h264_release(void *ctx);
int h264_initial(void *ctx,struct h264_parameter *h264_parm);
int h264_init_param(struct h264_parameter *h264_parm);
int h264_info();
void h264_set_force_iframe(void *ctx);
void h264_set_roi(void *ctx);

#endif