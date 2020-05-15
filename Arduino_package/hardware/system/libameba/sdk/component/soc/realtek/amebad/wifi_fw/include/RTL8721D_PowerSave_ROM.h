#ifndef __WLAN_POWERSAVE_H__
#define __WLAN_POWERSAVE_H__


/*--------------------Define --------------------------------------------*/

#define MACID_CLIENT        0

/*
    TIMER allcation
*/
#define TIMER_TC0_CTRL          REG_TC0_CTRL    //instead of PowerTranning
#define TIMER_TC1_CTRL          REG_TC1_CTRL    //by PS or APOffload(AP mode)
#define TIMER_TC2_CTRL          REG_TC2_CTRL    //by PS
#define TIMER_TC3_CTRL          REG_TC3_CTRL    //Rsvd for driver
#define TIMER_TC4_CTRL          REG_TC4_CTRL    //Rsvd for driver
#define TIMER_TCUNIT            REG_TCUNIT
#define TIMER_TC5_CTRL          REG_TC5_CTRL    //by BT i2c timeout ( 92e only)
#define TIMER_TC6_CTRL          REG_TC6_CTRL    //by BT Time Division

#define COUNTER_P2PCTW          TIMER_TC0_CTRL
#define COUNTER_BCNTO           TIMER_TC1_CTRL  // counter used by BCNTO is the same with DTIM
#define COUNTER_DTIM            TIMER_TC1_CTRL
#define COUNTER_PSTRX           TIMER_TC2_CTRL  

#define COUNTER_MCC_TXPKT       TIMER_TC1_CTRL
#define COUNTER_MCC_SWRETRY     TIMER_TC2_CTRL
#if 1 //CONFIG_MIFI_PLATFORM
#define COUNTER_WAITHOST        TIMER_TC3_CTRL
#endif      //#if CONFIG_WAIT_HOST_SUSPEND_GTIMER

#if 0 //CONFIG_FW_SND
      #define COUNTER_SOUNDING        TIMER_TC3_CTRL
#else
    //#define COUNTER_    TIMER_TC3_CTRL
#endif
/*
    PS_RX_INFO[7:0]: Power Save RX Information Register
    initial value: 0x00
    REG III.220 (Offset 0x 0692h) PS_RX_INFO Register Definition    
*/
#define RXDATAIN0               BIT0  //PSTX
#define RXDATAIN1               BIT1  //PSRX
#define RXDATAIN2               BIT2
#define RXMGTIN0                BIT3
#define RXCTRLIN0               BIT4

//Power Save Tuning Parameter
#define BCN_RX_ON_DELAY         2        //Beacons are transmitted in 1MHz, then 1bit per 1microsecond. If 2ms are added, it prevents from losing approximately 250bytes in worst case.
#define BCN_EARLY_32K_MARGIN    2        // 1ms would be enough, but 2ms is better for multicast receiving in open space
#define BCN_EARLY_DRV_MARGIN    1        // 1ms would be better in open space

#define BCN_RECEIVE_32K_BUFFER  (BCN_RX_ON_DELAY+BCN_EARLY_32K_MARGIN)
#define BCN_RECEIVE_DRV_BUFFER  (BCN_RX_ON_DELAY+BCN_EARLY_DRV_MARGIN)

#define BCN_DISTRIBUTION        6  //5ms ,generally is 5 ms

#define DEFAULT_BCN_TO_PERIOD   (BCN_RECEIVE_32K_BUFFER+BCN_DISTRIBUTION)  //choose the bigger one


#if 0 //IS_CATEGORY_WOWLAN(CONFIG_CATEGORY_SEL)

#define DEFAULT_BCN_TO_LIMIT    3 // 1

#else
#define DEFAULT_BCN_TO_LIMIT    2  // 1

