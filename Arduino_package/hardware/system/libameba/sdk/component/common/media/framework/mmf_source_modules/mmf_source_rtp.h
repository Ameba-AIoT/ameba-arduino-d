#ifndef _MMF_SOURCE_RTP_H_
#define _MMF_SOURCE_RTP_H_

#include "wifi_conf.h"
#include "lwip/api.h" //netconn use
#include <lwip/sockets.h>
#include "rtsp/rtp_api.h"

#define MAX_SELECT_SOCKET       8
#define DEFAULT_RTP_PORT        16384

#if CONFIG_EXAMPLE_MP3_STREAM_RTP    
#define G711_BLK_SIZE   (634)
#define AUDIO_BUF_SIZE  (646) //160 BYTE DATA + 12 BYTE RTP HEADER
#elif CONFIG_EXAMPLE_MEDIA_BISTREAM || CONFIG_EXAMPLE_MEDIA_H264_AND_TWO_WAY_AUDIO || CONFIG_MEDIA_STREAMING_AND_RECORDING
#define G711_BLK_SIZE   (160)
#define AUDIO_BUF_SIZE  (1500) //160 BYTE DATA + 12 BYTE RTP HEADER
#else
#define G711_BLK_SIZE   (160)
#define AUDIO_BUF_SIZE  (172) //160 BYTE DATA + 12 BYTE RTP HEADER
#endif

#define AUDIO_BUF_DEPTH   (4)

typedef struct rtp_exbuf_type
{
    u8 buf[AUDIO_BUF_SIZE];
    int len;
}rtp_exbuf_t;

typedef struct rtp_client_type
{
    TaskHandle_t task_handle;
    xQueueHandle cache_queue;
    xQueueHandle done_queue;
    struct connect_context connect_ctx;
    _sema rtp_sema;
    u8 rtp_shutdown;
	u8 rtp_socket_closed;	
}rtp_client_t;

void* rtp_mod_open(void);
void rtp_mod_close(void* ctx);
int rtp_mod_set_param(void* ctx, int cmd, int arg);
int rtp_mod_handle(void* ctx, void* b);

/*customized CMDs used for this module - all customized CMDs should start from 0x10 at least*/
#define CMD_RTP_SET_PRIV_BUF                0x10
#define CMD_RTP_SET_SINK_MODULE             0x11
#define CMD_RTP_SET_SUPPORTED_PAYLOAD_TYPE  0x12

#endif