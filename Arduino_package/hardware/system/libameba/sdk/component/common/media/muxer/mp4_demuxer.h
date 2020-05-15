#ifndef _MP4_DEMUXER_
typedef struct _header{
	unsigned int size;
	unsigned char str[4];
}header;

#define audio_bit	31
#define vkey_bit	30	

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
        FIL         m_file;
        char        filename[32];
        int         type;
	unsigned int video_len;
	unsigned int audio_len;
	unsigned int key_len;
	unsigned int sample_index;
	unsigned int channel_count;
	//unsigned int video_buf[2740][2];
	//unsigned int audio_buf[2740][2];
        //unsigned int (*video_buf)[2];
        //unsigned int (*audio_buf)[2];
        unsigned int (*all_buf)[2];
	//unsigned int all_buf[3200][2];
	unsigned int keyframe[100];
	unsigned char sps[0x40];
	unsigned char pps[0x10];
	unsigned short sps_length;
	unsigned short pps_length;
        unsigned int mp4_index;
        unsigned int total_len;
}mp4_demux,*pmp4_demux;

int get_video_trak(mp4_demux *mp4_demuxer,int index);
int get_audio_trak(mp4_demux *mp4_demuxer,int index);
void create_adts_header(int rate_idx,int channels,int length,unsigned char *adts_header);
int ccompare( const void *a , const void *b );
FRESULT fseek ( FIL * stream, long int offset, int origin );
FRESULT fread ( void * ptr, size_t size, size_t count, FIL * stream );
FRESULT fwrite ( const void * ptr, size_t size, size_t count, FIL * stream );
#endif