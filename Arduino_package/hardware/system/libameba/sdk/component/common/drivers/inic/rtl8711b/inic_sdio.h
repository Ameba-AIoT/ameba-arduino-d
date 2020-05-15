/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _INIC_SDIO_H_
#define _INIC_SDIO_H_

#include "rtl8710b_sdio.h"
#include "osdep_service.h"
#include "rtl8710b_inic.h"
#include <FreeRTOS.h>
#include "timers.h"
#include "semphr.h"

#if defined(__IAR_SYSTEMS_ICC__) //for IAR SDK
#include "platform_opts.h"
#endif

#define PURE_SDIO_INIC           0  // is a pure SDIO iNIC device or a SDIO iNIC + peripheral device

#ifndef PRIORITIE_OFFSET //PRIORITIE_OFFSET in FreeRTOSConfig.h
#define PRIORITIE_OFFSET 0
#endif

#define SDIO_RX_PKT_SIZE_OVER_16K					0	/* is support SDIO RX packet size > 16K. if true, 
															a big packet will be transmited via multiple RX_BD */
#define SDIO_MAILBOX_SIZE                           10  // the maximum number of message block can be stored in this mailbox
#define SDIO_PERIODICAL_TIMER_INTERVAL              2000    // in ms, the interval of SDIO periodical timer
#define SDIO_AVG_TP_WIN_SIZE                        20      // the number of entry to log the byte count for every periodical timer statistic, to calculate throughput


//TX BD setting
#define SDIO_TX_BD_BUF_SIZE						1540    //1514+24	//(2048+32)    // the size of a TX BD pointed buffer, WLan header = 26 bytes
#define SDIO_TX_BD_NUM								20	// Number of TX BD

//RX BD setting
#define SDIO_RX_BD_BUF_SIZE						1540 //1514+24	//2048
#define SDIO_RX_BD_NUM								32	// Number of RX BD, to make 32K of bus aggregation, it needs 22 RX_BD at least
#define RX_BD_FREE_TH								5   // trigger the interrupt when free RX BD over this threshold
#define MAX_RX_BD_BUF_SIZE			                		16380	// the Maximum size for a RX_BD point to, make it 4-bytes aligned
#define SDIO_RX_PKT_NUM							128	// Number of RX packet handler
#define MIN_RX_BD_SEND_PKT						2	/* the minum needed RX_BD to send a Packet to Host, we need 2: one for RX_Desc, the other for payload */

#define SDIO_IRQ_PRIORITY							10
#define SDIO_TASK_PRIORITY							1	// it can be 0(lowest) ~ configMAX_PRIORITIES-1(highest)
#define SDIO_MP_TASK_PRIORITY                       		2	// it can be 0(lowest) ~ configMAX_PRIORITIES-1(highest)
//#if SDIO_TASK_PRIORITY > (configMAX_PRIORITIES - 1)
#if SDIO_TASK_PRIORITY > (4 - 1)
#error "SDIO Task Priority Should be 0~(configMAX_PRIORITIES-1)"
#endif

typedef struct {
	u32	Address;    /* The TX buffer physical address, it must be 4-bytes aligned */
}SDIO_TX_BD;

#define TX_BD_STRUCTURE_SIZE	(sizeof(SDIO_TX_BD))	

/* The RX Buffer Descriptor format */
typedef struct {
	u32 BuffSize:14;		/* bit[13:0], RX Buffer Size, Maximum 16384-1 */
	u32 LS:1;					/* bit[14], is the Last Segment ? */
	u32 FS:1;					/* bit[15], is the First Segment ? */
	u32 Seq:16;				/* bit[31:16], The sequence number, it's no use for now */
	u32 PhyAddr;			/* The RX buffer physical address, it must be 4-bytes aligned */
} SDIO_RX_BD;
#define RX_BD_STRUCTURE_SIZE	(sizeof(SDIO_RX_BD))	

// define the TX BD buffer size with unite of 64 byets
// Be carefull!! the setting of hardware's TX BD buffer size may exceed the real size of 
// the TX BD buffer size, and then it may cause the hardware DMA write the buffer overflow
#define SDIO_TX_BUF_SZ_UNIT          64
#define SDIO_TX_BD_BUF_USIZE        ((((SDIO_TX_BD_BUF_SIZE+sizeof(INIC_TX_DESC)-1)/SDIO_TX_BUF_SZ_UNIT)+1)&0xff)

typedef struct {
	INIC_TX_DESC TX_Desc;
	u8 TX_Buffer[SDIO_TX_BD_BUF_SIZE];
}SDIO_TX_BD_BUFFER;

typedef struct {
	INIC_RX_DESC RxDesc;	// The RX Descriptor for this packet, to be send to Host ahead this packet
#if CONFIG_INIC_EN
	struct sk_buff *skb;		//sdk->data will set to pData
#endif
	u8 *pData;				// point to the head of payload of this packet
	u16 Offset;         // the offset from the pData to the payload buffer
	_list list;			// the link list to chain packets
	u8 isDyna;          // is Dynamic allocated
} SDIO_RX_PACKET;

/* the data structer to bind a TX_BD with a TX Packet */
typedef struct {
#if CONFIG_INIC_EN
	struct sk_buff *skb;
#endif
	u8 isFree;		// is this TX BD free 
} SDIO_TX_BD_HANDLE;

/* the data structer to bind a RX_BD with a RX Packet */
typedef struct {
	SDIO_RX_BD *pRXBD;		// Point to the RX_BD buffer
	SDIO_RX_PACKET *pPkt;	// point to the Rx Packet
	u8 isPktEnd;		// For a packet over 1 BD , this flag to indicate is this BD contains a packet end
	u8 isFree;		// is this RX BD free (DMA done and its RX packet has been freed)
} SDIO_RX_BD_HANDLE;

