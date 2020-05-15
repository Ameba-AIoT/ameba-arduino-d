/**
  ******************************************************************************
  * @file    rtl8721d_can.h
  * @author
  * @version V1.0.0
  * @date    2018-07-05
  * @brief   This file contains all the functions prototypes for the CAN firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
  ****************************************************************************** 
  */

#ifndef _RTL8721D_CAN_H_
#define _RTL8721D_CAN_H_

/** @addtogroup AmebaD_Periph_Driver
  * @{
  */

/** @defgroup CAN
  * @{
  */


/*===================================================================================================*/
/*CAN IP register*/
typedef struct {
	__IO u32	CAN_CTL;				/*!< CAN control register,	Address offset: 0x00*/

	__IO u32	CAN_STS;				/*!< CAN status register,	Address offset: 0x04*/

	__IO u32	CAN_FIFO_STS;			/*!< CAN protocol bit timing register,	Address offset: 0x08*/

	__IO u32	CAN_BIT_TIMING;		/*!< CAN bit timing register,	Address offset: 0x0C*/

	__IO u32	CAN_FD_BIT_TIMING;	/*!< CAN bit timing register,	Address offset: 0x10*/

	__IO u32	CAN_FD_SSP_CAL;		/*!< CAN bit timing register,	Address offset: 0x14*/

	__IO u32	CAN_INT_EN;			/*!< CAN interrupt flag register,	Address offset: 0x18*/

	__IO u32	CAN_MB_RXINT_EN;		/*!< CAN message buffer rx interrupt register,	Address offset: 0x1c*/

	__IO u32	CAN_MB_TXINT_EN;		/*!< CAN message buffer tx interrupt register,	Address offset: 0x20*/

	__IO u32	CAN_INT_FLAG;			/*!< CAN interrupt status register,	Address offset: 0x24*/

	__IO u32	CAN_ERR_FLAG;		/*!< CAN error status register,	Address offset: 0x28*/

	__IO u32	CAN_ERR_CNT_CTL;		/*!< CAN error counter control register,	Address offset: 0x2c*/

	__IO u32	CAN_ERR_CNT_STS;		/*!< CAN error counter status register,	Address offset: 0x30*/

	__IO u32	CAN_TX_ERROR_FLAG;	/*!< CAN tx error flag register,	Address offset: 0x34*/

	__IO u32	CAN_TX_FLAG;			/*!< CAN tx error flag register,	Address offset: 0x38*/

	__IO u32	CAN_RX_FLAG;			/*!< CAN tx error flag register,	Address offset: 0x3c*/	
	
	__IO u32	CAN_TIME_STAMP;		/*!< CAN time stamp register,	Address offset: 0x40*/

	__IO u32	CAN_MB_TRIGGER;		/*!< CAN time stamp register,	Address offset: 0x44*/	

	__IO u32	CAN_RXDMA_BA;		/*!< CAN time stamp register,	Address offset: 0x48*/

	__IO u32 CAN_RX_DMA_DATA;		/*!< CAN time stamp register,	Address offset: 0x4c*/

	__IO u32	CAN_WAKEPIN_FLT;		/*!< CAN wakeup pin filter setting register,	Address offset: 0x50*/

	__IO u32	CAN_TEST;				/*!< CAN test register,	Address offset: 0x54*/

	__IO u32	RSVD0[42];				/*!< CAN reserved register,	Address offset: 0x58~0x9c*/

	__IO u32 CAN_MB_STS[16];		/*!< CAN message buffer status,	Address offset: 0x100~0x13c*/

	__IO u32 RSVD1[48];				/*!< CAN reserved register,	Address offset: 0x140~0x1fc*/

	__IO u32 CAN_MB_CTRL[16];		/*!< CAN message buffer control register,	Address offset: 0x200~0x23c*/

	__IO u32 RSVD2[44];				/*!< CAN reserved register,	Address offset: 0x240~0x2ec*/

	__IO u32 CAN_MB_BA_END;		/*!< CAN message buffer base end register,	Address offset: 0x2f0*/

	__IO u32 RSVD3[3];				/*!< CAN message buffer base end register,	Address offset: 0x2f4~0x2fc*/	

	__IO u32 CAN_RAM_DATA[16];		/*!< CAN message buffer base end register,	Address offset: 0x300~0x33c*/	

	__IO u32	CAN_RAM_ARB;			/*!< CAN RAM arbitration register,	Address offset: 0X340*/

	__IO u32	CAN_RAM_MASK;		/*!< CAN RAM MASK register,	Address offset: 0x344*/

	__IO u32	CAN_RAM_CS;			/*!< CAN ram control/status register,	Address offset: 0x348*/
	
	__IO u32	CAN_RAM_CMD;			/*!< CAN ram command register,	Address offset: 0x34c*/

	__IO u32 RSVD4[43];				/*!< CAN reserved register,	Address offset: 0x450~0x4f8*/	

	__IO u32	CAN_DUMMY;				/*!< CAN dummy register,	Address offset: 0x4fc*/

	//__IO u32	CAN_RAM_DATA_H;

	//__IO u32	CAN_RAM_DATA_L;	
}RCAN_TypeDef;


#define RCAN_REG_BASE 				KEYSCAN_REG_BASE  //(SDIO_DEVICE_REG_BASE) 	//(0x4002C000)

#define RCAN				((RCAN_TypeDef *) RCAN_REG_BASE)

#define APB_CLK_RCAN	(20000000)

#define RCAN_MESSAGE_BUFFER_LEN	(5)

#define RCAN_MESSAGE_BUFFER_SIZE	(16)

#define RCAN_MESSAGE_FIFO_SIZE		(4)

#define RCAN_RX_FIFO_READ_MSG_IDX	(12)
/*===================================================================================================*/



/* Exported Types --------------------------------------------------------*/

/** @defgroup RCAN_Exported_Types RCAN Exported Types
  * @{
  */

/** 
  * @brief  RCAN Mode Initialization structure definition
  */
