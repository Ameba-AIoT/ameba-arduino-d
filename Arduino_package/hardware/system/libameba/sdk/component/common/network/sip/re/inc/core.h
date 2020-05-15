/**
 * @file core.h  Internal API
 *
 * Copyright (C) 2010 Creytiv.com
 */

	 


#include "baresip.h"
#include <limits.h>
#include "sip/ua_api.h"

/* max bytes in pathname */
#if defined (PATH_MAX)
#define FS_PATH_MAX PATH_MAX
#elif defined (_POSIX_PATH_MAX)
#define FS_PATH_MAX _POSIX_PATH_MAX
#else
#define FS_PATH_MAX 512
#endif


/**
 * RFC 3551:
 *
 *    0 -  95  Static payload types
 *   96 - 127  Dynamic payload types
 */
enum {
	PT_CN       = 13,
	PT_STAT_MIN = 0,
	PT_STAT_MAX = 95,
	PT_DYN_MIN  = 96,
	PT_DYN_MAX  = 127
};


/** Media constants */
enum {
	AUDIO_BANDWIDTH = 128000,  /**< Bandwidth for audio in bits/s      */
	VIDEO_SRATE     =  90000,  /**< Sampling rate for video            */
};


/* forward declarations */
struct stream_param;


/*
 * Account
 */


struct account {
	char *buf;                   /**< Buffer for the SIP address         */
	struct sip_addr laddr;       /**< Decoded SIP address                */
	struct uri luri;             /**< Decoded AOR uri                    */
	char *dispname;              /**< Display name                       */
	char *aor;                   /**< Local SIP uri                      */

	/* parameters: */
	enum answermode answermode;  /**< Answermode for incoming calls      */
	struct le acv[8];            /**< List elements for aucodecl         */
	struct list aucodecl;        /**< List of preferred audio-codecs     */
	char *auth_user;             /**< Authentication username            */
	char *auth_pass;             /**< Authentication password            */
	char *mnatid;                /**< Media NAT handling                 */
	char *mencid;                /**< Media encryption type              */
	const struct mnat *mnat;     /**< MNAT module                        */
	const struct menc *menc;     /**< MENC module                        */
	char *outboundv[2];          /**< Optional SIP outbound proxies      */
	uint32_t ptime;              /**< Configured packet time in [ms]     */
	uint32_t regint;             /**< Registration interval in [seconds] */
	uint32_t pubint;             /**< Publication interval in [seconds]  */
	char *regq;                  /**< Registration Q-value               */
	char *rtpkeep;               /**< RTP Keepalive mechanism            */
	char *sipnat;                /**< SIP Nat mechanism                  */
	char *stun_user;             /**< STUN Username                      */
	char *stun_pass;             /**< STUN Password                      */
	char *stun_host;             /**< STUN Hostname                      */
	uint16_t stun_port;          /**< STUN Port number                   */
	struct le vcv[4];            /**< List elements for vidcodecl        */
	struct list vidcodecl;       /**< List of preferred video-codecs     */
};


/*
 * Audio Player
 */

struct auplay_st {
	struct auplay *ap;
};

struct auplay {
	struct le        le;
	const char      *name;
	auplay_alloc_h  *alloch;
};


/*
 * Audio Source
 */

struct ausrc_st {
	const struct ausrc *as;
};

struct ausrc {
	struct le        le;
	const char      *name;
	ausrc_alloc_h   *alloch;
};


/*
 * Audio Stream
 */

struct audio;

typedef void (audio_event_h)(int key, bool end, void *arg);
typedef void (audio_err_h)(int err, const char *str, void *arg);

int audio_alloc(struct audio **ap, const struct stream_param *stream_prm,
		const struct config *cfg,
		struct call *call, struct sdp_session *sdp_sess, int label,
		const struct mnat *mnat, struct mnat_sess *mnat_sess,
		const struct menc *menc, struct menc_sess *menc_sess,
		uint32_t ptime, const struct list *aucodecl, bool offerer,
		audio_event_h *eventh, audio_err_h *errh, void *arg);
#ifndef ENABLE_SIP_VIDEO
int  audio_start_sip(struct audio *a);
void audio_stop(struct audio *a);
int  audio_encoder_set(struct audio *a, const struct aucodec *ac,
		       int pt_tx, const char *params);