typedef struct _HAL_SDIO_ADAPTER_ {
	SDIO_TX_BD_HANDLE*	pTXBDHdl;		/* point to the allocated memory for TX_BD Handle array */
	u16				TXBDWPtr;		    /* The SDIO TX(Host->Device) BD local write index, different with HW maintained write Index. */
	u16				TXBDRPtr;		    /* The SDIO TX(Host->Device) BD read index */
	u16				TXBDRPtrReg;		/* The SDIO TX(Host->Device) BD read index has been write to HW register */
	u8				TxOverFlow;

	SDIO_RX_BD_HANDLE*	pRXBDHdl;		/* point to the allocated memory for RX_BD Handle array */
	u16				RXBDWPtr;		    /* The SDIO RX(Device->Host) BD write index */
	u16				RXBDRPtr;		    /* The SDIO RX(Device->Host) BD local read index, different with HW maintained Read Index. */
	u32				Events;				/* The Event to the SDIO Task */

	u8              CCPWM;              /* the value write to register CCPWM, which will sync to Host HCPWM */
	u8              reserve1;
	u16             CCPWM2;             /* the value write to register CCPWM2, which will sync to Host HCPWM2 */
	u8              CRPWM;              /* sync from Host HRPWM */
	u8              reserve2;
	u16             CRPWM2;             /* sync from Host HRPWM2 */

	_sema		TxSema;             /* Semaphore for SDIO TX, use to wakeup the SDIO TX task */	
	_sema           RxSema;             /* Semaphore for SDIO RX, use to wakeup the SDIO RX task */
	_sema		IrqSema;             /* Semaphore for SDIO RX, use to wakeup the SDIO RX task */

	xTaskHandle	xSDIOTxTaskHandle;	/* The handle of the SDIO Task for TX, can be used to delte the task */
	xTaskHandle	xSDIORxTaskHandle;  /* The handle of the SDIO Task speical for RX, can be used to delte the task */
	xTaskHandle	xSDIOIrqTaskHandle;  /* The handle of the SDIO Task speical for RX, can be used to delte the task */

	s8		(*Tx_Callback)(VOID *pAdapter, u8 *pPkt, u16 Offset, u16 PktSize, u8 type);	/* to hook the WLan driver TX callback function to handle a Packet TX */
	VOID	*pTxCb_Adapter;		/* a pointer will be used to call the TX Callback function, 
				which is from the TX CallBack function register */
	s8		(*pTxCallback_Backup)(VOID *pAdapter, u8 *pPkt, u16 Offset, u16 PktSize, u8 type);	// Use to back up the registered TX Callback function, for MP/Normal mode switch
	VOID	*pTxCb_Adapter_Backup;	// Backup the pTxCb_Adapter, for MP/Normal mode switch
	_list	RxPktList;			/* The list to queue RX packets */
	_list	FreeRxPktList;		/* The list to queue free Rx packets handler */
	_mutex	RxMutex;			/* The Mutex to protect RxPktList */
	u32	RxInQCnt;			/* The packet count for Rx In Queue */

} HAL_SDIO_ADAPTER, *PHAL_SDIO_ADAPTER;

// function not extern
VOID SDIO_TxTask(IN VOID *pData);
VOID SDIO_RxTask(IN VOID *pData);
VOID SDIO_IRQ_Handler_BH(VOID *pData);
u8 SDIO_Process_RPWM(	IN PHAL_SDIO_ADAPTER pSDIODev);
u8 SDIO_Process_RPWM2(IN PHAL_SDIO_ADAPTER pSDIODev);
VOID SDIO_Reset_Cmd(IN PHAL_SDIO_ADAPTER pSDIODev);
VOID SDIO_Return_Rx_Data(IN PHAL_SDIO_ADAPTER pSDIODev);
VOID SDIO_Process_H2C_IOMsg(IN PHAL_SDIO_ADAPTER pSDIODev);
VOID SDIO_TX_FIFO_DataReady(IN PHAL_SDIO_ADAPTER pSDIODev);
VOID SDIO_Recycle_Rx_BD (IN PHAL_SDIO_ADAPTER pSDIODev);
VOID SDIO_IRQ_Handler(VOID *pData);

//function extern
extern BOOL SDIO_Device_Init(VOID);
extern VOID SDIO_Device_DeInit(VOID);
extern VOID SDIO_Send_C2H_IOMsg(IN PHAL_SDIO_ADAPTER pSDIODev, IN u32 *C2HMsg);
extern u8 SDIO_Send_C2H_PktMsg(IN PHAL_SDIO_ADAPTER pSDIODev, IN u8 *C2HMsg, IN u16 MsgLen);
extern VOID SDIO_Register_Tx_Callback(
	IN PHAL_SDIO_ADAPTER pSDIODev,
	IN s8 (*Tx_Callback)(VOID *pAdapter, u8 *pPkt, u16 Offset, u16 PktSize, u8 Type),
	IN VOID *pAdapter	
);
extern s8 SDIO_Rx_Callback(
	IN PHAL_SDIO_ADAPTER pSDIODev,
	IN VOID *pData,
	IN u16 Offset,
	IN u16 Length,
	IN u8 CmdType
);
extern void SDIO_INTClear(void);

extern PHAL_SDIO_ADAPTER pgSDIODev;
#define HalSdioInit		SDIO_Device_Init
#define HalSdioDeInit		SDIO_Device_DeInit
#endif	// #ifndef _INIC_SDIO_H_
