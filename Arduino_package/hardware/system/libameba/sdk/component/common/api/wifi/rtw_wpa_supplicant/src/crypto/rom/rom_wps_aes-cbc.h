/*
 * AES-128 CBC
 *
 * Copyright (c) 2003-2007, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */
#ifndef ROM_WPS_AES_CBC_H
#define ROM_WPS_AES_CBC_H

int rom_aes_128_cbc_encrypt(const u8 *key, const u8 *iv, u8 *data, size_t data_len);
int rom_aes_128_cbc_decrypt(const u8 *key, const u8 *iv, u8 *data, size_t data_len);

#endif /* ROM_WPS_AES_CBC_H */