#endif
#define DEFAULT_BCN_TO_TIMES_LIMIT  2 // 20140806
#define DEFAULT_DTIM_TIMEOUT    15  // 7         // 7 ms
#define DEFAULT_PS_TIMEOUT      20  // 20       // 20 ms
#define DEFAULT_PS_DTIM_PERIOD  7
#if 0 //IS_CATEGORY_AP(CONFIG_CATEGORY_SEL)
#define DEFAULT_PS_DRV_EARLY    5
#else
#define DEFAULT_PS_DRV_EARLY    2
#endif
#define PS_DRV_BCN_SHIFT_MAX   (DEFAULT_PS_DRV_EARLY-1)

#if 0 //IS_CATEGORY_AP(CONFIG_CATEGORY_SEL)
#define DEFAULT_32K_EARLY(x)       (5+(x))
#else
#define DEFAULT_32K_EARLY(x)       (4+(x))
#if CONFIG_BCNEARLY_ADJUST
#define DEFAULT_32K_EARLY_RAM(x)       (3+(x))
#elif CONFIG_BCNEARLY_ADJUST_V2
#define DEFAULT_32K_EARLY_RAM(x)       (3+(x))
#endif
#endif

#define BCN_ADVANCE_MAX         10
#define BCN_EARLY_MARGIN        1       //unit 256us
#define RF_ON_COMPENSATION          50//70   1030        //unit 1us
#define BCN_RX_INT_DELAY              90//  70   1030   //unit 1us

#define BCN_DELAY_MAX           10
#define BCN_CALCULATION_MAX     (BCN_ADVANCE_MAX+BCN_DELAY_MAX)

#define BCN_SHIFT_MAX(x)        (3+(x))
//#define BCN_SHIFT_MAX_RAM(x)    (2+(x))
#define BCN_ADJUST_COUNT        100
#define BCN_EXCLUDED_NUM        (BCN_ADJUST_COUNT/20)

#define BCN_ENLARGE_NUM         5

#define SHIFT_COMPENSATION      1
#define BCN_DRIFT_COMPENSATION  2


#define NULL_DATA0_ALLOW        1
#define NULL_DATA0_DENY         0

#define PS_RF_OFF               0
#define PS_GO_ON                BIT0
#define PS_TX_NULL              BIT1
#define PS_RF_ON                BIT2
#define PS_REGISTER_ACTIVE      BIT3
#define PS_ACK                  BIT6
#define PS_TOGGLE               BIT7

#define PS_STATE_MASK           (0x0F)
//#define PS_STATE(x)             (PS_STATE_MASK & (x))
#define PS_IS_TX_NULL(x)        ((x) & PS_TX_NULL )
//#define PS_IS_ACK(x)            ((x) & PS_ACK ) 
#define PS_IS_CLK_ON(x)         ((x) & (PS_RF_OFF |PS_ALL_ON ))
#define PS_IS_RF_OFF(x)         ((x)|PS_RF_OFF)
#define PS_IS_RF_ON(x)          ((x) & (PS_RF_ON))
#define PS_IS_ACTIVE(x)         ((x) & (PS_REGISTER_ACTIVE))

#define PS_STATUS_S0            (PS_REGISTER_ACTIVE | PS_RF_ON)                    //(1,1,0) all on = register active + rf on
#define PS_STATUS_S1            (PS_REGISTER_ACTIVE | PS_RF_ON | PS_TX_NULL)        //(1,1,1) all on + tx null(1)
#define PS_STATUS_S2            (PS_RF_ON)                                         //(0,1,0) register sleep + rf on
#define PS_STATUS_S3            (PS_RF_ON | PS_TX_NULL)                            //(0,1,1) register sleep + rf on + tx null(0)
#define PS_STATUS_S4            0                                                 //(0,0,0) all OFF
#define PS_STATUS_S5            (PS_TX_NULL )                                      //(0,0,1) SCAN = register sleep + rf on + scan enable
#define PS_STATUS_S6            (PS_REGISTER_ACTIVE)                               //(1,0,0) NoA off = register active + rf off

