#ifndef __STORAGE_H
#define __STORAGE_H

//#include "us_os_wrap_via_osdep_api.h"

/*
 * linux/usb/storage.h
 *
 * Copyright Matthew Wilcox for Intel Corp, 2010
 *
 * This file contains definitions taken from the
 * USB Mass Storage Class Specification Overview
 *
 * Distributed under the terms of the GNU GPL, version two.
 */

/* Storage subclass codes */

/*
 * Definitions for sparse validation
 * (http://kernel.org/pub/linux/kernel/people/josh/sparse/)
 */

#ifndef min
#define min(x, y) ((x) < (y) ? (x) : (y))
#endif
#ifndef max
#define max(x, y) ((x) > (y) ? (x) : (y))
#endif

#ifdef __CHECKER__
#define __force __attribute__((force))
#define __bitwise __attribute__((bitwise))
#else
#define __force
#define __bitwise
#endif

typedef u16 __bitwise be16;
typedef u16 __bitwise le16;
typedef u32 __bitwise be32;
typedef u32 __bitwise le32;
typedef u64 __bitwise be64;
typedef u64 __bitwise le64;


#define USB_SC_RBC	0x01		/* Typically, flash devices */
#define USB_SC_8020	0x02		/* CD-ROM */
#define USB_SC_QIC	0x03		/* QIC-157 Tapes */
#define USB_SC_UFI	0x04		/* Floppy */
#define USB_SC_8070	0x05		/* Removable media */
#define USB_SC_SCSI	0x06		/* Transparent */
#define USB_SC_LOCKABLE	0x07		/* Password-protected */

#define USB_SC_ISD200	0xf0		/* ISD200 ATA */
#define USB_SC_CYP_ATACB	0xf1	/* Cypress ATACB */
#define USB_SC_DEVICE	0xff		/* Use device's value */

/* Storage protocol codes */

#define USB_PR_CBI	0x00		/* Control/Bulk/Interrupt */
#define USB_PR_CB	0x01		/* Control/Bulk w/o interrupt */
#define USB_PR_BULK	0x50		/* bulk only */
#define USB_PR_UAS	0x62		/* USB Attached SCSI */

#define USB_PR_USBAT	0x80		/* SCM-ATAPI bridge */
#define USB_PR_EUSB_SDDR09	0x81	/* SCM-SCSI bridge for SDDR-09 */
#define USB_PR_SDDR55	0x82		/* SDDR-55 (made up) */
#define USB_PR_DPCM_USB	0xf0		/* Combination CB/SDDR09 */
#define USB_PR_FREECOM	0xf1		/* Freecom */
#define USB_PR_DATAFAB	0xf2		/* Datafab chipsets */
#define USB_PR_JUMPSHOT	0xf3		/* Lexar Jumpshot */
#define USB_PR_ALAUDA	0xf4		/* Alauda chipsets */
#define USB_PR_KARMA	0xf5		/* Rio Karma */

#define USB_PR_DEVICE	0xff		/* Use device's value */

 /*
  * Bulk only data structures
  */

/*
 *      SCSI opcodes
 */

#define TEST_UNIT_READY       0x00
#define REZERO_UNIT           0x01
#define REQUEST_SENSE         0x03
#define FORMAT_UNIT           0x04
#define READ_BLOCK_LIMITS     0x05
#define REASSIGN_BLOCKS       0x07
#define INITIALIZE_ELEMENT_STATUS 0x07
#define READ_6                0x08
#define WRITE_6               0x0a
#define SEEK_6                0x0b
#define READ_REVERSE          0x0f
#define WRITE_FILEMARKS       0x10
#define SPACE                 0x11
#define INQUIRY               0x12
#define RECOVER_BUFFERED_DATA 0x14
#define MODE_SELECT           0x15
#define RESERVE               0x16
#define RELEASE               0x17
#define COPY                  0x18
#define ERASE                 0x19
#define MODE_SENSE            0x1a
#define START_STOP            0x1b
#define RECEIVE_DIAGNOSTIC    0x1c
#define SEND_DIAGNOSTIC       0x1d
#define ALLOW_MEDIUM_REMOVAL  0x1e

