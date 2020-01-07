#ifndef _MMF_SOURCE_H264_UNIT_H_
#define _MMF_SOURCE_H264_UNIT_H_

#include "h264_encode.h"

void* h264_unit_open(void);
void h264_unit_close(void* ctx);
int h264_unit_set_param(void* ctx, int cmd, int arg);
int h264_unit_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/

#endif