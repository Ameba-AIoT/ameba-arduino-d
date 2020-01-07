#ifndef _MMF_SOURCE_H1V6_JPEG_H_
#define _MMF_SOURCE_H1V6_JPEG_H_

#include "hantro_jpeg_encoder.h"

void* h1v6_jpeg_open(void);
void h1v6_jpeg_close(void* ctx);
int h1v6_jpeg_set_param(void* ctx, int cmd, int arg);
int h1v6_jpeg_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/
#define CMD_HANTRO_JPEG_SET_HEIGHT           0x10
#define CMD_HANTRO_JPEG_SET_WIDTH            0x11
#define CMD_HANTRO_JPEG_LEVEL                0x12
#define CMD_HANTRO_JPEG_FPS                  0x13
#define CMD_HANTRO_JPEG_HEADER_TYPE          0x14
#define CMD_HANTRO_JPEG_APPLY                0x15

#define CMD_ISP_STREAMID                     0X20
#define CMD_ISP_HW_SLOT                      0X21
#define CMD_ISP_SW_SLOT                      0X22

#define CMD_H1V6_MEMORY_SIZE                 0X30
#define CMD_H1V6_BLOCK_SIZE                  0X31
#define CMD_H1V6_MAX_FRAME_SIZE              0X32

#define CMD_SNAPSHOT_CB                      0X40

#endif