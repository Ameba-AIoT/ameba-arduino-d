/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _CRYPTOSIM_H_
#define _CRYPTOSIM_H_


#include <stddef.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct ipsec_function{
	void ( *crypto_sim_set_len)(int klen, int mlen, int ilen, int alen);
	void ( *esp_sim_set)(const uint32_t mode, const unsigned char *Iv, const int initcount, const unsigned char* AAD);
	void ( *rtl_crypto_auth_sim)(const uint32_t authtype, const unsigned char *Key, const unsigned char* Text, unsigned char *pDigest);
	int (*rtl_crypto_esp_sim)(const unsigned char *Key, const unsigned char *Text, unsigned char *pResult, unsigned char *pTag);
	int (*aead_chacha20_poly1305_enc)(unsigned char *out, const unsigned char *Text,  const unsigned char *non, const unsigned char key[32]);
	int (*aead_chacha20_poly1305_dec)(unsigned char *out, const unsigned char *Text,  const unsigned char *non, const unsigned char key[32]);	
}ipsec_func_ns;

// AES

#define SWDECRYPT_ECB_AES 0x20
#define SWDECRYPT_CBC_AES 0x21
#define SWDECRYPT_CFB_AES 0x22
#define SWDECRYPT_OFB_AES 0x23
#define SWDECRYPT_CTR_AES 0x24
#define SWDECRYPT_GCM_AES 0x28

#define SWENCRYPT_ECB_AES 0xa0
#define SWENCRYPT_CBC_AES 0xa1
#define SWENCRYPT_CFB_AES 0xa2
#define SWENCRYPT_OFB_AES 0xa3
#define SWENCRYPT_CTR_AES 0xa4
#define SWENCRYPT_GCM_AES 0xa8

// DES

#define SWDECRYPT_ECB_DES		0x00
#define SWDECRYPT_CBC_DES		0x01
#define SWDECRYPT_CFB_DES		0x02
#define SWDECRYPT_OFB_DES		0x03
#define SWDECRYPT_CTR_DES		0x04
#define SWDECRYPT_ECB_3DES		0x10
#define SWDECRYPT_CBC_3DES		0x11
#define SWDECRYPT_CFB_3DES		0x12
#define SWDECRYPT_OFB_3DES		0x13
#define SWDECRYPT_CTR_3DES		0x14

#define SWENCRYPT_ECB_DES		0x80
#define SWENCRYPT_CBC_DES		0x81
#define SWENCRYPT_CFB_DES		0x82
#define SWENCRYPT_OFB_DES		0x83
#define SWENCRYPT_CTR_DES		0x84
#define SWENCRYPT_ECB_3DES		0x90
#define SWENCRYPT_CBC_3DES		0x91
#define SWENCRYPT_CFB_3DES		0x92
#define SWENCRYPT_OFB_3DES		0x93
#define SWENCRYPT_CTR_3DES		0x94

// chacha
void CRYPTO_sim_chacha_20(unsigned char *out,
                          const unsigned char *in, size_t in_len,
                          const unsigned char key[32],
                          const unsigned char nonce[8],
                          size_t counter);
// poly1305
//
struct poly1305_state_st {
    uint32_t r0,r1,r2,r3,r4;
    uint32_t s1,s2,s3,s4;
    uint32_t h0,h1,h2,h3,h4;
    unsigned char buf[16];
    unsigned int buf_used;
    unsigned char key[16];
};

typedef struct poly1305_state_st poly1305_state;
void CRYPTO_sim_poly1305_init(poly1305_state *state, const unsigned char key[32]);
void CRYPTO_sim_poly1305_update(poly1305_state *state, const unsigned char *in, size_t in_len);
void CRYPTO_sim_poly1305_finish(poly1305_state *state, unsigned char mac[16]);

#define CHACHA20_NONCE_LEN         8
#define POLY1305_TAG_LEN           16
#define CHACHA20POLY1305_KEY_LEN   32

int aead_chacha20_poly1305_enc(unsigned char *out, 
                               const unsigned char *Text,  const unsigned char *non, const unsigned char key[CHACHA20POLY1305_KEY_LEN]);
int aead_chacha20_poly1305_dec(unsigned char *out, 
                               const unsigned char *Text,  const unsigned char *non, const unsigned char key[CHACHA20POLY1305_KEY_LEN]);
void rtl_crypto_auth_sim(const uint32_t authtype,
                         const unsigned char *Key, const unsigned char* Text,
                         unsigned char *pDigest);


int rtl_crypto_esp_sim(const unsigned char *Key, const unsigned char *Text, unsigned char *pResult, unsigned char *pTag);
void crypto_sim_set_len(int klen, int mlen, int ilen, int alen);
void esp_sim_set(const uint32_t mode, const unsigned char *Iv, const int initcount, const unsigned char* AAD);
#ifdef __cplusplus
}
#endif


#endif


