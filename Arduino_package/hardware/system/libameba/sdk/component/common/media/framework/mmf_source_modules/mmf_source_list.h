#ifndef _MEDIA_MODULES_H
#define _MEDIA_MODULES_H
#include "../mmf_source.h"

//list all avaliable modules here
extern msrc_module_t uvc_module;
extern msrc_module_t geo_module;
extern msrc_module_t mjpgf_module;
extern msrc_module_t h264f_module;
extern msrc_module_t h264_unit_module;
extern msrc_module_t h264sd_module;
extern msrc_module_t aacf_module;
extern msrc_module_t pcmuf_module;
extern msrc_module_t i2s_module;
extern msrc_module_t rtp_src_module;
extern msrc_module_t mp4de_module;
extern msrc_module_t audio_module;
extern msrc_module_t audio_wrapper_module;
#ifdef CONFIG_PLATFORM_8195BHP
extern msrc_module_t aac_enc_module;
extern msrc_module_t h1v6_nv12_module;
#endif
#ifdef CONFIG_PLATFORM_8195BHP
extern msrc_module_t h1v6_h264_module;
extern msrc_module_t h1v6_jpeg_module;
#endif
#endif