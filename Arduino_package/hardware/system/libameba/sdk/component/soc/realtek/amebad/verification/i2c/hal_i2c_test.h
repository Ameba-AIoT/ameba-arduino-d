/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _HAL_I2C_H_     //#ifndef _HAL_I2C_H_
#define _HAL_I2C_H_

#include "ameba_soc.h"


//================= I2C CONFIGURATION START ==================
// I2C SAL User Configuration Flags

// I2C SAL operation types
#define I2C_POLL_OP_TYPE            1
#define I2C_INTR_OP_TYPE            1
#define I2C_DMA_OP_TYPE             1
                                 
// I2C SAL used module. Please set the I2C module flag to 1 to enable the related 
// I2C module functions.
#define I2C0_USED                   1
#define I2C1_USED                   1
#define I2C2_USED                   1
#define I2C3_USED                   1
//================= I2C CONFIGURATION END  ===================


//================= I2C HAL START  ==========================
// I2C debug output
#define I2C_PREFIX      "RTL8195A[i2c]: "
#define I2C_PREFIX_LVL  "    [i2c_DBG]: "

typedef enum _I2C_DBG_LVL_ {
    HAL_I2C_LVL         =   0x01,
    SAL_I2C_LVL         =   0x02,
    VERI_I2C_LVL        =   0x03,
}I2C_DBG_LVL,*PI2C_DBG_LVL;

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
typedef enum _I2C_EXD_SUPPORT_{
    I2C_EXD_USER_REG    =   0x40,           //BIT_6, Using User Register Address
    I2C_EXD_MTR_ADDR_RTY=   0x100,          //BIT_8, Master retries to send start condition and Slave address when the slave doesn't ack 
                                            //         the address.
    I2C_EXD_MTR_ADDR_UPD=   0x200,         //BIT_9, Master dynamically updates slave address
    I2C_EXD_MTR_HOLD_BUS=   0x400,         //BIT_10, Master doesn't generate STOP when the FIFO is empty. This would make Master hold
                                            //         the bus.
}I2C_EXD_SUPPORT,*PI2C_EXD_SUPPORT;

// I2C operation type
typedef enum _I2C_OP_TYPE_ {
    I2C_POLL_TYPE   =   0x0,
    I2C_DMA_TYPE    =   0x1,
    I2C_INTR_TYPE   =   0x2,
}I2C_OP_TYPE, *PI2C_OP_TYPE;

// I2C device status 
typedef enum _I2C_Device_STATUS_ {
    I2C_STS_UNINITIAL   =   0x00,
    I2C_STS_INITIALIZED =   0x01,
    I2C_STS_IDLE        =   0x02,
    
    I2C_STS_TX_READY    =   0x03,    
    I2C_STS_TX_ING      =   0x04,
    
    I2C_STS_RX_READY    =   0x05,
    I2C_STS_RX_ING      =   0x06,

    I2C_STS_ERROR       =   0x10,
    I2C_STS_TIMEOUT     =   0x11, 
}I2C_Device_STATUS, *PI2C_Device_STATUS;

//I2C DMA module number
typedef enum _I2C_DMA_MODULE_SEL_ {
    I2C_DMA_MODULE_0    =   0x0,
    I2C_DMA_MODULE_1    =   0x1
}I2C_DMA_MODULE_SEL, *PI2C_DMA_MODULE_SEL;

// I2C0 DMA peripheral number
typedef enum _I2C0_DMA_PERI_NUM_ {
    I2C0_DMA_TX_NUM =   0x8,
    I2C0_DMA_RX_NUM =   0x9,
}I2C0_DMA_PERI_NUM,*PI2C0_DMA_PERI_NUM;

// I2C1 DMA peripheral number
typedef enum _I2C1_DMA_PERI_NUM_ {
    I2C1_DMA_TX_NUM =   0xA,
    I2C1_DMA_RX_NUM =   0xB,
}I2C1_DMA_PERI_NUM,*PI2C1_DMA_PERI_NUM;

// I2C0 DMA module used
typedef enum _I2C0_DMA_MODULE_ {
    I2C0_DMA0   =   0x0,
    I2C0_DMA1   =   0x1,
}I2C0_DMA_MODULE,*PI2C0_DMA_MODULE;

// I2C0 DMA module used
typedef enum _I2C1_DMA_MODULE_ {
    I2C1_DMA0   =   0x0,
    I2C1_DMA1   =   0x1,
}I2C1_DMA_MODULE,*PI2C1_DMA_MODULE;

// I2C command type 
typedef enum _I2C_COMMAND_TYPE_ {
    I2C_WRITE_CMD   =   0x0,
    I2C_READ_CMD    =   0x1,
}I2C_COMMAND_TYPE,*PI2C_COMMAND_TYPE;

// I2C STOP BIT 
typedef enum _I2C_STOP_TYPE_ {
    I2C_STOP_DIS    =   0x0,
    I2C_STOP_EN     =   0x1,
}I2C_STOP_TYPE, *PI2C_STOP_TYPE;

