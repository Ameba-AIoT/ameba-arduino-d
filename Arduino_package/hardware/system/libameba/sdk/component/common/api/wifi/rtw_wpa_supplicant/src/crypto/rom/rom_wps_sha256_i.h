/*
 * SHA-256 internal definitions
 * Copyright (c) 2003-2011, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef ROM_WPS_SHA256_I_H
#define ROM_WPS_SHA256_I_H

#define SHA256_BLOCK_SIZE 64

//extern struct sha256_state;

struct wps_sha256_state {
	u64 length;
	u32 state[8], curlen;
	u8 buf[64];
};


static void sha256_init(struct wps_sha256_state *md);
static int wps_sha256_process(struct wps_sha256_state *md, const unsigned char *in,
		   unsigned long inlen);
static int sha256_done(struct wps_sha256_state *md, unsigned char *out);

#endif /* ROM_WPS_SHA256_I_H */
