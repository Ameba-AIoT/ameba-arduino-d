/**
  ******************************************************************************
  * @file    rtl8721d_ecc.h
  * @author
  * @version V1.0.0
  * @date    2018-06-05
  * @brief   This file contains all the functions prototypes for the ECC engine firmware
  *          library.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2018, Realtek Semiconductor Corporation. All rights reserved.
  ****************************************************************************** 
  */

#ifndef _RTL8721D_ECC_H_
#define _RTL8721D_ECC_H_

/** @addtogroup AmebaD_Periph_Driver
  * @{
  */

/** @defgroup ECC
  * @{
  */


/*===================================================================================================*/
/*ECC IP Slave configure address space*/
typedef struct {
	__IO u32	reg_ec_control;			

	__IO u32	reg_ec_ins_fifoin;	

	__IO u32	reg_ec_status;

	__IO u32	reg_msglen;

	__IO u32	reg_msgaddr;	

	__IO u32 debug_ctrls;

	__IO u32 debug_output;	
}ECC_SLAVE_TypeDef;

/*ECC IP master memory space*/
typedef struct {

	#if 1
	u8	Rsvd1[32*4];	
	#endif

	u8	EC_BX[32];

	u8	EC_BY[32];

	union {
		u8 Private_Key[32];
		u8 I[32];
	} Key_I;

	u8	Secret_Key[32];

	#if 1
	u8	Rsvd[32];	
	#endif
	
	u8	S[32];

	u8	R[32];

	u8	A[32];
}ECC_Ed25519_MASTER_TypeDef;

/*ECC ed25519 result*/
typedef struct {
	u8	Public_Key[32];

	u8	Sig[64];
}ECC_Ed25519_Res_TypeDef;

/*ECC hardware FIFO size*/
#define ECC_INS_FIFO_SIZE			(64)

/*ECC master operate unit size*/
#define ECC_MASTER_OPERATE_UNIT	(32)

/*ECC master header size*/
#define ECC_MASTER_HDR_SIZE			(12)

/*Master memory header  size*/
#define ECC_MASTER_HDR_OFFSET		(ECC_MASTER_OPERATE_UNIT*ECC_MASTER_HDR_SIZE)

#define ECC_SLAVE_BASE 				(SDIO_DEVICE_REG_BASE) 	//(0x4002C000)

#define ECC_CONFIG_BASE				((ECC_SLAVE_TypeDef *) ECC_SLAVE_BASE)

/*===================================================================================================*/



/* Exported Types --------------------------------------------------------*/

/** @defgroup ECC_Exported_Types ECC Exported Types
  * @{
  */

/** 
  * @brief  ECC ed25519 Mode Initialization structure definition
  */
typedef struct
{
	u32	ECC_Ed25519MsgLen; 		/*!< Specifies the message length.
                                      				This parameter can be a number between 0x000000 and 0xffffff, unit: Byte. */
	                             
	u32	ECC_Ed25519MsgAddr;       	/*!< Specifies the message address. */

	//u32	ECC_Ed25519BaseXP;       	/*!< Specifies the base point X buffer start address. */

	//u32	ECC_Ed25519BaseYP;       	/*!< Specifies the base point Y buffer start address. */

	u32	ECC_Ed25519ScretKeyP;	/*!< Specifies the secret key buffer start address. */
	
	u32	ECC_Ed25519ScretKeyLen;	/*!< Specifies the secret key length. */
} ECC_Ed25519GenSigInitTypeDef;

/**
  * @}
  */

/** 
  * @brief  ECC ed25519 Mode Initialization structure definition
  */
typedef struct
{
	u32	ECC_Ed25519MsgLen;
	                             
	u32	ECC_Ed25519MsgAddr;

	//u32	ECC_Ed25519BaseXP;

	//u32	ECC_Ed25519BaseYP;

	u32	ECC_Ed25519PublicKeyP;
	
	u32	ECC_Ed25519SigP;
} ECC_Ed25519VerifySigInitTypeDef;

/**
  * @}
  */

/* Exported constants --------------------------------------------------------*/
/** @defgroup ECC_Exported_Constants ECC Exported Constants
  * @{
  */

/** @defgroup ECC_MODE_define
  * @{
  */
#define ECC_CFG_ENGINE_SM2						((u32)0x00000040)
#define ECC_CFG_ENGINE_NIST256					((u32)0x00000020)
#define ECC_CFG_ENGINE_ED25519					((u32)0x00000010)
#define IS_ECC_WORK_MODE(MODE) (((MODE) == ECC_CFG_ENGINE_SM2) || \
									((MODE) == ECC_CFG_ENGINE_NIST256) || \
									((MODE) == ECC_CFG_ENGINE_ED25519))


/**
  * @}
  */

/** @defgroup ECC_ERROR_define
  * @{
  */