int  audio_decoder_set(struct audio *a, const struct aucodec *ac,
		       int pt_rx, const char *params);
int  audio_send_digit(struct audio *a, char key);
void audio_sdp_attr_decode(struct audio *a);
int  audio_print_rtpstat(struct re_printf *pf, const struct audio *au);
#endif

/*
 * BFCP
 */

struct bfcp;
int bfcp_alloc(struct bfcp **bfcpp, struct sdp_session *sdp_sess,
	       const char *proto, bool offerer,
	       const struct mnat *mnat, struct mnat_sess *mnat_sess);
int bfcp_start(struct bfcp *bfcp);


/*
 * Call Control
 */

enum {
	CALL_LINENUM_MIN  =   1,
	CALL_LINENUM_MAX  = 256
};

struct call;

/** Call parameters */
struct call_prm {
	struct sa laddr;
	enum vidmode vidmode;
	int af;
	bool use_rtp;
};

int  call_alloc(struct call **callp, const struct config *cfg,
		struct list *lst,
		const char *local_name, const char *local_uri,
		struct account *acc, struct ua *ua, const struct call_prm *prm,
		const struct sip_msg *msg, struct call *xcall,
		struct dnsc *dnsc,
		call_event_h *eh, void *arg);
int  call_connect(struct ua_context *ua_ctx,struct call *call, const struct pl *paddr);
int  call_accept(struct call *call, struct sipsess_sock *sess_sock,
		 const struct sip_msg *msg);
int  call_hangup(struct call *call, uint16_t scode, const char *reason);
int  call_progress(struct ua_context *ua_ctx,struct call *call);
int  call_answer(struct call *call, uint16_t scode);
int  call_sdp_get(const struct call *call, struct mbuf **descp, bool offer);
int  call_jbuf_stat(struct re_printf *pf, const struct call *call);
int  call_info(struct re_printf *pf, const struct call *call);
int  call_reset_transp(struct call *call, const struct sa *laddr);
int  call_notify_sipfrag(struct call *call, uint16_t scode,
			 const char *reason, ...);
int  call_af(const struct call *call);
void call_set_xrtpstat(struct call *call);
struct account *call_account(const struct call *call);


/*
 * Conf
 */

int conf_get_range(const struct conf *conf, const char *name,
		   struct range *rng);
int conf_get_csv(const struct conf *conf, const char *name,
		 char *str1, size_t sz1, char *str2, size_t sz2);


/*
 * Media control
 */

int mctrl_handle_media_control(struct pl *body, bool *pfu);


/*
 * Media NAT traversal
 */

struct mnat {
	struct le le;
	const char *id;
	const char *ftag;
	mnat_sess_h *sessh;
	mnat_media_h *mediah;
	mnat_update_h *updateh;
};

const struct mnat *mnat_find(const struct list *mnatl, const char *id);


/*
 * Metric
 */

struct metric {
	/* internal stuff: */
	struct tmr tmr;
	uint64_t ts_start;
	bool started;

	/* counters: */
	uint32_t n_packets;
	uint32_t n_bytes;
	uint32_t n_err;

	/* bitrate calculation */
	uint32_t cur_bitrate;
	uint64_t ts_last;
	uint32_t n_bytes_last;
};

void     metric_init(struct metric *metric);
void     metric_reset(struct metric *metric);
void     metric_add_packet(struct metric *metric, size_t packetsize);
double   metric_avg_bitrate(const struct metric *metric);


/*
 * Module
 */

int module_init(const struct conf *conf);
void module_app_unload(void);


/*
 * Register client
 */

struct reg;

int  reg_add(struct list *lst, struct ua *ua, int regid);
int  reg_register(struct reg *reg, const char *reg_uri,
		    const char *params, uint32_t regint, const char *outbound);
void reg_unregister(struct reg *reg);
bool reg_isok(const struct reg *reg);
int  reg_debug(struct re_printf *pf, const struct reg *reg);
int  reg_status(struct re_printf *pf, const struct reg *reg);


/*
 * RTP Header Extensions
 */

#define RTPEXT_HDR_SIZE        4
#define RTPEXT_TYPE_MAGIC 0xbede

enum {
	RTPEXT_ID_MIN  =  1,
	RTPEXT_ID_MAX  = 14,
};

enum {
	RTPEXT_LEN_MIN =  1,
	RTPEXT_LEN_MAX = 16,
};

