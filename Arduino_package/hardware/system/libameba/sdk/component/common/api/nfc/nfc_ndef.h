/*
 *  NFC tag type 2 APIs
 *
 *  Copyright (c) 2015 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifdef CONFIG_NFC_EN

#ifndef NFC_NDEF_H
#define NFC_NDEF_H

#define NFC_NDEF_TNF_EMPTY           0x0
#define NFC_NDEF_TNF_WELL_KNOWN      0x1
#define NFC_NDEF_TNF_MIME_MEDIA_TYPE 0x2
#define NFC_NDEF_TNF_ABSOLUTE_URI    0x3
#define NFC_NDEF_TNF_EXTERNAL        0x4
#define NFC_NDEF_TNF_UNKNOWN         0x5
#define NFC_NDEF_TNF_UNCHANGED       0x6
#define NFC_NDEF_TNF_RESERVED        0x7

// RTD type for TNF_WELL_KNOWN
#define NFC_NDEF_RTD_WELL_KNOWN_TEXT             "T"
#define NFC_NDEF_RTD_WELL_KNOWN_SMART_POSTER     "Sp"
#define NFC_NDEF_RTD_WELL_KNOWN_URI              "U"
#define NFC_NDEF_RTD_WELL_KNOWN_HANDOVER_REQUEST "Hr"
#define NFC_NDEF_RTD_WELL_KNOWN_HANDOVER_SELECT  "Hs"
#define NFC_NDEF_RTD_WELL_KNOWN_HANDOVER_CARRIER "Hc"
#define NFC_NDEF_RTD_WELL_KNOWN_SIGNATURE        "Sig"

// RTD type for TNF_EXTERNAL
#define NFC_NDEF_RTD_EXTERNAL_AAR                "com.android:pkg"

// identifier code for RTD type well known uri
#define NFC_NDEF_RTD_WELL_KNOWN_URI_NA         0x00
#define NFC_NDEF_RTD_WELL_KNOWN_URI_HTTP_WWW   0x01
#define NFC_NDEF_RTD_WELL_KNOWN_URI_HTTPS_WWW  0x02
#define NFC_NDEF_RTD_WELL_KNOWN_URI_HTTP       0x03
#define NFC_NDEF_RTD_WELL_KNOWN_URI_HTTPS      0x04
#define NFC_NDEF_RTD_WELL_KNOWN_URI_TEL        0x05
#define NFC_NDEF_RTD_WELL_KNOWN_URI_MAILTO     0x06

typedef struct {
	unsigned char flag_mb;
	unsigned char flag_me;
	unsigned char flag_cf;
	unsigned char flag_sr;
	unsigned char flag_il;
	unsigned char tnf;

	unsigned char record_type_len;
	unsigned char *record_type;

	unsigned char id_len;
	unsigned char *id;

	unsigned int  payload_len;
	unsigned char *payload;
} nfc_ndef;

nfc_ndef* nfc_ndef_parse_byte(unsigned char *buf, unsigned int buf_size);
unsigned int nfc_ndef_length(nfc_ndef *ndef);
unsigned int nfc_ndef_convert_byte(nfc_ndef *ndef, unsigned char *buf, unsigned int buf_size);
void nfc_ndef_free(nfc_ndef *ndef);
void nfc_ndef_dump(nfc_ndef *ndef);

nfc_ndef* nfc_ndef_get_well_known_text(const char *text);
nfc_ndef* nfc_ndef_get_well_known_uri(unsigned char identifier_code, const char *uri);
nfc_ndef* nfc_ndef_get_external_aar(const char *app_name);

#endif
#endif // end of #ifdef CONFIG_NFC_EN