#define ECC_EC_INT_STS						((u32)0x00000001)
#define ECC_AXIM_ERR_STS					((u32)0x00000002)
#define ECC_ENGNE_ERR_STS					((u32)0x00000004)

#define IS_ECC_CLEAR_ERR(ERR) ((((ERR) & (u32)0xFFFFFFF8) == 0x00) && ((ERR) != 0x00))

/**
  * @}
  */

/** @defgroup ECC_RESET_TYPE_define
  * @{
  */
#define ECC_FIFO_RST				((u32)0x00000001)
#define ECC_ENGINE_RST			((u32)0x00000002)
#define ECC_AXIM_RST				((u32)0x00000004)

#define IS_ECC_RESET_TYPE(TYPE) ((((TYPE) & (u32)0xFFFFFFF8) == 0x00) && ((TYPE) != 0x00))

/**
  * @}
  */

/** @defgroup ECC_type_define
  * @{
  */

#define IS_ECC_ALL_PERIPH(PERIPH)   (PERIPH == ECC_CONFIG_BASE)

/**
  * @}
  */

/** @defgroup ECC_debug_type_define
  * @{
  */


#define  ECC_RF_DEBUG_MUX						((u32)0x00000000)
#define  ECC_ID_DEBUG							((u32)0x00000020)
#define  ECC_EX_DEBUG							((u32)0x00000040)
#define  ECC_AXIM_DEBUG							((u32)0x00000060)
#define  ECC_FSM_DEBUG							((u32)0x00000080)

#define IS_ECC_DEBUG_MODE(MODE) (((MODE) == ECC_RF_DEBUG_MUX) || \
									((MODE) == ECC_ID_DEBUG) || \
									((MODE) == ECC_EX_DEBUG) || \
									((MODE) == ECC_EX_DEBUG) || \
									((MODE) == ECC_FSM_DEBUG))

/**
  * @}
  */


/**
  * @}
  */ 

/* Exported functions --------------------------------------------------------*/
/** @defgroup ECC_Exported_Functions ECC Exported Functions
  * @{
  */
/** @defgroup ECC_ed25519_functions 
  * @{
  */

_LONG_CALL_ void ECC_Ed25519GenSigStructInit(ECC_Ed25519GenSigInitTypeDef * ECC_Ed25519GenSigInitStruct);
_LONG_CALL_ void ECC_Ed25519GenSigInit(ECC_SLAVE_TypeDef* ECCx, ECC_Ed25519GenSigInitTypeDef * ECC_Ed25519GenSigInitStruct);
_LONG_CALL_ void ECC_Ed25519GenSigResGet(ECC_Ed25519_Res_TypeDef * Res, ECC_Ed25519_MASTER_TypeDef * MasterMem);

_LONG_CALL_ void ECC_Ed25519VerifySigStructInit(ECC_Ed25519VerifySigInitTypeDef * ECC_Ed25519VerifySigInitStruct);
_LONG_CALL_ void ECC_Ed25519VerifySigInit(ECC_SLAVE_TypeDef* ECCx, ECC_Ed25519VerifySigInitTypeDef * ECC_Ed25519VerifySigInitStruct);
_LONG_CALL_ void ECC_Ed25519VerifySigResGet(ECC_Ed25519_Res_TypeDef * Res, ECC_Ed25519_MASTER_TypeDef * MasterMem);

/**
  * @}
  */ 

/** @defgroup ECC_common_functions 
  * @{
  */
_LONG_CALL_ void ECC_FillInstruction(ECC_SLAVE_TypeDef* ECCx, u32 * Ins, u32 InsLen);
_LONG_CALL_ u32 ECC_GetFIFOEmptySize(ECC_SLAVE_TypeDef* ECCx);
_LONG_CALL_ u32 ECC_GetErrorStatus(ECC_SLAVE_TypeDef* ECCx);
_LONG_CALL_ void ECC_ClearErrorStatus(ECC_SLAVE_TypeDef* ECCx, u32 ErrorType);
_LONG_CALL_ void ECC_Reset(ECC_SLAVE_TypeDef* ECCx, u32 RstType);
_LONG_CALL_ void ECC_Cmd(ECC_SLAVE_TypeDef* ECCx, u32 NewState);

/**
  * @}
  */ 


/** @defgroup ECC_Debug_functions 
  * @{
  */
_LONG_CALL_ u32 ECC_DebugOutputGet(ECC_SLAVE_TypeDef* ECCx);
_LONG_CALL_ void ECC_DebugRfMuxSel(ECC_SLAVE_TypeDef* ECCx, u32 Dbg_RfMux);
_LONG_CALL_ void ECC_DebugSelect(ECC_SLAVE_TypeDef* ECCx, u32 Dbg_Sel2, u32 Dbg_Sel_Sub);
_LONG_CALL_ void ECC_DebugCmd(ECC_SLAVE_TypeDef* ECCx, u32 NewState);

