#ifndef _MMF_SOURCE_MP4DE_H_
#define _MMF_SOURCE_MP4DE_H_


#define CMD_SET_SAMPLERATE		0x06
#define CMD_SET_CHANNEL			0x07
#define CMD_SET_CODEC			0x08
#define CMD_SET_STREAMNUM		0x09
#define CMD_SET_APPLY			0x1f

/*mp4 storage*/
#define CMD_SET_ST_PERIOD               0X60
#define CMD_SET_ST_TOTAL                0X61
#define CMD_SET_ST_TYPE                 0X62
#define CMD_SET_ST_FILENAME             0x63
#define CMD_SET_ST_START                0x64
/*mp4 storage*/
#define STORAGE_ALL     0
#define STORAGE_VIDEO   1
#define STORAGE_AUDIO   2

#endif