typedef struct
{
	u32	CAN_TimStampEn; 		/*!< Specifies the CAN stamp timer enable or disable.
                                      				This parameter can be ENABLE or DISABLE. */
														
	u32	CAN_TimStampDiv;		/*!< Specifies the CAN stamp timer divider.
                                      					This parameter can be a number between 0x01 and 0xff. */

	u32	CAN_AutoReTxEn;        		/*!< Specifies the message auto re-transmission configuration.
                                      				This parameter can be a ENABLE or DISABLE. */

	u32	CAN_TriSampleEn;			/*!< Specifies the tripple sampling configuration.
                                      				This parameter can be ENABLE or DISABLE. */

	u32	CAN_WorkMode;        		/*!< Specifies the CAN work mode.
                                      				This parameter can be of @ref RCAN_WORK_MODE_define. */

	u32	CAN_FDCrcMode;        		/*!< Specifies the CAN FD CRC mode.
                                      				This parameter can be of @ref RCAN_FD_CRC_MODE_define. */

	u32	CAN_ErrCntThreshold;        	/*!< Specifies the message length.
                                      				This parameter can be a number between 0x000 and 0x1ff*/

	u32	CAN_RxFifoEn;      			/*!< Specifies the CAN FIFO mode configure register.
                                      				This parameter can be ENABLE or DISABLE. */

	u32	CAN_BitPrescaler;			/*!< Specifies the bit timing prescaler.
                                      				This parameter can be a number between 0x1 and 0x100*/

	u32	CAN_SJW;				/*!< Specifies the CAN bit timing SJW.
                                      				This parameter can be a number between 0x1 and 0x8. */

	u32	CAN_TSEG1;				/*!< Specifies the CAN TSEG1.
                                      				This parameter can be a number between 0x1 and 0x10. */

	u32	CAN_TSEG2;				/*!< Specifies the CAN TSEG2.
                                      				This parameter can be a number between 0x1 and 0x10. */

	u32	CAN_FdBitPrescaler;			/*!< Specifies the CAN fd protocol bit timing prescaler.
                                      				This parameter can be a number between 0x1 and 0x100*/

	u32	CAN_FdSJW;				/*!< Specifies the CAN fd protocol bit timing SJW.
                                      				This parameter can be a number between 0x1 and 0x8. */

	u32	CAN_FdTSEG1;				/*!< Specifies the CAN fd protocol TSEG1.
                                      				This parameter can be a number between 0x1 and 0x10. */

	u32	CAN_FdTSEG2;				/*!< Specifies the CAN fd protocol TSEG2.
                                      				This parameter can be a number between 0x1 and 0x10. */

	u32 CAN_FdCtrl;				/*!< Specifies the CAN fd Protocol ENABLE or DISABLE.
                                      				This parameter can be ENABLE or DISABLE. */
														
	u32	CAN_FdSspAuto;			/*!< Specifies the automatic calculat ssp delay.*/

	u32	CAN_FdSspDco;				/*!< Specifies the delay compensation offset.
                                      				This parameter can be a number between 0x0 and 0xff. */

	u32	CAN_FdSspMin;			/*!< Specifies the minimum of ssp from start of one bit, only functional when can_fd_ssp_auto set.
                                      				 This parameter can be a number between 0x0 and 0xff.*/

	u32 CAN_FdSsp;				/*!< Specifies the ssp position, delay from start of one bit, if automatic calculation is enabled, the field will update after calculation.
                                      				This parameter can be a number between 0x0 and 0xff. */
} RCAN_InitTypeDef;

/**
  * @}
  */

/** 
  * @brief  RCAN DMA mode ram buffer structure definition
  */
typedef struct
{
  	__attribute__ ((aligned (4)))
	u32 CAN_RAM_DATA[16];		/*!< CAN message buffer base end,	Address offset mapping: 0x300~0x33c*/	

	u32	CAN_RAM_ARB;			/*!< CAN RAM arbitration,	Address offset mapping : 0X340*/

	u32	CAN_RAM_MASK;		/*!< CAN RAM MASK register,	Address offset mapping: 0x344*/

	u32	CAN_RAM_CS;			/*!< CAN ram control/status ,	Address offset mapping: 0x348*/
	
	u32	CAN_RAM_CMD;			/*!< CAN ram command ,	Address offset mapping: 0x34c*/
} RCAN_DmaBufferTypeDef;

/**
  * @}
  */

/** 
  * @brief  CAN Tx message descriptor structure definition  
  */
typedef struct
{

  u32 ProtocolType;	/*!< Specifies the protocol type of frame for the message that will 
                        	be transmitted. This parameter can be a value of 
                        	@ref RCAN_PROTOCOL_TYPE_define */

  u32 StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF. */

  u32 ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF. */

  u32 MsgBufferIdx;     /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0xf. */

  u32 IDE;     /*!< Specifies the type of identifier for the message that 
                        will be transmitted. This parameter can be a value 
                        of @ref RCAN_ID_TYPE_define */

  u32 RTR;     /*!< Specifies the type of frame for the message that will 
                        be transmitted. This parameter can be a value of 
                        @ref RCAN_FRAME_TYPE_define */

  u32 DLC;     /*!< Specifies the length of the frame that will be 
                        transmitted. This parameter can be a value between 
                        0 to 8 */

  u32 ESI;     /*!< Specifies the FD frame esi bit, indicate error passive of message sending node, 
  		read bit, should always write 0, hardware automatic fill*/

  u32 BRS;     /*!< Specifies the FD frame brs bit, indicate switch bit timing*/

  u32 EDL;     /*!< Specifies the FD frame edl bit, indicate it's a FD frame*/
					
  __attribute__ ((aligned (4)))
  u8 Data[64]; /*!< Contains the data to be transmitted. It ranges from 0 
                        to 0xFF. */

  u32 IDE_MASK;     /*!< Specifies the IDE MASK, ####*/

  u32 ID_MASK;     /*!< Specifies the ID MASK, ####*/

  u32 RTR_Mask;     /*!< Specifies the RTR MASK, ####*/
} RCAN_TxMsgTypeDef;

/**
  * @}
  */

