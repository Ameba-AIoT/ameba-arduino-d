#ifndef ROM_WLAN_FUNC_RENAME_H
#define ROM_WLAN_FUNC_RENAME_H

// aes
#define aes_set_key                          _aes_set_key
#define aes_encrypt                          _aes_encrypt
#define aes_decrypt                          _aes_decrypt
#define AES_WRAP                             _AES_WRAP
#define AES_UnWRAP                           _AES_UnWRAP
// arc4
#define crc32_get                            _crc32_get
#define arc4_byte                            _arc4_byte
#define rt_arc4_init                         _rt_arc4_init
#define rt_arc4_crypt                        _rt_arc4_crypt
// md5
#define rt_md5_init                          _rt_md5_init
#define rt_md5_append                        _rt_md5_append
#define rt_md5_final                         _rt_md5_final
#define rt_md5_hmac                          _rt_md5_hmac
// rc4
#define RC4                                  _RC4
#define RC4_set_key                          _RC4_set_key
// rtw_ieee80211
#define rtw_get_bit_value_from_ieee_value    _rtw_get_bit_value_from_ieee_value
#define rtw_is_cckrates_included             _rtw_is_cckrates_included
#define rtw_is_cckratesonly_included         _rtw_is_cckratesonly_included
#define rtw_check_network_type               _rtw_check_network_type
#define rtw_set_fixed_ie                     _rtw_set_fixed_ie
#define rtw_set_ie                           _rtw_set_ie
#define rtw_get_ie                           _rtw_get_ie
#define rtw_set_supported_rate               _rtw_set_supported_rate
#define rtw_get_rateset_len                  _rtw_get_rateset_len
#define rtw_get_wpa_ie                       _rtw_get_wpa_ie
#define rtw_get_wpa2_ie                      _rtw_get_wpa2_ie
#define rtw_get_wpa_cipher_suite             _rtw_get_wpa_cipher_suite
#define rtw_get_wpa2_cipher_suite            _rtw_get_wpa2_cipher_suite
#define rtw_parse_wpa_ie                     _rtw_parse_wpa_ie
#define rtw_parse_wpa2_ie                    _rtw_parse_wpa2_ie
#define rtw_get_sec_ie                       _rtw_get_sec_ie
#define rtw_get_wps_ie                       _rtw_get_wps_ie
#define rtw_get_wps_attr                     _rtw_get_wps_attr
#define rtw_get_wps_attr_content             _rtw_get_wps_attr_content
#define rtw_ieee802_11_parse_elems           _rtw_ieee802_11_parse_elems
#define str_2char2num                        _str_2char2num
#define key_2char2num                        _key_2char2num
#define convert_ip_addr                      _convert_ip_addr
#define RTW_WPA_OUI_TYPE                     _RTW_WPA_OUI_TYPE
#define WPA_CIPHER_SUITE_NONE                _WPA_CIPHER_SUITE_NONE
#define WPA_CIPHER_SUITE_WEP40               _WPA_CIPHER_SUITE_WEP40
#define WPA_CIPHER_SUITE_TKIP                _WPA_CIPHER_SUITE_TKIP
#define WPA_CIPHER_SUITE_CCMP                _WPA_CIPHER_SUITE_CCMP
#define WPA_CIPHER_SUITE_WEP104              _WPA_CIPHER_SUITE_WEP104
#define RSN_VERSION_BSD                      _RSN_VERSION_BSD
#define RSN_AUTH_KEY_MGMT_UNSPEC_802_1X      _RSN_AUTH_KEY_MGMT_UNSPEC_802_1X
#define RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X    _RSN_AUTH_KEY_MGMT_PSK_OVER_802_1X
#define RSN_CIPHER_SUITE_NONE                _RSN_CIPHER_SUITE_NONE
#define RSN_CIPHER_SUITE_WEP40               _RSN_CIPHER_SUITE_WEP40
#define RSN_CIPHER_SUITE_TKIP                _RSN_CIPHER_SUITE_TKIP
#define RSN_CIPHER_SUITE_CCMP                _RSN_CIPHER_SUITE_CCMP
#define RSN_CIPHER_SUITE_WEP104              _RSN_CIPHER_SUITE_WEP104
// rtw_message
#define rom_e_rtw_msgp_str_                  _rom_e_rtw_msgp_str_
#define rom_f_rtw_msgp_str_                  _rom_f_rtw_msgp_str_
// rtw_psk
#define rom_psk_PasswordHash                 _rom_psk_PasswordHash
#define rom_psk_CalcGTK                      _rom_psk_CalcGTK
#define rom_psk_CalcPTK                      _rom_psk_CalcPTK
// rtw_security
#define wep_80211_encrypt                    _wep_80211_encrypt
#define wep_80211_decrypt                    _wep_80211_decrypt
#define tkip_micappendbyte                   _tkip_micappendbyte
#define rtw_secmicsetkey                     _rtw_secmicsetkey
#define rtw_secmicappend                     _rtw_secmicappend
#define rtw_secgetmic                        _rtw_secgetmic
#define rtw_seccalctkipmic                   _rtw_seccalctkipmic
#define tkip_phase1                          _tkip_phase1
#define tkip_phase2                          _tkip_phase2
#define tkip_80211_encrypt                   _tkip_80211_encrypt
#define tkip_80211_decrypt                   _tkip_80211_decrypt
#define aes1_encrypt                         _aes1_encrypt
#define aesccmp_construct_mic_iv             _aesccmp_construct_mic_iv
#define aesccmp_construct_mic_header1        _aesccmp_construct_mic_header1
#define aesccmp_construct_mic_header2        _aesccmp_construct_mic_header2
#define aesccmp_construct_ctr_preload        _aesccmp_construct_ctr_preload
#define aes_80211_encrypt                    _aes_80211_encrypt
#define aes_80211_decrypt                    _aes_80211_decrypt
// sha1
#define rt_sha1_init                         _rt_sha1_init
#define rt_sha1_update                       _rt_sha1_update
#define rt_sha1_finish                       _rt_sha1_finish
#define rt_hmac_sha1                         _rt_hmac_sha1
// wps-aes
#define rom_aes_128_cbc_encrypt              _rom_aes_128_cbc_encrypt
#define rom_aes_128_cbc_decrypt              _rom_aes_128_cbc_decrypt
#define rom_rijndaelKeySetupEnc              _rom_rijndaelKeySetupEnc
#define rom_aes_decrypt_init                 _rom_aes_decrypt_init
#define rom_aes_internal_decrypt             _rom_aes_internal_decrypt
#define rom_aes_decrypt_deinit               _rom_aes_decrypt_deinit
#define rom_aes_encrypt_init                 _rom_aes_encrypt_init
#define rom_aes_internal_encrypt             _rom_aes_internal_encrypt
#define rom_aes_encrypt_deinit               _rom_aes_encrypt_deinit
// wps-bignum
#define bignum_init                          _bignum_init
#define bignum_deinit                        _bignum_deinit
#define bignum_get_unsigned_bin_len          _bignum_get_unsigned_bin_len
#define bignum_get_unsigned_bin              _bignum_get_unsigned_bin
#define bignum_set_unsigned_bin              _bignum_set_unsigned_bin
#define bignum_cmp                           _bignum_cmp
#define bignum_cmp_d                         _bignum_cmp_d
#define bignum_add                           _bignum_add
#define bignum_sub                           _bignum_sub
#define bignum_mul                           _bignum_mul
#define bignum_exptmod                       _bignum_exptmod
// wps-os
#define WPS_realloc                          _WPS_realloc
#define os_zalloc                            _os_zalloc
// wps-sha256
#define rom_hmac_sha256_vector               _rom_hmac_sha256_vector
#define rom_hmac_sha256                      _rom_hmac_sha256
#define rom_sha256_vector                    _rom_sha256_vector

// for wlan ram map in WPS ROM
#include <rom_wlan_ram_map.h>
extern struct _rom_wlan_ram_map rom_wlan_ram_map;
#define os_malloc(sz) rom_wlan_ram_map.rtw_malloc(sz)
#define os_free(p, sz) rom_wlan_ram_map.rtw_mfree(((u8*)(p)), (sz))

#endif // ROM_WLAN_FUNC_RENAME_H