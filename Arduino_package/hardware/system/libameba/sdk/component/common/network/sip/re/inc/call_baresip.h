#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>
#include "re.h"
#include "baresip.h"
#include "lwip/sockets.h"
#include "osdep_service.h"
#include "FreeRTOS.h"
#include "task.h"
#include "platform/platform_stdlib.h"
#include "avcodec_sip.h"



#if 1
/** Call constants */
enum {
	PTIME           = 20,    /**< Packet time for audio               */
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


/** SIP Call Control object */
struct call {
	MAGIC_DECL                /**< Magic number for debugging           */
	struct le le;             /**< Linked list element                  */
	struct ua *ua;            /**< SIP User-agent                       */
	struct account *acc;      /**< Account (ref.)                       */
	struct sipsess *sess;     /**< SIP Session                          */
	struct sdp_session *sdp;  /**< SDP Session                          */
	struct sipsub *sub;       /**< Call transfer REFER subscription     */
	struct sipnot *not;       /**< REFER/NOTIFY client                  */
	struct list streaml;      /**< List of mediastreams (struct stream) */
#ifdef ENABLE_SIP_MMFV2
	struct stream *strm;          /**< Generic media stream            */
	u8 allow_stream;			/*mmfv2 flag to indicate start or stop streaming*/
#else
	struct audio *audio;      /**< Audio stream                         */
#endif
#ifdef USE_VIDEO
	struct video *video;      /**< Video stream                         */
	struct bfcp *bfcp;        /**< BFCP Client                          */
#endif
	enum state state;         /**< Call state                           */
	char *local_uri;          /**< Local SIP uri                        */
	char *local_name;         /**< Local display name                   */
	char *peer_uri;           /**< Peer SIP Address                     */
	char *peer_name;          /**< Peer display name                    */
	char *id;                 /**< Cached session call-id               */
	struct tmr tmr_inv;       /**< Timer for incoming calls             */
	struct tmr tmr_dtmf;      /**< Timer for incoming DTMF events       */
	time_t time_start;        /**< Time when call started               */
	time_t time_conn;         /**< Time when call initiated             */
	time_t time_stop;         /**< Time when call stopped               */
	bool outgoing;            /**< True if outgoing, false if incoming  */
	bool got_offer;           /**< Got SDP Offer from Peer              */
	bool on_hold;             /**< True if call is on hold              */
	struct mnat_sess *mnats;  /**< Media NAT session                    */
	bool mnat_wait;           /**< Waiting for MNAT to establish        */
	struct menc_sess *mencs;  /**< Media encryption session state       */
	int af;                   /**< Preferred Address Family             */
	uint16_t scode;           /**< Termination status code              */
	call_event_h *eh;         /**< Event handler                        */
#ifdef ENANBLE_DTMF_8195A
	call_dtmf_h *dtmfh;       /**< DTMF handler                         */
#endif
	void *arg;                /**< Handler argument                     */

	struct config_avt config_avt;    /**< AVT config                    */
	struct config_call config_call;  /**< Call config                   */

	uint32_t rtp_timeout_ms;  /**< RTP Timeout in [ms]                  */
	uint32_t linenum;         /**< Line number from 1 to N              */
};


#endif