#define READ_FORMAT_CAPACITIES 0x23
#define SET_WINDOW            0x24
#define READ_CAPACITY         0x25
#define READ_10               0x28
#define WRITE_10              0x2a
#define SEEK_10               0x2b
#define POSITION_TO_ELEMENT   0x2b
#define WRITE_VERIFY          0x2e
#define VERIFY                0x2f
#define SEARCH_HIGH           0x30
#define SEARCH_EQUAL          0x31
#define SEARCH_LOW            0x32
#define SET_LIMITS            0x33
#define PRE_FETCH             0x34
#define READ_POSITION         0x34
#define SYNCHRONIZE_CACHE     0x35
#define LOCK_UNLOCK_CACHE     0x36
#define READ_DEFECT_DATA      0x37
#define MEDIUM_SCAN           0x38
#define COMPARE               0x39
#define COPY_VERIFY           0x3a
#define WRITE_BUFFER          0x3b
#define READ_BUFFER           0x3c
#define UPDATE_BLOCK          0x3d
#define READ_LONG             0x3e
#define WRITE_LONG            0x3f
#define CHANGE_DEFINITION     0x40
#define WRITE_SAME            0x41
#define UNMAP		      0x42
#define READ_TOC              0x43
#define READ_HEADER           0x44
#define GET_CONFIGURATION     0x46
#define GET_EVENT_STATUS_NOTIFICATION 0x4a
#define LOG_SELECT            0x4c
#define LOG_SENSE             0x4d
#define READ_DISC_INFORMATION			0x51
#define READ_TRACK_INFORMATION		0x52
#define XDWRITEREAD_10        0x53
#define MODE_SELECT_10        0x55
#define RESERVE_10            0x56
#define RELEASE_10            0x57
#define MODE_SENSE_10         0x5a
#define PERSISTENT_RESERVE_IN 0x5e
#define PERSISTENT_RESERVE_OUT 0x5f
#define VARIABLE_LENGTH_CMD   0x7f
#define REPORT_LUNS           0xa0
#define SECURITY_PROTOCOL_IN  0xa2
#define MAINTENANCE_IN        0xa3
#define MAINTENANCE_OUT       0xa4
#define MOVE_MEDIUM           0xa5
#define EXCHANGE_MEDIUM       0xa6
#define READ_12               0xa8
#define WRITE_12              0xaa
#define READ_MEDIA_SERIAL_NUMBER 0xab
#define WRITE_VERIFY_12       0xae
#define VERIFY_12	      0xaf
#define SEARCH_HIGH_12        0xb0
#define SEARCH_EQUAL_12       0xb1
#define SEARCH_LOW_12         0xb2
#define SECURITY_PROTOCOL_OUT 0xb5
#define READ_ELEMENT_STATUS   0xb8
#define SEND_VOLUME_TAG       0xb6
#define WRITE_LONG_2          0xea
#define EXTENDED_COPY         0x83
#define RECEIVE_COPY_RESULTS  0x84
#define ACCESS_CONTROL_IN     0x86
#define ACCESS_CONTROL_OUT    0x87
#define READ_16               0x88
#define COMPARE_AND_WRITE     0x89
#define WRITE_16              0x8a
#define READ_ATTRIBUTE        0x8c
#define WRITE_ATTRIBUTE	      0x8d
#define VERIFY_16	      0x8f
#define SYNCHRONIZE_CACHE_16  0x91
#define WRITE_SAME_16	      0x93
#define SERVICE_ACTION_IN     0x9e

/* values for service action in */
#define	SAI_READ_CAPACITY_16  0x10
#define SAI_GET_LBA_STATUS    0x12
#define SAI_REPORT_REFERRALS  0x13
/* values for VARIABLE_LENGTH_CMD service action codes
 * see spc4r17 Section D.3.5, table D.7 and D.8 */