/**
  * @}
  */


/* Registers Definitions --------------------------------------------------------*/
/**************************************************************************//**
 * @defgroup ECC_Register_Definitions ECC Register Definitions
 * @{
 *****************************************************************************/
 
/**************************************************************************//**
 * @defgroup ECC_REG_EC_CONTROL
 * @{
 *****************************************************************************/
#define ECC_CFG_FIFO_RST				((u32)0x00000001)			/*BIT[0], fifo reset*/
#define ECC_CFG_ENGINE_RST				((u32)0x00000002)			/*BIT[1], engine reset*/
#define ECC_CFG_AXIM_RST				((u32)0x00000004)			/*BIT[2], axim reset*/
#define ECC_ENGINE_START				((u32)0x00000008)			/*BIT[3], engine start*/

#define ECC_ENGINE_ED25519				((u32)0x00000010)			/*BIT[4], ed25519 mode*/
#define ECC_ENGINE_NISTP256				((u32)0x00000020)			/*BIT[5], nist P 256 mode*/
#define ECC_ENGINE_SM2					((u32)0x00000040)			/*BIT[6], SM2 mode*/

/** @} */

/**************************************************************************//**
 * @defgroup ECC_REG_EC_INS_FIFOIN
 * @{
 *****************************************************************************/
#define ECC_INS_FIFO_WDATA				((u32)0xFFFFFFFF)			/*BIT[31:0], write instructon fifo*/

/** @} */

/**************************************************************************//**
 * @defgroup ECC_REG_EC_STATUS
 * @{
 *****************************************************************************/
#define ECC_ERR_MASK					((u32)0x00000007)			/*BIT[2:0], error field */
#define ECC_EC_INT						((u32)0x00000001)			/*BIT[0], interrupt assert*/
#define ECC_AXIM_ERR						((u32)0x00000002)			/*BIT[1], axi-bus protocol error*/
#define ECC_ENGNE_ERR					((u32)0x00000004)			/*BIT[2], ins-excute-error*/

#define ECC_INS_FIFO_SLOTNUM			((u32)0x0000FF00)			/*BIT[15:8], slot number not used in ins-fifo*/

/** @} */

/**************************************************************************//**
 * @defgroup ECC_REG_MSGLEN
 * @{
 *****************************************************************************/
#define ECC_CFG_MSGLEN					((u32)0x00FFFFFF)			/*BIT[23:0], message length used by hash, counted by byte*/

#define ECC_CFG_SKLEN					((u32)0x3f000000)			/*BIT[29:24], secret key length, counted by byte*/

/** @} */

/**************************************************************************//**
 * @defgroup ECC_REG_MSGADDR
 * @{
 *****************************************************************************/
#define ECC_CFG_MSGADDR				((u32)0xFFFFFFFF)			/*BIT[31:0], message address*/

/** @} */

/**************************************************************************//**
 * @defgroup ECC_REG_DEBUG_STRLS
 * @{
 *****************************************************************************/
#define ECC_CFG_DEBUG_SEL_SUB			((u32)0x0000001F)			/*BIT[4:0], select the sub-signals from ex_dbg or id_dbg or rf_dbg or axim_dbg*/

#define ECC_CFG_DEBUG_SEL2				((u32)0x000000E0)			/*BIT[7:5], debug port selection2*/
#define ECC_CFG_DEBUG_RF				((u32)0x00000000)			/*BIT[7:5], rf_debug_mux*/
#define ECC_CFG_DEBUG_ID				((u32)0x00000020)			/*BIT[7:5], id_debug*/
#define ECC_CFG_DEBUG_EX				((u32)0x00000040)			/*BIT[7:5], ex_debug*/
#define ECC_CFG_DEBUG_AXIM				((u32)0x00000060)			/*BIT[7:5], axim_debug*/
#define ECC_CFG_DEBUG_FSM				((u32)0x00000080)			/*BIT[7:5], FSM_debug*/

#define ECC_CFG_DEBUG_RFMUX			((u32)0x00000700)			/*BIT[10:8], rf debug*/

#define ECC_CFG_DEBUG_EN				((u32)0x00008000)			/*BIT[15], debug enable*/


/** @} */

/**************************************************************************//**
 * @defgroup ECC_REG_DEBUG_OUTPUT
 * @{
 *****************************************************************************/
#define ECC_CFG_DEBUG_OUTPUT			((u32)0XFFFFFFFF)			/*BIT[31:0], debug info output from ec-engine*/


/** @} */

/** @} */

/**
  * @}
  */

/**
  * @}
  */ 

/* Other Definitions --------------------------------------------------------*/

#endif




/******************* (C) COPYRIGHT 2016 Realtek Semiconductor *****END OF FILE****/
