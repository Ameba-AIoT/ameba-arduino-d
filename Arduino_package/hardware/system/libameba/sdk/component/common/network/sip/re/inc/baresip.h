/**
 * @file baresip.h  Public Interface to Baresip
 *
 * Copyright (C) 2010 Creytiv.com
 */

#ifndef BARESIP_H__
#define BARESIP_H__

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_PLATFORM_8195BHP
#include <platform/platform_stdlib.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#include "lwip/sockets.h"
#include "FreeRTOS.h"
#include "task.h"
#include "re.h"
//#include "sip/ua_api.h"


#endif

/** Defines the Baresip version string */
#define BARESIP_VERSION "0.0.1"


#ifndef NET_MAX_NS
#define NET_MAX_NS (4)
#endif


/* forward declarations */
struct ua_context;
struct sa;
struct sdp_media;
struct sdp_session;
struct sip_msg;
struct stream;
struct ua;
struct call;
struct vidframe;
struct vidrect;
struct vidsz;

#define SIPTEST_DEBUG 0

#if SIPTEST_DEBUG
#define SIP_PRINTF(fmt, args...)    printf("\n\r%s: " fmt, __FUNCTION__, ## args)
#define SIP_ERROR(fmt, args...)     printf("\n\r%s: " fmt, __FUNCTION__, ## args)
#else
#define SIP_PRINTF(fmt, args...)    
#define SIP_ERROR(fmt, args...)   
#endif


#if 0
#define warning(fmt, args...)    printf("\n\r%s: " fmt, __FUNCTION__, ## args)
#define debug(fmt, args...)    printf("\n\r%s: " fmt, __FUNCTION__, ## args)
#else
#define debug(fmt, args...)
#define info(fmt, args...)    printf("\n\r%s: " fmt, __FUNCTION__, ## args)
#define warning(fmt, args...)    printf("\n\r%s: " fmt, __FUNCTION__, ## args)
#endif

//extern struct call *call;




/*
 * Account
 */

/** Defines the answermodes */
enum answermode {
	ANSWERMODE_MANUAL = 0,
	ANSWERMODE_EARLY,
	ANSWERMODE_AUTO
};

struct account;

int account_alloc(struct account **accp, const char *sipaddr);
int account_debug(struct re_printf *pf, const struct account *acc);
int account_set_auth_pass(struct account *acc, const char *pass);
int account_set_display_name(struct account *acc, const char *dname);
int account_auth(const struct account *acc, char **username, char **password,
		 const char *realm);
struct list *account_aucodecl(const struct account *acc);
struct list *account_vidcodecl(const struct account *acc);
struct sip_addr *account_laddr(const struct account *acc);
uint32_t account_regint(const struct account *acc);
uint32_t account_pubint(const struct account *acc);
uint32_t account_ptime(const struct account *acc);
enum answermode account_answermode(const struct account *acc);
const char *account_aor(const struct account *acc);
const char *account_auth_user(const struct account *acc);
const char *account_auth_pass(const struct account *acc);
const char *account_outbound(const struct account *acc, unsigned ix);
const char *account_stun_user(const struct account *acc);
const char *account_stun_pass(const struct account *acc);
const char *account_stun_host(const struct account *acc);


/*
 * Audio-level
 */


#define AULEVEL_MIN  (-96.0)
#define AULEVEL_MAX    (0.0)


double aulevel_calc_dbov(const int16_t *sampv, size_t sampc);


/*
 * Call
 */

enum call_event {
	CALL_EVENT_INCOMING,
	CALL_EVENT_RINGING,
	CALL_EVENT_PROGRESS,
	CALL_EVENT_ESTABLISHED,
	CALL_EVENT_CLOSED,
	CALL_EVENT_TRANSFER,
	CALL_EVENT_TRANSFER_FAILED,
};

/** Call States */
enum state {
	STATE_IDLE = 0,
	STATE_INCOMING,
	STATE_OUTGOING,
	STATE_RINGING,
	STATE_EARLY,
	STATE_ESTABLISHED,
	STATE_TERMINATED
};




typedef void (call_event_h)(struct call *call, enum call_event ev,
			    const char *str, void *arg);
typedef void (call_dtmf_h)(struct call *call, char key, void *arg);

int  call_modify(struct call *call);
int  call_hold(struct call *call, bool hold);
int  call_send_digit(struct call *call, char key);
bool call_has_audio(const struct call *call);
bool call_has_video(const struct call *call);
int  call_transfer(struct call *call, const char *uri);
int  call_status(struct re_printf *pf, const struct call *call);
int  call_debug(struct re_printf *pf, const struct call *call);
void call_set_handlers(struct call *call, call_event_h *eh,
		       call_dtmf_h *dtmfh, void *arg);
uint16_t      call_scode(const struct call *call);
uint32_t      call_duration(const struct call *call);
uint32_t      call_setup_duration(const struct call *call);
const char   *call_id(const struct call *call);
const char   *call_peeruri(const struct call *call);
const char   *call_peername(const struct call *call);
const char   *call_localuri(const struct call *call);
struct audio *call_audio(const struct call *call);
#ifndef ENABLE_SIP_VIDEO
struct stream *call_stream(const struct call *call);
#endif
struct video *call_video(const struct call *call);
struct list  *call_streaml(const struct call *call);
struct ua    *call_get_ua(const struct call *call);
bool          call_is_onhold(const struct call *call);
bool          call_is_outgoing(const struct call *call);
void          call_enable_rtp_timeout(struct call *call, uint32_t timeout_ms);
uint32_t      call_linenum(const struct call *call);
struct call  *call_find_linenum(const struct list *calls, uint32_t linenum);
void call_set_current(struct list *calls, struct call *call);