#define VLC_SA_RECEIVE_CREDENTIAL 0x1800
/* values for maintenance in */
#define MI_REPORT_IDENTIFYING_INFORMATION 0x05
#define MI_REPORT_TARGET_PGS  0x0a
#define MI_REPORT_ALIASES     0x0b
#define MI_REPORT_SUPPORTED_OPERATION_CODES 0x0c
#define MI_REPORT_SUPPORTED_TASK_MANAGEMENT_FUNCTIONS 0x0d
#define MI_REPORT_PRIORITY   0x0e
#define MI_REPORT_TIMESTAMP  0x0f
#define MI_MANAGEMENT_PROTOCOL_IN 0x10
/* value for MI_REPORT_TARGET_PGS ext header */
#define MI_EXT_HDR_PARAM_FMT  0x20
/* values for maintenance out */
#define MO_SET_IDENTIFYING_INFORMATION 0x06
#define MO_SET_TARGET_PGS     0x0a
#define MO_CHANGE_ALIASES     0x0b
#define MO_SET_PRIORITY       0x0e
#define MO_SET_TIMESTAMP      0x0f
#define MO_MANAGEMENT_PROTOCOL_OUT 0x10
/* values for variable length command */
#define XDREAD_32	      0x03
#define XDWRITE_32	      0x04
#define XPWRITE_32	      0x06
#define XDWRITEREAD_32	      0x07
#define READ_32		      0x09
#define VERIFY_32	      0x0a
#define WRITE_32	      0x0b
#define WRITE_SAME_32	      0x0d

//=========================================================
//
//1 Inquiry
//
//=========================================================
#define PDT_CDROM						0x05; // 0x5h is CD-ROM device
#define REMOVABLE						0x80; //Bit7 is Removable Medium Bit
#define INQUIRY_VENDOR_LENGTH			8
#define INQUIRY_PRODUCT_LENGTH			16
#define INQUIRY_PRODUCT_VERSION_LENGTH	4

typedef struct inquiry_response {
	u8 Peripheral;
	u8 Removable;
	u8 Version;
	u8 RespDataFormat;
	u8 AdditionalLength;
	u8 Sccstp;
	u8 bqueetc;
	u8 CmdQue;
	u8 VendorID[INQUIRY_VENDOR_LENGTH];
	u8 ProductID[INQUIRY_PRODUCT_LENGTH];
	u8 ProductRev[INQUIRY_PRODUCT_VERSION_LENGTH];
}Inq_resp;


/* command block wrapper */
struct bulk_cb_wrap {
	u32     Signature;		/* contains 'USBC' */
	u32	    Tag;			/* unique per command id */
	le32	DataTransferLength;	/* size of data */
	u8	    Flags;			/* direction in bit 0 */
	u8	    Lun;			/* LUN normally 0 */
	u8	    Length;			/* of of the CDB */
	u8	    CDB[16];		/* max command */
};

#define US_BULK_CB_WRAP_LEN	31
#define US_BULK_CB_SIGN		0x43425355	/*spells out USBC */
#define US_BULK_FLAG_IN		(1 << 7)
#define US_BULK_FLAG_OUT	0

/* command status wrapper */
struct bulk_cs_wrap {
	le32	Signature;	/* should = 'USBS' */
	u32	    Tag;		/* same as original command */
	le32	Residue;	/* amount not transferred */
	u8	    Status;		/* see below */
};

#define US_BULK_CS_WRAP_LEN	13
#define US_BULK_CS_SIGN		0x53425355      /* spells out 'USBS' */
#define US_BULK_STAT_OK		0
#define US_BULK_STAT_FAIL	1
#define US_BULK_STAT_PHASE	2

/* bulk-only class specific requests */
#define US_BULK_RESET_REQUEST   0xff
#define US_BULK_GET_MAX_LUN     0xfe

/* Default size of buffer length. */
#define FSG_BUFLEN	((u32)16384)

/* Maximal number of LUNs supported in mass storage function */
#define FSG_MAX_LUNS	1

#define FSG_VENDOR_ID	0x0525	/* NetChip */
#define FSG_PRODUCT_ID	0xa4a5	/* Linux-USB File-backed Storage Gadget */


/*-------------------------------------------------------------------------*/

/* CBI Interrupt data structure */
struct interrupt_data {
	u8	bType;
	u8	bValue;
};

#define CBI_INTERRUPT_DATA_LEN		2

/* CBI Accept Device-Specific Command request */
#define USB_CBI_ADSC_REQUEST		0x00


/* Length of a SCSI Command Data Block */
#define MAX_COMMAND_SIZE	16