/* DATA FIN Condition Flags */
#define STA_DATA_OPEN           BIT0                                            // indicate that FW open due to TIM = 1 condition. (PS-POLL as trigger frame)
#define BC_DATA_OPEN            BIT1                                            // indicate that FW open due to DTIM = 1 condition.  (BC & MC)
#define QOS_DATA_OPEN           BIT2                                            // indicate that FW open due to UAPSD trigger condition. (QNULL)
    
#define ALL_80211_DATA_OPEN     (STA_DATA_OPEN | BC_DATA_OPEN | QOS_DATA_OPEN)
#define IS_80211_DATA_OPEN(x)   ((x) & ALL_80211_DATA_OPEN)

#define C2H_DATA_OPEN           BIT3                                            // indicate that FW open due to C2H event 
#define IS_C2H_DATA_OPEN(x)     ((x) & C2H_DATA_OPEN)

#define BCN_DATA_OPEN           BIT4
#define APP_DATA_OPEN           BIT5

#define SET_DATA_OPEN(x, type)  ((x) |= (type))
#define CLR_DATA_OPEN(x, type)  ((x) &= (~type))
#define IS_DATA_OPEN(x, type)   ((x) & (type))



//pwr state
#define PS_TYPE_32KPERMISSION       0
#define PS_TYPE_CURRENT_PS_STATE    1
#define PS_TYPE_LASTRPWM            2

/*--------------------Define Enum---------------------------------------*/
#if 1

typedef enum _RxListenBeaconMode_
{
    RLBM_MIN                =0,
    RLBM_MAX,
    RLBM_SELF_DEFINED

}RxListenBeaconMode, *PRxListenBeaconMode;

typedef enum _RT_MEDIA_STATUS{
    RT_MEDIA_DISCONNECT     = 0,
    RT_MEDIA_CONNECT        = 1
}RT_MEDIA_STATUS;
#endif

typedef enum _PS_MODE_SETTING_SELECTION_
{
    MODE_SETTING_ACTIVE     = 0,
    MODE_SETTING_LEGACY     = 1,
    MODE_SETTING_WMMPS      = 2
    
}PS_MODE_SETTING_SELECTION, *PPS_MODE_SETTING_SELECTION;

typedef struct _LEGACY_PS_PPARM_ {
    u8      ps_mode:7;
    u8      ClkRequestEnable:1;
    u8      RLBM:4;             //RX Listen BCN Mode
    u8      smart_ps:4;
    u8      AwakeInterval;      //Unit: beacon interval, this field is only valid in PS Self-Defined mode
    u8      bAllQueueUAPSD:1;     // 1: all queue are uapsd 0: not all queue are uapsd
    u8      bMaxTrackingBcnMode:1;
    u8      bBCNEarlyReport:1;
    u8      XtalSel:1;
    u8      rsvd:4;
#if 1 //CONFIG_FAST_CPWM
    u8      PwrState;
#else
    u8      permission32k:1;
    u8      rsvd1:7;
#endif

    u8      LowPwrRxBCN     :1;
    u8      AntAutoSwitch   :1;
    u8      PSAllowBTHighPri:1;
    u8      ProtectBCN      :1;
    u8      SilencePeriod   :1;
    u8      FastBTConnect   :1;
    u8      TwoAntennaEn    :1;
    u8      rsvd2           :1;
    u8      AdoptUserSetting:1;
    u8      DrvBcnEarlyShift :3;
    u8      DrvBcnTimeOut :4;

}LEGACY_PS_PARM, *PLEGACY_PS_PARM;

typedef struct _PS_TUNING_PPARM_ {
    u8      BCNTOLimit;             
    u8      DTIMTimeOut;    
    u8      Adopt:1;
    u8      PSTimeOut:7;                        
    u8      PS_DTIMPeriod;
    // added by jackie for reserved
    u8      rsvd1;
    u8      rsvd2;
    u8      rsvd3;
}PS_TUNING_PPARM, *PPS_TUNING_PPARM;