/*
 * Conf (utils)
 */


/** Defines the configuration line handler */
typedef int (confline_h)(const struct pl *addr, void *arg);

int  conf_configure(void);
int  conf_modules(void);
void conf_path_set(const char *path);
int  conf_path_get(char *path, size_t sz);
int  conf_parse(const char *filename, confline_h *ch, void *arg);
int  conf_get_vidsz(const struct conf *conf, const char *name,
		    struct vidsz *sz);
int  conf_get_sa(const struct conf *conf, const char *name, struct sa *sa);
bool conf_fileexist(const char *path);
void conf_close(void);
struct conf *conf_cur(void);


/*
 * Config (core configuration)
 */

/** A range of numbers */
struct range {
	uint32_t min;  /**< Minimum number */
	uint32_t max;  /**< Maximum number */
};

static inline bool in_range_sip(const struct range *rng, uint32_t val)
{
	return rng ? (val >= rng->min && val <= rng->max) : false;
}

/** Audio transmit mode */
enum audio_mode {
	AUDIO_MODE_POLL = 0,         /**< Polling mode                  */
	AUDIO_MODE_THREAD,           /**< Use dedicated thread          */
};


/** SIP User-Agent */
struct config_sip {
	uint32_t trans_bsize;   /**< SIP Transaction bucket size    */
	char uuid[64];          /**< Universally Unique Identifier  */
	char local[64];         /**< Local SIP Address              */
	char cert[256];         /**< SIP Certificate                */
};

/** Call config */
struct config_call {
	uint32_t local_timeout; /**< Incoming call timeout [sec] 0=off */
	uint32_t max_calls;     /**< Maximum number of calls, 0=unlimited */
};

/** Audio */
struct config_audio {
	char audio_path[256];   /**< Audio file directory           */
	char src_mod[16];       /**< Audio source module            */
	char src_dev[128];      /**< Audio source device            */
	char play_mod[16];      /**< Audio playback module          */
	char play_dev[128];     /**< Audio playback device          */
	char alert_mod[16];     /**< Audio alert module             */
	char alert_dev[128];    /**< Audio alert device             */
	struct range srate;     /**< Audio sampling rate in [Hz]    */
	struct range channels;  /**< Nr. of audio channels (1=mono) */
	uint32_t srate_play;    /**< Opt. sampling rate for player  */
	uint32_t srate_src;     /**< Opt. sampling rate for source  */
	uint32_t channels_play; /**< Opt. channels for player       */
	uint32_t channels_src;  /**< Opt. channels for source       */
	bool src_first;         /**< Audio source opened first      */
	enum audio_mode txmode; /**< Audio transmit mode            */
	bool level;             /**< Enable audio level indication  */
	int src_fmt;            /**< Audio source sample format     */
	int play_fmt;           /**< Audio playback sample format   */
	int enc_fmt;            /**< Audio encoder sample format    */
	int dec_fmt;            /**< Audio decoder sample format    */
};

#if 1 
/** Video */
struct config_video {
	char src_mod[16];       /**< Video source module            */
	char src_dev[128];      /**< Video source device            */
	char disp_mod[16];      /**< Video display module           */
	char disp_dev[128];     /**< Video display device           */
	unsigned width, height; /**< Video resolution               */
	uint32_t bitrate;       /**< Encoder bitrate in [bit/s]     */
	uint32_t fps;           /**< Video framerate                */
	bool fullscreen;        /**< Enable fullscreen display      */
	int enc_fmt;            /**< Encoder pixelfmt (enum vidfmt) */
};
#endif

/** Audio/Video Transport */
struct config_avt {
	uint8_t rtp_tos;        /**< Type-of-Service for outg. RTP  */
	struct range rtp_ports; /**< RTP port range                 */
	struct range rtp_bw;    /**< RTP Bandwidth range [bit/s]    */
	bool rtcp_enable;       /**< RTCP is enabled                */
	bool rtcp_mux;          /**< RTP/RTCP multiplexing          */
	struct range jbuf_del;  /**< Delay, number of frames        */
	bool rtp_stats;         /**< Enable RTP statistics          */
	uint32_t rtp_timeout;   /**< RTP Timeout in seconds (0=off) */
};

/* Network */
struct config_net {
	char ifname[16];        /**< Bind to interface (optional)   */
	struct {
		char addr[64];
	} nsv[NET_MAX_NS];      /**< Configured DNS nameservers     */
	size_t nsc;             /**< Number of DNS nameservers      */
};

#ifdef USE_VIDEO
/* BFCP */
struct config_bfcp {
	char proto[16];         /**< BFCP Transport (optional)      */
};
#endif

/** SDP */
struct config_sdp {
	bool ebuacip;           /**< Enable EBU-ACIP parameters     */
};


/** Core configuration */
struct config {

	struct config_sip sip;

	struct config_call call;

	struct config_audio audio;

#if 1 
	struct config_video video;
#endif
	struct config_avt avt;

	struct config_net net;

#ifdef USE_VIDEO
	struct config_bfcp bfcp;
#endif

	struct config_sdp sdp;
};