struct rtpext {
	unsigned id:4;
	unsigned len:4;
	uint8_t data[RTPEXT_LEN_MAX];
};


int rtpext_hdr_encode(struct mbuf *mb, size_t num_bytes);
int rtpext_encode(struct mbuf *mb, unsigned id, unsigned len,
		  const uint8_t *data);
int rtpext_decode(struct rtpext *ext, struct mbuf *mb);


/*
 * RTP keepalive
 */

struct rtpkeep;

int  rtpkeep_alloc(struct rtpkeep **rkp, const char *method, int proto,
		   struct rtp_sock *rtp, struct sdp_media *sdp);
void rtpkeep_refresh(struct rtpkeep *rk, uint32_t ts);


/*
 * SDP
 */

int sdp_decode_multipart(const struct pl *ctype_prm, struct mbuf *mb);
const struct sdp_format *sdp_media_format_cycle(struct sdp_media *m);


/*
 * Stream
 */

struct rtp_header;

enum {STREAM_PRESZ = 4+12}; /* same as RTP_HEADER_SIZE */

typedef void (stream_rtp_h)(const struct rtp_header *hdr,
			    struct rtpext *extv, size_t extc,
			    struct mbuf *mb, void *arg);
typedef void (stream_rtcp_h)(struct rtcp_msg *msg, void *arg);

typedef void (stream_error_h)(struct stream *strm, int err, void *arg);

/** Common parameters for media stream */
struct stream_param {
	bool use_rtp;
};

/** Defines a generic media stream */
struct stream {
	struct le le;            /**< Linked list element                   */
	struct config_avt cfg;   /**< Stream configuration                  */
	struct call *call;       /**< Ref. to call object                   */
	struct sdp_media *sdp;   /**< SDP Media line                        */
	struct rtp_sock *rtp;    /**< RTP Socket                            */
	struct rtpkeep *rtpkeep; /**< RTP Keepalive                         */
	struct rtcp_stats rtcp_stats;/**< RTCP statistics                   */
	struct jbuf *jbuf;       /**< Jitter Buffer for incoming RTP        */
	struct mnat_media *mns;  /**< Media NAT traversal state             */
	const struct menc *menc; /**< Media encryption module               */
	struct menc_sess *mencs; /**< Media encryption session state        */
	struct menc_media *mes;  /**< Media Encryption media state          */
	struct metric metric_tx; /**< Metrics for transmit                  */
	struct metric metric_rx; /**< Metrics for receiving                 */
	char *cname;             /**< RTCP Canonical end-point identifier   */
	uint32_t ssrc_rx;        /**< Incoming syncronizing source          */
	uint32_t pseq;           /**< Sequence number for incoming RTP      */
	int pt_enc;              /**< Payload type for encoding             */
	bool rtcp;               /**< Enable RTCP                           */
	bool rtcp_mux;           /**< RTP/RTCP multiplex supported by peer  */
	bool jbuf_started;       /**< True if jitter-buffer was started     */
	stream_rtp_h *rtph;      /**< Stream RTP handler                    */
	stream_rtcp_h *rtcph;    /**< Stream RTCP handler                   */
	void *arg;               /**< Handler argument                      */
	stream_error_h *errorh;  /**< Stream error handler                  */
	void *errorh_arg;        /**< Error handler argument                */
	struct tmr tmr_rtp;      /**< Timer for detecting RTP timeout       */
	uint64_t ts_last;        /**< Timestamp of last received RTP pkt    */
	bool terminated;         /**< Stream is terminated flag             */
	uint32_t rtp_timeout_ms; /**< RTP Timeout value in [ms]             */
	bool rtp_estab;          /**< True if RTP stream is established     */
};

int  stream_alloc(struct stream **sp, const struct stream_param *prm,
		  const struct config_avt *cfg,
		  struct call *call, struct sdp_session *sdp_sess,
		  const char *name, int label,
		  const struct mnat *mnat, struct mnat_sess *mnat_sess,
		  const struct menc *menc, struct menc_sess *menc_sess,
		  const char *cname,
		  stream_rtp_h *rtph, stream_rtcp_h *rtcph, uint16_t media_port,void *arg);
struct sdp_media *stream_sdpmedia(const struct stream *s);
struct rtp_sock *stream_rtp(const struct stream *s);
int  stream_send(struct stream *s, bool ext, bool marker, int pt, uint32_t ts,
		 struct mbuf *mb);