typedef struct _PS_BT_Parm_ {
    u8      PspollTxFlag;
    u8      LowPwrRxBCN     :1;
    u8      AntAutoSwitch   :1;
    u8      PSAllowBTHighPri:1;
    u8      ProtectBCN      :1;
    u8      SilencePeriod   :1;
    u8      FastBTConnect   :1;
    u8      TwoAntennaEn    :1;
    u8      LowPwrRxBCNLeave:1;
}PS_BT_Parm, *PPS_BT_Parm;
/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

typedef struct _BEACON_ADJUST_PARM_ {

	u8    BcnReceive32KBuffer;          //unit 128us
	u8    BcnReceiveDrvBuffer;          //unit 128us
	u16   BcnAdjustCount;
	u8    BcnExcludedNum;
	u8    BcnEnlargeNum;

}BEACON_ADJUST_PARM, *PBEACON_ADJUST_PARM;

typedef struct _PS_PARM_ {

    u8      Enter32KHzPermission:1;
    u8      bAllQueueUAPSD:1;   
    u8      ps_dtim_flag:1;             // indicate dtim of current beacon.
    //u8      pstx_rxcnt_period:1;        //filen:set to indicates that it is a period to receive any "data frame"
    //u8      psrx_rxcnt_period:1;        //filen:set to indicates that it is a period to receive any "data frame"
    u8      pstrx_rxcnt_period:1;
    u8      NoConnect32k:1;
    u8      ack_last_rpwm:1;
    u8      TxNull0:1;
    u8      TxNull1:1;
    
    u8      TxNull0ok:1;
    u8      TxNull1ok:1;
    u8      RfOffLicenseForBCNRx:1;     //filen: After we received ps_bcn_cnt beacons, we can sleep(rf off).
    u8      BCNAggEn:1;
    u8      IsGoingTo32K:1;    
	u8      bMaxTrackingBcnMode:1;
    //chiaoling, 20140716
    //u8      rsvd:2;
    u8      BcnTraceDone:1;
    u8      BcnEarlyPeriodFlag:1;
	u8      BcnEarlySettingFlag:1;
	u8      bBCNEarlyReport:1;
    u8        BcnEnter32kMode:1;
    u8        XtalSel:1;
	u8      rsvd:4;


        
/*
    filen: to indicate whether it is smart power saving or not
    0: Legacy PS 
    1: Smart PS(RX use ps_poll)
    2: Smart PS (RX use null_data(0))
*/
    u8      smart_ps:4;                 //enum SMART_PS_MODE
    u8      RLBM:4;                     // RX BCN MODE (min, max, active, ...)
    u8      AwakeInterval;
    u8      ps_mode;                    // ps type (avtive, legacy, wmmps)
    u8      ClkRequestEnable;
    u8      last_rpwm;
    u8      current_ps_state;
    u8      ps_data_open;
    u8      ps_bcn_pass_time;           // fw will only report one beacon information to driver after ps_bcn_pass_time ms. Unit: 100ms

    u8      ps_dtim_period;
    u8      ps_dtim_cnt;
    u8      ps_bcn_to;                  //unit 128us        // beacon timeout (ms).
    u8      bcn_to_cnt;                 // indicate the total number of contnuous BCN_TO we have received.
    u8      bcn_to_times_cnt;                 //20140806
//    u8      lps_control;
    u16     ps_drv_early_itv;           // Current intritv setting = 2ms
    u32     RFECtrl;
    u8      BcnAheadShift;          //unit 128us
    u8      BcnEarlyShift;          //unit 128us
    u8      BcnEarlyShiftMax;           //unit 128us	
	u8      DefaultBcnEarly;            //unit 128us
    u8      RxBcnCount;
    u8      TBTTCount;
    u8      CurrentEarly;           //unit 128us
    u8      CurrentTimeOut;         //unit 128us
    u8      ReachBcnLimitCount;
    u8      BcnDelayInAheadGroupOfAP;
    u8      BcnDelayInRearGroupOfAP;
    u8		XtalDelay;
    u8      BcnDelay[BCN_CALCULATION_MAX<<3];
    u8      DelayRem[BCN_CALCULATION_MAX<<3];
    union{
        u8      BcnDurationArray[2];
        u16     BcnDuration;
    }BcnDurationUnion;    
    u32     TSFOnTBTT;              //unit in TU
    //ben modify for open/close RF speedup.
     union{
    u8      TSFOnRxBcnArray[4];
    u32     TSFOnRxBcn;
    }TSFOnRxBcnUnion;
    union{
    u8      TSFOnBcnEarlyArray[4];
    u32     TSFOnBcnEarly;
    }TSFOnBcnEarlyUnion;


    
    BEACON_ADJUST_PARM BeaconAdjustParm;

        
#if 1   //!IS_CUT_A(CONFIG_CHIP_SEL)
    u8      AckNull0;    
#endif

    u8 Fix_Delay_forward;
    u8 bTIM_Flag;
    u8 Null0_rty_cnt;
    u8 Null0_rty_lmt;
    u8 BCNNumForDRV;//brian_zhang 20170829,driver deric

    u8 ScanEn;
    u8 DLPS_En;
    u8 DLPS_End_Entry;
}PS_PARM, *PPS_PARM;

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
extern void 
Change_PS_State(
    IN  u8,
    IN  u8
) ;