int config_parse_conf(struct config *cfg, const struct conf *conf);
int config_print(struct re_printf *pf, const struct config *cfg);
int config_write_template(const char *file, const struct config *cfg);
struct config *conf_config(void);


/*
 * Contact
 */

enum presence_status {
	PRESENCE_UNKNOWN,
	PRESENCE_OPEN,
	PRESENCE_CLOSED,
	PRESENCE_BUSY
};


struct contact;
typedef void (contact_update_h)(struct contact *c, bool removed, void *arg);

struct contacts {
	struct list cl;
	struct hash *cht;

	contact_update_h *handler;
	void* handler_arg;
};


int  contact_init(struct contacts *contacts);
void contact_close(struct contacts *contacts);
int  contact_add(struct contacts *contacts,
		 struct contact **contactp, const struct pl *addr);
void contact_remove(struct contacts *contacts, struct contact *c);
void contact_set_update_handler(struct contacts *contacs,
				contact_update_h *updateh, void *arg);
int  contacts_print(struct re_printf *pf, const struct contacts *contacts);
enum presence_status contact_presence(const struct contact *c);
void contact_set_presence(struct contact *c, enum presence_status status);
bool contact_block_access(const struct contacts *contacts, const char *uri);
struct contact  *contact_find(const struct contacts *contacts,
			      const char *uri);
struct sip_addr *contact_addr(const struct contact *c);
struct list     *contact_list(const struct contacts *contacts);
const char      *contact_str(const struct contact *c);
const char      *contact_presence_str(enum presence_status status);


/*
 * Media Context
 */

/** Media Context */
struct media_ctx {
	const char *id;  /**< Media Context identifier */
};


/*
 * Message
 */

typedef void (message_recv_h)(const struct pl *peer, const struct pl *ctype,
			      struct mbuf *body, void *arg);

struct message;
struct message_lsnr;

int  message_init(struct message **messagep);
int  message_listen(struct message_lsnr **lsnrp, struct message *message,
		    message_recv_h *h, void *arg);
int  message_send(struct ua *ua, const char *peer, const char *msg,
		  sip_resp_h *resph, void *arg);


/*
 * Audio Source
 */

struct ausrc;
struct ausrc_st;

/** Audio Source parameters */
struct ausrc_prm {
	uint32_t   srate;       /**< Sampling rate in [Hz] */
	uint8_t    ch;          /**< Number of channels    */
	uint32_t   ptime;       /**< Wanted packet-time in [ms] */
	int        fmt;         /**< Sample format (enum aufmt) */
};

typedef void (ausrc_read_h)(const void *sampv, size_t sampc, void *arg);
typedef void (ausrc_error_h)(int err, const char *str, void *arg);

typedef int  (ausrc_alloc_h)(struct ausrc_st **stp, const struct ausrc *ausrc,
			     struct media_ctx **ctx,
			     struct ausrc_prm *prm, const char *device,
			     ausrc_read_h *rh, ausrc_error_h *errh, void *arg);

int ausrc_register(struct ausrc **asp, struct list *ausrcl, const char *name,
		   ausrc_alloc_h *alloch);
const struct ausrc *ausrc_find(const struct list *ausrcl, const char *name);
int ausrc_alloc(struct ausrc_st **stp, struct list *ausrcl,
		struct media_ctx **ctx,
		const char *name,
		struct ausrc_prm *prm, const char *device,
		ausrc_read_h *rh, ausrc_error_h *errh, void *arg);


/*
 * Audio Player
 */

struct auplay;
struct auplay_st;

/** Audio Player parameters */
struct auplay_prm {
	uint32_t   srate;       /**< Sampling rate in [Hz] */
	uint8_t    ch;          /**< Number of channels    */
	uint32_t   ptime;       /**< Wanted packet-time in [ms] */
	int        fmt;         /**< Sample format (enum aufmt) */
};

typedef void (auplay_write_h)(void *sampv, size_t sampc, void *arg);

typedef int  (auplay_alloc_h)(struct auplay_st **stp, const struct auplay *ap,
			      struct auplay_prm *prm, const char *device,
			      auplay_write_h *wh, void *arg);

int auplay_register(struct auplay **pp, struct list *auplayl,
		    const char *name, auplay_alloc_h *alloch);
const struct auplay *auplay_find(const struct list *auplayl, const char *name);
int auplay_alloc(struct auplay_st **stp, struct list *auplayl,
		 const char *name,
		 struct auplay_prm *prm, const char *device,
		 auplay_write_h *wh, void *arg);


/*
 * Audio Filter
 */

struct aufilt;

/* Base class */
struct aufilt_enc_st {
	const struct aufilt *af;
	struct le le;
};

struct aufilt_dec_st {
	const struct aufilt *af;
	struct le le;
};

/** Audio Filter Parameters */
struct aufilt_prm {
	uint32_t srate;       /**< Sampling rate in [Hz]        */
	uint8_t  ch;          /**< Number of channels           */
	uint32_t ptime;       /**< Wanted packet-time in [ms]   */
};

typedef int (aufilt_encupd_h)(struct aufilt_enc_st **stp, void **ctx,
			      const struct aufilt *af, struct aufilt_prm *prm);
typedef int (aufilt_encode_h)(struct aufilt_enc_st *st,
			      int16_t *sampv, size_t *sampc);

typedef int (aufilt_decupd_h)(struct aufilt_dec_st **stp, void **ctx,
			      const struct aufilt *af, struct aufilt_prm *prm);
