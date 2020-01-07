#ifndef _MMF_SINK_AUDIO_H_
#define _MMF_SINK_AUDIO_H_

#include "audio_api.h"
#include "g711/g711_codec.h"

#include "mmf_source.h"
#include "cmsis.h"
//#include "shell.h"
//#include "main.h"
//#include "hal_cache.h"
//#include "hal_timer.h"
#include "cmsis_os.h"
#include <math.h>
#include "hal_timer.h"
#include "sgpio_api.h"
#include "audio_api.h"

typedef enum {
	AUDIO_MODE_UNKNOWN = -1,
	AUDIO_MODE_G711A = 1,
	AUDIO_MODE_G711U = 2 
}Audio_mode;

typedef struct {
	Audio_mode mode;	//intend to encode as G711A or G711U
	int len;	//data length
//	_sema RWSema;
	unsigned char *raw_data;	//address of buffered data (not encoded
	int data_start; // a shift value to show where the data starts
	int data_end;	// a shift value shows data ending
}audio_buf_context;

void audio_mod_close(void* ctx);
void* audio_mod_open(void);
int audio_mod_set_param(void* ctx, int cmd, int arg);
int audio_mod_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/

#endif