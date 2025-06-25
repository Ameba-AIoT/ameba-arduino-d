/**
  ******************************************************************************
  * The header file for usbd_msc.c
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2021, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef USBD_MSC_H
#define USBD_MSC_H

/* Includes ------------------------------------------------------------------*/

#include "usbd.h"

/* Exported defines ----------------------------------------------------------*/

/* CTRL buffer size */
#define USBD_MSC_CTRL_BUF_SIZE						512U

/* MSC Device parameters */
#define USBD_MSC_VID								USB_VID
#define USBD_MSC_PID								USB_PID
#define USBD_MSC_SELF_POWERED						1U
#define USBD_MSC_CONFIG_DESC_SIZE					32U
#define USBD_MSC_LANGID_STRING						0x0409U
#define USBD_MSC_MFG_STRING							"Realtek"
#define USBD_MSC_PROD_HS_STRING						"Realtek MSC (HS)"
#define USBD_MSC_PROD_FS_STRING						"Realtek MSC (FS)"
#define USBD_MSC_SN_STRING							"1234567890"

/* MSC Endpoint parameters */
#define USBD_MSC_BULK_IN_EP							0x81U  /* EP1 for BULK IN */
#define USBD_MSC_BULK_OUT_EP						0x02U  /* EP2 for BULK OUT */
#define USBD_MSC_HS_MAX_PACKET_SIZE					512U   /* High speed BULK IN & OUT packet size */
#define USBD_MSC_FS_MAX_PACKET_SIZE					64U    /* Full speed BULK IN & OUT packet size */

/* MSC configurations */
#define USBD_MSC_RAM_DISK							0      /* Use RAM as storage media, for test purpose only */
#define USBD_MSC_FIX_CV_TEST_ISSUE					0      /* Enable for CV test */

#define USBD_MSC_BLK_BITS							9
#define USBD_MSC_BLK_SIZE							(1 << USBD_MSC_BLK_BITS)
#define USBD_MSC_BUFLEN								(16*1024) /* Default size of buffer length */
/* RAM disk configurations */
#if USBD_MSC_RAM_DISK
#define USBD_MSC_RAM_DISK_SIZE						(USBD_MSC_BUFLEN * 8) // Should be > 64KB to support ATTO benchmark test
#define USBD_MSC_RAM_DISK_SECTORS					(USBD_MSC_RAM_DISK_SIZE >> USBD_MSC_BLK_BITS)
#endif

/* MSC Request Codes */
#define USBD_MSC_REQUEST_RESET						0xFF
#define USBD_MSC_REQUEST_GET_MAX_LUN				0xFE

/* CBW/CSW configurations */
#define USBD_MSC_CB_WRAP_LEN						31U
#define USBD_MSC_CB_SIGN							0x43425355U    /*spells out USBC */
#define USBD_MSC_MAX_DATA							256U
#define USBD_MSC_CS_WRAP_LEN						13U
#define USBD_MSC_CS_SIGN							0x53425355U      /* spells out 'USBS' */

/* CSW Status Definitions */
#define USBD_MSC_CSW_CMD_PASSED						0x00U
#define USBD_MSC_CSW_CMD_FAILED						0x01U
#define USBD_MSC_CSW_PHASE_ERROR					0x02U

/* BOT State */
#define USBD_MSC_IDLE								0U       /* Idle state */
#define USBD_MSC_DATA_OUT							1U       /* Data Out state */
#define USBD_MSC_DATA_IN							2U       /* Data In state */
#define USBD_MSC_LAST_DATA_IN						3U       /* Last Data In Last */
#define USBD_MSC_SEND_DATA							4U       /* Send Immediate data */
#define USBD_MSC_NO_DATA							5U       /* No data Stage */

/* BOT Status */
#define USBD_MSC_STATUS_NORMAL						0U
#define USBD_MSC_STATUS_RECOVERY					1U
#define USBD_MSC_STATUS_ERROR						2U

/* Sense */
#define USBD_MSC_SENSE_LIST_DEPTH                   4U

/* SD access retry */
#define USBD_MSC_SD_ACCESS_RETRY					3U

typedef struct {
	int(*disk_init)(void);
	int(*disk_deinit)(void);
	int(*disk_getcapacity)(u32 *sectors);
	int(*disk_read)(u32 sector, u8 *buffer, u32 count);
	int(*disk_write)(u32 sector, const u8 *buffer, u32 count);
} usbd_msc_disk_ops_t;

/* Command Block Wrapper */
typedef struct {
	u32 dCBWSignature;			/* CBW ID: 'USBC' */
	u32 dCBWTag;				/* A Command Block Tag sent by the host, unique per command id */
	u32 dCBWDataTransferLength;	/* The number of bytes of data that the host expects to transfer */
	u8 bmCBWFlags;				/* Data transfer direction */
	u8 bCBWLUN;					/* The device Logical Unit Number to which the command block is being sent */
	u8 bCBWCBLength;			/* The valid length of the CBWCB in bytes */
	u8 CBWCB[16];				/* The command block to be executed by the device */
} usbd_msc_cbw_t;

/* Command Status Wrapper */
typedef struct {
	u32 dCSWSignature;			/* CSW ID: 'USBS' */
	u32 dCSWTag;				/* The device shall set this field to the value received in the dCBWTag of the associated CBW */
	u32 dCSWDataResidue;		/* The amount of data not transferred, shall <= dCBWDataTransferLength */
	u8 bCSWStatus;				/* Execute command status */
} usbd_msc_csw_t;

/* SCSI Sense data */
typedef struct {
	char skey;				/* Sense key */
	union {
		struct _asc {
			char asc;		/* additional sense code */
			char ascq;		/* additional sense code qualifier */
		} b;
		u8 asc;				/* additional sense code */
	} w;
} usbd_msc_scsi_sense_data_t;

typedef struct {
	void (*status_changed)(u8 status);
} usbd_msc_cb_t;

typedef struct {
	u8 phase_error;
	u8 is_open;
	u8 ro;
	u8 bot_state;
	u8 bot_status;
	u32 data_length;
	u8 *data;
	u8 *ctrl_buf;
	usbd_msc_scsi_sense_data_t scsi_sense_data[USBD_MSC_SENSE_LIST_DEPTH];
	u8 scsi_sense_head;
	u8 scsi_sense_tail;
	__IO u8 is_ready;
	u32 num_sectors;
	u32 lba; // logic block address
	u32 blkbits; /* bits of logical block size of bound block device */
	u32 blksize;
	u32 blklen;
	usbd_msc_cbw_t *cbw;
	usbd_msc_csw_t *csw;
	usbd_msc_disk_ops_t disk_ops;
	usbd_msc_cb_t *cb;
	usb_dev_t *dev;
} usbd_msc_dev_t;

int usbd_msc_init(usbd_msc_cb_t *cb);
void usbd_msc_deinit(void);

#endif // USBD_MSC_H