typedef int (aufilt_decode_h)(struct aufilt_dec_st *st,
			      int16_t *sampv, size_t *sampc);

struct aufilt {
	struct le le;
	const char *name;
	aufilt_encupd_h *encupdh;
	aufilt_encode_h *ench;
	aufilt_decupd_h *decupdh;
	aufilt_decode_h *dech;
};

void aufilt_register(struct list *aufiltl, struct aufilt *af);
void aufilt_unregister(struct aufilt *af);


/*
 * Log
 */

enum log_level {
	LEVEL_DEBUG = 0,
	LEVEL_INFO,
	LEVEL_WARN,
	LEVEL_ERROR,
};

typedef void (log_h)(uint32_t level, const char *msg);

struct log {
	struct le le;
	log_h *h;
};

void log_register_handler(struct log *logh);
void log_unregister_handler(struct log *logh);
void log_enable_debug(bool enable);
void log_enable_info(bool enable);
void log_enable_stdout(bool enable);
void vlog(enum log_level level, const char *fmt, va_list ap);
void loglv(enum log_level level, const char *fmt, ...);
#ifndef CONFIG_PLATFORM_8195BHP

void debug(const char *fmt, ...);
void info(const char *fmt, ...);
void warning(const char *fmt, ...);
void error_msg(const char *fmt, ...);
#endif

/*
 * Menc - Media encryption (for RTP)
 */

struct menc;
struct menc_sess;
struct menc_media;


typedef void (menc_error_h)(int err, void *arg);

typedef int  (menc_sess_h)(struct menc_sess **sessp, struct sdp_session *sdp,
			   bool offerer, menc_error_h *errorh, void *arg);

typedef int  (menc_media_h)(struct menc_media **mp, struct menc_sess *sess,
			    struct rtp_sock *rtp, int proto,
			    void *rtpsock, void *rtcpsock,
			    struct sdp_media *sdpm);

struct menc {
	struct le le;
	const char *id;
	const char *sdp_proto;
	menc_sess_h *sessh;
	menc_media_h *mediah;
};

void menc_register(struct list *mencl, struct menc *menc);
void menc_unregister(struct menc *menc);
const struct menc *menc_find(const struct list *mencl, const char *id);


/*
 * Net - Networking
 */

struct network;

typedef void (net_change_h)(void *arg);

int  net_alloc(struct network **netp, const struct config_net *cfg, int af);
int  net_use_nameserver(struct network *net, const struct sa *ns);
void net_change(struct network *net, uint32_t interval,
		net_change_h *ch, void *arg);
void net_force_change(struct network *net);
bool net_check(struct network *net);
int  net_af(const struct network *net);
int  net_debug(struct re_printf *pf, const struct network *net);
const struct sa *net_laddr_af(const struct network *net, int af);
const char      *net_domain(const struct network *net);
struct dnsc     *net_dnsc(const struct network *net);


/*
 * Play - audio file player
 */

struct play;
struct player;

int  play_file(struct play **playp, struct player *player,
	       const char *filename, int repeat);
int  play_tone(struct play **playp, struct player *player,
	       struct mbuf *tone,
	       uint32_t srate, uint8_t ch, int repeat);
int  play_init(struct player **playerp);
void play_set_path(struct player *player, const char *path);


/*
 * User Agent
 */

struct ua;

/** Events from User-Agent */
enum ua_event {
	UA_EVENT_REGISTERING = 0,
	UA_EVENT_REGISTER_OK,
	UA_EVENT_REGISTER_FAIL,
	UA_EVENT_UNREGISTERING,
	UA_EVENT_SHUTDOWN,
	UA_EVENT_EXIT,

	UA_EVENT_CALL_INCOMING,
	UA_EVENT_CALL_RINGING,
	UA_EVENT_CALL_PROGRESS,
	UA_EVENT_CALL_ESTABLISHED,
	UA_EVENT_CALL_CLOSED,
	UA_EVENT_CALL_TRANSFER_FAILED,
	UA_EVENT_CALL_DTMF_START,
	UA_EVENT_CALL_DTMF_END,
	UA_EVENT_CALL_RTCP,

	UA_EVENT_MAX,
};

/** Video mode */
enum vidmode {
	VIDMODE_OFF = 0,    /**< Video disabled                */
	VIDMODE_ON,         /**< Video enabled                 */
};

/** Defines the User-Agent event handler */
typedef void (ua_event_h)(struct ua *ua, enum ua_event ev,
			  struct call *call, const char *prm, void *arg);
typedef void (options_resp_h)(int err, const struct sip_msg *msg, void *arg);

typedef void (ua_exit_h)(void *arg);

/* Multiple instances */
//int  ua_set_server_ip(struct ua *uap, const char *aor);
int  ua_alloc(struct ua **uap, const char *aor,const char *reg_server,bool register_flag);
int ua_connect(struct ua_context *ua_ctx,struct ua *ua, struct call **callp,
	       const char *from_uri, const char *uri,
	       const char *params, enum vidmode vmode);
void ua_hangup(struct ua *ua, struct call *call,
	       uint16_t scode, const char *reason);
int  ua_answer(struct ua *ua, struct call *call);
int  ua_progress(struct ua *ua, struct call *call);
int  ua_hold_answer(struct ua *ua, struct call *call);
int  ua_options_send(struct ua *ua, const char *uri,
		     options_resp_h *resph, void *arg);
