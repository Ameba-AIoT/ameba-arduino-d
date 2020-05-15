#ifndef __RTL8721D_POWERCTRL_RAM_H__
#define __RTL8721D_POWERCTRL_RAM_H__


/*--------------------Define --------------------------------------------*/
//RPWM Definition
#define RPWM_ENTER32K_GRANT     BIT0


//CPWM Definition
#define CPWM_40M_STABLE         BIT0
#define CLK_DOWN_RDY            BIT4

#define WAIT_TXSM_STABLE_CNT            (1000)
#define WAIT_TXSM_STABLE_ONCE_TIME      (50)

#define WAIT_RPWM_STABLE_BREAK_CNT      (100)

#define WAIT_TSF_STABLE_BREAK_CNT       (5000)
#define WAIT_TSF_STABLE_CNT             (50)
#define WAIT_TSF_STABLE_ONCE_TIME       (20)
#define TSFIS32K                        1
#define TSFIS40M                        0
#define GET_TSF_STATE() ((ReadMACRegByte(REG_PLAYFORM_CLOCK) & CLK_STABLE_32K) ? TSFIS32K : TSFIS40M)

#define U1_ENABLE               BIT3
#define U2_ENABLE               BIT4

#define TPM_RESTORE             0
#define TPM_TEMP                0
/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

typedef struct _RF_DLPS_Parm_ {
    u32      X2K_DATA1       :1; //XTALX2_EN_DELAY              0xC8[18]
    u32      X2K_DATA2       :1; //XTALX2_DELAY_DIR            0xC8[17]
    u32      X2K_DATA3       :4; //SYN_X2_DELAY[3:0]            0xC8[16:13]
    u32      X2K_DATA4       :3; //SYN_X2_DELAY_FINE[2:0]   0xC8[5:3]
    u32      SYN_AAC_DATA1   :2; //SYN_VCO_SWCAP_MSB_2bit 0xA2[7:6]
    u32      SYN_AAC_DATA2   :5; //VCO_IB[4:0]                       0xA3[19:15]
    u32      SYNLP_AAC_DATA1 :2; //SYNLP_VCO_SWCAP_MSB_2bit 0x95[7:6]
    u32      SYNLP_AAC_DATA2 :5; //VCOLP_IB[4:0]                       0x96[19:15]
    u32      RCK_DATA        :5; //RCK_RXBBIN[4:0]                0x1B[19:15]
    u32      rsvd0           :4;
    u32      LOK_DATA        :12;//BTG_TXMOD_IDAC_IS[5:0] BTG_TXMOD_IDAC_QS[5:0]
    u32      rsvd1           :4;
    u32      LCK_DATA1       :8; //SYN_VCO_SWCAP[7:0]
    u32      LCK_DATA2       :8; //SYNLP_VCO_CAP[7:0]
}RF_DLPS_Parm, *PRF_DLPS_Parm;

typedef struct _RF_DLPS_Parm2_ {
    u32      RF_Reg1;
    u32      RF_Reg2;
}RF_DLPS_Parm2, *PRF_DLPS_Parm2;


/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/

extern void
PauseRxDMA(
    IN u8 PauseOrNot,
    IN u8 rcode
) ;


//-------------------------------------------------------
//
//      Power Control Function
//
//-------------------------------------------------------

extern BOOLEAN
WaitTxStateMachineOkCommon(
    void
);

extern void
PsOpenRFCommon(
    void
);

extern void
PsCloseRFCommon(
    void
);

//-------------------------------------------------------
//
//      32K Power Control Function
//
//-------------------------------------------------------

extern u8
GetRPWMValueCommon(
    void
);

extern u16
GetRPWMValue2Common(
    void
);

extern void
CheckTSFIsStableCommon(
    IN  u8  ReqState
);

extern void
WaitHWStateReadyCommon(
    void
);

extern void
SysClkDownCommon(
    void
);

extern void
SysClkUpCommon(
    void
);

void
LowLPSOff(
    u8 LowLPSEnable
);

BOOLEAN
CheckTxQueueEmpty(
    void
);

extern void 
SetTxPauseFWInfo(
    IN u8 BitMask, 
    IN u8 UserValue
);


extern void
WriteMACIDPause(
    IN u8 index,
    IN u8 mask,
    IN u8 value,
    IN u8 rcode
);

void 
SetMACIDPauseFWInfo(
    IN u8 Index, 
    IN u8 BitMask, 
    IN u8 UserValue
);

extern BOOLEAN 
PS_32K_Condition_Match(
    void
);


extern void
PsOpenRF_8721D(
    void
);

extern void
PsCloseRF_8721D(
    void
);

extern void
SysClkDown_8721D(
    void
);

extern void
SysClkUp_8721D(
    void
);

extern void
LowLPSOff_8721D(
    u8 LowLPSEnable 
);    

extern BOOLEAN
CheckTxQueueEmpty_8721D(
    void
);

extern BOOLEAN
WaitTxStateMachineOk_8721D(
    void
);

extern void
WriteTxPause_8721D(
    IN u8 value,
    IN u8 rcode
) ;

extern void
WriteTxPauseWithMask_8721D(
	IN u8 mask,
	IN u8 value,
	IN u8 rcode
);

#endif  //__RTL8721D_POWERCTRL_RAM_H__

