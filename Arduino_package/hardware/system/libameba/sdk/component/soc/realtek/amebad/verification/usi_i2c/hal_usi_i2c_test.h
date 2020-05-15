/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _HAL_USI_I2C_H_     //#ifndef _HAL_I2C_H_
#define _HAL_USI_I2C_H_

// USI I2C debug output
#define USI_I2C_PREFIX      "RTL8195A[usi_i2c]: "
#define USI_I2C_PREFIX_LVL  "    [usi_i2c_DBG]: "

//================= I2C CONFIGURATION START ==================
// I2C SAL User Configuration Flags

#ifdef CONFIG_DEBUG_LOG
#define CONFIG_DEBUG_LOG_I2C_HAL 1
#define DBG_I2C_LOG_PERD    100
#ifdef CONFIG_DEBUG_LOG_I2C_HAL
#define I2CDBGLVL   0xFF
#endif
#endif

//======================================================

//================= I2C HAL END   ===========================


//================= I2C SAL START  ==========================
//I2C SAL Macros

//======================================================
// I2C SAL related enumerations
// I2C Extend Features
typedef enum _USI_I2C_EXD_SUPPORT_{
	USI_I2C_EXD_USER_REG    =   0x40,           //BIT_6, Using User Register Address
	USI_I2C_EXD_MTR_ADDR_RTY=   0x100,          //BIT_8, Master retries to send start condition and Slave address when the slave doesn't ack 
	                                        //         the address.
	USI_I2C_EXD_MTR_ADDR_UPD=   0x200,         //BIT_9, Master dynamically updates slave address
	USI_I2C_EXD_MTR_HOLD_BUS=   0x400,         //BIT_10, Master doesn't generate STOP when the FIFO is empty. This would make Master hold
	                                        //         the bus.
}USI_I2C_EXD_SUPPORT,*PUSI_I2C_EXD_SUPPORT;

// I2C operation type
typedef enum _USI_I2C_OP_TYPE_ {
	USI_I2C_POLL_TYPE   =   0x0,
	USI_I2C_DMA_TYPE    =   0x1,
	USI_I2C_INTR_TYPE   =   0x2,
}USI_I2C_OP_TYPE, *PUSI_I2C_OP_TYPE;

// I2C device status 
typedef enum _USI_I2C_Device_STATUS_ {
	USI_I2C_STS_UNINITIAL   =   0x00,
	USI_I2C_STS_INITIALIZED =   0x01,
	USI_I2C_STS_IDLE        =   0x02,

	USI_I2C_STS_TX_READY    =   0x03,    
	USI_I2C_STS_TX_ING      =   0x04,

	USI_I2C_STS_RX_READY    =   0x05,
	USI_I2C_STS_RX_ING      =   0x06,

	USI_I2C_STS_ERROR       =   0x10,
	USI_I2C_STS_TIMEOUT     =   0x11, 
}USI_I2C_Device_STATUS, *PUSI_I2C_Device_STATUS;

// I2C command type 
typedef enum _USI_I2C_COMMAND_TYPE_ {
	USI_I2C_WRITE_CMD   =   0x0,
	USI_I2C_READ_CMD    =   0x1,
}USI_I2C_COMMAND_TYPE,*PUSI_I2C_COMMAND_TYPE;

// I2C STOP BIT 
typedef enum _USI_I2C_STOP_TYPE_ {
	USI_I2C_STOP_DIS    =   0x0,
	USI_I2C_STOP_EN     =   0x1,
}I2C_USI_STOP_TYPE, *PI2C_STOP_TYPE;

// I2C error type 
typedef enum _USI_I2C_ERR_TYPE_ {
	USI_I2C_ERR_RX_UNDER    =   0x01,           //I2C RX FIFO Underflow
	USI_I2C_ERR_RX_OVER     =   0x02,           //I2C RX FIFO Overflow
	USI_I2C_ERR_TX_OVER     =   0x04,           //I2C TX FIFO Overflow
	USI_I2C_ERR_TX_ABRT     =   0x08,           //I2C TX terminated
	USI_I2C_ERR_SLV_TX_NACK =   0x10,           //I2C slave transmission terminated by master NACK, 
	                                        //but there are data in slave TX FIFO
	USI_I2C_ERR_USER_REG_TO =   0x20,

	USI_I2C_ERR_RX_CMD_TO   =   0x21,
	USI_I2C_ERR_RX_FF_TO    =   0x22,
	USI_I2C_ERR_TX_CMD_TO   =   0x23,
	USI_I2C_ERR_TX_FF_TO    =   0x24,

	USI_I2C_ERR_TX_ADD_TO   =   0x25,
	USI_I2C_ERR_RX_ADD_TO   =   0x26,

	USI_I2C_ERR_TX_TO_NOT_SET   =   0x27,
	USI_I2C_ERR_RX_TO_NOT_SET   =   0x28,
}USI_I2C_ERR_TYPE, *PUSI_I2C_ERR_TYPE;

// I2C Time Out type
typedef enum _USI_I2C_TIMEOUT_TYPE_ {
	USI_I2C_TIMEOOUT_DISABLE    =   0x00,           
	USI_I2C_TIMEOOUT_ENDLESS    =   0xFFFFFFFF,
}USI_I2C_TIMEOUT_TYPE, *PUSI_I2C_TIMEOUT_TYPE;

//======================================================
// SAL I2C related data structures
// I2C user callback adapter
typedef struct _SAL_USI_I2C_USERCB_ADPT_ {
	VOID (*USERCB)      (VOID *Data);
	u32  USERData;
}SAL_USI_I2C_USERCB_ADPT, *PSAL_USI_I2C_USERCB_ADPT;

