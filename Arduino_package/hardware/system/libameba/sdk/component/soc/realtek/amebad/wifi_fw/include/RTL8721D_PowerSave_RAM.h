#ifndef __WLAN_POWERSAVE_RAM_H__
#define __WLAN_POWERSAVE_RAM_H__


/*--------------------------Define -------------------------------------------*/
#define MACID_CLIENT        0
#define MAX_CFG_FILE        5
/*------------------------------Define Enum-----------------------------------*/
#if 0
enum PS_MODE_TYPE
{
    PS_MODE_ACTIVE          = 0,
    PS_MODE_MIN,
    PS_MODE_MAX,
    PS_MODE_SELF_DEFINED,
    PS_MODE_NUM
};
#endif

typedef enum _SMART_PS_MODE_FOR_LEGACY_
{
    SMART_PS_MODE_LEGACY_PWR1           = 0,        // TRX all use PS_POLL
    SMART_PS_MODE_TX_PWR0               = 1,        // TX: pwr bit = 0, RX: PS_POLL
    SMART_PS_MODE_TRX_PWR0              = 2         // TX: pwr bit = 0, RX: NULL(0)
}SMART_PS_MODE_FOR_LEGACY, *PSMART_PS_MODE_FOR_LEGACY;


typedef enum _SMART_PS_MODE_FOR_UAPSD_
{
    SMART_PS_MODE_UAPSD                 = 0,
    SMART_PS_MODE_UAPSD_TX_QOSNULL      = 1
}SMART_PS_MODE_FOR_UAPSD, *PSMART_PS_MODE_FOR_UAPSD;



enum PS_MODE_OBJECTIVE
{
    PS_MODE_OBJECTIVE_THROUGHPUT        = 0,
    PS_MODE_OBJECTIVE_BATTERY
};

typedef struct _PS_TUNING_PPARM2_ {
    u8      adopt:1;
    u8      ps_bcn_to:7;                
    u8      ps_drv_early_itv;   
}PS_TUNING_PPARM2, *PPS_TUNING_PPARM2;

typedef struct _SAPPS_PARM_ {
    u8      Enable:1;
    u8      EnPS:1;
    u8      EnLPRx:1;
    u8      Manual32k:1;    //revised by isaac on 20141202
	u8      Active:1;
    u8      FlagToWaitHostCutPower:1;
    u8      RF2Path:1;
    u8      rsvd:1;
    u8      Duration;
    u8      Duration_On;
    u8      Duration_Off;
    u8      AntSwitchOutside;
    u8      TwoAnt;
    u8      AntSelReverse;
    u8      TxPathCtrl;
}SAPPSParm, *PSAPPSParm;

typedef struct _INACTIVEPS_Parm_ {
    u8      Enable:1;
    u8      IgnorePsCondition:1;
    u8      rsvd:6;
    u8      Frequency;
    u8      Duration;
    u8      FrequencyCount;
}INACTIVEPSParm, *PINACTIVEPSParm;

typedef struct _NOLINK_Parm_ {
    u8      Enable:1;
    u8      rsvd:7;
    u8      CurrentFTIMR[4];
    u8      CurrentWLANIMR[4];

}NOLINKPSParm, *PNOLINKPSParm;

typedef struct _PG_PARM_ {

    u8      Enable:1;
    u8      rsvd:7;

}PG_PARM, *PPG_PARM;

typedef struct _PS_BEADJ_V1_{
    u8      curBcnEarly;        //unit 32us
    u8     minBcnEarly;     //unit 32us
    u16     minDeltaTSF;        //unit 1us
    u16     maxDeltaTSF;       //unit 1us
    u8     maxBcnTo;           //unit 1ms
    u16     curDeltaTSF;        //unit 1us
    u8      bToFlag;
    u8      count;
    u8      rxCount;
    u8      toCount;
    u8      bTryFlag;
    u8      adjStep;            //unit 32us
    u8      toAdjStep;          //unit 32us
    u8      rxCntLimit;
    u8      rxCntForTo;
    u16     tmpBcnTo;          //unit 32us
    //u8      goldBcnEarly;       //unit 32us
}PS_BEAdj_V1, *PPS_BEAdj_V1;

typedef struct _Bcnearly_adjust_PID_{
	u32 DeltaTSFSum;  //unit us
	u32 PerDeltaTSF;  //unit us
	u32 PerError;
	u32 Error;
	u32 Per1_TSFOnRxBcn;
	u32 Per2_TSFOnRxBcn;
	u16 Pinc;
	u16 Pbase;	
	u8  PbaseGetTime;
	u32 PbaseSum;
	u16 PbasePart;
	u8  Output;
	u8  LimitOutput;  //unit 128us
	u8  LimitOPGetTime;
	u8  LimitOutputSetDown;
	u32 LimitMinDeltaTSF;
	u32 LimitMaxDeltaTSF;
	u8  MaxOutput;   //unit 128us
	u8  PbaseGetting;
	u8  BcnLossCount;
	//u16 TSFRxBCNOffset;
}BCNEARLY_ADJUST_PID, *PBCNEARLY_ADJUST_PID;

/*------------------------Export global variable------------------------------*/

