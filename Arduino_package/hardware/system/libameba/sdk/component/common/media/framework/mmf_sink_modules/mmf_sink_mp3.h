#ifndef MMF_SINK_MP3_FILE_H
#define MMF_SINK_MP3_FILE_H
#include "mmf_sink.h"
#include "FreeRTOS.h"
#include "task.h"
#include "platform_opts.h"
#include "mp3/mp3_codec.h"
#include "diag.h"
#include "i2s_api.h"
#include "analogin_api.h"
#include <stdlib.h>
#include "section_config.h"
#if CONFIG_EXAMPLE_MP3_STREAM_SGTL5000
#include "sgtl5000.h"
#else
#include "alc5651.h"
#endif

void mp3_sink_mod_close(void* ctx);
void* mp3_sink_mod_open(void);
int mp3_sink_mod_set_param(void* ctx, int cmd, int arg);
int mp3_sink_mod_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/

#endif /* MMF_SINK_MP3_FILE_H */