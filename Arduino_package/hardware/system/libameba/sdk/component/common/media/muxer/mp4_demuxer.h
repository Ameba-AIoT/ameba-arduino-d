#ifndef _MP4_DEMUX_
typedef struct _header{
	unsigned int size;
	unsigned char str[4];
}header;

#include "basic_types.h"
#include "osdep_service.h"
#include "fatfs_sdcard_api.h"

#define mvhd_length 8
#define trak_length 8
#define mdia_length 8
#define minf_length 8

#define video_type 0
#define audio_type 1

typedef struct _mp4_demux{
        FATFS       m_fs;
	int         drv_num;
	char        _drv[4];
	FIL             *m_file;
	unsigned int video_len;
	unsigned int audio_len;
	unsigned int key_len;
	unsigned int sample_index;
	unsigned int channel_count;
	unsigned char sps[0x40];
	unsigned char pps[0x40];
	unsigned short sps_length;
	unsigned short pps_length;
	unsigned int *video_size_buffer;
	unsigned int *video_offset_buffer;
	unsigned int *video_keyframe_buffer;
	unsigned int *audio_size_buffer;
	unsigned int *audio_offset_buffer;
	unsigned int *video_timestamp_buf;
	unsigned int *audio_timestamp_buf;
	unsigned int video_max_size;
	unsigned int audio_max_size;
	unsigned int video_exist;
	unsigned int audio_exist;
        unsigned int fps;
        unsigned int width;
        unsigned int height;
        unsigned int audio_sample_rate;
        unsigned int audio_channel;
}mp4_demux;

int get_video_frame(mp4_demux *mp4_demuxer,unsigned char *buf,int index,unsigned char *key_frame,unsigned int *duration_time,unsigned int *timestamp);
int get_audio_frame(mp4_demux *mp4_demuxer,unsigned char *buf,int index,unsigned int *duration_time,unsigned int *timestamp);
int mp4_demuxer_open(mp4_demux *mp4_demuxer,char *filename);
void mp4_demuxer_close(mp4_demux *mp4_demuxer);
void set_mp4_demuxer_fatfs_param(mp4_demux *mp4_demuxer, fatfs_sd_params_t* fatfs_param);
#endif _MP4_DEMUX_