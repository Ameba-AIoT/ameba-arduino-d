#ifndef _MMF_SOURCE_MJPG_FILE_H_
#define _MMF_SOURCE_MJPG_FILE_H_

#include "sample_jpeg.h"

void* mjpgf_mod_open(void);
void mjpgf_mod_close(void* ctx);
int mjpgf_mod_set_param(void* ctx, int cmd, int arg);
int mjpgf_mod_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/

#endif