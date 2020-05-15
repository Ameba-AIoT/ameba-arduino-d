/*! 
 * \mainpage rtstream API reference
 *
 * \section Introduction
 *
 * rtstream is a library that helps you to develop your multimedia application using realtek's soc camera
 *
 * \section features
 *
 * 1.a stream represent a video or an audio
 *
 * 2.a stream will contain several units
 *
 * 3.a unit represent a device or an encoder
 *
 * \section start
 *
 * Some example programs can be found in the examples subdirectory
 *
 * \example example_isp_stream.c
 * \example example_h264_stream.c
 * \example example_mjpeg_stream.c
 * \example example_isp_ctrl.c
 * \example example_osd.c
 * \example example_md.c
 * \example example_mask.c
 * \example example_audio_capture_stream.c
 * \example example_audio_playback_stream.c
 * \example example_audio_ctrl.c
 * \example example_custom_unit.c
 *
 * Realtek Semiconductor Corp.
 *
 * inc/rtsavapi.h
 *
 * Copyright (C) 2014      Ming Qian<ming_qian@realsil.com.cn>
 *
 */
#ifndef _INC_RTSAVAPI_H
#define _INC_RTSAVAPI_H

#include <rtstream.h>

#ifdef __cplusplus
extern "C"
{
#endif

#define rts_av_fourcc(a, b, c, d)\
	((uint32_t)(a) | ((uint32_t)(b) << 8) | ((uint32_t)(c)<<16) | ((uint32_t)(d)<<24))

/*!
 * new a rts_av_buffer object
 *
 * length : > 0, will alloc memory and assign the address to vm_addr
 *           = 0, will not alloc memory
 *
 * @return the pointer of the buffer when success, or NULL when fail
 */
struct rts_av_buffer *rts_av_new_buffer(uint32_t length);

/*!
 * delete a rts_av_buffer object
 */
int rts_av_delete_buffer(struct rts_av_buffer *buffer);

/*!
 * get the reference count for buffer
 */
unsigned int rts_av_get_buffer_refs(struct rts_av_buffer *buffer);

enum rts_av_ability_type {
	rts_av_ability_undefined = 0,
	rts_av_ability_v4l2,
	rts_av_ability_isp,
	rts_av_ability_reserved
};

enum rts_v4l2_frmsizetypes {
	RTS_V4L2_FRMSIZE_TYPE_DISCRETE = 1,
	RTS_V4L2_FRMSIZE_TYPE_CONTINUOUS = 2,
	RTS_V4L2_FRMSIZE_TYPE_STEPWISE = 3
};

enum rts_v4l2_frmivaltypes {
	RTS_V4L2_FRMIVAL_TYPE_DISCRETE = 1,
	RTS_V4L2_FRMIVAL_TYPE_CONTINUOUS = 2,
	RTS_V4L2_FRMIVAL_TYPE_STEPWISE = 3
};

struct rts_fract {
	uint32_t numerator;
	uint32_t denominator;
};

struct rts_v4l2_frmival_stepwise {
	struct rts_fract min;
	struct rts_fract max;
	struct rts_fract step;
};

struct rts_v4l2_resolution_discrets {
	uint32_t width;
	uint32_t height;
};

struct rts_v4l2_resolution_stepwise {
	uint32_t min_width;
	uint32_t max_width;
	uint32_t step_width;
	uint32_t min_height;
	uint32_t max_height;
	uint32_t step_height;
};

struct rts_v4l2_frmival {
	uint32_t type;
	union {
		struct rts_fract                 discrete;
		struct rts_v4l2_frmival_stepwise stepwise;
	};
};

struct rts_v4l2_resolution {
	uint32_t type;
	union {
		struct rts_v4l2_resolution_discrets discrete;
		struct rts_v4l2_resolution_stepwise stepwise;
	};
	int number;
	struct rts_v4l2_frmival *pfrmivals;
};

struct rts_v4l2_format {
	uint32_t format;
	int number;
	struct rts_v4l2_resolution *presolutions;
};

struct rts_v4l2_ability {
	int number;
	struct rts_v4l2_format *pformats;
};

struct rts_isp_ability {
	int fmt_number;
	uint32_t *pformats;
	struct rts_v4l2_resolution_stepwise resolution;
	int frmival_num;
	struct rts_fract *pfrmivals;
};

struct rts_av_ability_t {
	uint32_t type;
	union {
		struct rts_v4l2_ability v4l2_ability;
		struct rts_isp_ability isp_ability;
	};
};

struct rts_av_unit;

/*!
 * new a rts_av_unit object
 *
 * unitid
 *
 * @return the pointer of the unit : success, NULL : fail
 */
struct rts_av_unit *rts_av_new_unit(unsigned int unitid);

/*!
 * new a rts_av_unit object
 *
 * unitid
 * arg
 *
 * @return the pointer of the unit : success, NULL : fail
 */
struct rts_av_unit *rts_av_new_unit_ex(unsigned int unitid, void *arg);

/*!
 * delete the rts_av_unit object
 *
 */
int rts_av_delete_unit(struct rts_av_unit *unit);

/*!
 * get unit id
 *
 */
int rts_av_get_unit_id(struct rts_av_unit *unit, unsigned int *id);

/*!
 * get unit type
 *
 */
int rts_av_get_unit_type(struct rts_av_unit *unit);

/*!
 * set unit attribute, should call before rts_av_apply
 *
 * @return 0 : success, minus : fail
 */
int rts_av_set_attr(struct rts_av_unit *unit, void *attr);

/*!
 * get unit attibute
 *
 * @return 0 : success, minus : fail
 */
int rts_av_get_attr(struct rts_av_unit *unit, void *attr);

/*!
 * get unit ability
 *
 * @return 0 : success, minus : fail
 * if success, call rts_av_release_ability to release it.
 */
int rts_av_query_ability(struct rts_av_unit *unit,
		struct rts_av_ability_t **ability);

/*!
 * relesee ability after query_ability
 *
 */
void rts_av_release_ability(struct rts_av_ability_t *ability);

/*!
 * get unit output profile
 * unit type : source or filter
 *
 * @return 0 : success, minus : fail.
 * */
int rts_av_get_profile(struct rts_av_unit *unit,
		struct rts_av_profile *profile);

/*!
 * set unit intput profile
 * unit type : sink or filter
 *
 * @return 0 : success, minus : fail.
 * */
int rts_av_set_profile(struct rts_av_unit *unit,
		struct rts_av_profile *profile);

/*!
 * new a stream object
 *
 */
RtStream rts_av_new_stream();

/*!
 * delete a stream object
 *
 */
int rts_av_delete_stream(RtStream stream);

/*!
 * add a unit to stream
 *
 * @return 0 : success, minus : fail
 */
int rts_av_stream_add_tail(RtStream stream, struct rts_av_unit *unit);
/*!
 * rm a unit from stream
 *
 * @return 0 : success, minus : fail
 */
int rts_av_stream_del(RtStream stream, struct rts_av_unit *unit);

/*!
 * insert a unit to stream  before the pos unit
 *
 * @return 0 : success, minus : fail
 */
int rts_av_stream_insert(RtStream stream,
		struct rts_av_unit *unit, struct rts_av_unit *pos);
/*!
 * insert a unit to stream at index
 *
 * @return 0 : success, minus : fail
 */
int rts_av_stream_insert_at(RtStream stream,
		struct rts_av_unit *unit, int index);
/*!
 * swap two unit in stream
 *
 */
int rts_av_stream_swap(RtStream stream,
		struct rts_av_unit *unita, struct rts_av_unit *unitb);

/*!
 * send data to the stream
 *
 * @return 0 : success, minus : fail
 */
int rts_av_stream_send(RtStream stream, struct rts_av_buffer *pbuf);

/*!
 * rts_av_stream_get_item: get unit from stream
 *
 * unit : NULL : get the first unit.
 *         other : get next of it
 *
 * @return the pointer of the unit when success, or NULL when fail
 */
struct rts_av_unit *rts_av_stream_get_item(RtStream stream,
		struct rts_av_unit *unit);

#define RTS_AV_ID_V4L2			rts_av_fourcc('v','4','l','2')
#define RTS_AV_ID_ISP			rts_av_fourcc('i','s','p', 0)
#define RTS_AV_ID_H264			rts_av_fourcc('h','2','6','4')
#define RTS_AV_ID_MJPGENC		rts_av_fourcc('m','j','p','g')
#define RTS_AV_ID_AUDIO_PLAYBACK	rts_av_fourcc('a','u','p','l')
#define RTS_AV_ID_AUDIO_CAPTURE		rts_av_fourcc('a','u','c','a')
#define RTS_AV_ID_AUDIO_MIXER		rts_av_fourcc('a','m','i','x')
#define RTS_AV_ID_AUDIO_PLAYBACK_AEC	rts_av_fourcc('a','p','l','a')
#define RTS_AV_ID_AUDIO_CAPTURE_AEC	rts_av_fourcc('a','c','a','a')
#define RTS_AV_ID_AUDIO_ENCODE		rts_av_fourcc('a','e','n','c')

struct rts_v4l2_attr {
	uint32_t pixelformat;
	uint32_t width;
	uint32_t height;
	uint32_t numerator;
	uint32_t denominator;
	int v4l2_buf_num;
	char dev_node[64]; /*it will only be used in get attr*/
};

struct rts_isp_attr {
	uint32_t pixelformat;
	uint32_t width;
	uint32_t height;
	uint32_t numerator;
	uint32_t denominator;
	int isp_buf_num;
	int isp_id;	/*it will only be used in get attr*/
};

struct rts_h264_attr {
	enum rts_h264_profile profile;
	enum rts_h264_level level;

	int qp;
	unsigned int bps;
	unsigned int gop;
	enum rts_av_rotation rotation;
	int videostab;
};

struct rts_jpgenc_attr {
	enum rts_av_rotation rotation;
};

struct rts_audio_attr {
	char dev_node[64];
	uint32_t format;
	uint32_t channels;
	uint32_t rate;

	unsigned long period_frames;
};


#ifdef __cplusplus
}
#endif
#endif
