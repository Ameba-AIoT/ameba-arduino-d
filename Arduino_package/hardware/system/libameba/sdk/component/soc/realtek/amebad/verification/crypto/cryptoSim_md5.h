/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */


#ifndef _CRYPTOSIM_MD5_H_
#define _CRYPTOSIM_MD5_H_


#include "rtl8711b_crypto.h"



/**
 * \brief          MD5 context structure
 */
typedef struct
{
    uint32_t total[2]  __attribute__ ((aligned(4)));          /*!< number of bytes processed  */
    uint32_t state[4]  __attribute__ ((aligned(4)));          /*!< intermediate digest state  */
    unsigned char buffer[64]  __attribute__ ((aligned(4)));   /*!< data block being processed */

    unsigned char ipad[64] __attribute__ ((aligned(4)));     /*!< HMAC: inner padding        */
    unsigned char opad[64] __attribute__ ((aligned(4)));     /*!< HMAC: outer padding        */
}
md5_context;


/**
 * \brief          MD5 context setup
 *
 * \param ctx      context to be initialized
 */
void md5_starts( md5_context *ctx );

/**
 * \brief          MD5 process buffer
 *
 * \param ctx      MD5 context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
void md5_update( md5_context *ctx, const unsigned char *input, size_t ilen );

/**
 * \brief          MD5 final digest
 *
 * \param ctx      MD5 context
 * \param output   MD5 checksum result
 */
void md5_finish( md5_context *ctx, unsigned char output[16] );

/* Internal use */
void md5_process( md5_context *ctx, const unsigned char data[64] );


/**
 * \brief          Output = MD5( input buffer )
 *
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   MD5 checksum result
 */
void md5( const unsigned char *input, size_t ilen, unsigned char output[16] );


/**
 * \brief          MD5 HMAC context setup
 *
 * \param ctx      HMAC context to be initialized
 * \param key      HMAC secret key
 * \param keylen   length of the HMAC key
 */
void md5_hmac_starts( md5_context *ctx,
                      const unsigned char *key, size_t keylen );

/**
 * \brief          MD5 HMAC process buffer
 *
 * \param ctx      HMAC context
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 */
void md5_hmac_update( md5_context *ctx,
                      const unsigned char *input, size_t ilen );

/**
 * \brief          MD5 HMAC final digest
 *
 * \param ctx      HMAC context
 * \param output   MD5 HMAC checksum result
 */
void md5_hmac_finish( md5_context *ctx, unsigned char output[16] );

/**
 * \brief          MD5 HMAC context reset
 *
 * \param ctx      HMAC context to be reset
 */
void md5_hmac_reset( md5_context *ctx );

/**
 * \brief          Output = HMAC-MD5( hmac key, input buffer )
 *
 * \param key      HMAC secret key
 * \param keylen   length of the HMAC key
 * \param input    buffer holding the  data
 * \param ilen     length of the input data
 * \param output   HMAC-MD5 result
 */
void md5_hmac( const unsigned char *key, size_t keylen,
               const unsigned char *input, size_t ilen,
               unsigned char output[16] );

/**
 * \brief          Checkup routine
 *
 * \return         0 if successful, or 1 if the test failed
 */
int md5_self_test( int verbose );


// TODO:
// merge with upper software in the future. 
//
/* compatibility */
#define MD5_CTX				md5_context
#define MD5Init(x)			md5_starts((x))
#define MD5Update(x, y, z)	md5_update((x), (y), (z))
#define MD5Final(x, y)  	md5_finish((x), (y))

#endif /* ! _CRYPTOSIM_MD5_H_*/