/* SCSI Sense Key/Additional Sense Code/ASC Qualifier values */
#define SS_NO_SENSE				0
#define SS_COMMUNICATION_FAILURE		0x040800
#define SS_INVALID_COMMAND			0x052000
#define SS_INVALID_FIELD_IN_CDB			0x052400
#define SS_LOGICAL_BLOCK_ADDRESS_OUT_OF_RANGE	0x052100
#define SS_LOGICAL_UNIT_NOT_SUPPORTED		0x052500
#define SS_MEDIUM_NOT_PRESENT			0x023a00
#define SS_MEDIUM_REMOVAL_PREVENTED		0x055302
#define SS_NOT_READY_TO_READY_TRANSITION	0x062800
#define SS_RESET_OCCURRED			0x062900
#define SS_SAVING_PARAMETERS_NOT_SUPPORTED	0x053900
#define SS_UNRECOVERED_READ_ERROR		0x031100
#define SS_WRITE_ERROR				0x030c02
#define SS_WRITE_PROTECTED			0x072700

#define SK(x)		((u8) ((x) >> 16))	/* Sense Key byte, etc. */
#define ASC(x)		((u8) ((x) >> 8))
#define ASCQ(x)		((u8) (x))

//cd rom size define
#define SCSI_BUFFER_SIZE	512
#define BLOCK_LENGTH		2048
#define BLOCK_BIT 		    11
//#define ISO_SIZE			0x28a800
#define DISK_SIZE			0x08000000
#define FLASH_START_ADDRESS	0x08011000
#define FLASH_END_ADDRESS_2M	0x08200000 //0x08080000~0x08900000 8.5M flash size
#define FLASH_END_ADDRESS_8M	0x08800000

enum fsg_state {
	/* This one isn't used anywhere */
	FSG_STATE_COMMAND_PHASE = -10,
	FSG_STATE_DATA_PHASE,
	FSG_STATE_STATUS_PHASE,

	FSG_STATE_IDLE = 0,
	FSG_STATE_ABORT_BULK_OUT,
	FSG_STATE_RESET,
	FSG_STATE_INTERFACE_CHANGE,
	FSG_STATE_CONFIG_CHANGE,
	FSG_STATE_DISCONNECT,
	FSG_STATE_EXIT,
	FSG_STATE_TERMINATED
};

enum data_direction {
	DATA_DIR_UNKNOWN = 0,
	DATA_DIR_FROM_HOST,
	DATA_DIR_TO_HOST,
	DATA_DIR_NONE
};


/*-------------------------------------------------------------------------*/


static  u32 get_unaligned_be24(u8 *buf)
{
    u32 i;
    i= (buf[0] << 16) |
       (buf[1] <<  8) |
       (buf[2] <<  0);
    return 0xffffff & i;
}

static  u32 get_unaligned_be32(u8 *buf)
{
    u32 i;
    i= (buf[0] << 24) |
       (buf[1] << 16) |
       (buf[2] <<  8) |
       (buf[3] <<  0);
    return i;
}

static void put_unaligned_be32(u32 source,u8 *buf)
{
    buf[0]=(u8)(source>>24);
    buf[1]=(u8)(source>>16);
    buf[2]=(u8)(source>>8);
    buf[3]=(u8)(source>>0);
}

static  u32 get_unaligned_be16(u8 *buf)
{
    u32 i;
    i= (buf[0] <<  8) |
       (buf[1] <<  0);
    return 0xffff & i;
}

static void put_unaligned_be16(u16 source,u8 *buf)
{
    buf[0]=(u8)(source>>8);
    buf[1]=(u8)(source>>0);
}

/*-------------------------------------------------------------------------*/


enum {
	FSG_STRING_INTERFACE
};


/* Data shared by all the FSG instances. */
struct fsg_common {

	volatile USOC_TX_BD AITXBD[USOC_TXBD_RING_SIZE];
	volatile USOC_RX_BD AIRXBD[USOC_RXBD_RING_SIZE];
	u32		file_length;
    long	num_sectors;

    u32		sense_data;
	u32		sense_data_info;
	u32		unit_attention_data;

	/* lock protects: state, all the req_busy's */
	//_lock		lock;

	int			cmnd_size;
	u8			cmnd[MAX_COMMAND_SIZE];

	unsigned int		nluns;
	unsigned int		lun;


	enum fsg_state		state;		/* For exception handling */
	unsigned int		exception_req_tag;

	enum data_direction	data_dir;
	u32			data_size;
	u32			data_size_from_cmnd;
	u32			tag;
	u32			residue;
	u32			usb_amount_left;

	unsigned int		phase_error:1;

};

#endif

