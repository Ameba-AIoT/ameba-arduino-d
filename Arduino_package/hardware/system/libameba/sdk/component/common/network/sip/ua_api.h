#ifndef _UA_API_H_
#define _UA_API_H_



#include "dlist.h"
#include "basic_types.h"
#include "osdep_service.h"
#if defined(CONFIG_PLATFORM_8195A)
#include "mmf_dbg.h"
#endif
#if defined(CONFIG_PLATFORM_8195BHP)
#include "mmf2_dbg.h"
#endif
#include "rtsp/rtp_api.h"
//#include "sip/rtp_sip_api.h"
#include "avcodec.h"
#include "re/inc/baresip.h"




#define ENABLE_PROXY_SEVER 0

#if ENABLE_PROXY_SEVER
#define PROXY_SERVER_IP	"52.198.104.214"
#define PROXY_SERVER_PORT 5000
#endif



/* clock usage */
#define UA_DEPEND_CLK_HZ      configTICK_RATE_HZ      

#define UA_SERVICE_PRIORITY    3
#define UA_MAX_STREAM_NUM	2
#define UA_REQUEST_BUF_SIZE   512
#define UA_RESPONSE_BUF_SIZE 	MAX_SDP_SIZE //max size for response buffer  

#define DEF_SIP_PORT 5060
#define UA_SERVICE_PRIORITY    3
#define SIP_MAX_STREAM_NUM	2

/*rtsp cast mode list*/
#define UNICAST_UDP_MODE 1
#define UNICAST_TCP_MODE 2
#define MULTICAST_MODE 3


struct __internal_sip_payload{
	int codec_id;
	struct rtp_object payload;
};

struct ua_stream_context
{
	struct ua_context *parent;
	int stream_id; //sync with stream_flow id
	struct list_head input_queue;
	_mutex input_lock;
	struct list_head output_queue;
	_mutex output_lock;
	struct codec_info *codec;
	uint8_t media_type;
	uint8_t framerate;
	uint32_t bitrate;
	uint32_t samplerate;
	uint32_t channel;
	uint32_t old_depend_clock_tick;
	uint32_t rtp_timestamp;
	uint8_t use_rtp_tick_inc;
	uint8_t index;
	struct rtp_statistics statistics;
	struct rtp_periodic_report_s periodic_report;
	struct __internal_sip_payload rtpobj;
};




struct ua_session
{
	uint32_t id;
	uint8_t version;
	uint32_t start_time;
	uint32_t end_time;
	uint8_t *user;
	uint8_t *name;
};

struct ua_sip_transport
{
	uint8_t isRtp; //transport protocol
	uint8_t isTcp; //lower transport protocol
	uint8_t castMode; //unicast UDP(1) or unicast TCP(2) or multicast(3)
	int port_low;
	int port_high;
	int clientport_low;
	int clientport_high;
	int serverport_low;
	int serverport_high;
	uint8_t ttl; //multicast time to live
	//to be added if necessary
};

#if 1
struct ua_context
{
	int id;
	uint8_t is_connected_to_proxy;
	int proxy_socket;
	int proxy_port;
	u16 rtpseq[SIP_MAX_STREAM_NUM];
	char *peer_uri;
	struct ua *user_agent;
	struct call *call;	
	struct ua_sip_transport transport[SIP_MAX_STREAM_NUM]; /*<<mmfv2 connect info for RTP and encoder setting >>*/
	struct connect_context connect_ctx; /*<mmfv2 connect info for RTP and encoder setting >*/
	uint8_t is_ua_start;
	uint8_t is_rtp_start;			/*<RTP mmfv2 flag>*/
	_sema start_rtp_sema;		/*<RTP mmfv2 RTP send Semaphore, when up, start to RTP packet send>*/
	_sema start_ua_sema;
	void (* rtp_service_handle) (struct ua_context* ua_ctx);
	_sema start_proxy_connect_sema;
	_sema rtp_input_sema;
	_sema rtp_output_sema;
#ifdef SUPPORT_RTCP
	uint8_t is_rtcp_start;
	_sema start_rtcp_sema;
	void (* rtcp_service_handle) (struct ua_context* ua_ctx);
#endif
#ifdef SUPPORT_HTTP
    //to be added
#endif
	uint8_t nb_streams_setup;
	uint8_t nb_streams;
	struct ua_stream_context *stream_ctx;
	uint32_t pre_filter_packet;
};
#endif

void ua_time_sync_disable(void);
void ua_time_sync_enable(void);

void ua_transport_init(struct ua_context *ua_ctx);
void ua_stream_context_init(struct ua_context *ua_ctx, struct ua_stream_context *stream_ctx);
void ua_stream_context_clear(struct ua_stream_context *stream_ctx);

void ua_enable_stream(struct call *call);
void ua_disable_stream(struct call *call);

void ua_rtp_stream_statistics_sync(struct ua_stream_context *stream_ctx);
uint32_t ua_get_timestamp(struct ua_stream_context *stream_ctx, uint32_t current_clock_tick);


struct ua_context *ua_context_create(uint8_t nb_streams);
void ua_context_free(struct ua_context *ua_ctx);
int ua_is_stream_enabled(struct ua_context *ua_ctx);
int ua_is_service_enabled(struct ua_context *ua_ctx);
int ua_open(struct ua_context *ua_ctx);
void ua_close(struct ua_context *ua_ctx);
void ua_start(struct ua_context *ua_ctx);
void ua_stop(struct ua_context *ua_ctx);
void ua_rtp_object_in_stream_queue(struct rtp_object *payload, struct ua_stream_context *stream_ctx);
struct rtp_object *ua_rtp_object_out_stream_queue(struct ua_stream_context *stream_ctx);	





#endif
