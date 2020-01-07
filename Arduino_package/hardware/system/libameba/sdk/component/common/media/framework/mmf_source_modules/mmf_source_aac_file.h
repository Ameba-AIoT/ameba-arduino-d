#ifndef _MMF_SOURCE_AAC_FILE_H_
#define _MMF_SOURCE_AAC_FILE_H_

#include "sample_aac.h"

void* aacf_mod_open(void);
void aacf_mod_close(void* ctx);
int aacf_mod_set_param(void* ctx, int cmd, int arg);
int aacf_mod_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/

#endif