int  ua_debug(struct re_printf *pf, const struct ua *ua);
int  ua_print_calls(struct re_printf *pf, const struct ua *ua);
int  ua_print_status(struct re_printf *pf, const struct ua *ua);
int  ua_print_supported(struct re_printf *pf, const struct ua *ua);
int  ua_register(struct ua *ua,const char *reg_server);
void ua_unregister(struct ua *ua);
bool ua_isregistered(const struct ua *ua);
void ua_pub_gruu_set(struct ua *ua, const struct pl *pval);
const char     *ua_aor(const struct ua *ua);
const char     *ua_cuser(const struct ua *ua);
const char     *ua_local_cuser(const struct ua *ua);
struct account *ua_account(const struct ua *ua);
const char     *ua_outbound(const struct ua *ua);
struct call    *ua_call(const struct ua *ua);
struct call    *ua_prev_call(const struct ua *ua);
struct list    *ua_calls(const struct ua *ua);
enum presence_status ua_presence_status(const struct ua *ua);
void ua_presence_status_set(struct ua *ua, const enum presence_status status);
void ua_set_media_af(struct ua *ua, int af_media);


/* One instance */
int  ua_init(const char *software, bool udp, bool tcp, bool tls,
	     bool prefer_ipv6);
void ua_sip_close(void);
void ua_stop_all(bool forced);
void uag_set_exit_handler(ua_exit_h *exith, void *arg);
int  uag_reset_transp(bool reg, bool reinvite);
int  uag_event_register(ua_event_h *eh, void *arg);
void uag_event_unregister(ua_event_h *eh);
void uag_set_sub_handler(sip_msg_h *subh);
int  ua_print_sip_status(struct re_printf *pf, void *unused);
int  uag_set_extra_params(const char *eprm);
struct ua   *uag_find(const struct pl *cuser);
struct ua   *uag_find_aor(const char *aor);
struct ua   *uag_find_param(const char *name, const char *val);
struct sip  *uag_sip(void);
const char  *uag_event_str(enum ua_event ev);
struct list *uag_list(void);
void         uag_current_set(struct ua *ua);
struct ua   *uag_current(void);
struct sipsess_sock  *uag_sipsess_sock(void);
struct sipevent_sock *uag_sipevent_sock(void);


/*
 * User Interface
 */

struct ui_sub {
	struct list uil;  /**< List of UIs (struct ui) */
	struct cmd_ctx *uictx;
};

typedef int  (ui_output_h)(const char *str);

/** Defines a User-Interface module */
struct ui {
	struct le le;          /**< Linked-list element                   */
	const char *name;      /**< Name of the UI-module                 */
	ui_output_h *outputh;  /**< Handler for output strings (optional) */
};

#ifndef CONFIG_PLATFORM_8195BHP

void ui_register(struct ui_sub *uis, struct ui *ui);
void ui_unregister(struct ui *ui);

void ui_reset(struct ui_sub *uis);
void ui_input_key(struct ui_sub *uis, char key, struct re_printf *pf);
void ui_input_str(const char *str);
int  ui_input_pl(struct re_printf *pf, const struct pl *pl);
void ui_output(struct ui_sub *uis, const char *fmt, ...);
bool ui_isediting(const struct ui_sub *uis);
int  ui_password_prompt(char **passwordp);

#endif
/*
 * Command interface
 */

/* special keys */
#define KEYCODE_NONE   (0x00)
#define KEYCODE_REL    (0x04)    /* Key was released */
#define KEYCODE_ESC    (0x1b)


/** Command flags */
enum {
	CMD_PRM  = (1<<0),              /**< Command with parameter */
	CMD_PROG = (1<<1),              /**< Show progress          */

	CMD_IPRM = CMD_PRM | CMD_PROG,  /**< Interactive parameter  */
};

/** Command arguments */
struct cmd_arg {
	char key;         /**< Which key was pressed  */
	char *prm;        /**< Optional parameter     */
	bool complete;    /**< True if complete       */
	void *data;       /**< Application data       */
};

/** Defines a command */
struct cmd {
	const char *name; /**< Long command           */
	char key;         /**< Short command          */
	int flags;        /**< Optional command flags */
	const char *desc; /**< Description string     */
	re_printf_h *h;   /**< Command handler        */
};

struct cmd_ctx;
struct commands;

#ifndef CONFIG_PLATFORM_8195BHP

int  cmd_init(struct commands **commandsp);
int  cmd_register(struct commands *commands,
		  const struct cmd *cmdv, size_t cmdc);
void cmd_unregister(struct commands *commands, const struct cmd *cmdv);
int  cmd_process(struct commands *commands, struct cmd_ctx **ctxp, char key,
		 struct re_printf *pf, void *data);
int  cmd_process_long(struct commands *commands, const char *str, size_t len,
		      struct re_printf *pf_resp, void *data);
int cmd_print(struct re_printf *pf, const struct commands *commands);
const struct cmd *cmd_find_long(const struct commands *commands,
				const char *name);
struct cmds *cmds_find(const struct commands *commands,
		       const struct cmd *cmdv);
#endif

/*
 * Video Source
 */

struct vidsrc;
struct vidsrc_st;

/** Video Source parameters */
struct vidsrc_prm {
	int orient;       /**< Wanted picture orientation (enum vidorient) */
	int fps;          /**< Wanted framerate                            */
};

typedef void (vidsrc_frame_h)(struct vidframe *frame, void *arg);
typedef void (vidsrc_error_h)(int err, void *arg);