/** 
  * @brief  CAN Rx message descriptor structure definition  
  */
typedef struct
{
  u32 ProtocolType;	/*!< Specifies the protocol type of frame for the message that will 
                        	be transmitted. This parameter can be a value of 
                        	@ref RCAN_PROTOCOL_TYPE_define */

  u32 StdId;  /*!< Specifies the standard identifier.
                        This parameter can be a value between 0 to 0x7FF, ####*/

  u32 ExtId;  /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0x1FFFFFFF, #### */

  u32 MsgBufferIdx;     /*!< Specifies the extended identifier.
                        This parameter can be a value between 0 to 0xf, ####*/

  u32 IDE;     /*!< Specifies the type of identifier for the message that 
                        will be transmitted. This parameter can be a value 
                        of @ref RCAN_ID_TYPE_define , ####*/

  u32 RTR;     /*!< Specifies the type of frame for the message that will 
                        be transmitted. This parameter can be a value of 
                        @ref RCAN_FRAME_TYPE_define, ####*/

  u32 DLC;     /*!< Specifies the length of the frame that will be 
                        transmitted. This parameter can be a value between 
                        0 to 8*/
  u32 ESI;     /*!< Specifies the FD frame esi bit, indicate error passive of message sending node, 
  		read bit, should always write 0, hardware automatic fill*/

  u32 BRS;     /*!< Specifies the FD frame brs bit, indicate switch bit timing*/

  u32 EDL;     /*!< Specifies the FD frame edl bit, indicate it's a FD frame*/
		
__attribute__ ((aligned (4)))
  u8 Data[64]; /*!< Contains the data to be transmitted. It ranges from 0 
                        to 0xFF. */

  u32 ID_MASK;     /*!< Specifies the ID MASK*/

  u32 RTR_Mask;     /*!< Specifies the RTR MASK, refer to RCAN_RTR_MASK_TYPE_define*/

  u32 IDE_Mask;     /*!< Specifies the IDE MASK, refer to RCAN_IDE_MASK_TYPE_define*/

  u32 RxTimStamp;      /*!< Specifies the rx time stamp */

  u32 RxLost;		     /*!< Specifies the rx lost times */
} RCAN_RxMsgTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup RCAN_Exported_Constants RCAN Exported Constants
  * @{
  */

/** @defgroup RCAN_WORK_MODE_define
  * @{
  */
#define RCAN_NORMAL_MODE					((u32)0x00000000)
#define RCAN_SILENCE_MODE					((u32)0x00000001)
#define RCAN_LOOPBACK_MODE					((u32)0x00000002)
#define IS_RCAN_WORK_MODE(MODE) (((MODE) == RCAN_NORMAL_MODE) || \
									((MODE) == RCAN_SILENCE_MODE) || \
									((MODE) == RCAN_LOOPBACK_MODE))

/**
  * @}
  */

/** @defgroup RCAN_PROTOCOL_TYPE_define
  * @{
  */
#define RCAN_CAN20_PROTOCOL_FRAME			((u32)0x00000000)
#define RCAN_FD_PROTOCOL_FRAME				((u32)0x00000001)
#define IS_RCAN_PROTOCOL_TYPE(TYPE) (((TYPE) == RCAN_CAN20_PROTOCOL_FRAME) || \
									((TYPE) == RCAN_FD_PROTOCOL_FRAME))

/**
  * @}
  */

/** @defgroup RCAN_ID_TYPE_define
  * @{
  */
#define RCAN_STANDARD_FRAME				((u32)0x00000000)
#define RCAN_EXTEND_FRAME					((u32)0x20000000)
#define IS_RCAN_ID_TYPE(TYPE) (((TYPE) == RCAN_STANDARD_FRAME) || \
									((TYPE) == RCAN_EXTEND_FRAME))

/**
  * @}
  */

/** @defgroup RCAN_FD_CRC_MODE_define
  * @{
  */
#define RCAN_ISO_CRC						((u32)0x00000000)
#define RCAN_NON_ISO_CRC					((u32)0x00000100)
#define IS_RCAN_CRC_TYPE(TYPE) (((TYPE) == RCAN_ISO_CRC) || \
									((TYPE) == RCAN_NON_ISO_CRC))

/**
  * @}
  */

/** @defgroup RCAN_FD_BIT_TIMING_SWITCH_TYPE_define
  * @{
  */
#define RCAN_FD_SWITCH_BIT_TIMING			((u32)0x00000200)
#define RCAN_FD_NOT_SWITCH_BIT_TIMING		((u32)0x00000000)
#define IS_RCAN_FD_BIT_RATE_SWITCH_TYPE(TYPE) (((TYPE) == RCAN_FD_NOT_SWITCH_BIT_TIMING) || \
									((TYPE) == RCAN_FD_SWITCH_BIT_TIMING))

/**
  * @}
  */

/** @defgroup RCAN_FD_ER_STS_INDICATOR_TYPE_define
  * @{
  */
#define RCAN_FD_ERROR_ACTIVE_NODE		((u32)0x00000000)
#define RCAN_FD_ERROR_PASSIVE_NODE		((u32)0x00000400)
#define IS_RCAN_FD_ER_NODE_TYPE(TYPE) (((TYPE) == RCAN_FD_ERROR_ACTIVE_NODE) || \
									((TYPE) == RCAN_FD_ERROR_PASSIVE_NODE))
/**
  * @}
  */

/** @defgroup RCAN_PROTOCOL_FRAME_TYPE_define
  * @{
  */
#define RCAN_FD_FRAME_TYPE					((u32)0x00000100)
#define RCAN_NOT_FD_FRAME_TYPE				((u32)0x00000000)
#define IS_RCAN_PRO_FRAEME_TYPE(TYPE) (((TYPE) == RCAN_FD_FRAME_TYPE) || \
									((TYPE) == RCAN_NOT_FD_FRAME_TYPE))

/**
  * @}
  */

/** @defgroup RCAN_FRAME_TYPE_define
  * @{
  */