// I2C error type 
typedef enum _I2C_ERR_TYPE_ {
    I2C_ERR_RX_UNDER    =   0x01,           //I2C RX FIFO Underflow
    I2C_ERR_RX_OVER     =   0x02,           //I2C RX FIFO Overflow
    I2C_ERR_TX_OVER     =   0x04,           //I2C TX FIFO Overflow
    I2C_ERR_TX_ABRT     =   0x08,           //I2C TX terminated
    I2C_ERR_SLV_TX_NACK =   0x10,           //I2C slave transmission terminated by master NACK, 
                                            //but there are data in slave TX FIFO
    I2C_ERR_USER_REG_TO =   0x20,

    I2C_ERR_RX_CMD_TO   =   0x21,
    I2C_ERR_RX_FF_TO    =   0x22,
    I2C_ERR_TX_CMD_TO   =   0x23,
    I2C_ERR_TX_FF_TO    =   0x24,

    I2C_ERR_TX_ADD_TO   =   0x25,
    I2C_ERR_RX_ADD_TO   =   0x26,

    I2C_ERR_TX_TO_NOT_SET   =   0x27,
    I2C_ERR_RX_TO_NOT_SET   =   0x28,
}I2C_ERR_TYPE, *PI2C_ERR_TYPE;

// I2C Time Out type
typedef enum _I2C_TIMEOUT_TYPE_ {
    I2C_TIMEOOUT_DISABLE    =   0x00,           
    I2C_TIMEOOUT_ENDLESS    =   0xFFFFFFFF,
}I2C_TIMEOUT_TYPE, *PI2C_TIMEOUT_TYPE;

//======================================================
// SAL I2C related data structures
// I2C user callback adapter
typedef struct _SAL_I2C_USERCB_ADPT_ {
    VOID (*USERCB)      (VOID *Data);
    u32  USERData;
}SAL_I2C_USERCB_ADPT, *PSAL_I2C_USERCB_ADPT;

// I2C user callback structure
typedef struct _SAL_I2C_USER_CB_ {
    SAL_I2C_USERCB_ADPT    TXCCB;         //I2C Transmit Complete Callback
    SAL_I2C_USERCB_ADPT    RXCCB;         //I2C Receive Complete Callback
    SAL_I2C_USERCB_ADPT    RDREQCB;       //I2C Read Request Callback
    SAL_I2C_USERCB_ADPT    ERRCB;         //I2C Error Callback
    SAL_I2C_USERCB_ADPT    DMATXCCB;      //I2C DMA Transmit Complete Callback
    SAL_I2C_USERCB_ADPT    DMARXCCB;      //I2C DMA Receive Complete Callback
    SAL_I2C_USERCB_ADPT    GENCALLCB;     //I2C General Call Callback
}SAL_I2C_USER_CB, *PSAL_I2C_USER_CB;

// I2C Transmit Buffer
typedef struct _SAL_I2C_TRANSFER_BUF_ {
    u16     DataLen;                        //I2C Transmfer Length
    u16     TargetAddr;                     //I2C Target Address. It's only valid in Master Mode.
    u32     RegAddr;                        //I2C Register Address. It's only valid in Master Mode.
    u32     RSVD;                           //
    u8      *pDataBuf;                      //I2C Transfer Buffer Pointer
}SAL_I2C_TRANSFER_BUF,*PSAL_I2C_TRANSFER_BUF;


//======================================================
// I2C SAL Function Prototypes



//================= I2C SAL MANAGEMENT START  =================
// I2C SAL management macros
#define SAL_USER_CB_NUM     (sizeof(SAL_I2C_USER_CB) / sizeof(PSAL_I2C_USERCB_ADPT))

//======================================================
//I2C SAL management adapter
typedef struct _SAL_I2C_MNGT_ADPT_ {
	I2C_InitTypeDef       	HalInitDat;                //Pointer to HAL I2C initial data( HAL_I2C_INIT_DAT )
	I2C_TypeDef*            I2Cx;					   //Pointer to I2C Device
	IRQn_Type   IrqNum;
	SAL_I2C_USER_CB        	UserCB;                    //Pointer to SAL user callbacks (SAL_I2C_USER_CB )
	volatile u32            		MstRDCmdCnt;                //Used for Master Read command count

	GDMA_InitTypeDef       	I2CTxGdmaInitStruct;              //Pointer to GDMA_InitTypeDef
	GDMA_InitTypeDef       	I2CRxGdmaInitStruct;              //Pointer to GDMA_InitTypeDef

	PSAL_I2C_TRANSFER_BUF   pTXBuf;             //Pointer to I2C TX buffer
	PSAL_I2C_TRANSFER_BUF   pRXBuf;             //Pointer to I2C RX buffer
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
} SAL_I2C_MNGT_ADPT, *PSAL_I2C_MNGT_ADPT;

//======================================================
//SAL I2C management function prototype
HAL_Status RtkI2CInit(IN  PSAL_I2C_MNGT_ADPT  pSalI2CMngtAdpt);
HAL_Status RtkI2CDeInit(IN  PSAL_I2C_MNGT_ADPT  pSalI2CMngtAdpt);
HAL_Status RtkI2CSend(IN  PSAL_I2C_MNGT_ADPT  pSalI2CMngtAdpt);
HAL_Status RtkI2CReceive(IN  PSAL_I2C_MNGT_ADPT  pSalI2CMngtAdpt);
//================= I2C SAL END   ===========================

//======================================================
extern VOID I2CISRHandle(IN  VOID *Data);
extern VOID I2CTXGDMAISRHandle(IN  VOID *Data);
extern VOID I2CRXGDMAISRHandle(IN  VOID *Data);
extern HAL_Status I2CIsTimeout (IN  u32 StartCount, IN  u32 TimeoutCnt);
//======================================================


//================= I2C SAL MANAGEMENT END   ==================
#endif                  //#ifndef _HAL_I2C_H_