typedef int  (vidsrc_alloc_h)(struct vidsrc_st **vsp, const struct vidsrc *vs,
			      struct media_ctx **ctx, struct vidsrc_prm *prm,
			      const struct vidsz *size,
			      const char *fmt, const char *dev,
			      vidsrc_frame_h *frameh,
			      vidsrc_error_h *errorh, void *arg);

typedef void (vidsrc_update_h)(struct vidsrc_st *st, struct vidsrc_prm *prm,
			       const char *dev);
#ifndef CONFIG_PLATFORM_8195BHP

int vidsrc_register(struct vidsrc **vp, struct list *vidsrcl, const char *name,
		    vidsrc_alloc_h *alloch, vidsrc_update_h *updateh);
const struct vidsrc *vidsrc_find(const struct list *vidsrcl, const char *name);
int vidsrc_alloc(struct vidsrc_st **stp, struct list *vidsrcl,
		 const char *name,
		 struct media_ctx **ctx, struct vidsrc_prm *prm,
		 const struct vidsz *size, const char *fmt, const char *dev,
		 vidsrc_frame_h *frameh, vidsrc_error_h *errorh, void *arg);
#endif

/*
 * Video Display
 */

struct vidisp;
struct vidisp_st;

/** Video Display parameters */
struct vidisp_prm {
	void *view;       /**< Optional view (set by application or module) */
	bool fullscreen;  /**< Enable fullscreen display                    */
};

typedef void (vidisp_resize_h)(const struct vidsz *size, void *arg);

typedef int  (vidisp_alloc_h)(struct vidisp_st **vp,
			      const struct vidisp *vd, struct vidisp_prm *prm,
			      const char *dev,
			      vidisp_resize_h *resizeh, void *arg);
typedef int  (vidisp_update_h)(struct vidisp_st *st, bool fullscreen,
			       int orient, const struct vidrect *window);
typedef int  (vidisp_disp_h)(struct vidisp_st *st, const char *title,
			     const struct vidframe *frame);
typedef void (vidisp_hide_h)(struct vidisp_st *st);

#ifndef CONFIG_PLATFORM_8195BHP

int vidisp_register(struct vidisp **vp, struct list *vidispl, const char *name,
		    vidisp_alloc_h *alloch, vidisp_update_h *updateh,
		    vidisp_disp_h *disph, vidisp_hide_h *hideh);
int vidisp_alloc(struct vidisp_st **stp, struct list *vidispl,
		 const char *name,
		 struct vidisp_prm *prm, const char *dev,
		 vidisp_resize_h *resizeh, void *arg);
int vidisp_display(struct vidisp_st *st, const char *title,
		   const struct vidframe *frame);
const struct vidisp *vidisp_find(const struct list *vidispl, const char *name);
#endif

/*
 * Audio Codec
 */

/** Audio Codec parameters */
struct auenc_param {
	uint32_t ptime;  /**< Packet time in [ms]   */
	uint32_t bitrate;/**< Wanted bitrate in [bit/s] */
};

struct auenc_state;
struct audec_state;
struct aucodec;

typedef int (auenc_update_h)(struct auenc_state **aesp,
			     const struct aucodec *ac,
			     struct auenc_param *prm, const char *fmtp);
typedef int (auenc_encode_h)(struct auenc_state *aes,
			     uint8_t *buf, size_t *len,
			     int fmt, const void *sampv, size_t sampc);

typedef int (audec_update_h)(struct audec_state **adsp,
			     const struct aucodec *ac, const char *fmtp);
typedef int (audec_decode_h)(struct audec_state *ads,
			     int fmt, void *sampv, size_t *sampc,
			     const uint8_t *buf, size_t len);
typedef int (audec_plc_h)(struct audec_state *ads,
			  int fmt, void *sampv, size_t *sampc);

struct aucodec {
	struct le le;
	const char *pt;
	const char *name;
	uint32_t srate;             /* Audio samplerate */
	uint32_t crate;             /* RTP Clock rate   */
	uint8_t ch;
	const char *fmtp;
	auenc_update_h *encupdh;
	auenc_encode_h *ench;
	audec_update_h *decupdh;
	audec_decode_h *dech;
	audec_plc_h    *plch;
	sdp_fmtp_enc_h *fmtp_ench;
	sdp_fmtp_cmp_h *fmtp_cmph;
};

void aucodec_register(struct list *aucodecl, struct aucodec *ac);
void aucodec_unregister(struct aucodec *ac);
const struct aucodec *aucodec_find(const struct list *aucodecl,
				   const char *name, uint32_t srate,
				   uint8_t ch);


/*
 * Video Codec
 */

/** Video Codec parameters */
struct videnc_param {
	unsigned bitrate;  /**< Encoder bitrate in [bit/s] */
	unsigned pktsize;  /**< RTP packetsize in [bytes]  */
	unsigned fps;      /**< Video framerate            */
	uint32_t max_fs;
};

struct videnc_state;
struct viddec_state;
struct vidcodec;

typedef int (videnc_packet_h)(bool marker, uint32_t rtp_ts,
			      const uint8_t *hdr, size_t hdr_len,
			      const uint8_t *pld, size_t pld_len,
			      void *arg);

typedef int (videnc_update_h)(struct videnc_state **vesp,
			      const struct vidcodec *vc,
			      struct videnc_param *prm, const char *fmtp,
			      videnc_packet_h *pkth, void *arg);
