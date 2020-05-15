/*
 * AES (Rijndael) cipher
 * Copyright (c) 2003-2012, Jouni Malinen <j@w1.fi>
 *
 * This software may be distributed under the terms of the BSD license.
 * See README for more details.
 */

#ifndef ROM_WPS_AES_I_H
#define ROM_WPS_AES_I_H

#include "rom_wps_aes.h"

/* #define FULL_UNROLL */
#ifndef AES_SMALL_TABLES
#define AES_SMALL_TABLES
#endif
extern const u32 rom_wps_Te0[256];
extern const u32 rom_wps_Te1[256];
extern const u32 rom_wps_Te2[256];
extern const u32 rom_wps_Te3[256];
extern const u32 rom_wps_Te4[256];
extern const u32 rom_wps_Td0[256];
extern const u32 rom_wps_Td1[256];
extern const u32 rom_wps_Td2[256];
extern const u32 rom_wps_Td3[256];
extern const u32 rom_wps_Td4[256];
extern const u32 rom_wps_rcon[10];
extern const u8 rom_wps_Td4s[256];
extern const u8 rom_wps_rcons[10];


#ifndef AES_SMALL_TABLES

#define WPS_RCON(i) rom_wps_rcon[(i)]

#define WPS_TE0(i) rom_wps_Te0[((i) >> 24) & 0xff]
#define WPS_TE1(i) rom_wps_Te1[((i) >> 16) & 0xff]
#define WPS_TE2(i) rom_wps_Te2[((i) >> 8) & 0xff]
#define WPS_TE3(i) rom_wps_Te3[(i) & 0xff]
#define WPS_TE41(i) (rom_wps_Te4[((i) >> 24) & 0xff] & 0xff000000)
#define WPS_TE42(i) (rom_wps_Te4[((i) >> 16) & 0xff] & 0x00ff0000)
#define WPS_TE43(i) (rom_wps_Te4[((i) >> 8) & 0xff] & 0x0000ff00)
#define WPS_TE44(i) (rom_wps_Te4[(i) & 0xff] & 0x000000ff)
#define WPS_TE421(i) (rom_wps_Te4[((i) >> 16) & 0xff] & 0xff000000)
#define WPS_TE432(i) (rom_wps_Te4[((i) >> 8) & 0xff] & 0x00ff0000)
#define WPS_TE443(i) (rom_wps_Te4[(i) & 0xff] & 0x0000ff00)
#define WPS_TE414(i) (rom_wps_Te4[((i) >> 24) & 0xff] & 0x000000ff)
#define WPS_TE411(i) (rom_wps_Te4[((i) >> 24) & 0xff] & 0xff000000)
#define WPS_TE422(i) (rom_wps_Te4[((i) >> 16) & 0xff] & 0x00ff0000)
#define WPS_TE433(i) (rom_wps_Te4[((i) >> 8) & 0xff] & 0x0000ff00)
#define WPS_TE444(i) (rom_wps_Te4[(i) & 0xff] & 0x000000ff)
#define WPS_TE4(i) (rom_wps_Te4[(i)] & 0x000000ff)

#define WPS_TD0(i) rom_wps_Td0[((i) >> 24) & 0xff]
#define WPS_TD1(i) rom_wps_Td1[((i) >> 16) & 0xff]
#define WPS_TD2(i) rom_wps_Td2[((i) >> 8) & 0xff]
#define WPS_TD3(i) rom_wps_Td3[(i) & 0xff]
#define WPS_TD41(i) (rom_wps_Td4[((i) >> 24) & 0xff] & 0xff000000)
#define WPS_TD42(i) (rom_wps_Td4[((i) >> 16) & 0xff] & 0x00ff0000)
#define WPS_TD43(i) (rom_wps_Td4[((i) >> 8) & 0xff] & 0x0000ff00)
#define WPS_TD44(i) (rom_wps_Td4[(i) & 0xff] & 0x000000ff)
#define WPS_TD0_(i) rom_wps_Td0[(i) & 0xff]
#define WPS_TD1_(i) rom_wps_Td1[(i) & 0xff]
#define WPS_TD2_(i) rom_wps_Td2[(i) & 0xff]
#define WPS_TD3_(i) rom_wps_Td3[(i) & 0xff]



#else /* AES_SMALL_TABLES */

#define WPS_RCON(i) (rom_wps_rcons[(i)] << 24)

#if 0
static inline u32 wps_rotr(u32 val, int bits)
{
	return (val >> bits) | (val << (32 - bits));
}
#else
#define wps_rotr(val, bits) ((u32)((((u32) val) >> (int) bits) | (((u32) val) << (32 - (int) bits))))
#endif

