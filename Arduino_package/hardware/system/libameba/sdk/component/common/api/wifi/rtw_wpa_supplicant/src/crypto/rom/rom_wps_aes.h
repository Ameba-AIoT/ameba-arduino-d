/*
 * AES functions
 * Copyright (c) 2003-2006, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef ROM_WPS_AES_H
#define ROM_WPS_AES_H

#define AES_BLOCK_SIZE 16

//void *aes_encrypt_init(const u8 *key, size_t len);
void *rom_aes_encrypt_init(unsigned char *mem, const u8 *key, size_t len);

#if 1
	void rom_aes_internal_encrypt(void *ctx, const u8 *plain, u8 *crypt);
#else
	void aes_encrypt(void *ctx, const u8 *plain, u8 *crypt);
#endif
void rom_aes_encrypt_deinit(void *ctx);

//void *aes_decrypt_init(const u8 *key, size_t len);
void *rom_aes_decrypt_init(unsigned char *mem, const u8 *key, size_t len);

#if 1
	void rom_aes_internal_decrypt(void *ctx, const u8 *plain, u8 *crypt);
#else
	void aes_decrypt(void *ctx, const u8 *crypt, u8 *plain);
#endif
void rom_aes_decrypt_deinit(void *ctx);


#if 0
void *aes_encrypt_init(const u8 *key, size_t len)
{
  	return NULL;
}

void *aes_decrypt_init(const u8 *key, size_t len)
{
  	return NULL;
}

#define  aes_encrypt(...)		do { } while (0)
#define  aes_encrypt_deinit(...)	do { } while (0)

#define  aes_decrypt(...)		do { } while (0)
#define  aes_decrypt_deinit(...)	do { } while (0)
#endif

#endif /* ROM_WPS_AES_H */