typedef int (videnc_encode_h)(struct videnc_state *ves, bool update,
			      const struct vidframe *frame);

typedef int (viddec_update_h)(struct viddec_state **vdsp,
			      const struct vidcodec *vc, const char *fmtp);
typedef int (viddec_decode_h)(struct viddec_state *vds, struct vidframe *frame,
                              bool *intra, bool marker, uint16_t seq,
                              struct mbuf *mb);

struct vidcodec {
	struct le le;
	const char *pt;
	const char *name;
	const char *variant;
	const char *fmtp;
	videnc_update_h *encupdh;
	videnc_encode_h *ench;
	viddec_update_h *decupdh;
	viddec_decode_h *dech;
	sdp_fmtp_enc_h *fmtp_ench;
	sdp_fmtp_cmp_h *fmtp_cmph;
};

#ifndef CONFIG_PLATFORM_8195BHP

void vidcodec_register(struct list *vidcodecl, struct vidcodec *vc);
void vidcodec_unregister(struct vidcodec *vc);
const struct vidcodec *vidcodec_find(const struct list *vidcodecl,
				     const char *name, const char *variant);
const struct vidcodec *vidcodec_find_encoder(const struct list *vidcodecl,
					     const char *name);
const struct vidcodec *vidcodec_find_decoder(const struct list *vidcodecl,
					     const char *name);
#endif

/*
 * Video Filter
 */

struct vidfilt;

/* Base class */
struct vidfilt_enc_st {
	const struct vidfilt *vf;
	struct le le;
};

struct vidfilt_dec_st {
	const struct vidfilt *vf;
	struct le le;
};

typedef int (vidfilt_encupd_h)(struct vidfilt_enc_st **stp, void **ctx,
			       const struct vidfilt *vf);
typedef int (vidfilt_encode_h)(struct vidfilt_enc_st *st,
			       struct vidframe *frame);

typedef int (vidfilt_decupd_h)(struct vidfilt_dec_st **stp, void **ctx,
			       const struct vidfilt *vf);
typedef int (vidfilt_decode_h)(struct vidfilt_dec_st *st,
			       struct vidframe *frame);

struct vidfilt {
	struct le le;
	const char *name;
	vidfilt_encupd_h *encupdh;
	vidfilt_encode_h *ench;
	vidfilt_decupd_h *decupdh;
	vidfilt_decode_h *dech;
};
#ifndef CONFIG_PLATFORM_8195BHP

void vidfilt_register(struct list *vidfiltl, struct vidfilt *vf);
void vidfilt_unregister(struct vidfilt *vf);
int vidfilt_enc_append(struct list *filtl, void **ctx,
		       const struct vidfilt *vf);
int vidfilt_dec_append(struct list *filtl, void **ctx,
		       const struct vidfilt *vf);
#endif

/*
 * Audio stream
 */

struct audio;


#ifdef CONFIG_PLATFORM_8195BHP

void audio_mute(struct audio *a, bool muted);
bool audio_ismuted(const struct audio *a);
void audio_set_devicename(struct audio *a, const char *src, const char *play);
int  audio_set_source(struct audio *au, const char *mod, const char *device);
int  audio_set_player(struct audio *au, const char *mod, const char *device);
void audio_encoder_cycle(struct audio *audio);
int  audio_level_get(const struct audio *au, double *level);
int  audio_debug(struct re_printf *pf, const struct audio *a);
struct stream *audio_strm(const struct audio *a);
int audio_set_bitrate(struct audio *au, uint32_t bitrate);
#endif

/*
 * Video stream
 */

struct video;

#if 1 
struct stream *video_strm(const struct video *v);
#endif

#ifndef CONFIG_PLATFORM_8195BHP

void  video_mute(struct video *v, bool muted);
void *video_view(const struct video *v);
int   video_set_fullscreen(struct video *v, bool fs);
int   video_set_orient(struct video *v, int orient);
void  video_vidsrc_set_device(struct video *v, const char *dev);
int   video_set_source(struct video *v, const char *name, const char *dev);
void  video_set_devicename(struct video *v, const char *src, const char *disp);
void  video_encoder_cycle(struct video *video);
int   video_debug(struct re_printf *pf, const struct video *v);
uint32_t video_calc_rtp_timestamp(int64_t pts, unsigned fps);
double video_calc_seconds(uint32_t rtp_ts);


#endif
/*
 * Generic stream
 */

const struct rtcp_stats *stream_rtcp_stats(const struct stream *strm);


/*
 * Media NAT
 */

struct mnat;
struct mnat_sess;
struct mnat_media;

typedef void (mnat_estab_h)(int err, uint16_t scode, const char *reason,
			    void *arg);

typedef int (mnat_sess_h)(struct mnat_sess **sessp, struct dnsc *dnsc,
			  int af, const char *srv, uint16_t port,
			  const char *user, const char *pass,
			  struct sdp_session *sdp, bool offerer,
			  mnat_estab_h *estabh, void *arg);

typedef int (mnat_media_h)(struct mnat_media **mp, struct mnat_sess *sess,
			   int proto, void *sock1, void *sock2,
			   struct sdp_media *sdpm);

typedef int (mnat_update_h)(struct mnat_sess *sess);

int mnat_register(struct mnat **mnatp, struct list *mnatl,
		  const char *id, const char *ftag,
		  mnat_sess_h *sessh, mnat_media_h *mediah,
		  mnat_update_h *updateh);


/*
 * Real-time
 */
int realtime_enable(bool enable, int fps);


