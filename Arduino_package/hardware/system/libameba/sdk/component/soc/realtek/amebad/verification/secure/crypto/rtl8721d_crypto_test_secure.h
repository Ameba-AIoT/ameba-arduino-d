#ifndef __RTL8721D_CRYPTO_TEST_H__
#define __RTL8721D_CRYPTO_TEST_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdint.h>

#ifndef CRYPTO_MAX_MSG_LENGTH
#define CRYPTO_MAX_MSG_LENGTH		8192 //8192 // 18432  // 4096 //32768 // 64 // 16383
#endif

#define AUTH_NOCRYPTO	((u32)-1)
#define CRYPTO_NOAUTH	((u32)-1)
/** @defgroup CRYPTO_MODE_definitions 
  * @{
  */
#define CRYPTO_MS_TYPE_CIPHER	(0)
#define CRYPTO_MS_TYPE_AUTH		(1)
#define CRYPTO_MS_TYPE_MIX		(2)
/**
  * @}
  */

#define ERRNO_CRYPTO_TEST_ARG	(-100)
#define ERRNO_CRYPTO_TEST_INIT	(-200)

int throughput_test(u32 modetype, u32 cipher_type, u32 authtype, u32 loops);
int rtl_crypto_test(u16 argc, u8 *argv[]);
int vector_test_auth(uint32_t authtype);
int vector_test_cipher(uint32_t cipher_type);

#ifdef __cplusplus
}
#endif


#endif // __RTL8721D_CRYPTO_TEST_H__