extern void 
SetPwrStateReg_8721D_ROM(
    IN u8 PwrStateType,
    IN u8 value
);

extern BOOLEAN
ChkTxQueueIsEmpty_8721D_ROM(
    void
) ;

extern void 
ResetPSParm_8721D_ROM(
    void
) ;

extern void 
ConfigListenBeaconPeriod_8721D_ROM(
    IN u8  RLBM,
    IN u8  AwakeInterval
) ;


extern void 
PSModeSetting_8721D_ROM(
    IN u8 on
) ;

extern void 
PSSetMode_8721D_ROM(
    IN PLEGACY_PS_PARM pparm
);

extern void
PS_S1ToS0orS2State_8721D_ROM(
    void
);

extern void 
PS_S4ToS2State_8721D_ROM(
    IN  u8  ReleaseTxPause
) ;

extern void 
PS_S3ToS2orS0State_8721D_ROM(
    void
);

extern void 
PS_S2ToS3State_8721D_ROM(
    IN u8  nulldata0Allow
) ;

extern void
PS_S2ToS0State_8721D_ROM(
    void
);

extern void 
PS_S0ToS1State_8721D_ROM(
    void
);

extern void 
PS_S2ToS4State_8721D_ROM(
    void
) ;

extern void 
PS_S5ToS2State_8721D_ROM(
    void
) ;

extern void
PS_S2ToS5State_8721D_ROM(
    void
) ;

extern void
PS_S6ToS0State_8721D_ROM(
    void
);

extern void
PS_S0ToS6State_8721D_ROM(
    void
);





extern void 
SmartPS2InitTimerAndToGetRxPkt_8721D_ROM(
    void
) ;

#if 0
extern void 
SetBcnEarlyAndTimeout_CallPtr(
    u8 BcnAheadShift,
    u8 BcnEarlyShift,
    u8 BcnTimeout,
    u8 ReasonCode
);
#endif

extern void SetBcnEarlyAndTimeout_8721D_ROM(
    u8 BcnAheadShift,
    u8 BcnEarlyShift,
    u8 BcnTimeout,
    u8 ReasonCode
);

extern void 
ResetBcnEarlyAdjustParm_8721D_ROM(
IN	BOOLEAN OnJoin
);

#if 0 //move to ram by jackie;
extern void 
BcnEarlyAdjustByTSF_8721D_ROM(
    void
);
#endif


extern void 
EnlargeBcnEarlyAndTimeout_8721D_ROM(
void
);

extern void 
ChangePSStateByRPWM(
    void
);

extern void 
SetSmartPSTimer_8721D_ROM(
    void
) ;



void
Legacy_PS_Setting_8721D_ROM(
    void
) ;


#endif  //__WLAN_POWERSAVE_H__

