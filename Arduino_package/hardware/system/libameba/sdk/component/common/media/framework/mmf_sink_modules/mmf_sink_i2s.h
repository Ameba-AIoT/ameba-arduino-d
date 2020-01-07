#ifndef _MMF_SINK_I2S_H_
#define _MMF_SINK_I2S_H_

#include "i2s_api.h"
#include "g711/g711_codec.h"
#include "alc5651.h"

void i2s_mod_close(void* ctx);
void* i2s_mod_open(void);
int i2s_mod_set_param(void* ctx, int cmd, int arg);
int i2s_mod_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/

#endif