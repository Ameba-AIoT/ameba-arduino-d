/*
 *  NFC tag type 2 APIs
 *
 *  Copyright (c) 2015 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifdef CONFIG_NFC_EN

#ifndef _NFC_TAG_TYPE2_API_H
#define _NFC_TAG_TYPE2_API_H

#define NFC_TAG_PAGE_SIZE  42
#define NFC_TAG_UID_LENGTH  7

#define NFC_TAG_PAGE_USERDATA_SIZE  36 // page 4~39

#define NFC_TLV_TYPE_NULL        0x00
#define NFC_TLV_TYPE_LOCK_CTRL   0x01
#define NFC_TLV_TYPE_MEM_CTRL    0x02
#define NFC_TLV_TYPE_NDEF_MSG    0x03
#define NFC_TLV_TYPE_PROPRIETARY 0xfd
#define NFC_TLV_TYPE_TERMINATOR  0xfe

// variable definitions

// NFC PAGES
typedef struct {
	unsigned int data;
	unsigned char lock;
	unsigned char dirty;
} nfc_tag_type2_page;

typedef struct {
	nfc_tag_type2_page pages[NFC_TAG_PAGE_SIZE];
	unsigned char uid[NFC_TAG_UID_LENGTH];
} nfc_tag_type2;

// NFC TAG TLV
#define NFC_MAX_TLV_SIZE 5

typedef struct {
	unsigned char type;
	unsigned char len;
	unsigned char *value;
} nfc_tlv;

typedef struct {
	int size;
	nfc_tlv tlv[NFC_MAX_TLV_SIZE];
} nfc_tlvs;

// function prototypes
int  nfc_tag_type2_init(nfc_tag_type2 *tag, unsigned int pages[NFC_TAG_PAGE_SIZE], unsigned int pagesize);
void nfc_tag_type2_init_default(nfc_tag_type2 *tag, unsigned char uid[NFC_TAG_UID_LENGTH]);
void nfc_tag_type2_dump(nfc_tag_type2 *tag);

nfc_tlvs*    nfc_parse_tlvs(nfc_tag_type2 *tag);
int          nfc_overwrite_tlv(nfc_tlvs *tlvs, unsigned type, unsigned char *buf, unsigned int buf_size);
unsigned int nfc_tlvs_convert_page(nfc_tlvs *tlvs, unsigned int *page_buf, unsigned int page_buf_size);
void         nfc_free_tlvs(nfc_tlvs *tlvs);
void         nfc_dump_tlvs(nfc_tlvs *tlvs);

#endif
#endif // end of #ifdef CONFIG_NFC_EN