#define RCAN_REMOTE_FRAME				((u32)0x40000000)
#define RCAN_DATA_FRAME					((u32)0x00000000)
#define IS_RCAN_FRAME_TYPE(TYPE) (((TYPE) == RCAN_REMOTE_FRAME) || \
									((TYPE) == RCAN_DATA_FRAME))

/**
  * @}
  */

/** @defgroup RCAN_RTR_MASK_TYPE_define
  * @{
  */
#define RCAN_RTR_BIT_MASK				((u32)0x40000000)
#define RCAN_RTR_NIT_NOT_MASK			((u32)0x00000000)
#define IS_RCAN_RTR_BIT_MASK_TYPE(TYPE) (((TYPE) == RCAN_RTR_BIT_MASK) || \
									((TYPE) == RCAN_RTR_NIT_NOTE_MASK))

/**
  * @}
  */

/** @defgroup RCAN_IDE_MASK_TYPE_define
  * @{
  */
#define RCAN_IDE_BIT_MASK				((u32)0x20000000)
#define RCAN_IDE_NIT_NOT_MASK			((u32)0x00000000)
#define IS_RCAN_IDE_BIT_MASK_TYPE(TYPE) (((TYPE) == RCAN_IDE_BIT_MASK) || \
									((TYPE) == RCAN_IDE_NIT_NOT_MASK))

/**
  * @}
  */

/** @defgroup RCAN_MESSAGE_BUFFER_STATUS_define
  * @{
  */
#define RCAN_FRAME_PENDING_TX			((u32)0x00000001)
#define RCAN_FRAME_FINISAH_TX			((u32)0x00000002)
#define RCAN_FRAME_OVWRITR_SEND_TX	((u32)0x00000003)
#define RCAN_FRAME_PENDING_RX			((u32)0x00000004)
#define RCAN_FRAME_FINISH_RX			((u32)0x0000000C)

#define IS_RCAN_MSG_STS_TYPE(TYPE) (((TYPE) == RCAN_FRAME_PENDING_TX) || \
									((TYPE) == RCAN_FRAME_FINISAH_TX) || \
									((TYPE) == RCAN_FRAME_OVWRITR_SEND_TX) || \
									((TYPE) == RCAN_SILENCE_MODE) || \
									((TYPE) == RCAN_FRAME_PENDING_RX))

/**
  * @}
  */

/** @defgroup RCAN_INTERRUPTS_define
  * @{
  */
#define RCAN_TX_INT					((u32)0x00000001)
#define RCAN_RX_INT				((u32)0x00000002)
#define RCAN_ERR_INT				((u32)0x00000004)
#define RCAN_WKUP_INT				((u32)0x00000008)
#define RCAN_BUSOFF_INT				((u32)0x00000010)
#define RCAN_RAM_MOVE_DONE_INT		((u32)0x00000020)
#define IS_RCAN_CONFIG_IT(IT) (((IT) == RCAN_TX_INT) || \
								((IT) == RCAN_RX_INT) || \
									((IT) == RCAN_ERR_INT)|| \
									((IT) == RCAN_WKUP_INT)|| \
									((IT) == RCAN_RAM_MOVE_DONE_INT)|| \
									((IT) == RCAN_BUSOFF_INT))

#define IS_RCAN_CLEAR_IT(IT) ((((IT) & (u32)0xFFFFFFC0) == 0x00) && ((IT) != 0x00))

/**
  * @}
  */

/** @defgroup RCAN_type_define
  * @{
  */

#define IS_RCAN_ALL_PERIPH(PERIPH)   (PERIPH == RCAN)

/**
  * @}
  */

/**
  * @}
  */ 

/* Exported functions --------------------------------------------------------*/
/** @defgroup RCAN_Exported_Functions RCAN Exported Functions
  * @{
  */
/** @defgroup RCAN_global_functions 
  * @{
  */

_LONG_CALL_ void RCAN_StructInit(RCAN_InitTypeDef * RCAN_InitStruct);
_LONG_CALL_ void RCAN_Init(RCAN_TypeDef* RCANx, RCAN_InitTypeDef * RCAN_InitStruct);

_LONG_CALL_ void RCAN_LpFilterCmd(RCAN_TypeDef* RCANx, u32 NewState);
_LONG_CALL_ void RCAN_LpFilterConfig(RCAN_TypeDef* RCANx, u32 FltNum);
	
_LONG_CALL_ void RCAN_BusCmd(RCAN_TypeDef* RCANx, u32 NewState);
_LONG_CALL_ u32 RCAN_BusStatusGet(RCAN_TypeDef* RCANx);

_LONG_CALL_ u32 RCAN_FifoStatusGet(RCAN_TypeDef* RCANx);
_LONG_CALL_ u32 RCAN_FifoLvlGet(RCAN_TypeDef* RCANx);
_LONG_CALL_ void RCAN_ReadRxMsgFromFifo(RCAN_TypeDef* RCANx, RCAN_RxMsgTypeDef * RxMsg);

_LONG_CALL_ u32 RCAN_TXErrCntGet(RCAN_TypeDef* RCANx);
_LONG_CALL_ u32 RCAN_RXErrCntGet(RCAN_TypeDef* RCANx);
_LONG_CALL_ u32 RCAN_TXErrCntClear(RCAN_TypeDef* RCANx);
_LONG_CALL_ u32 RCAN_RXErrCntClear(RCAN_TypeDef* RCANx);
_LONG_CALL_ u32 RCAN_RXErrCntSTS(RCAN_TypeDef* RCANx);


_LONG_CALL_ void RCAN_WriteMsg(RCAN_TypeDef* RCANx, RCAN_TxMsgTypeDef* TxMsg);
_LONG_CALL_ void RCAN_SetRxMsgBuf(RCAN_TypeDef* RCANx, RCAN_RxMsgTypeDef * RxMsg);
_LONG_CALL_ void RCAN_ReadMsg(RCAN_TypeDef* RCANx, RCAN_RxMsgTypeDef * RxMsg);
_LONG_CALL_ void RCAN_TxAutoReply(RCAN_TypeDef* RCANx, RCAN_TxMsgTypeDef* TxMsg);
_LONG_CALL_ void RCAN_RxAutoReply(RCAN_TypeDef* RCANx, RCAN_RxMsgTypeDef * RxMsg);