// I2C user callback structure
typedef struct _SAL_USI_I2C_USER_CB_ {
	SAL_USI_I2C_USERCB_ADPT    TXCCB;         //I2C Transmit Complete Callback
	SAL_USI_I2C_USERCB_ADPT    RXCCB;         //I2C Receive Complete Callback
	SAL_USI_I2C_USERCB_ADPT    RDREQCB;       //I2C Read Request Callback
	SAL_USI_I2C_USERCB_ADPT    ERRCB;         //I2C Error Callback
	SAL_USI_I2C_USERCB_ADPT    DMATXCCB;      //I2C DMA Transmit Complete Callback
	SAL_USI_I2C_USERCB_ADPT    DMARXCCB;      //I2C DMA Receive Complete Callback
	SAL_USI_I2C_USERCB_ADPT    GENCALLCB;     //I2C General Call Callback
}SAL_USI_I2C_USER_CB, *PSAL_USI_I2C_USER_CB;

// I2C Transmit Buffer
typedef struct _SAL_USI_I2C_TRANSFER_BUF_ {
	u16     DataLen;                        //I2C Transmfer Length
	u16     TargetAddr;                     //I2C Target Address. It's only valid in Master Mode.
	u32     RegAddr;                        //I2C Register Address. It's only valid in Master Mode.
	u32     RSVD;                           //
	u8      *pDataBuf;                      //I2C Transfer Buffer Pointer
}SAL_USI_I2C_TRANSFER_BUF,*PSAL_USI_I2C_TRANSFER_BUF;


//======================================================
// I2C SAL Function Prototypes



//================= I2C SAL MANAGEMENT START  =================
// I2C SAL management macros
#define SAL_USER_CB_NUM     (sizeof(SAL_USI_I2C_USER_CB) / sizeof(PSAL_USI_I2C_USERCB_ADPT))

//======================================================
//I2C SAL management adapter
typedef struct _SAL_USI_I2C_MNGT_ADPT_ {
	USI_I2C_InitTypeDef       	HalInitDat;                //Pointer to HAL I2C initial data( HAL_I2C_INIT_DAT )
	USI_TypeDef*            USIx;					   //Pointer to I2C Device
	IRQn_Type   IrqNum;
	SAL_USI_I2C_USER_CB        	UserCB;                    //Pointer to SAL user callbacks (SAL_I2C_USER_CB )
	volatile u32            		MstRDCmdCnt;                //Used for Master Read command count

	GDMA_InitTypeDef       	I2CTxGdmaInitStruct;              //Pointer to GDMA_InitTypeDef
	GDMA_InitTypeDef       	I2CRxGdmaInitStruct;              //Pointer to GDMA_InitTypeDef

	PSAL_USI_I2C_TRANSFER_BUF   pTXBuf;             //Pointer to I2C TX buffer
	PSAL_USI_I2C_TRANSFER_BUF   pRXBuf;             //Pointer to I2C RX buffer
	u32                     TimeOut;            //I2C IO Timeout count,  in ms
	u8                      PinMux;             //I2C pin mux seletion
	u8                      OpType;             //I2C operation type selection
	volatile u8             DevSts;             //I2C device status
	u32                     I2CExd;             //I2C extended options:
	                                            //bit 0: I2C RESTART supported,
	                                            //          0 for NOT supported,
	                                            //          1 for supported
	                                            //bit 1: I2C General Call supported
	                                            //          0 for NOT supported,
	                                            //          1 for supported
	                                            //bit 2: I2C START Byte supported
	                                            //          0 for NOT supported,
	                                            //          1 for supported
	                                            //bit 3: I2C Slave-No-Ack
	                                            //         supported
	                                            //          0 for NOT supported,
	                                            //          1 for supported
	                                            //bit 4: I2C bus loading,
	                                            //          0 for 100pf, 
	                                            //          1  for 400pf
	                                            //bit 5: I2C slave ack to General
	                                            //         Call
	                                            //bit 6: I2C User register address
	                                            //bit 7: I2C 2-Byte User register
	                                            //         address
	                                            //bit 8: I2C slave address no ack retry,
	                                            //        It's only for Master mode,
	                                            //        when slave doesn't ack the
	                                            //        address
	                                            //bit 31~bit 8: Reserved
} SAL_USI_I2C_MNGT_ADPT, *PSAL_USI_I2C_MNGT_ADPT;

//======================================================
//SAL I2C management function prototype
HAL_Status RtkUSII2CInit(IN  PSAL_USI_I2C_MNGT_ADPT  pSalUSII2CMngtAdpt);
HAL_Status RtkUSII2CDeInit(IN  PSAL_USI_I2C_MNGT_ADPT  pSalUSII2CMngtAdpt);
HAL_Status RtkUSII2CSend(IN  PSAL_USI_I2C_MNGT_ADPT  pSalUSII2CMngtAdpt);
HAL_Status RtkUSII2CReceive(IN  PSAL_USI_I2C_MNGT_ADPT  pSalUSII2CMngtAdpt);
//================= I2C SAL END   ===========================

//======================================================
extern VOID USII2CISRHandle(IN  VOID *Data);
extern VOID USII2CTXGDMAISRHandle(IN  VOID *Data);
extern VOID USII2CRXGDMAISRHandle(IN  VOID *Data);
extern HAL_Status USII2CIsTimeout (IN  u32 StartCount, IN  u32 TimeoutCnt);
//======================================================


//================= I2C SAL MANAGEMENT END   ==================
#endif                  //#ifndef _HAL_USI_I2C_H_

