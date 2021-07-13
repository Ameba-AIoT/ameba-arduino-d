/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     platform_misc.h
  * @brief    Head file for platform miscellaneous.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2017-3-3
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PLATFORM_MISC_H
#define _PLATFORM_MISC_H

/* Add Includes here */
#include <string.h>
#include "platform_types.h"
//#include "rtl876x_wdg.h"

BEGIN_DECLS

/** @addtogroup Platform_Misc
  * @{
  */

/** @defgroup Platform_Misc_Exported_Functions Platform Misc Exported Functions
  * @brief
  * @{
  */

/**
  * @brief Swap the endianess of the data
  * @param[in] pdata: data pointer
  * @param[out] pdata: data pointer
  * @param[in] len: data length
  * @return none
  */
void plt_swap1(uint8_t *pdata, uint16_t len);

/**
  * @brief Swap the endianess from the src addr to the dst addr who
  *     have different address range
  * @param[out] dst: destination data pointer
  * @param[in] src: source data pointer
  * @param[in] len: data length
  * @return none
  */
void plt_swap2(uint8_t *dst, uint8_t *src, uint16_t len);

/**
  * @brief Swap the endianess from the src addr to the dst addr who
  *     have any address range
  * @param[out] dst: destination data pointer
  * @param[in] src: source data pointer
  * @param[in] len: data length
  * @return none
  */
void plt_swap(uint8_t *dst, uint8_t *src, uint16_t len);

/** @brief calculate the bytes needed by the bit pool */
#define plt_bit_pool_size(bit_num)              (((bit_num) + 7) / 8)

/**
  * @brief get the bit field value from the bit pool
  * @param[in] pool: pool addr
  * @param[in] bit: bit num
  * @return bit field value
  */
bool plt_bit_pool_get(uint8_t *pool, uint32_t bit);

/**
  * @brief set the bit field of the bit pool
  * @param[in] pool: pool addr
  * @param[in] bit: bit num
  * @param[in] value: bit value
  * @return none
  */
void plt_bit_pool_set(uint8_t *pool, uint32_t bit, bool value);

/**
  * @brief count the bit 1 of the bit pool
  * @param[in] pool: pool addr
  * @param[in] pool_len: pool length
  * @return bit 1 number
  */
uint32_t plt_bit_pool_count(uint8_t *pool, uint32_t pool_len);

/**
  * @brief set the seed
  * @param[in] seed
  * @return none
  */
void plt_srand(uint32_t seed);

/**
  * @brief generate the random number
  * @param[out] prand: the addr to output
  * @param[in] len: the length of the rand
  * @return none
  */
void plt_rand(uint8_t *prand, uint16_t len);

/**
  * @brief logarithm
  * @param[in] value: exp value
  * @return log value
  */
uint8_t plt_log2(uint32_t value);

/**
  * @brief exponent
  * @param[in] log: log value
  * @return exp value
  */
uint32_t plt_exp2(uint8_t log);

/**
  * @brief trasnform the hex string to a binary array (big endian)
  *
  * The binary array will be filled with 0 if the string length is not enough.
  * @param[out] pbin: binary output
  * @param[in] phex: hex ascii input
  * @param[in] pbin_len: binary output max lenght
  * @return actual binary output length
  *
  * <b>Example usage</b>
  * \code{.c}
    void test(void)
    {
        ...
        uint8_t *string = "01020304";
        uint8_t uuid[16];
        plt_hex_to_bin(uuid, string, sizeof(uuid));
        // result: uuid[16] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x01, 0x02, 0x03, 0x04}
        ...
    }
  * \endcode
  */
uint16_t plt_hex_to_bin(uint8_t *pbin, uint8_t *phex, uint16_t pbin_len);

/** @brief reset */
#define plt_reset(reason)                       //WDG_SystemReset(RESET_ALL, UPPER_CMD_RESET)

/** @} */
/** @} */

END_DECLS

#endif /* _PLATFORM_MISC_H */