_LONG_CALL_ void RCAN_Cmd(RCAN_TypeDef* RCANx, u32 NewState);

/**
  * @}
  */ 


/** @defgroup RCAN_Interrupt_status_functions 
  * @{
  */

_LONG_CALL_ void RCAN_INTConfig(RCAN_TypeDef* RCANx, u32 RCAN_IT, u32 NewState);
_LONG_CALL_ u32 RCAN_GetINTStatus(RCAN_TypeDef* RCANx);
_LONG_CALL_ void RCAN_ClearINT(RCAN_TypeDef* RCANx, u32 RCAN_IT);
_LONG_CALL_ void RCAN_ClearAllINT(RCAN_TypeDef* RCANx);
_LONG_CALL_ u32 RCAN_GetErrStatus(RCAN_TypeDef* RCANx);
_LONG_CALL_ u32 RCAN_ClearErrStatus(RCAN_TypeDef* RCANx, u32 ERR_STS);

_LONG_CALL_ void RCAN_RxMsgBufINTConfig(RCAN_TypeDef* RCANx, u32 BUF_IT, u32 NewState);
_LONG_CALL_ void RCAN_TxMsgBufINTConfig(RCAN_TypeDef* RCANx, u32 BUF_IT, u32 NewState);

_LONG_CALL_ u32 RCAN_TxMsgBufErrGet(RCAN_TypeDef* RCANx);
_LONG_CALL_ u32 RCAN_TxMsgBufErrClear(RCAN_TypeDef* RCANx, u32 ERR);

_LONG_CALL_ u32 RCAN_MsgBufStatusRegGet(RCAN_TypeDef* RCANx, u32 MsgBufIdx);
_LONG_CALL_ u32 RCAN_MsgBufStatusGet(RCAN_TypeDef* RCANx, u32 MsgBufIdx);
_LONG_CALL_ u32 RCAN_CheckMsgBufAvailable(RCAN_TypeDef* RCANx, u32 MsgBufIdx);

_LONG_CALL_ u32 RCAN_TxDoneStatusGet(RCAN_TypeDef* RCANx);
_LONG_CALL_ u32 RCAN_MsgBufTxDoneStatusGet(RCAN_TypeDef* RCANx, u32 MsgBufIdx);
_LONG_CALL_ u32 RCAN_RxDoneStatusGet(RCAN_TypeDef* RCANx);
_LONG_CALL_ u32 RCAN_MsgBufRxDoneStatusGet(RCAN_TypeDef* RCANx, u32 MsgBufIdx);

_LONG_CALL_ void RCAN_TxDoneStatusClear(RCAN_TypeDef* RCANx, u32 Status);
_LONG_CALL_ void RCAN_MsgBufTxDoneStatusClear(RCAN_TypeDef* RCANx, u32 MsgBufIdx);

_LONG_CALL_ void RCAN_RxDoneStatusClear(RCAN_TypeDef* RCANx, u32 Status);
_LONG_CALL_ void RCAN_MsgBufRxDoneStatusClear(RCAN_TypeDef* RCANx, u32 MsgBufIdx);


_LONG_CALL_ void RCAN_TxMsgTriggerCmd(RCAN_TypeDef* RCANx, u32 State);
_LONG_CALL_ void RCAN_TxMsgTriggerConfig(RCAN_TypeDef* RCANx, u32 CloseOffset, u32 Begin);

_LONG_CALL_ void RCAN_RxDmaDestBaseAddrConfig(RCAN_TypeDef* RCANx, u32 Addr);
_LONG_CALL_ u32 RCAN_GetRxDmaData(RCAN_TypeDef* RCANx);

_LONG_CALL_ void RCAN_MsgCtrlRegConfig(RCAN_TypeDef* RCANx, u32 MsgIdx, u32 RxDmaEn, u32 DmaOffset);
_LONG_CALL_ void RCAN_MsgBaseAddrConfig(RCAN_TypeDef* RCANx, u32 MsgIdx, u32 BaseAddr);
_LONG_CALL_ void RCAN_MsgBaseAddrEndReg(RCAN_TypeDef* RCANx, u32 EndAddr);

_LONG_CALL_ u32 RCAN_RamBufferMapConfig(RCAN_TypeDef* RCANx, u32 *pPara);
/**
  * @}
  */ 

/** @defgroup RCAN_dma_functions 
  * @{
  */
_LONG_CALL_ BOOL RCAN_TXGDMA_Init(u8 CANIndex, GDMA_InitTypeDef *GDMA_InitStruct, void *CallbackData, IRQ_FUN CallbackFunc, u8 *pTxBuf, int TxCount);
_LONG_CALL_ BOOL RCAN_RXGDMA_Init(u8 CANIndex, GDMA_InitTypeDef *GDMA_InitStruct, void *CallbackData, IRQ_FUN CallbackFunc);
_LONG_CALL_ void RCAN_FillTXDmaBuffer(RCAN_DmaBufferTypeDef* RCANxBufer, RCAN_TxMsgTypeDef* TxMsg);
/**
  * @}
  */


/**
  * @}
  */


/* Registers Definitions --------------------------------------------------------*/
/**************************************************************************//**
 * @defgroup RCAN_Register_Definitions RCAN Register Definitions
 * @{
 *****************************************************************************/
 
/**************************************************************************//**
 * @defgroup CAN_CTL
 * @{
 *****************************************************************************/
