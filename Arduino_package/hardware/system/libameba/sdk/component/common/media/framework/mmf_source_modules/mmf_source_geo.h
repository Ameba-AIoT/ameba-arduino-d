#ifndef _MMF_SOURCE_GEO_H_
#define _MMF_SOURCE_GEO_H_

#include "videodev2.h"
#include "uvcvideo.h"
#include "v4l2_intf.h"

#include "driver/geo/patch_uvc_geo.h"

void* geo_mod_open(void);
void geo_mod_close(void* ctx);
int geo_mod_set_param(void* ctx, int cmd, int arg);
int geo_mod_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/
#define CMD_GEO_SELECT_CHANNEL          0x10
#define CMD_GEO_SET_HEIGHT              0x11
#define CMD_GEO_SET_WIDTH               0x12
#define CMD_GEO_SET_FRAMERATE           0x13
#define CMD_GEO_SET_SAMPLERATE          0x14
#define CMD_GEO_SET_CPZRATIO            0x15
#define CMD_GEO_SET_FRAMETYPE           0x16
#define CMD_GEO_SET_APPLY               0x17
#define CMD_GEO_SET_CHANNEL             0x18


#endif