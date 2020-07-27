/*
	X-Modem Header File

	1999/09/03	sprite, support Xmode Tx & Rx
*/

#ifndef	_FLOADER_XMODE_H_
#define	_FLOADER_XMODE_H_

#include <basic_types.h>

/*****************
 * X-Modem status
 *****************/
#define	XMODEM_OK		1
#define	XMODEM_CANCEL	2
#define	XMODEM_ACK		3
#define	XMODEM_NAK		4
#define	XMODEM_COMPLETE	5
#define	XMODEM_NO_SESSION	6
#define	XMODEM_ABORT	7
#define	XMODEM_TIMEOUT	8

/****************************
 * flow control character
 ****************************/
#define	SOH			0x01		/* Start of header */
#define	STX			0x02		/* Start of header XModem-1K */
#define	EOT			0x04		/* End of transmission */
#define	ACK			0x06		/* Acknowledge */
#define	NAK			0x15		/* Not acknowledge */
#define	CAN			0x18		/* Cancel */
#define	ESC			0x1b		/* User Break */
#define	BAUDSET		0x05		/* Part of shakehands, added by Realtek*/
#define	BAUDCHK	0x07		/* Part of shakehands, added by Realtek*/
#define	XMERASE	0x17		/* Erase Flash, added by Realtek*/
#define	XMREAD		0x19		/* Read Flash, added by Realtek*/
#define	XMREADV2	0x20		/* Read Flash, added by Realtek*/
#define	RXSTATUS	0x21		/* Read Status Register, added by Realtek*/
#define	TXSTATUS	0x26		/* Write Status Register, added by Realtek, AZ ACUT ROM not support*/
#define	XM_CHECKSUM	0x27		/* check flash write checksum: AZ ACUT ROM not support */
#define	XM_TXREG	0x29		/* Write REG or RAM, AZ ACUT ROM not support */
#define	XM_RXREG	0x31		/* Read REG or RAM, AZ ACUT ROM not support */
//#define	XM_REDOWNLOAD 0x35


/****************************
 * Xmode paramters
 ****************************/
#define	FRAME_SIZE	132							/* X-modem structure */
#define	FRAME_SIZE_1K	1028						/* X-modem structure */
#define	XM_BUFFER_SIZE	1024						/* X-modem buffer */
#define	XMODEM_READ_MAXRETRANS		25

/***********************
 * frame structure
 ***********************/
typedef	struct
{
	 unsigned char	soh;
	 unsigned char	recordNo;
	 unsigned char	recordNoInverted;
	 unsigned char	buffer[XM_BUFFER_SIZE];
	 unsigned char	CRC;
} XMODEM_FRAME;

typedef struct _XMODEM_CTRL_ {
    u16 currentFrame; /* current frame number */
    u16 expected;
    s32 rFinish;
    u32 total_frame;
}XMODEM_CTRL, *PXMODEM_CTRL;

VOID xModemRxBuffer(u32 baud_rate);
u32 xModemGetFirst(XMODEM_CTRL *pXMCtrl);

/* wll define in section header later */
#define FWU_DATA_SECTION

#endif /* _FLOADER_XMODE_H_ */