/*
 * SDP
 */

bool sdp_media_has_media(const struct sdp_media *m);
int  sdp_media_find_unused_pt(const struct sdp_media *m);
int  sdp_fingerprint_decode(const char *attr, struct pl *hash,
			    uint8_t *md, size_t *sz);
uint32_t sdp_media_rattr_u32(const struct sdp_media *sdpm, const char *name);
const char *sdp_rattr(const struct sdp_session *s, const struct sdp_media *m,
		      const char *name);


/*
 * SIP Request
 */

int sip_req_send(struct ua *ua, const char *method, const char *uri,
		 sip_resp_h *resph, void *arg, const char *fmt, ...);

#ifndef CONFIG_PLATFORM_8195BHP
/*
 * H.264
 */

/** NAL unit types (RFC 3984, Table 1) */
enum {
	H264_NAL_UNKNOWN      = 0,
	/* 1-23   NAL unit  Single NAL unit packet per H.264 */
	H264_NAL_SLICE        = 1,
	H264_NAL_DPA          = 2,
	H264_NAL_DPB          = 3,
	H264_NAL_DPC          = 4,
	H264_NAL_IDR_SLICE    = 5,
	H264_NAL_SEI          = 6,
	H264_NAL_SPS          = 7,
	H264_NAL_PPS          = 8,
	H264_NAL_AUD          = 9,
	H264_NAL_END_SEQUENCE = 10,
	H264_NAL_END_STREAM   = 11,
	H264_NAL_FILLER_DATA  = 12,
	H264_NAL_SPS_EXT      = 13,
	H264_NAL_AUX_SLICE    = 19,

	H264_NAL_STAP_A       = 24,  /**< Single-time aggregation packet */
	H264_NAL_STAP_B       = 25,  /**< Single-time aggregation packet */
	H264_NAL_MTAP16       = 26,  /**< Multi-time aggregation packet  */
	H264_NAL_MTAP24       = 27,  /**< Multi-time aggregation packet  */
	H264_NAL_FU_A         = 28,  /**< Fragmentation unit             */
	H264_NAL_FU_B         = 29,  /**< Fragmentation unit             */
};

/**
 * H.264 Header defined in RFC 3984
 *
 * <pre>
      +---------------+
      |0|1|2|3|4|5|6|7|
      +-+-+-+-+-+-+-+-+
      |F|NRI|  Type   |
      +---------------+
 * </pre>
 */
struct h264_hdr {
	unsigned f:1;      /**< 1 bit  - Forbidden zero bit (must be 0) */
	unsigned nri:2;    /**< 2 bits - nal_ref_idc                    */
	unsigned type:5;   /**< 5 bits - nal_unit_type                  */
};

int h264_hdr_encode(const struct h264_hdr *hdr, struct mbuf *mb);
int h264_hdr_decode(struct h264_hdr *hdr, struct mbuf *mb);

/** Fragmentation Unit header */
struct h264_fu {
	unsigned s:1;      /**< Start bit                               */
	unsigned e:1;      /**< End bit                                 */
	unsigned r:1;      /**< The Reserved bit MUST be equal to 0     */
	unsigned type:5;   /**< The NAL unit payload type               */
};

int h264_fu_hdr_encode(const struct h264_fu *fu, struct mbuf *mb);
int h264_fu_hdr_decode(struct h264_fu *fu, struct mbuf *mb);

const uint8_t *h264_find_startcode(const uint8_t *p, const uint8_t *end);

int h264_packetize(uint32_t rtp_ts, const uint8_t *buf, size_t len,
		   size_t pktsize, videnc_packet_h *pkth, void *arg);
int h264_nal_send(bool first, bool last,
		  bool marker, uint32_t ihdr, uint32_t rtp_ts,
		  const uint8_t *buf, size_t size, size_t maxsz,
		  videnc_packet_h *pkth, void *arg);
static inline bool h264_is_keyframe(int type)
{
	return type == H264_NAL_SPS;
}


/*
 * Modules
 */

#ifdef STATIC
#define DECL_EXPORTS(name) exports_ ##name
#else
#define DECL_EXPORTS(name) exports
#endif


int module_preload(const char *module);
int  module_load(const char *name);
void module_unload(const char *name);


/*
 * MOS (Mean Opinion Score)
 */

double mos_calculate(double *r_factor, double rtt,
		     double jitter, uint32_t num_packets_lost);

#endif
/*
 * Generic event
 */

int event_encode_dict(struct odict *od, struct ua *ua, enum ua_event ev,
		      struct call *call, const char *prm);


/*
 * Baresip instance
 */

int  baresip_init(struct config *cfg, bool prefer_ipv6);
void baresip_close(void);
struct network *baresip_network(void);
struct contacts *baresip_contacts(void);
struct commands *baresip_commands(void);
struct player *baresip_player(void);
struct message *baresip_message(void);
struct list   *baresip_mnatl(void);
struct list   *baresip_mencl(void);
struct list   *baresip_aucodecl(void);
struct list   *baresip_ausrcl(void);
struct list   *baresip_auplayl(void);
struct list   *baresip_aufiltl(void);
struct list   *baresip_vidcodecl(void);
struct list   *baresip_vidsrcl(void);
struct list   *baresip_vidispl(void);
struct list   *baresip_vidfiltl(void);
struct ui_sub *baresip_uis(void);





#ifdef __cplusplus
}
#endif


#endif /* BARESIP_H__ */
