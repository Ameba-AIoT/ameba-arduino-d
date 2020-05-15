#ifndef _MMF_WRAPPER_PRO_AUDIO_H_
#define _MMF_WRAPPER_PRO_AUDIO_H_

#include "g711/g711_codec.h"

#define ENABLE_SPEEX_AEC (1)
#define ENABLE_AAC_ENC   (1)
#define ENABLE_AAC_DEC   (1)

#define AUDIO_DMA_PAGE_NUM 2
#define AUDIO_DMA_PAGE_SIZE (320)

#define G711_FSIZE (160)

void *audio_wrapper_open();
void audio_wrapper_close();
int audio_wrapper_set_param(void *ctx, int cmd, int arg);
int audio_wrapper_sink_mod_handle(void *ctx, void *b);
int audio_wrapper_source_mod_handle(void *ctx, void *b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/
#define CMD_AUDIO_SET_SOURCE_FRAMETYPE              0x10
#define CMD_AUDIO_SET_SINK_FRAMETYPE                0x11
#define CMD_AUDIO_TX_ENABLE                         0x12
#define CMD_AUDIO_RX_ENABLE                         0x13

#define CMD_AUDIO_AACEN_SET_SAMPLERATE              0x20
#define CMD_AUDIO_AACEN_SET_CHANNEL                 0x21
#define CMD_AUDIO_AACEN_SET_APPLY                   0x22

#define CMD_AUDIO_AACDE_SET_SAMPLERATE              0x30
#define CMD_AUDIO_AACDE_SET_CHANNEL                 0x31
#define CMD_AUDIO_AACDE_SET_APPLY                   0x32

#endif