#define RCAN_EN						((u32)0x00000001)			/*BIT[0], ECAN enable*/
#define RCAN_BUS_ON_REQ				((u32)0x00000002)			/*BIT[1], ECAN bus on request*/
#define RCAN_TRI_SAMPLE					((u32)0x00000004)			/*BIT[2], ECAN tripple sample mode*/
#define RCAN_AUTO_RPY_TX_EN			((u32)0x00000008)			/*BIT[3], auto re-transmission enabled*/
#define RCAN_TEST_MODE_EN				((u32)0x00000010)			/*BIT[4], test mode enable, for lbk or silent mode use*/
#define RCAN_RX_FIFO_EN					((u32)0x00000020)			/*BIT[5], rx fifo function enable*/
#define RCAN_FD_EN						((u32)0x00000040)			/*BIT[6], CAN FD enable*/
#define RCAN_RXDMA_EN					((u32)0x00000080)			/*BIT[7], RX DMA enable*/
#define RCAN_CAN_FD_CRC_MODE			((u32)0x00000100)			/*BIT[8], CAN FD CRC mode. 0: ISO CRC, 1: non-ISO CRC*/



/** @} */

/**************************************************************************//**
 * @defgroup CAN_STS
 * @{
 *****************************************************************************/
#define RCAN_BUS_ON_STS				((u32)0x00000001)			/*BIT[0], 1 means the can moudle is already bus on*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_FIFO_STS
 * @{
 *****************************************************************************/
#define RCAN_FIFO_MSG_FULL				((u32)0x00000001)			/*BIT[0], rx fifo full  */
#define RCAN_FIFO_MSG_EMTY				((u32)0x00000002)			/*BIT[1], rx fifo empty*/
#define RCAN_FIFO_MSG_OVERFLW			((u32)0x00000004)			/*BIT[2], rx fifo overflow*/
#define RCAN_FIFO_MSG_LVL				((u32)0x00000070)			/*BIT[7:4], rx fifo level*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_BIT_TIMING
 * @{
 *****************************************************************************/
#define RCAN_TSEG1						((u32)0x000000ff)			/*BIT[7:0], TSEG1 length = can_tseg1+1 */
#define RCAN_TSEG2						((u32)0x0000ff00)			/*BIT[15:8], TSEG2 length = can_tseg2+1*/
#define RCAN_SJW						((u32)0x0070000)			/*BIT[18:16], SJW length = can_sjw+1*/
#define RCAN_BRP						((u32)0xff000000)			/*BIT[31:24], CAN bit timing perscaler = can_brp+1*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_FD_BIT_TIMING
 * @{
 *****************************************************************************/
#define RCAN_FD_TSEG1						((u32)0x0000000f)			/*BIT[3:0], FD TSEG1 length = can_tseg1+1 */
#define RCAN_FD_TSEG2						((u32)0x000000f0)			/*BIT[7:4], FD TSEG2 length = can_tseg2+1*/
#define RCAN_FD_SJW							((u32)0x00000700)			/*BIT[10:8], FD SJW length = can_sjw+1*/
#define RCAN_FD_BRP							((u32)0xff000000)			/*BIT[31:24], FD CAN bit timing perscaler = can_brp+1*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_FD_SSP_CAL
 * @{
 *****************************************************************************/
#define RCAN_CAN_FD_SSP						((u32)0x000000ff)			/*BIT[7:0], ssp position, delay from start of one bit, if automatic calculation is enabled, the field will update after calculation */
#define RCAN_CAN_FD_SSP_MIN				((u32)0x0000ff00)			/*BIT[15:8], minimum of ssp from start of one bit, only functional when can_fd_ssp_auto set*/
#define RCAN_CAN_FD_SSP_DCO				((u32)0x00ff0000)			/*BIT[23:16], delay compensation offset*/
#define RCAN_CAN_FD_SSP_AUTO				((u32)0x01000000)			/*BIT[24], automatic calculat ssp delay */

/** @} */

/**************************************************************************//**
 * @defgroup CAN_INT_EN
 * @{
 *****************************************************************************/
#define RCAN_TX_INT_EN					((u32)0x00000001)			/*BIT[0], tx interrupt enable */
#define RCAN_RX_INT_EN					((u32)0x00000002)			/*BIT[1], rx interrupt enable*/
#define RCAN_ERR_INT_EN					((u32)0x00000004)			/*BIT[2], error interrupt enable*/
#define RCAN_WAKEUP_INT_EN				((u32)0x00000008)			/*BIT[3], wake up interrupt enable*/
#define RCAN_BUSOFF_INT_EN				((u32)0x00000010)			/*BIT[4], bus off interrupt enable*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_MB_RXINT_EN
 * @{
 *****************************************************************************/
#define RCAN_MB_RXINT_EN				((u32)0x0000FFFF)			/*BIT[15:0], message buffer rx interrupt enbale */

/** @} */

/**************************************************************************//**
 * @defgroup CAN_MB_TXINT_EN
 * @{
 *****************************************************************************/
#define RCAN_MB_TXINT_EN				((u32)0x0000FFFF)			/*BIT[15:0], message buffer tx interrupt enbale */

/** @} */

/**************************************************************************//**
 * @defgroup CAN_INT_FLAG
 * @{
 *****************************************************************************/
#define RCAN_TX_INT_FLAG					((u32)0x00000001)			/*BIT[0], tx interrupt flag */
#define RCAN_RX_INT_FLAG					((u32)0x00000002)			/*BIT[1], rx interrupt flag*/
#define RCAN_ERR_INT_FLAG					((u32)0x00000004)			/*BIT[2], error interrupt flag*/
#define RCAN_WAKEUP_INT_FLAG				((u32)0x00000008)			/*BIT[3], wake up interrupt flag*/
#define RCAN_BUSOFF_INT_FLAG				((u32)0x00000010)			/*BIT[4], bus off interrupt flag*/
#define RCAN_RAM_MOVE_DONE_INT_FLAG		((u32)0x00000020)			/*BIT[5], data exchange between RAM and register interface done interrupt flag*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_ERR_STATUS
 * @{
 *****************************************************************************/
#define RCAN_ERR_BIT0						((u32)0x00000001)			/*BIT[0], CAN error bit0*/
#define RCAN_ERR_BIT1						((u32)0x00000002)			/*BIT[1], CAN error bit1*/
#define RCAN_ERR_FRAME						((u32)0x00000004)			/*BIT[2], CAN frame error*/
#define RCAN_ERR_CRC						((u32)0x00000008)			/*BIT[3], CAN CRC error*/
#define RCAN_ERR_STUFF						((u32)0x00000010)			/*BIT[4], CAN stuff error*/
#define RCAN_ERR_ACK						((u32)0x00000020)			/*BIT[5], CAN ACK error*/
#define RCAN_ERR_TX							((u32)0x00000100)			/*BIT[8], CAN tx error*/
#define RCAN_ERR_RX							((u32)0x00000200)			/*BIT[9], CAN rx error*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_ERR_CNT_CTL
 * @{
 *****************************************************************************/
#define RCAN_TX_ERR_CNT_CLR					((u32)0x00000001)			/*BIT[0], CAN tx error counter clear*/
#define RCAN_RX_ERR_CNT_CLR					((u32)0x00000002)			/*BIT[1], CAN rx error counter clear*/
#define RCAN_ERR_WARN_THRES				((u32)0x0000FF00)			/*BIT[16:8], error counter warning threshold, programable, default 96*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_ERR_CNT_STS
 * @{
 *****************************************************************************/
#define RCAN_TX_ERR_CNT_TEC					((u32)0x000001FF)			/*BIT[8:0], transmit error count*/
#define RCAN_TX_ERR_CNT_REC					((u32)0x01ff0000)			/*BIT[24:16], receive error count*/
#define RCAN_TX_ERR_PASSIVE					((u32)0x10000000)			/*BIT[28], error counter arrive at error passive level*/
#define RCAN_TX_ERR_BUSOFF					((u32)0x20000000)			/*BIT[29], error counter arrive at bus off level*/
#define RCAN_TX_ERR_WARNING				((u32)0x40000000)			/*BIT[30], error counter arrive at warning threshold*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_TX_ERROR_FLAG
 * @{
 *****************************************************************************/
#define RCAN_TX_ERR_FLAG					((u32)0x0000FFFF)			/*BIT[15:0], indicate which message tx buffer have error detected, cpu can write 1 to clear */

/** @} */

/**************************************************************************//**
 * @defgroup CAN_TX_DONE_FLAG
 * @{
 *****************************************************************************/
#define RCAN_TX_DONE_FLAG					((u32)0x0000FFFF)			/*BIT[15:0], tx done for each message buffer*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_RX_DONE_FLAG
 * @{
 *****************************************************************************/
#define RCAN_RX_DONE_FLAG					((u32)0x0000FFFF)			/*BIT[15:0], rx done for each message buffer*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_TIME_STAMP
 * @{
 *****************************************************************************/
#define RCAN_TIME_STAMP						((u32)0x0000FFFF)			/*BIT[15:0], time stamp counter value, up count*/
#define RCAN_TIME_STAMP_DIV					((u32)0x00FF0000)			/*BIT[23:16], time stamp divider, up count*/
#define RCAN_TIME_STAMP_EN					((u32)0x80000000)			/*BIT[31], time stamp function enable and time stamp counter start*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_MB_TRIGGER
 * @{
 *****************************************************************************/
#define RCAN_TX_TRIGGER_BEGIN				((u32)0x0000FFFF)		/*BIT[15:0] start of trigger time*/
#define RCAN_TX_TRIGGER_CLOSE_OFFSET		((u32)0x00FF0000)		/*BIT[23:16], end of trigger time = tx_trigger_begin + tx_trigger_close*/
#define RCAN_TX_TRIGGER_EN					((u32)0x80000000)		/*BIT[24], End of message buffer base address in RAM*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_RXDMA_BA
 * @{
 *****************************************************************************/
#define RCAN_RXDMA_DEST_BA				((u32)0xFFFFFFFF)		/*BIT[31:0] can rx dma destation base address */


/** @} */

/**************************************************************************//**
 * @defgroup CAN_RX_DMA_DATA
 * @{
 *****************************************************************************/
#define RCAN_RX_DMA_DATA				((u32)0xFFFFFFFF)		/*BIT[31:0] For DMA read received data */


/** @} */



/**************************************************************************//**
 * @defgroup CAN_RX_DMA_DATA
 * @{
 *****************************************************************************/
#define RCAN_RX_DMA_DATA				((u32)0xFFFFFFFF)		/*BIT[31:0] For DMA read received data */


/** @} */

/**************************************************************************//**
 * @defgroup CAN_MB_CTRL
 * @{
 *****************************************************************************/
#define RCAN_MSG_BA				((u32)0x000001FF)		/*BIT[8:0] message buffer base address in RAM*/
#define RCAN_MSG_DMA_OFFSET	((u32)0x0007FC00)		/*BIT[18:10] dma offset from dma base address*/
#define RCAN_MSG_RXDMA_EN		((u32)0x01000000)		/*BIT[24] dma enable when the message buffer used as rx*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_MB_BA_END
 * @{
 *****************************************************************************/
#define RCAN_MSG_BA_END		((u32)0x000003FF)		/*BIT[9:0] End of message buffer base address in RAM*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_RAM_CMD
 * @{
 *****************************************************************************/
#define RCAN_RAM_ACC_NUM					((u32)0x000000FF)			/*BIT[7:0], access buffer number, one buffer take 5 words in RAM*/

#define RCAN_RAM_ACC_DAT0					((u32)0x00000800)			/*BIT[11], access  field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_DAT1					((u32)0x00001000)			/*BIT[12], access  field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_DAT2					((u32)0x00002000)			/*BIT[13], access  field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_DAT3					((u32)0x00004000)			/*BIT[14], access  field, the field will exchange with RAM space*/

#define RCAN_RAM_ACC_DAT4					((u32)0x00008000)			/*BIT[15], access  field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_DAT5					((u32)0x00010000)			/*BIT[16], access  field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_DAT6					((u32)0x00020000)			/*BIT[17], access  field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_DAT7					((u32)0x00040000)			/*BIT[18], access  field, the field will exchange with RAM space*/

#define RCAN_RAM_ACC_DAT8					((u32)0x00080000)			/*BIT[19], access  field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_DAT9					((u32)0x00100000)			/*BIT[20], access  field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_DAT10					((u32)0x00200000)			/*BIT[21], access  field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_DAT11					((u32)0x00400000)			/*BIT[22], access  field, the field will exchange with RAM space*/

#define RCAN_RAM_ACC_DAT12					((u32)0x00800000)			/*BIT[23], access  field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_DAT13					((u32)0x01000000)			/*BIT[24], access  field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_DAT14					((u32)0x02000000)			/*BIT[25], access  field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_DAT15					((u32)0x04000000)			/*BIT[26], access  field, the field will exchange with RAM space*/

#define RCAN_RAM_ACC_DAT_MASK				((u32)0x07FFF800)			/*BIT[23:8], access  field mask*/

#define RCAN_RAM_ACC_MASK					((u32)0x00000100)			/*BIT[8], access CAN_RAM_MASK field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_CS					((u32)0x00000200)			/*BIT[9], access CAN_RAM_CS field, the field will exchange with RAM space*/
#define RCAN_RAM_ACC_ARB					((u32)0x00000400)			/*BIT[10], access CAN_RAM_ARB field, the field will exchange with RAM space*/
#define RCAN_RAM_BUFFER_EN					((u32)0x20000000)			/*BIT[29], enable the can_ram_acc_num specified message buffer for rx/tx*/
#define RCAN_RAM_DIR						((u32)0x40000000)			/*BIT[30], 0 is for read from RAM to CAN_RAM_* registers, 1 is for write from CAN_RAM_* to RAM*/
#define RCAN_RAM_START						((u32)0x80000000)			/*BIT[31], start to exchange data with RAM, the bit will clear automatically after the data change is finshed.*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_RAM_CS
 * @{
 *****************************************************************************/
#define RCAN_RAM_DLC						((u32)0x0000000F)			/*BIT[3:0], the data length*/
#define RCAN_RAM_LOST						((u32)0x00000010)			/*BIT[4], data have lost in the read buffer, means more than one frame have beed received since last read*/
#define RCAN_RAM_RXTX						((u32)0x00000020)			/*BIT[5], can frame RX or TX, 0 is for RX, 1 is for TX*/
#define RCAN_RAM_EDL						((u32)0x00000100)			/*BIT[8], FD frame edl bit, indicate it¡¯s a FD frameX*/
#define RCAN_RAM_BRS						((u32)0x00000200)			/*BIT[9], FD frame brs bit, indicate switch bit timing*/
#define RCAN_RAM_ESI						((u32)0x00000400)			/*BIT[10], FD frame esi bit, indicate error passive of message sending node, read bit, should always write 0, hardware automatic fill*/
#define RCAN_RAM_AUTORLY					((u32)0x00000040)			/*BIT[6], can frame RTR bit, determine DATA or REMOTE frame*/
#define RCAN_RAM_TIMESTAMP					((u32)0xFFFF0000)			/*BIT[31:16], the received message time stamp*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_RAM_ARB
 * @{
 *****************************************************************************/
#define RCAN_RAM_ID							((u32)0x1FFFFFFF)			/*BIT[28:0], can frame ID, including low 18 bit extend ID and high 11 bit standard ID*/
#define RCAN_RAM_STD_ID_MASK				((u32)0x1FFC0000)			/*BIT[28:18], can frame standard ID, high 11 bit*/
#define RCAN_RAM_EXT_ID_MASK				((u32)0x0003FFFF)			/*BIT[17:0], can frame extend ID, low 18 bit*/
#define RCAN_RAM_IDE						((u32)0x20000000)			/*BIT[29], can frame IDE bit, determine standard or extend format*/
#define RCAN_RAM_RTR						((u32)0x40000000)			/*BIT[30], can frame RTR bit, determine DATA or REMOTE frame*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_RAM_MASK
 * @{
 *****************************************************************************/
#define RCAN_RAM_MASK						((u32)0x1FFFFFFF)			/*BIT[28:0], can frame ID mask, 1 means the ID bit in CAN_RAM_ARB don't care, 0 means the bit should match.*/
#define RCAN_RAM_IDE_MASK					((u32)0x20000000)			/*BIT[29], can frame IDE mask, 1 means don't care, 0 means the bit should match*/
#define RCAN_RAM_RTR_MASK					((u32)0x40000000)			/*BIT[30], can frame RTR mask, 1 means don't care, 0 means the bit should match*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_WAKEPIN_FLT
 * @{
 *****************************************************************************/
#define RCAN_WAKEUP_PIN_FLT_LEN				((u32)0X000000FF)			/*BIT[7:0], wakeup pin digital filter length.*/
#define RCAN_WAKEUP_PIN_FLT_EN				((u32)0X80000000)			/*BIT[31], wakeup pin digital filter enable*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_TEST
 * @{
 *****************************************************************************/
#define RCAN_LPB_EN						((u32)0X00000001)			/*BIT[0], can loop back enable*/
#define RCAN_SILENCE_MODE_EN				((u32)0X00000002)			/*BIT[1], can silence modle enable*/

/** @} */

/**************************************************************************//**
 * @defgroup CAN_MB_STS
 * @{
 *****************************************************************************/
#define RCAN_MSG_TX_REQ					((u32)0X00000001)			/*BIT[0], tx message is pending for transmit*/
#define RCAN_MSG_TX_DONE					((u32)0X00000002)			/*BIT[1], tx message in the message buffer finish sending*/
#define RCAN_MSG_TX_RDY					((u32)0X00000004)			/*BIT[2], the message buffer is ready for receiving a new message*/
#define RCAN_MSG_TX_VLD					((u32)0X00000008)			/*BIT[3], new message have been received in the message buffer*/

/** @} */

/** @} */

/**
  * @}
  */

/**
  * @}
  */ 

/* Other Definitions --------------------------------------------------------*/
typedef struct
{
	RCAN_TypeDef* RCANx;
	u32 Tx_HandshakeInterface;
	u32 Rx_HandshakeInterface;
	IRQn_Type	IrqNum;
} CAN_DevTable;

//extern RCAN_DmaBufferTypeDef  TestDmaRamBuffer;

#endif

/******************* (C) COPYRIGHT 2017 Realtek Semiconductor *****END OF FILE****/