#define WPS_TE0(i) rom_wps_Te0[((i) >> 24) & 0xff]
#define WPS_TE1(i) wps_rotr(rom_wps_Te0[((i) >> 16) & 0xff], 8)
#define WPS_TE2(i) wps_rotr(rom_wps_Te0[((i) >> 8) & 0xff], 16)
#define WPS_TE3(i) wps_rotr(rom_wps_Te0[(i) & 0xff], 24)
#define WPS_TE41(i) ((rom_wps_Te0[((i) >> 24) & 0xff] << 8) & 0xff000000)
#define WPS_TE42(i) (rom_wps_Te0[((i) >> 16) & 0xff] & 0x00ff0000)
#define WPS_TE43(i) (rom_wps_Te0[((i) >> 8) & 0xff] & 0x0000ff00)
#define WPS_TE44(i) ((rom_wps_Te0[(i) & 0xff] >> 8) & 0x000000ff)
#define WPS_TE421(i) ((rom_wps_Te0[((i) >> 16) & 0xff] << 8) & 0xff000000)
#define WPS_TE432(i) (rom_wps_Te0[((i) >> 8) & 0xff] & 0x00ff0000)
#define WPS_TE443(i) (rom_wps_Te0[(i) & 0xff] & 0x0000ff00)
#define WPS_TE414(i) ((rom_wps_Te0[((i) >> 24) & 0xff] >> 8) & 0x000000ff)
#define WPS_TE411(i) ((rom_wps_Te0[((i) >> 24) & 0xff] << 8) & 0xff000000)
#define WPS_TE422(i) (rom_wps_Te0[((i) >> 16) & 0xff] & 0x00ff0000)
#define WPS_TE433(i) (rom_wps_Te0[((i) >> 8) & 0xff] & 0x0000ff00)
#define WPS_TE444(i) ((rom_wps_Te0[(i) & 0xff] >> 8) & 0x000000ff)
#define WPS_TE4(i) ((rom_wps_Te0[(i)] >> 8) & 0x000000ff)

#define WPS_TD0(i) rom_wps_Td0[((i) >> 24) & 0xff]
#define WPS_TD1(i) wps_rotr(rom_wps_Td0[((i) >> 16) & 0xff], 8)
#define WPS_TD2(i) wps_rotr(rom_wps_Td0[((i) >> 8) & 0xff], 16)
#define WPS_TD3(i) wps_rotr(rom_wps_Td0[(i) & 0xff], 24)
#define WPS_TD41(i) (rom_wps_Td4s[((i) >> 24) & 0xff] << 24)
#define WPS_TD42(i) (rom_wps_Td4s[((i) >> 16) & 0xff] << 16)
#define WPS_TD43(i) (rom_wps_Td4s[((i) >> 8) & 0xff] << 8)
#define WPS_TD44(i) (rom_wps_Td4s[(i) & 0xff])
#define WPS_TD0_(i) rom_wps_Td0[(i) & 0xff]
#define WPS_TD1_(i) wps_rotr(rom_wps_Td0[(i) & 0xff], 8)
#define WPS_TD2_(i) wps_rotr(rom_wps_Td0[(i) & 0xff], 16)
#define WPS_TD3_(i) wps_rotr(rom_wps_Td0[(i) & 0xff], 24)

#endif /* AES_SMALL_TABLES */


#ifdef _MSC_VER
#define SWAP(x) (_lrotl(x, 8) & 0x00ff00ff | _lrotr(x, 8) & 0xff00ff00)
#define GETU32(p) SWAP(*((u32 *)(p)))
#define PUTU32(ct, st) { *((u32 *)(ct)) = SWAP((st)); }
#else
#define GETU32(pt) (((u32)(pt)[0] << 24) ^ ((u32)(pt)[1] << 16) ^ \
((u32)(pt)[2] <<  8) ^ ((u32)(pt)[3]))
#define PUTU32(ct, st) { \
(ct)[0] = (u8)((st) >> 24); (ct)[1] = (u8)((st) >> 16); \
(ct)[2] = (u8)((st) >>  8); (ct)[3] = (u8)(st); }
#endif

#define WPS_AES_PRIV_SIZE (4* 44)//(4 * 4 * 15 + 4)
#define WPS_AES_PRIV_NR_POS (4 * 15)

//int rijndaelKeySetupEnc(u32 rk[], const u8 cipherKey[], int keyBits);
void rom_rijndaelKeySetupEnc(u32 rk[/*44*/], const u8 cipherKey[]);

#endif /* ROM_WPS_AES_I_H */
