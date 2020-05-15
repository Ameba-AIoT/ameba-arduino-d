/*
 * Copyright (C) 1995, 1996, 1997, and 1998 WIDE Project.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. Neither the name of the project nor the names of its contributors
 *    may be used to endorse or promote products derived from this software
 *    without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE PROJECT AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE PROJECT OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */
/*
 * FIPS pub 180-1: Secure Hash Algorithm (SHA-1)
 * based on: http://csrc.nist.gov/fips/fip180-1.txt
 * implemented by Jun-ichiro itojun Itoh <itojun@itojun.org>
 */

#ifndef _CRYPTOSIM_SHA_H_
#define _CRYPTOSIM_SHA_H_


#include "rtl8711b_crypto.h"




#define UL64(x) x##ULL


/**
 * \brief          SHA-1 context structure
 */
typedef struct
{
    uint32_t total[2] 	__attribute__ ((aligned(4)));           /*!< number of bytes processed  */
    uint32_t state[5] 	__attribute__ ((aligned(4)));          /*!< intermediate digest state  */
    unsigned char buffer[64] __attribute__ ((aligned(4)));   /*!< data block being processed */

    unsigned char ipad[64] __attribute__ ((aligned(4)));     /*!< HMAC: inner padding        */
    unsigned char opad[64] __attribute__ ((aligned(4)));     /*!< HMAC: outer padding        */
}
sha1_context;

#define SHA1_SIZE 20

typedef struct 
{
    uint32_t Intermediate_Hash[SHA1_SIZE/4]; /* Message Digest */
    uint32_t Length_Low;            /* Message length in bits */
    uint32_t Length_High;           /* Message length in bits */
    uint16_t Message_Block_Index;   /* Index into message block array   */
    uint8_t Message_Block[64] __attribute__ ((aligned(4)));;      /* 512-bit message blocks */
} SHA1_CTX;


typedef struct
{
    uint32_t total[2];          /*!< number of bytes processed  */
    uint32_t state[8];          /*!< intermediate digest state  */
    unsigned char buffer[64];   /*!< data block being processed */

    unsigned char ipad[64];     /*!< HMAC: inner padding        */
    unsigned char opad[64];     /*!< HMAC: outer padding        */
    int is224;                  /*!< 0 => SHA-256, else SHA-224 */
}
sha256_context;

typedef struct
{
    uint64_t total[2];          /*!< number of bytes processed  */
    uint64_t state[8];          /*!< intermediate digest state  */
    unsigned char buffer[128];  /*!< data block being processed */

    unsigned char ipad[128];    /*!< HMAC: inner padding        */
    unsigned char opad[128];    /*!< HMAC: outer padding        */
    int is384;                  /*!< 0 => SHA-512, else SHA-384 */
}
sha512_context;



/**
 * \brief          SHA-1 context setup
 *
 * \param ctx      context to be initialized
 */
extern void sha1_starts( sha1_context *ctx );

/**
 * \brief          SHA-1 process buffer
 *
 * \param ctx      SHA-1 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
extern void sha1_update( sha1_context *ctx, const unsigned char *input, size_t ilen );

/**
 * \brief          SHA-1 final digest
 *
 * \param ctx      SHA-1 context
 * \param output   SHA-1 checksum result
 */
extern void sha1_finish( sha1_context *ctx, unsigned char output[20] );

/* Internal use */
extern void sha1_process( sha1_context *ctx, const unsigned char data[64] );


/**
 * \brief          Output = SHA-1( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   SHA-1 checksum result
 */
 
extern void sha1( const unsigned char *input, size_t ilen, unsigned char output[20] );
extern void sha2( const unsigned char *input, size_t ilen, unsigned char output[32], uint32 digestlen);


/**
 * \brief          SHA-1 HMAC context setup
 *
 * \param ctx      HMAC context to be initialized
 * \param key      HMAC secret key
 * \param keylen   length of the HMAC key
 */
extern void sha1_hmac_starts( sha1_context *ctx, const unsigned char *key, size_t keylen );

/**
 * \brief          SHA-1 HMAC process buffer
 *
 * \param ctx      HMAC context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
extern void sha1_hmac_update( sha1_context *ctx, const unsigned char *input, size_t ilen );

/**
 * \brief          SHA-1 HMAC final digest
 *
 * \param ctx      HMAC context
 * \param output   SHA-1 HMAC checksum result
 */
extern void sha1_hmac_finish( sha1_context *ctx, unsigned char output[20] );

/**
 * \brief          SHA-1 HMAC context reset
 *
 * \param ctx      HMAC context to be reset
 */
extern void sha1_hmac_reset( sha1_context *ctx );

/**
 * \brief          Output = HMAC-SHA-1( hmac key, input buffer )
 *
 * \param key      HMAC secret key
 * \param keylen   length of the HMAC key
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   HMAC-SHA-1 result
 */
extern void sha1_hmac( const unsigned char *key, size_t keylen,
                const unsigned char *input, size_t ilen,
                unsigned char output[20] );

/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
extern int sha1_self_test( int verbose );


//typedef sha1_context SHA1_CTX;
//#define SHA1Init(x)		sha1_starts((x))
//#define SHA1Update(x, y, z)	sha1_update((x), (y), (z))
//#define SHA1Final(x, y)		sha1_finish((y), (x))

extern void sha256_hmac( const unsigned char *key, size_t keylen,
                const unsigned char *input, size_t ilen,
                unsigned char output[32], int is224);


#endif /*_CRYPTOSIM_SHA_H_*/
