/*
 * Realtek Semiconductor Corp.
 *
 * include/rtstream.h
 *
 * Copyright (C) 2016      Ming Qian<ming_qian@realsil.com.cn>
 */
#ifndef _INCLUDE_RTSTREAM_H
#define _INCLUDE_RTSTREAM_H

#include <stdint.h>
#include <rtsavdef.h>

#ifdef __cplusplus
extern "C"
{
#endif

enum {
	RTSTREAM_PKT_FLAG_KEY		= (1 << 0),
	RTSTREAM_PKT_FLAG_NO_OUTPUT	= (1 << 1),
};

struct rts_av_buffer {
	void *vm_addr;
	uint32_t length;
	uint32_t bytesused;
	uint32_t phy_addr;
	uint32_t flags;
	uint32_t index;
	uint32_t type;
	uint64_t timestamp;
	void *priv;
};

struct rts_av_profile {
	enum RTS_AV_FMT fmt;
	union {
		struct {
			uint32_t width;
			uint32_t height;
			uint32_t numerator;
			uint32_t denominator;
		} video;
		struct {
			uint32_t samplerate;
			uint32_t bitfmt;
			uint32_t channel;
			uint32_t reserved;
		} audio;
	};
};

typedef struct rts_av_stream *RtStream;

/*!
 * init rtstream library
 */
int rts_av_init();

/*!
 * release rtstream library
 */
int rts_av_release();


/*!
 * initialize custom buffer
 */
int rts_av_init_buffer(struct rts_av_buffer *buffer,
		void *vm_addr, uint32_t length, uint32_t phy_addr);

/*!
 * uninitialize custom buffer
 */
int rts_av_uninit_buffer(struct rts_av_buffer *buffer);

/*!
 * increment reference count for buffer
 */
struct rts_av_buffer *rts_av_get_buffer(struct rts_av_buffer *buffer);

/*!
 * decrement reference count for buffer
 */
int rts_av_put_buffer(struct rts_av_buffer *buffer);

/*!
 * apply stream : start streaming
 *
 * @return 0 : success, minus : fail
 */
int rts_av_apply(RtStream stream);

/*!
 * cancel stream : stop streaming
 *
 * @return 0 : success, minus : fail
 */
int rts_av_cancel(RtStream stream);

/*!
 * check is there any output
 *
 * @return 0 : success, minus : fail
 */
int rts_av_stream_poll(RtStream stream);

/*!
 * receive data from the streaming stream
 *
 * @return 0 : success, minus : fail
 */
int rts_av_stream_recv(RtStream stream, struct rts_av_buffer **ppbuf);

/*!
 * rts_av_stream_get_profile: get stream output profile
 *
 * if the stream has unit whose type is unit_type_sink,
 * this api is not support
 *
 * @return 0 : success, minus : fail.
 */
int rts_av_stream_get_profile(RtStream stream,
		struct rts_av_profile *profile);

/*!
 * rts_av_stream_set_profile: set stream intput profile
 *
 * if the stream has unit whose type is unit_type_source,
 * this api is not support
 *
 * @return 0 : success, minus : fail.
 */
int rts_av_stream_set_profile(RtStream stream,
		struct rts_av_profile *profile);

/*!
 * rts_destroy_stream: cancel stream, delete all units, delete stream
 *
 */
void rts_destroy_stream(RtStream stream);

enum rts_h264_profile {
	H264_PROFILE_UNKNOWN,
	H264_PROFILE_BASE,
	H264_PROFILE_MAIN,
	H264_PROFILE_HIGH
};

enum rts_h264_level {
	H264_LEVEL_UNKNOWN = 0,
	H264_LEVEL_1,
	H264_LEVEL_1_b,
	H264_LEVEL_1_1,
	H264_LEVEL_1_2,
	H264_LEVEL_1_3,
	H264_LEVEL_2,
	H264_LEVEL_2_1,
	H264_LEVEL_2_2,
	H264_LEVEL_3,
	H264_LEVEL_3_1,
	H264_LEVEL_3_2,
	H264_LEVEL_4,
	H264_LEVEL_4_1,
	H264_LEVEL_4_2,
	H264_LEVEL_5,
	H264_LEVEL_5_1,
	H264_LEVEL_RESERVED
};

struct rts_isp_stream_cfg {
	int isp_id;
	enum RTS_AV_FMT format;
	uint32_t width;
	uint32_t height;
	uint32_t numerator;
	uint32_t denominator;
	int isp_buf_num;
};

/*!
 * rts_create_isp_stream : create an isp stream
 *
 * cfg->numerator:		in
 * cfg->denominator:	in
 *
 * @return the pointer of the stream when success, or NULL when fail
 */
RtStream rts_create_isp_stream(struct rts_isp_stream_cfg *cfg);

struct rts_h264_stream_cfg {
	struct rts_isp_stream_cfg isp_cfg;

	enum rts_h264_profile profile;
	enum rts_h264_level level;

	int qp;
	unsigned int bps;
	unsigned int gop;
	enum rts_av_rotation rotation;
	int videostab;
};

/*!
 * rts_create_h264_stream: create a h264 stream
 *
 * cfg->rotation:	in
 *
 * @return the pointer of the stream when success, or NULL when fail
 */
RtStream rts_create_h264_stream(struct rts_h264_stream_cfg *cfg);

struct rts_mjpeg_stream_cfg {
	struct rts_isp_stream_cfg isp_cfg;

	enum rts_av_rotation rotation;
};

/*!
 * rts_create_mjpeg_stream: create a mjpeg stream
 *
 * cfg->rotation:	in
 *
 * @return the pointer of the stream when success, or NULL when fail
 */
RtStream rts_create_mjpeg_stream(struct rts_mjpeg_stream_cfg *cfg);

struct rts_audio_stream_cfg {
	char dev_node[64];
	uint32_t format;
	uint32_t channels;
	uint32_t rate;
};

/*!
 * rts_create_audio_playback_stream: create a audio playback stream
 *
 * @return the pointer of the stream when success, or NULL when fail
 */
RtStream rts_create_audio_playback_stream(struct rts_audio_stream_cfg *cfg);

/*!
 * rts_create_audio_capture_stream: create a audio capture stream
 *
 * @return the pointer of the stream when success, or NULL when fail
 */
RtStream rts_create_audio_capture_stream(struct rts_audio_stream_cfg *cfg);

/*!
 * rts_create_audio_mixer_playback_stream: create a audio mixer + playback stream
 *
 * @return the pointer of the stream when success, or NULL when fail
 */
RtStream rts_create_audio_mixer_playback_stream(struct rts_audio_stream_cfg *cfg);

/*!
 * rts_create_audio_capture_encode_stream: create a audio capture + encode stream
 *
 * @return the pointer of the stream when success, or NULL when fail
 */
RtStream rts_create_audio_capture_encode_stream(struct rts_audio_stream_cfg *cfg);

/*!
 * rts_create_audio_decode_stream: create a audio decode stream
 *
 * @return the pointer of the stream when success, or NULL when fail
 */
RtStream rts_create_audio_decode_stream(struct rts_audio_stream_cfg *cfg);

/*!
 * rts_create_audio_encode_stream: create a audio encode stream
 *
 * @return the pointer of the stream when success, or NULL when fail
 */
RtStream rts_create_audio_encode_stream(struct rts_audio_stream_cfg *cfg);

/*!
 * rts_create_audio_server_stream: create a audio server stream
 *
 * @return the pointer of the stream when success, or NULL when fail
 */
RtStream rts_create_audio_server_stream(struct rts_audio_stream_cfg *cfg_p,
		struct rts_audio_stream_cfg *cfg_c);

#ifdef __cplusplus
}
#endif

#endif
