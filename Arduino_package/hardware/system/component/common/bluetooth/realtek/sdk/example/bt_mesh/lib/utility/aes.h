#ifndef _AES_H_
#define _AES_H_

/* aes 128 implementation*/
#include <stdint.h>

/**
 * @addtogroup AES
 * @{
 */

/**
 * @defgroup Aes_Exported_Macros AES Exported Macros
 * @brief
 * @{
 */
// #define the macros below to 1/0 to enable/disable the mode of operation.
//
// CBC enables AES128 encryption in CBC-mode of operation and handles 0-padding.
// ECB enables the basic ECB 16-byte block algorithm. Both can be enabled simultaneously.
// The #ifndef-guard allows it to be configured before #include'ing or at compile time.
#define CBC 0
#define ECB 1
/** @} */

/**
 * @defgroup Aes_Exported_Functions AES Exported Functions
 * @brief
 * @{
 */
#if ECB
void AES128_ECB_encrypt(const uint8_t input[16], const uint8_t key[16], uint8_t *output);
void AES128_ECB_decrypt(uint8_t *input, const uint8_t *key, uint8_t *output);
#endif

#if CBC
void AES128_CBC_encrypt_buffer(uint8_t *output, uint8_t *input, uint32_t length, const uint8_t *key,
                               const uint8_t *iv);
void AES128_CBC_decrypt_buffer(uint8_t *output, uint8_t *input, uint32_t length, const uint8_t *key,
                               const uint8_t *iv);
#endif
/** @} */
/** @} */

#endif //_AES_H_

