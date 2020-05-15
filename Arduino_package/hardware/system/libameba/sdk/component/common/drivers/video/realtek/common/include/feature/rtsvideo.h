/*
 * Realtek Semiconductor Corp.
 *
 * include/rtsvideo.h
 *
 * Copyright (C) 2014      Ming Qian<ming_qian@realsil.com.cn>
 */
#ifndef _INCLUDE_RTSVIDEO_H
#define _INCLUDE_RTSVIDEO_H
#include <stdint.h>
#include <rtsisp_def.h>
#include <rtsisp.h>
#include <rtsosd.h>
#include <rtstream.h>

#ifdef __cplusplus
extern "C"
{
#endif

enum enum_rts_video_ctrl_id {
	RTS_VIDEO_CTRL_ID_BRIGHTNESS = 1,
	RTS_VIDEO_CTRL_ID_CONTRAST,
	RTS_VIDEO_CTRL_ID_HUE,
	RTS_VIDEO_CTRL_ID_SATURATION,
	RTS_VIDEO_CTRL_ID_SHARPNESS,
	RTS_VIDEO_CTRL_ID_GAMMA,
	RTS_VIDEO_CTRL_ID_AWB_CTRL,
	RTS_VIDEO_CTRL_ID_WB_TEMPERATURE,
	RTS_VIDEO_CTRL_ID_BLC,
	RTS_VIDEO_CTRL_ID_GAIN,
	RTS_VIDEO_CTRL_ID_PWR_FREQUENCY,
	RTS_VIDEO_CTRL_ID_EXPOSURE_MODE,
	RTS_VIDEO_CTRL_ID_EXPOSURE_PRIORITY,
	RTS_VIDEO_CTRL_ID_EXPOSURE_TIME,
	RTS_VIDEO_CTRL_ID_AF,
	RTS_VIDEO_CTRL_ID_FOCUS,
	RTS_VIDEO_CTRL_ID_ZOOM,
	RTS_VIDEO_CTRL_ID_PAN,
	RTS_VIDEO_CTRL_ID_TILT,
	RTS_VIDEO_CTRL_ID_ROLL,
	RTS_VIDEO_CTRL_ID_FLIP,
	RTS_VIDEO_CTRL_ID_MIRROR,
	RTS_VIDEO_CTRL_ID_ROTATE,
	RTS_VIDEO_CTRL_ID_ISP_SPECIAL_EFFECT,
	RTS_VIDEO_CTRL_ID_EV_COMPENSATE,
	RTS_VIDEO_CTRL_ID_COLOR_TEMPERATURE_ESTIMATION,
	RTS_VIDEO_CTRL_ID_AE_LOCK,
	RTS_VIDEO_CTRL_ID_AWB_LOCK,
	RTS_VIDEO_CTRL_ID_AF_LOCK,
	RTS_VIDEO_CTRL_ID_LED_TOUCH_MODE,
	RTS_VIDEO_CTRL_ID_LED_FLASH_MODE,
	RTS_VIDEO_CTRL_ID_ISO,
	RTS_VIDEO_CTRL_ID_SCENE_MODE,
	RTS_VIDEO_CTRL_ID_ROI_MODE,
	RTS_VIDEO_CTRL_ID_3A_STATUS,
	RTS_VIDEO_CTRL_ID_IDEA_EYE_SENSITIVITY,
	RTS_VIDEO_CTRL_ID_IDEA_EYE_STATUS,
	RTS_VIDEO_CTRL_ID_IEDA_EYE_MODE,
	RTS_VIDEO_CTRL_ID_GRAY_MODE,
	RTS_VIDEO_CTRL_ID_WDR_MODE,
	RTS_VIDEO_CTRL_ID_WDR_LEVEL,
	RTS_VIDEO_CTRL_ID_GREEN_BALANCE,
	RTS_VIDEO_CTRL_ID_RED_BALANCE,
	RTS_VIDEO_CTRL_ID_BLUE_BALANCE,
	RTS_VIDEO_CTRL_ID_AE_GAIN,
	RTS_VIDEO_CTRL_ID_3DNR,
	RTS_VIDEO_CTRL_ID_DEHAZE,
	RTS_VIDEO_CTRL_ID_IR_MODE,
	RTS_VIDEO_CTRL_ID_RESERVED,

	/*deprecated*/
	RTS_VIDEO_CTRL_WDR_MODE = RTS_VIDEO_CTRL_ID_WDR_MODE,
	RTS_VIDEO_CTRL_WDR_LEVEL = RTS_VIDEO_CTRL_ID_WDR_LEVEL,
	RTS_VIDEO_CTRL_GREEN_BALANCE = RTS_VIDEO_CTRL_ID_GREEN_BALANCE,
	RTS_VIDEO_CTRL_RED_BALANCE = RTS_VIDEO_CTRL_ID_RED_BALANCE,
	RTS_VIDEO_CTRL_BLUE_BALANCE = RTS_VIDEO_CTRL_ID_BLUE_BALANCE,
	RTS_VIDEO_CTRL_AE_GAIN = RTS_VIDEO_CTRL_ID_AE_GAIN,
	RTS_VIDEO_CTRL_3DNR = RTS_VIDEO_CTRL_ID_3DNR,
	RTS_VIDEO_CTRL_DEHAZE = RTS_VIDEO_CTRL_ID_DEHAZE,
};

enum RTS_ISP_WDR_MODE {
	RTS_ISP_WDR_DISABLE = 0,
	RTS_ISP_WDR_MANUAL,
	RTS_ISP_WDR_AUTO_WEAK,
	RTS_ISP_WDR_AUTO_MEDIUM,
	RTS_ISP_WDR_AUTO_STRONG,
	RTS_ISP_WDR_RESERVED,
};

struct rts_video_control {
	uint32_t type;
	char name[32];
	int32_t minimum;
	int32_t maximum;
	int32_t step;
	int32_t default_value;
	int32_t current_value;
	uint32_t flags;
	uint32_t reserved[4];
};

struct rts_video_rect {
	int32_t left;
	int32_t top;
	int32_t right;
	int32_t bottom;
};

struct rts_video_grid_unit {
	uint32_t width;
	uint32_t height;
};

struct rts_video_grid_bitmap {
	uint32_t rows;
	uint32_t columns;
	uint8_t bitmap[RTS_GRID_BITMAP_SIZE];
};

struct rts_video_grid {
	int32_t left;
	int32_t top;

	struct rts_video_grid_unit cell;
	uint32_t rows;
	uint32_t columns;

	int     length;
	uint8_t bitmap[RTS_GRID_BITMAP_SIZE];
};

struct rts_video_md_block {
	int type;
	int enable;
	union {
		struct rts_video_grid area;
		struct rts_video_rect rect;
	};
	uint32_t res_width;
	uint32_t res_height;

	uint32_t sensitivity;
	uint32_t percentage;
	uint32_t frame_interval;
};

struct rts_video_md_attr {
	int number;
	struct rts_video_md_block *blocks;
	uint32_t reserved[4];
};

struct rts_video_mask_block {
	int type;
	int enable;
	union {
		struct rts_video_grid area;
		struct rts_video_rect rect;
	};
	uint32_t res_width;
	uint32_t res_height;
};

struct rts_video_mask_attr {
	uint32_t color;    /*rgb24*/
	int number;
	struct rts_video_mask_block *blocks;

	uint32_t reserved[4];
};

struct rts_video_osd_block {
	struct rts_video_rect rect;
	uint8_t bg_enable;
	uint32_t bg_color;
	uint32_t ch_color;

	uint8_t h_gap : 4,
		v_gap : 4;

	uint8_t flick_enable;
	uint32_t flick_speed;
	uint8_t char_color_alpha;

	uint8_t stroke_enable;
	uint8_t stroke_direct;
	uint8_t stroke_delta;

	struct rts_osd_text_t *pshowtext;
	BITMAP_S *pbitmap;
	uint8_t blk_hide;
};

struct rts_video_osd_attr {
	int number;
	struct rts_video_osd_block *blocks;

	enum rts_osd_time_fmt time_fmt;
	uint8_t time_blkidx;
	int time_pos;
	enum rts_osd_date_fmt date_fmt;
	uint8_t date_blkidx;
	int date_pos;

	unsigned int single_font_addr;
	unsigned int double_font_addr;

	uint8_t osd_char_w;
	uint8_t osd_char_h;
	struct rts_osd_text_t *presettext;
        
        BITMAP_S *ppresetbitmap[4];
        int presetbmpidx[4];
        int presetbmpnum;
        
	uint32_t reserved[4];
};

enum rts_bitrate_mode {
	RTS_BITRATE_MODE_CBR        = (1 << 1),
	RTS_BITRATE_MODE_VBR        = (1 << 2),
	RTS_BITRATE_MODE_C_VBR      = (1 << 3),
};

struct rts_video_h264_info {
	int sps_pps_len;
	char sps_pps[48];
};

struct rts_video_h264_ctrl {
	uint32_t supported_bitrate_mode;
	uint32_t bitrate_mode;

	uint32_t bitrate;
	uint32_t max_bitrate;
	uint32_t min_bitrate;

	int32_t qp;
	uint32_t max_qp;
	uint32_t min_qp;
	int32_t intraQpDelta;

	uint32_t gop;

	uint32_t slice_size;
	uint32_t sei_messages;
	uint32_t video_full_range;
	uint32_t constrained_intra_prediction;
	uint32_t disable_deblocking_filter;
	uint32_t enable_cabac;
	uint32_t cabac_init_idc;
	uint32_t transform8x8mode;
	uint32_t gdr;

	uint32_t hrd;
	uint32_t hrd_cpb_size;
	int32_t longterm_pic_rate;

	int drop_frame_en;
	int drop_frame_th;
	int cvbr_en;
	int cvbr_diff_n;
	int p_wnd_size;
	int p_target_min_percentage;
	int p_target_max_percentage;
	int p_diff_x_th_percentage;
	int p_diff_adjust_percentage;
	int rs_rc_en;

	uint32_t reserved[4];
};

struct rts_video_mjpeg_ctrl {
	uint16_t normal_compress_rate:4; /*0.5 as the unit*/
};

enum rts_video_roi_type {
	RTS_VIDEO_ROI_UNKNOWN = 0,
	RTS_VIDEO_ROI_H264,
	RTS_VIDEO_ROI_RESERVED
};

struct rts_video_roi_ctrl {
	int enable;
	struct rts_video_rect area;
	int32_t value;
};

/*!
 * get video control,
 * id: defined in enum enum_rts_video_ctrl_id
 *
 * @return 0 : success, minus : fail
 */
int rts_video_get_ctrl(RtStream stream,
		uint32_t id, struct rts_video_control *pctrl);

/*!
 * set video control,
 * id: defined in enum enum_rts_video_ctrl_id
 *
 * @return 0 : success, minus : fail
 */
int rts_video_set_ctrl(RtStream stream,
		uint32_t id, struct rts_video_control *pctrl);

/*!
 * get motion detect attribute
 *
 * @return 0 : success, minus : fail
 * if success, you need to call rts_video_release_md_attr to release it.
 */
int rts_video_query_md_attr(RtStream stream,
		struct rts_video_md_attr **attr);

/*!
 * update motion detect setting
 *
 * @return 0 : success, minus : fail
 */
int rts_video_set_md_attr(RtStream stream,
		struct rts_video_md_attr *attr);

/*!
 * release motion detect attribute
 *
 */
void rts_video_release_md_attr(RtStream stream,
		struct rts_video_md_attr *attr);

/*!
 * check motion detect result
 *
 * @return 1 : detected, 0 : not
 */
int rts_video_check_md_status(RtStream stream, int mdidx);

/*!
 * get motion detect bitmap result
 *
 * @return 0 : success, minus : fail
 */
int rts_video_get_md_result(RtStream stream, int mdidx,
		struct rts_video_grid_bitmap *result);

/*!
 * get private mask attribute
 *
 * @return 0 : success, minus : fail
 * if success, you need to call rts_video_release_mask_attr to release if.
 *
 */
int rts_video_query_mask_attr(RtStream stream,
		struct rts_video_mask_attr **attr);

/*!
 * update private mask setting
 *
 * @return 0 : success, minus : fail
 */
int rts_video_set_mask_attr(RtStream stream,
		struct rts_video_mask_attr *attr);

/*!
 * release private mask attribute
 *
 */
void rts_video_release_mask_attr(RtStream stream,
		struct rts_video_mask_attr *attr);

/*!
 * get osd attribute
 *
 * @return 0 : success, minus : fail
 * if success, you need to call rts_video_release_osd_attr to release it
 *
 */

int rts_video_query_osd_attr(RtStream stream,
		struct rts_video_osd_attr **attr);

/*!
 * update osd setting
 *
 * @return 0 : success, minus : fail
 */
int rts_video_set_osd_attr(RtStream stream,
		struct rts_video_osd_attr *attr);

/*!
 * preset osd bmp data
 *
 * @return 0 : success, minus : fail
 */
int rts_video_preset_osd_bmp(RtStream stream,
		struct rts_video_osd_attr *attr);

/*!
 * fast update osd setting
 *
 * @return 0 : success, minus : fail
 */
int rts_video_fast_set_osd_attr(RtStream stream,
		struct rts_video_osd_attr *attr);

/*!
 * fast update one osd setting
 *
 * @return 0 : success, minus : fail
 */
int rts_video_fast_set_one_osd_attr(RtStream stream,
		struct rts_video_osd_attr *attr,
                int blknum);

/*!
 * update osd date/time
 *
 * @return 0 : success, minus : fail
 */
int rts_video_refresh_osd_datetime(RtStream stream,
		struct rts_video_osd_attr *attr);

/*!
 * release osd attribute
 *
 */
void rts_video_release_osd_attr(RtStream stream,
		struct rts_video_osd_attr *attr);

/*!
 * get h264 info
 *
 * @return 0 : success, minus : fail
 */
int rts_video_get_h264_mediainfo(RtStream stream,
		struct rts_video_h264_info *info);

/*!
 * get h264 control
 *
 * @return 0 : success, minus : fail
 */
int rts_video_get_h264_ctrl(RtStream stream,
		struct rts_video_h264_ctrl *rc);

/*!
 * set h264 control
 *
 * @return 0 : success, minus : fail
 */
int rts_video_set_h264_ctrl(RtStream stream,
		struct rts_video_h264_ctrl *rc);

/*!
 * request an h264 key frame
 *
 * @return 0
 */
int rts_video_request_h264_key_frame(RtStream stream);

/*!
 * get mjpeg control
 *
 * @return 0 : success, minus : fail
 */
int rts_video_get_mjpeg_ctrl(RtStream stream,
		struct rts_video_mjpeg_ctrl *pctrl);

/*!
 * set mjpeg control
 *
 * @return 0 : success, minus : fail
 */
int rts_video_set_mjpeg_ctrl(RtStream stream,
		struct rts_video_mjpeg_ctrl *pctrl);

int rts_video_get_ae_gain(RtStream stream,
		struct rts_isp_ae_gain *gain);

int rts_video_set_ae_gain(RtStream stream,
		struct rts_isp_ae_gain *gain);

int rts_video_get_ae_weight(RtStream stream,
		struct rts_isp_ae_weight *weight);

int rts_video_set_ae_weight(RtStream stream,
		struct rts_isp_ae_weight *weight);

int rts_video_get_ae_statis(RtStream stream,
		struct rts_isp_ae_statis *statis);

int rts_video_get_awb_statis(RtStream stream,
		struct rts_isp_awb_statis *statis);

int rts_video_get_awb_ct_gain(RtStream stream,
		uint8_t ct, struct rts_isp_awb_ct_gain *gain);

int rts_video_get_ae_target_delta(RtStream stream,
		int8_t *delta);

int rts_video_set_ae_target_delta(RtStream stream,
		int8_t delta);

int rts_video_get_roi_count(RtStream stream, int type);

int rts_video_get_roi_ctrl(RtStream stream,
		int type, int index, struct rts_video_roi_ctrl *roi);

int rts_video_set_roi_ctrl(RtStream stream,
		int type, int index, struct rts_video_roi_ctrl *roi);

/*!
 * get isp video control,
 * id: defined in enum enum_rts_video_ctrl_id
 *
 * @return 0 : success, minus : fail
 */
int rts_get_isp_ctrl(uint32_t id, struct rts_video_control *pctrl);

/*!
 * set isp video control,
 * id: defined in enum enum_rts_video_ctrl_id
 *
 * @return 0 : success, minus : fail
 */
int rts_set_isp_ctrl(uint32_t id, struct rts_video_control *pctrl);

/*!
 * get motion detect attribute
 *
 * @return 0 : success, minus : fail
 * if success, you need to call rts_release_isp_md_attr to release it.
 */
int rts_query_isp_md_attr(struct rts_video_md_attr **attr,
		uint32_t res_width, uint32_t res_height);

/*!
 * update motion detect setting
 *
 * @return 0 : success, minus : fail
 */
int rts_set_isp_md_attr(struct rts_video_md_attr *attr);

/*!
 * release motion detect attribute
 *
 */
void rts_release_isp_md_attr(struct rts_video_md_attr *attr);

/*!
 * check motion detect result
 *
 * @return 1 : detected, 0 : not
 */
int rts_check_isp_md_status(int mdidx);

/*!
 * get motion detect bitmap result
 *
 * @return 0 : success, minus : fail
 */
int rts_get_isp_md_result(int mdidx, struct rts_video_grid_bitmap *result);

/*!
 * get private mask attribute
 *
 * @return 0 : success, minus : fail
 * if success, you need to call rts_release_isp_mask_attr to release if.
 *
 */
int rts_query_isp_mask_attr(struct rts_video_mask_attr **attr,
		uint32_t res_width, uint32_t res_height);

/*!
 * update private mask setting
 *
 * @return 0 : success, minus : fail
 */
int rts_set_isp_mask_attr(struct rts_video_mask_attr *attr);

/*!
 * release private mask attribute
 *
 */
void rts_release_isp_mask_attr(struct rts_video_mask_attr *attr);

/*!
 * get osd attribute
 *
 * @return 0 : success, minus : fail
 * if success, you need to call rts_release_isp_osd_attr to release if.
 *
 */
int rts_query_isp_osd_attr(int isp_id, struct rts_video_osd_attr **attr);

/*!
 * update osd setting
 *
 * @return 0 : success, minus : fail
 */
int rts_set_isp_osd_attr(struct rts_video_osd_attr *attr);

/*!
 * preset osd setting
 *
 * @return 0 : success, minus : fail
 */
int rts_preset_isp_osd_attr(struct rts_video_osd_attr *attr);

/*!
 * fast update osd setting
 *
 * @return 0 : success, minus : fail
 */
int rts_fast_set_isp_osd_attr(struct rts_video_osd_attr *attr);

/*!
 * fast update 1 osd setting
 *
 * @return 0 : success, minus : fail
 */
int rts_fast_set_one_isp_osd_attr(struct rts_video_osd_attr *attr, int blknum);

/*!
 * update osd date/time
 *
 * @return 0 : success, minus : fail
 */
int rts_refresh_isp_osd_datetime(struct rts_video_osd_attr *attr);

/*!
 * update date w/o attr
 *
 */
void rts_write_isp_osd_date(int dNextDate, int dFormat, int dYear, int dMonth, int dDay);

/*!
 * release osd attribute
 *
 */
void rts_release_isp_osd_attr(struct rts_video_osd_attr *attr);

int rts_get_isp_ae_gain(struct rts_isp_ae_gain *gain);

int rts_set_isp_ae_gain(struct rts_isp_ae_gain *gain);

int rts_get_isp_ae_weight(struct rts_isp_ae_weight *weight);

int rts_set_isp_ae_weight(struct rts_isp_ae_weight *weight);

int rts_get_isp_ae_statis(struct rts_isp_ae_statis *statis);

int rts_get_isp_awb_statis(struct rts_isp_awb_statis *statis);

int rts_get_isp_awb_ct_gain(uint8_t ct, struct rts_isp_awb_ct_gain *gain);

int rts_get_isp_ae_target_delta(int8_t *delta);

int rts_set_isp_ae_target_delta(int8_t delta);

/*!
 * get version number
 *
 */
char *rts_get_lib_version();


#ifdef __cplusplus
}
#endif
#endif
