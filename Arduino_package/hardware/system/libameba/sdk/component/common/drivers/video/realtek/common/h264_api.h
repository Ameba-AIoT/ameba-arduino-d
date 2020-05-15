#ifndef _H264_API_H_
#define _H264_API_H_

#include "video_common_api.h"
#include "h264_encoder.h"

typedef enum {
	H264_OK = 0,
	
	H264_ERROR = -1,
	H264_NULL_ARGUMENT = -2,
	H264_INVALID_ARGUMENT = -3,
	H264_MEMORY_ERROR = -4,
	H264_EWL_ERROR = -5,
	H264_EWL_MEMORY_ERROR = -6,
	H264_INVALID_STATUS = -7,
	H264_OUTPUT_BUFFER_OVERFLOW = -8,
	H264_HW_BUS_ERROR = -9,
	H264_HW_DATA_ERROR = -10,
	H264_HW_TIMEOUT = -11,
	H264_HW_RESERVED = -12,
	H264_SYSTEM_ERROR = -13,
	H264_INSTANCE_ERROR = -14,
	H264_HRD_ERROR = -15,
	H264_HW_RESET = -16,
} H264_RET;

typedef enum {
	H264_RC_MODE_CBR = 1,
	H264_RC_MODE_VBR = 2,
	H264_RC_MODE_FIXQP = 3,
	H264_RC_MODE_RATEADP = 4,
} H264_RC_MODE;

H264_RET h264_create_encoder(struct h264_context* *p_h264_ctx);
H264_RET h264_free_encoder(struct h264_context* *p_h264_ctx);
H264_RET h264_set_parm(struct h264_context* h264_ctx, struct h264_parameter* parm);
H264_RET h264_get_parm(struct h264_context* h264_ctx, struct h264_parameter* parm);
H264_RET h264_init_encoder(struct h264_context* h264_ctx);
H264_RET h264_encode_frame(struct h264_context* h264_ctx, isp_buf_t* isp_buf, VIDEO_BUFFER* video_buf);
int h264_is_i_frame(u8* frame_buf);

#endif //#ifndef _H264_API_H_