void stream_update(struct stream *s);
void stream_update_encoder(struct stream *s, int pt_enc);
int  stream_jbuf_stat(struct re_printf *pf, const struct stream *s);
void stream_hold(struct stream *s, bool hold);
void stream_set_srate(struct stream *s, uint32_t srate_tx, uint32_t srate_rx);
void stream_send_fir(struct stream *s, bool pli);
void stream_reset(struct stream *s);
void stream_set_bw(struct stream *s, uint32_t bps);
void stream_set_error_handler(struct stream *strm,
			      stream_error_h *errorh, void *arg);
int  stream_debug(struct re_printf *pf, const struct stream *s);
int  stream_print(struct re_printf *pf, const struct stream *s);
void stream_enable_rtp_timeout(struct stream *strm, uint32_t timeout_ms);


/*
 * User-Agent
 */

struct ua;

void         ua_event(struct ua *ua, enum ua_event ev, struct call *call,
		      const char *fmt, ...);
void         ua_printf(const struct ua *ua, const char *fmt, ...);

struct tls  *uag_tls(void);
const char  *uag_allowed_methods(void);



/*
 * Video Display
 */

struct vidisp {
	struct le        le;
	const char      *name;
	vidisp_alloc_h  *alloch;
	vidisp_update_h *updateh;
	vidisp_disp_h   *disph;
	vidisp_hide_h   *hideh;
};




/*
 * Video Source
 */

struct vidsrc {
	struct le         le;
	const char       *name;
	vidsrc_alloc_h   *alloch;
	vidsrc_update_h  *updateh;
};

struct vidsrc *vidsrc_get(struct vidsrc_st *st);

struct vidisp *vidisp_get(struct vidisp_st *st);

/*
 * Video Stream
 */

struct video;

typedef void (video_err_h)(int err, const char *str, void *arg);

#if 1 
int  video_alloc(struct video **vp, const struct stream_param *stream_prm,
		 const struct config *cfg,
		 struct call *call, struct sdp_session *sdp_sess, int label,
		 const struct mnat *mnat, struct mnat_sess *mnat_sess,
		 const struct menc *menc, struct menc_sess *menc_sess,
		 const char *content, const struct list *vidcodecl,
		 video_err_h *errh, void *arg);
#endif

#ifndef CONFIG_PLATFORM_8195BHP

int  video_start(struct video *v, const char *peer);
void video_stop(struct video *v);
bool video_is_started(const struct video *v);
int  video_encoder_set(struct video *v, struct vidcodec *vc,
		       int pt_tx, const char *params);
int  video_decoder_set(struct video *v, struct vidcodec *vc, int pt_rx,
		       const char *fmtp);
void video_update_picture(struct video *v);
void video_sdp_attr_decode(struct video *v);
int  video_print(struct re_printf *pf, const struct video *v);

#endif

/*
 * Timestamp helpers
 */


/**
 * This struct is used to keep track of timestamps for
 * incoming RTP packets.
 */
struct timestamp_recv {
	uint32_t first;
	uint32_t last;
	bool is_set;
	unsigned num_wraps;
};


static inline uint64_t calc_extended_timestamp(uint32_t num_wraps, uint32_t ts)
{
	uint64_t ext_ts;

	ext_ts  = (uint64_t)num_wraps * 0x100000000ULL;
	ext_ts += (uint64_t)ts;

	return ext_ts;
}


static inline uint64_t timestamp_duration(const struct timestamp_recv *ts)
{
	uint64_t last_ext;

	if (!ts || !ts->is_set)
		return 0;

	last_ext = calc_extended_timestamp(ts->num_wraps, ts->last);

	return last_ext - ts->first;
}


/*
 *  -1  backwards wrap-around
 *   0  no wrap-around
 *   1  forward wrap-around
 */
static inline int timestamp_wrap(uint32_t ts_new, uint32_t ts_old)
{
	int32_t delta;

	if (ts_new < ts_old) {

		delta = (int32_t)ts_new - (int32_t)ts_old;

		if (delta > 0)
			return 1;
	}
	else if ((int32_t)(ts_old - ts_new) > 0) {

		return -1;
	}

	return 0;
}


/*
 * Timer
 */

uint64_t tmr_jiffies_usec(void);


