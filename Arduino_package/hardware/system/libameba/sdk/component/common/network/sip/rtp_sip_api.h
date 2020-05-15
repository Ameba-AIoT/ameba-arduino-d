#ifndef _RTP_SIP_API_H_
#define _RTP_SIP_API_H_

#include "rtsp/rtp_api.h"
#include <stdint.h>
#include "dlist.h"
#include "osdep_service.h"
#include "avcodec.h"
#include <lwip/def.h> //for host network byte order convertion

/* from error_base.h */
#define	EIO			5	/* I/O error */
#define	EAGAIN		11	/* Try again */
#define	ENOMEM		12	/* Out of memory */
#define	EINVAL		22	/* Invalid argument */

#define RTP_BIG_ENDIAN 0
#define RTP_HDR_SZ 12

#define RTP_SERVER_PORT_BASE 55608
#define RTP_PORT_BASE 50020
#define RTP_CLIENT_PORT_BASE 51020
/*
 * RTP data header from RFC1889
 */
/*
 *
 *
 *    The RTP header has the following format:
 *
 *  0                   1                   2                   3
 *  0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1 2 3 4 5 6 7 8 9 0 1
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |V=2|P|X|  CC   |M|     PT      |       sequence number         |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |                           timestamp                           |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 * |           synchronization source (SSRC) identifier            |
 * +=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+=+
 * |            contributing source (CSRC) identifiers             |
 * |                             ....                              |
 * +-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+-+
 *
 * RTP data header
 */
struct rtp_hdr_t;
struct connect_context;
struct rtp_statistics;
struct rtp_periodic_report_s;
enum rtp_object_state;
struct ua_stream_context;
struct rtp_object;

void sip_rtp_object_init(struct rtp_object *payload);
void sip_rtp_object_deinit(struct rtp_object *payload);
void sip_rtp_object_set_fs(struct rtp_object *payload, int flag);
void sip_rtp_object_set_fe(struct rtp_object *payload, int flag);
void sip_rtp_object_set_fk(struct rtp_object *payload, int flag);
void sip_rtp_object_set_fd(struct rtp_object *payload, int size);

void sip_rtp_fill_header(rtp_hdr_t *rtphdr, int version, int padding, int extension, int cc, int marker, int pt, uint16_t seq, uint32_t ts, uint32_t ssrc);
int sip_rtp_parse_header(uint8_t *src, rtp_hdr_t *rtphdr, int is_nbo);
void sip_rtp_dump_header(rtp_hdr_t *rtphdr, int is_nbo);


void sip_rtp_report(struct ua_stream_context *stream_ctx);
void sip_rtp_load_o_handler_by_codec_id(struct rtp_object *payload, int id);

#endif