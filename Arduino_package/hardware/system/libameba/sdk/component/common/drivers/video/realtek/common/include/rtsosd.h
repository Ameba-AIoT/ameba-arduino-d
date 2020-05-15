/*
 * Realtek Semiconductor Corp.
 *
 * libs/include/rtsosd.h
 *
 * Copyright (C) 2014      Ming Qian<ming_qian@realsil.com.cn>
 */
#ifndef _LIBS_INCLUDE_RTSOSD_H
#define _LIBS_INCLUDE_RTSOSD_H

#include <rtscolor.h>
#include <rtsisp_def.h>

#define RTS_OSD_MIN_GAP			2

enum rts_osd_char_rotation {
	osd_char_rotation_0 = 0,
	osd_char_rotation_90 = 1,
	osd_char_rotation_180 = 2,
	osd_char_rotation_270 = 3
};


enum rts_osd_time_fmt {
	osd_time_fmt_no = 0,	/*not show time*/
	osd_time_fmt_24,
	osd_time_fmt_12,
	osd_time_fmt_12_1,
	osd_time_fmt_12_2,
	osd_time_fmt_12_3,
	osd_time_fmt_12_4,
	osd_time_fmt_12_5,
	osd_time_fmt_12_6,
	osd_time_fmt_12_7,
	osd_time_fmt_reserved
};

enum rts_osd_date_fmt {
	osd_date_fmt_no = 0,	/*not show date*/
	osd_date_fmt_0,		/*dd/MM/yyyy, eg. 26/05/2015*/
	osd_date_fmt_1,		/*dd/MM/yy, eg. 26/05/15*/
	osd_date_fmt_2,		/*d/M/yy, eg. 26/5/15*/
	osd_date_fmt_3,		/*M/d/yyyy, eg. 5/26/2015*/
	osd_date_fmt_4,		/*M/d/yy, eg. 5/26/15*/
	osd_date_fmt_5,		/*MM/dd/yy, eg. 05/26/15*/
	osd_date_fmt_6,		/*MM/dd/yyyy, eg. 05/26/2015*/
	osd_date_fmt_7,		/*yyyy/M/d, eg. 2015/5/26*/
	osd_date_fmt_8,		/*yyyy-M-d, eg. 2015-5-26*/
	osd_date_fmt_9,		/*yyyy-MM-dd, eg. 2015-05-26*/
	osd_date_fmt_10,	/*yyyy/MM/dd, eg. 2015/05/26*/
	osd_date_fmt_11,	/*yy-MM-dd, eg. 15-05-26*/
	osd_date_fmt_12,	/*yy/M/d, eg. 15/5/26*/
	osd_date_fmt_13,	/*yy-M-d, eg. 15-5-26*/
	osd_date_fmt_14,	/*yy/MM/dd, eg. 15/05/26*/
	osd_date_fmt_15,	/*yyyy.mm.dd, eg. 2015.05.26*/
	osd_date_fmt_16,	/*dd.mm.yyyy, eg. 26.05.2015*/
	osd_date_fmt_17,	/*mm.dd.yyyy, eg. 05.26.2015*/
	osd_date_fmt_18,	/*mm-dd-yyyy, eg. 05-26-2015*/
	osd_date_fmt_19,	/*dd-mm-yyyy, eg. 26-05-2015*/
	osd_date_fmt_reserved
};

enum osd_char_type {
	char_type_single = 0,
	char_type_double = 1,
	char_type_picture = 2
};

struct rts_osd_char_t {
	enum osd_char_type char_type;
	unsigned char char_value[4];
};

struct rts_osd_text_t {
	struct rts_osd_char_t *content;
	unsigned int count;
};

struct rts_osd_datetime_t {
	enum rts_osd_time_fmt time_fmt;
	uint8_t time_blkidx;
	int time_pos;
	enum rts_osd_date_fmt date_fmt;
	uint8_t date_blkidx;
	int date_pos;
};

struct rts_osd_rect_t {
	uint16_t start_x;
	uint16_t start_y;
	uint16_t end_x;
	uint16_t end_y;
};

struct rts_osd_attr_t {
	uint8_t h_gap:4;
	uint8_t v_gap:4;
	uint8_t bg_enable; /*0 : no show, others : show*/
	rts_color bg_color;
	rts_color ch_color;
	uint8_t flick_enable;
	uint8_t flick_freq;
	uint8_t char_color_thd;
	uint8_t char_color_inc;
	uint8_t char_color_alpha;
	uint8_t stroke_enable; /* 1:enable char stroke, 0: disable*/
	uint8_t stroke_direct; /* 0:minus inc, 1:add inc */
	uint8_t stroke_inc;
};

struct rts_osd_lib_info {
	enum osd_char_type char_type;
	uint16_t char_width;
	uint16_t char_height;
	uint8_t font_type;
	uint8_t font_size;
	uint8_t coding_fmt;
	uint8_t char_num;
};

/*
 *
 * rts_new_osd : new a realtek osd object
 *
 * @fd : device handle, returned by open
 *
 * @strmidx : stream index
 *
 * return value : the pointer of the osd object when success, or NULL when fail
 *
 * */
void *rts_new_osd(int fd, uint8_t strmidx);

/*
 * rts_delete_osd : delete the osd object
 *
 * @p : returned by rts_new_osd
 *
 * */
void rts_delete_osd(void *p);

/*
 * rts_osd_add_font_lib : add lattice from file
 *
 * @p : returned by rts_new_osd
 *
 * @filename : lib file name
 *
 * return value : the index of font lib when success, or minus when fail
 *
 * */
int rts_osd_add_font_lib(void *p, const char *filename, struct rts_osd_lib_info *pinfo);
int rts_osd_rm_font_lib(void *p, int index);

int rts_osd_show_text(void *p, uint8_t blkidx, struct rts_osd_text_t *ptext);
int rts_osd_show_picture(void *p, uint8_t blkidx, int picidx);// __attribute__ ((deprecated));
int rts_osd_show_datetime(void *p, struct rts_osd_datetime_t *pdatetime);
int rts_osd_cancel(void *p, uint8_t blkidx);
int rts_osd_set_position(void *p, uint8_t blkidx, struct rts_osd_rect_t *pos);
int rts_osd_get_position(void *p, uint8_t blkidx, struct rts_osd_rect_t *pos);
int rts_osd_set_attr(void *p, uint8_t blkidx, struct rts_osd_attr_t *pattr);
int rts_osd_get_attr(void *p, uint8_t blkidx, struct rts_osd_attr_t *pattr);

int rts_osd_get_block_number(void *p);
int rts_osd_set_rotate(void *p, uint8_t _rotate);
int rts_osd_set_font_size(void *p, uint8_t osd_w, uint8_t osd_h);
int rts_osd_get_font_size(void *p, uint8_t *p_osd_char_w, uint8_t *p_osd_char_h);

int rts_osd_set_bitmap(void *p, uint8_t blkidx, const BITMAP_S *pstBitmap);

int rts_osd_refresh_datetime(void *p);
int rts_osd_get_time_length(enum rts_osd_time_fmt time_fmt);
int rts_osd_get_date_length(enum rts_osd_date_fmt date_fmt);
#endif
