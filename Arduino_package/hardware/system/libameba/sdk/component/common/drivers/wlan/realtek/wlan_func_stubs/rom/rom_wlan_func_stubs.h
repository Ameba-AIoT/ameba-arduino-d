#ifndef ROM_WLAN_FUNC_STUBS_H
#define ROM_WLAN_FUNC_STUBS_H

#include <drv_types.h>
#include <rom_aes.h>
#include <rom_arc4.h>
#include <rom_md5.h>
#include <rom_rc4.h>
#include <rtw_psk.h>
#include <rom_sha1.h>
#include <rom_wps_big_num.h>

typedef struct wlan_func_stubs_s {
	// aes
	int (*aes_set_key)(aes_context *ctx, u8 *key, int nbits);
	void (*aes_encrypt)(aes_context *ctx, u8 input[16], u8 output[16]);
	void (*aes_decrypt)(aes_context *ctx, u8 input[16], u8 output[16]);
	void (*AES_WRAP)(unsigned char *plain, int plain_len, unsigned char *iv, int iv_len, unsigned char *kek, int kek_len, unsigned char *cipher, unsigned short *cipher_len);
	void (*AES_UnWRAP)(unsigned char *cipher, int cipher_len, unsigned char *kek, int kek_len, unsigned char *plain);
	// arc4
	u32 (*crc32_get)(u8 *buf, sint len);
	u32 (*arc4_byte)(struct arc4context *parc4ctx);
	void (*rt_arc4_init)(struct arc4context *parc4ctx, u8 *key, u32 key_len);
	void (*rt_arc4_crypt)(struct arc4context *parc4ctx, u8 *dest, u8 *src, u32 len);
	// md5
	void (*rt_md5_init)(md5_ctx *context);
	void (*rt_md5_append)(md5_ctx *context, u8 *input, u32 inputLen);
	void (*rt_md5_final)(u8 digest[16], md5_ctx *context);
	void (*rt_md5_hmac)(unsigned char *text, int text_len, unsigned char *key, int key_len, void *digest);
	// rc4
	void (*RC4)(RC4_KEY *key, unsigned long len, const unsigned char *indata, unsigned char *outdata);
	void (*RC4_set_key)(RC4_KEY *key, int len, const unsigned char *data);
	// rtw_ieee80211
	int (*rtw_get_bit_value_from_ieee_value)(u8 val);
	uint (*rtw_is_cckrates_included)(u8 *rate);
	uint (*rtw_is_cckratesonly_included)(u8 *rate);
	int (*rtw_check_network_type)(unsigned char *rate, int ratelen, int channel);
	u8* (*rtw_set_fixed_ie)(unsigned char *pbuf, unsigned int len, unsigned char *source, unsigned int *frlen);
	u8* (*rtw_set_ie)(u8 *pbuf, sint index, uint len, u8 *source, uint *frlen);
	u8* (*rtw_get_ie)(u8 *pbuf, sint index, u32 *len, sint limit);
	void (*rtw_set_supported_rate)(u8* SupportedRates, uint mode);
	uint (*rtw_get_rateset_len)(u8 *rateset);
	unsigned char* (*rtw_get_wpa_ie)(unsigned char *pie, u32 *wpa_ie_len, int limit);
	unsigned char* (*rtw_get_wpa2_ie)(unsigned char *pie, u32 *rsn_ie_len, int limit);
	int (*rtw_get_wpa_cipher_suite)(u8 *s);
	int (*rtw_get_wpa2_cipher_suite)(u8 *s);
	int (*rtw_parse_wpa_ie)(u8* wpa_ie, int wpa_ie_len, int *group_cipher, int *pairwise_cipher, int *is_8021x);
	int (*rtw_parse_wpa2_ie)(u8* rsn_ie, int rsn_ie_len, int *group_cipher, int *pairwise_cipher, int *is_8021x);
	int (*rtw_get_sec_ie)(u8 *in_ie, uint in_len, u8 *rsn_ie, u16 *rsn_len, u8 *wpa_ie, u16 *wpa_len);
	u8* (*rtw_get_wps_ie)(u8 *in_ie, uint in_len, u8 *wps_ie, uint *wps_ielen);
	u8* (*rtw_get_wps_attr)(u8 *wps_ie, uint wps_ielen, u16 target_attr_id, u8 *buf_attr, u32 *len_attr);
	u8* (*rtw_get_wps_attr_content)(u8 *wps_ie, uint wps_ielen, u16 target_attr_id, u8 *buf_content, uint *len_content);
	ParseRes (*rtw_ieee802_11_parse_elems)(u8 *start, uint len, struct rtw_ieee802_11_elems *elems, int show_errors);
	u8 (*str_2char2num)(u8 hch, u8 lch);
	u8 (*key_2char2num)(u8 hch, u8 lch);
	u8 (*convert_ip_addr)(u8 hch, u8 mch, u8 lch);
	// rtw_message
	char **rom_e_rtw_msgp_str_;
	char **rom_f_rtw_msgp_str_;
	// rtw_psk
	int (*rom_psk_PasswordHash)(unsigned char *password, int passwordlength, unsigned char *ssid, int ssidlength, unsigned char *output);
	void (*rom_psk_CalcGTK)(unsigned char *addr, unsigned char *nonce, unsigned char *keyin, int keyinlen, unsigned char *keyout, int keyoutlen);
	void (*rom_psk_CalcPTK)(unsigned char *addr1, unsigned char *addr2, unsigned char *nonce1, unsigned char *nonce2, unsigned char *keyin, int keyinlen, unsigned char *keyout, int keyoutlen);
	// rtw_security
	void (*wep_80211_encrypt)(u8 *pframe, u32 wlan_hdr_len, u32 iv_len, u32 payload_len, u8 *key, u32 key_len);
	u8 (*wep_80211_decrypt)(u8 *pframe, u32 wlan_hdr_len, u32 iv_len, u32 payload_len, u8 *key, u32 key_len, union u_crc *pcrc);
	void (*tkip_micappendbyte)(struct mic_data *pmicdata, u8 b);
	void (*rtw_secmicsetkey)(struct mic_data *pmicdata, u8 *key);
	void (*rtw_secmicappend)(struct mic_data *pmicdata, u8 *src, u32 nbytes);
	void (*rtw_secgetmic)(struct mic_data *pmicdata, u8 *dst);
	void (*rtw_seccalctkipmic)(u8 *key, u8 *header, u8 *data, u32 data_len, u8 *mic_code, u8 pri);
	void (*tkip_phase1)(u16 *p1k, const u8 *tk, const u8 *ta, u32 iv32);
	void (*tkip_phase2)(u8 *rc4key, const u8 *tk, const u16 *p1k, u16 iv16);
	void (*tkip_80211_encrypt)(u8 *pframe, u32 wlan_hdr_len, u32 iv_len, u32 payload_len, u8 *key, u32 key_len, u8 *ta);
	u8 (*tkip_80211_decrypt)(u8 *pframe, u32 wlan_hdr_len, u32 iv_len, u32 payload_len, u8 *key, u32 key_len, u8 *ta, union u_crc *pcrc);
	void (*aes1_encrypt)(u8 *key, u8 *data, u8 *ciphertext);
	void (*aesccmp_construct_mic_iv)(u8 *mic_iv, sint qc_exists, sint a4_exists, u8 *mpdu, uint payload_length, u8 *pn_vector);
	void (*aesccmp_construct_mic_header1)(u8 *mic_header1, sint header_length, u8 *mpdu);
	void (*aesccmp_construct_mic_header2)(u8 *mic_header2, u8 *mpdu, sint a4_exists, sint qc_exists);
	void (*aesccmp_construct_ctr_preload)(u8 *ctr_preload, sint a4_exists, sint qc_exists, u8 *mpdu, u8 *pn_vector, sint c);
	u32 (*aes_80211_encrypt)(u8 *pframe, u32 wlan_hdr_len, u32 payload_len, u8 *key, u32 frame_type, u8 *mic);
	u32 (*aes_80211_decrypt)(u8 *pframe, u32 wlan_hdr_len, u32 payload_len, u8 *key, u32 frame_type, u8 *mic);
	// sha1
	int (*rt_sha1_init)(SHA1Context *context);
	int (*rt_sha1_update)(SHA1Context *context, const u8 *message_array, unsigned length);
	int (*rt_sha1_finish)(SHA1Context *context, u8 Message_Digest[SHA1HashSize]);
	void (*rt_hmac_sha1)(unsigned char *text, int text_len, unsigned char *key, int key_len, unsigned char *digest);
	// wlan_ram_map
	void (*init_rom_wlan_ram_map)(unsigned char * (*rtw_malloc)(unsigned int sz), void (*rtw_mfree)(unsigned char *pbuf, unsigned int sz));
	// wps-aes
	int (*rom_aes_128_cbc_encrypt)(const u8 *key, const u8 *iv, u8 *data, size_t data_len);
	int (*rom_aes_128_cbc_decrypt)(const u8 *key, const u8 *iv, u8 *data, size_t data_len);
	void (*rom_rijndaelKeySetupEnc)(u32 rk[/*44*/], const u8 cipherKey[]);
	void* (*rom_aes_decrypt_init)(unsigned char *mem, const u8 *key, size_t len);
	void (*rom_aes_internal_decrypt)(void *ctx, const u8 *crypt, u8 *plain);
	void (*rom_aes_decrypt_deinit)(void *ctx);
	void* (*rom_aes_encrypt_init)(unsigned char *mem, const u8 *key, size_t len);
	void (*rom_aes_internal_encrypt)(void *ctx, const u8 *plain, u8 *crypt);
	void (*rom_aes_encrypt_deinit)(void *ctx);
	// wps-bignum
	struct bignum* (*bignum_init)(void);
	void (*bignum_deinit)(struct bignum *n);
	size_t (*bignum_get_unsigned_bin_len)(struct bignum *n);
	int (*bignum_get_unsigned_bin)(const struct bignum *n, u8 *buf, size_t *len);
	int (*bignum_set_unsigned_bin)(struct bignum *n, const u8 *buf, size_t len);
	int (*bignum_cmp)(const struct bignum *a, const struct bignum *b);
	int (*bignum_cmp_d)(const struct bignum *a, unsigned long b);
	int (*bignum_add)(const struct bignum *a, const struct bignum *b, struct bignum *c);
	int (*bignum_sub)(const struct bignum *a, const struct bignum *b, struct bignum *c);
	int (*bignum_mul)(const struct bignum *a, const struct bignum *b, struct bignum *c);
	int (*bignum_exptmod)(const struct bignum *a, const struct bignum *b, const struct bignum *c, struct bignum *d);
	// wps-os
	u8* (*WPS_realloc)(u8 *old_buf, u32 old_sz, u32 new_sz);
	void* (*os_zalloc)(size_t size);
	// wps-sha256
	int (*rom_hmac_sha256_vector)(const u8 *key, size_t key_len, size_t num_elem, const u8 *addr[], const size_t *len, u8 *mac);
	int (*rom_hmac_sha256)(const u8 *key, size_t key_len, const u8 *data, size_t data_len, u8 *mac);
	int (*rom_sha256_vector)(size_t num_elem, const u8 *addr[], const size_t *len, u8 *mac);
} wlan_func_stubs_t;

#endif // ROM_WLAN_FUNC_STUBS_H