/*------------------------------Function declaration--------------------------*/
#if (!(IS_CHIP_8723D(CONFIG_CHIP_SEL) || IS_CHIP_8192F(CONFIG_CHIP_SEL)))&&(!(IS_CHIP_8710B(CONFIG_CHIP_SEL) || IS_CHIP_8721D(CONFIG_CHIP_SEL)))
extern void 
SetSmartPSTimer(
    void
) ;

extern void 
SetPwrStateReg(
    IN u8 PwrStateType,
    IN u8 value
);

extern void 
Change_PS_State(
    IN  u8,
    IN  u8
) ;

extern void
PS_S2ToS0State(
    void
);

#endif

extern void 
InitPS(
    void
) ;

extern void 
PSSetMode(
    PLEGACY_PS_PARM pparm
) ;
//20170712 Fix TIM Error do not clear 608[18]
#if (IS_CHIP_8710B(CONFIG_CHIP_SEL)&&IS_CATEGORY_NIC_IoT(CONFIG_CATEGORY_SEL))
extern void 
PSModeSetting_8710B_RAM(
    IN u8 on
) ;
#elif (IS_CHIP_8721D(CONFIG_CHIP_SEL)&&IS_CATEGORY_NIC_IoT(CONFIG_CATEGORY_SEL))
extern void 
PSModeSetting_8721D_RAM(
    IN u8 on
) ;
#endif

extern void 
PS_OnBeacon(
    void
) ;

extern void 
ChangePSStateByRPWM(
    void
);

extern BOOLEAN
ChkTxQueueIsEmpty(
    void
) ;

extern void 
PSBcnEarlyProcess(
    void
);

extern void 
PSBcnAggEarlyProcess(
    void
);

extern void 
PsBcnToProcess(
    void
) ;

extern void 
PSBcnEarlyC2H(
    BOOLEAN bDTIM
) ;

extern void
KeepRfOnForReceiving(
    void
);    


extern void 
TxPktInPSOn(
    void
);

extern void
PS_S0ToS1State(
    void
);

void
PS_S0ToS6State(
    void
);

void
PS_S6ToS0State(
    void
);



extern void 
SmartPS2InitTimerAndToGetRxPkt_8721D_ROM(
    void
) ;

extern void
PSSleepTo32K(
    void
);

extern void 
PS_RFoffTo32KToRFoffState(
    void
) ;

void
PSRxBcnProcess(
    void
);

void 
PS_S4ToS2State(
    IN  u8  ReleaseTxPause
) ;



void 
PS_S5ToS2State(
    void
) ;

void 
InitSAPPS(
    void
) ;

void
SAPPSTBTTHDL(
    void
) ;

void
SAPPSTimeOutHDL(
    void
) ;

void
SAPPSEarlyHDL(
    void
) ;

void 
InitInactivePS(
    void
) ;

void 
InactivePSTBTTHDL(
    void
) ;

void
InactivePSBCNAggEarlyHDL(
    void
) ;

void
InactivePSEnableHEL(
    IN u8 op
) ;

void
NoLinkPSEnableHEL(
    IN u8 op
) ;

#if (!IS_CHIP_8703B(CONFIG_CHIP_SEL))&&(!IS_CHIP_8188F(CONFIG_CHIP_SEL))
#if 0
void 
SetBcnEarlyAndTimeout(
IN u8 BcnAheadShift,IN u8 BcnEarlyShift,IN u8 BcnTimeout,IN u8 ReasonCode
);
#endif
#else
extern void 
SetBcnEarlyAndTimeout(
IN u8 BcnAheadShift,IN u8 BcnEarlyShift,IN u8 BcnTimeout
);
#endif //#if !CONFIG_ROM_32K

#if (!(IS_CHIP_8723D(CONFIG_CHIP_SEL) || IS_CHIP_8192F(CONFIG_CHIP_SEL)))&&(!(IS_CHIP_8710B(CONFIG_CHIP_SEL) || IS_CHIP_8721D(CONFIG_CHIP_SEL)))

void
ResetBcnEarlyAdjustParm(
IN	BOOLEAN OnJoin
);

#endif

void
InitBcnEarlyAdjustParm(
    void
);

void 
BcnEarlyAdjustByTSF(
    void
);

void 
InitBcnEarlyAdjustPID(
	void
);

void
BcnEarlyAdjustByDeltaTSFPID_8721D(
    void
);

#if (!(IS_CHIP_8710B(CONFIG_CHIP_SEL) || IS_CHIP_8721D(CONFIG_CHIP_SEL)))
void 
EnlargeBcnEarlyAndTimeout(
void
);
#endif

void 
RestoreNoLinkPSHEL(
void
) ;

void
LowPowRx(
    u8 enable
) ;

void
SAPLPS(
    void
) ;

extern void 
LowPowerRxBeacon(
    BOOLEAN on 
);

extern void 
SetBcnEarlyAndTimeout_8721D_RAM(
    u8 BcnAheadShift,
    u8 BcnEarlyShift,
    u8 BcnTimeout,
    u8 ReasonCode
);


#if 0
void 
BcnEalyAdjust(
    void
);

void 
ResetBcnEalyAdjust(
    void
);

void 
SelectBcnEalyAdjustPosition(
    void
);

void 
ResetBcnCounter(
    void
);

void 
SetBeaconEarlyTime(
    void
);

u8
SelectMaxEarly(
    u8 SelectNum
);
#endif
#endif  //__WLAN_POWERSAVE_RAM_H__

