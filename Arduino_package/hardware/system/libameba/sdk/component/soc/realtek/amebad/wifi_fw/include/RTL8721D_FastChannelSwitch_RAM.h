#ifndef __HALCOM_FASTCHANNELSWITCH_RAM_H__
#define __HALCOM_FASTCHANNELSWITCH_RAM_H__

#if !IS_CHIP_8192F(CONFIG_CHIP_SEL)

/*--------------------Define --------------------------------------------*/
#define MCC_STATE_CHANNEL_READY     0
#define MCC_STATE_TXNULL            1
#define MCC_STATE_READY_TO_SWITCH   5
#define MCC_STATE_NO_SWITCH_CH      9


//#define FCS_TXNULL_TH               0// 3

#define MCC_MAX_SCAN_CH_NUM         35
#define FCS_MAX_CHIFO_NUM           3

#define MCC_RETRY_NULL_INTERVAL     3
#define MCC_GO_STA_SWRTY_MAX        6
#define MCC_GC_STA_SWRTY_MAX        1

#define MCC_INTV_100MS              0x19000
#define MCC_INTV_200MS              0x32000

//This Value should be fine tune in the future.....
//In GO/STA mode, the period before NOA On to TxNull pkt
//#define MCC_GCSTA_PRENOAON_PERIOD   17 //isr_p2prfoff to switch channel done:10ms in 8723bs, txnull(1) retry reserve 7ms  
#define MCC_GCSTA_PRENOAON_PERIOD   4//5 //reserved for switch channel & txnull(1) 3 times

#define MCC_CTS2SELF_NAV_MAX        31
#define MCC_NOAON_SWCH_LATENCY      2 //set this latency to match switch channel duration.

#define MCC_USERIDX0                0
#define MCC_USERIDX1                1


#define CONFIG_MCC_DEBUG_NOTXNULL   0
/*--------------------Define Enum----------------------------------------*/
typedef enum _MCC_STATUS_
{
    MCC_RPT_SUCCESS                 = 0,
    MCC_RPT_TXNULL_FAIL             = 1,
    MCC_RPT_STOPFCS                 = 2,
    MCC_RPT_READY                   = 3,
    MCC_RPT_STOPFCS_FAIL            = 4,
    MCC_RPT_MCC_ENABLE_FAIL         = 5,
    MCC_RPT_MCC_SWITCH_CH_FAIL      = 6,
    MCC_RPT_MCC_SWITCH_EARLY_RPT    = 7,
    MCC_RPT_BCN_EARLY               = 8,
}MCC_STATUS, *PMCC_STATUS;

typedef enum _MCC_C2HTYPE_
{
    C2HRPT_DISABLE                  = 0,
    C2HRPT_RPT_FAIL                 = 1,
    C2HRPT_RPT_ALL                  = 2,
    
}MCC_C2HTYPE, *PMCC_C2HTYPE;

typedef enum _MCC_ROLE_
{
    MCC_ROLE_STA                    = 0,
    MCC_ROLE_AP                     = 1,
    MCC_ROLE_GC                     = 2,
    MCC_ROLE_GO                     = 3,
    
}MCC_ROLE, *PMCC_ROLE;

typedef enum _MCC_MODE_
{
    MCC_MODE_GO_STA                 = 0,
    MCC_MODE_AP_STA                 = 1,
    MCC_MODE_GC_STA                 = 2,
    
}MCC_MODE, *PMCC_MODE;


/*--------------------Define MACRO---------------------------------------*/
#define ALL_ACTION  0
#define ACTION      1
#define SLEEP       2

#if CONFIG_FCS_LOGO

/*--------------------Define Struct----------------------------------------*/
typedef struct _macid_info_{
    u16   Addr;
    u8    Offset;
} MacidInfo, *PMacidInfo;

typedef struct _channel_info_ {
    u8          Chidx;
    u8          BW:2;
    u8          BW40SC:3;
    u8          BW80SC:3;
    //u8          Duration;   // UNIT:8ms
    MacidInfo   Macid[2];
    u8          TxNull1:1;
    u8          TxNull1Ok:1;
    u8          DisTxNull:1;
    u8          ChannelScan:1;
    u8          rsvd:4;
    u8          Durationl;
    u8          Durationh;
    u8          Count;
    //u8    Queuesel;
    u8          loc_Nulldata;
    u8          ScanCh_Rsvdpage_loc;

    // Scan Channel set format
    //u8          ChSetLen; // total channel number
    //u8          ChidxSet[MCC_MAX_SCAN_CH_NUM];    
} ChannelInfo, *PChannelInfo;

#endif  //#if CONFIG_FCS_LOGO

#if CONFIG_MCC 

/*--------------------Define Struct----------------------------------------*/
typedef struct _MCC_IQK_INFO_ {
    u16         IQK_RX_X;
    u16         IQK_RX_Y;
    u16         IQK_TX_X;
    u16         IQK_TX_Y;

} MCC_IQK_INFO, *PMCC_IQK_INFO;

typedef struct _MCC_CH_INFO_ {
    //h2c cmd 0x11, Byte1~4
    u8          Chidx;
    u8          BW:2;
    u8          BW40SC:3;
    u8          BW80SC:3;
    u8          Duration;
    u8          Role:3;
    u8          InCurCh:1;
    u8          DisSwRetry:1;
    u8          MaxSwRetryIdx:1;
    u8          rsvd:2;
    //h2c cmd 0x11, Byte7[7]
    u8          ChannelScan:1;
    u8          rsvd1:7;
    //h2c cmd 0x16
    u8          LowBitMap;
    u8          HighBitMap;

    
    // not h2c cmd format
    u8          TxNull1:1;// 1: Tx Null(1) is done,  0: Tx Null(1) is not yet start.
    u8          TxNull1Ok:1;// 1: Tx Null(1) is done and ok.
    u8          TxNull1OkLate:1;//Tx Null (1) ok after ISR_P2PRFON    
    u8          rsvd2:5;
    u8          Count;
    //u8    Queuesel;
    u8          loc_packet;
    u8          ScanCh_Rsvdpage_loc;

    MCC_IQK_INFO    IQKInfo[RF90_PATH_MAX];

    // Scan Channel set format
    //u8          ChSetLen; // total channel number
    //u8          ChidxSet[MCC_MAX_SCAN_CH_NUM];    
} MCC_CH_INFO, *PMCC_CH_INFO;


#endif //#if CONFIG_MCC 

#if CONFIG_ECSA
typedef struct _ECSA_INFO_ {
    u8          channel;

    u8          BW:2;
    u8          BW40sc:3;
    u8          BW80sc:3;

    u8          RFEType:4;
    u8          ECSA_en:1;
    u8          rsvd:3;
    
    u8          target_band;
    u8          current_band;
} ECSA_INFO, *PECSA_INFO;
#endif // #if CONFIG_ECSA

typedef struct _fcs_info_ {
#if CONFIG_FCS_LOGO
    u8          LogoEnable:1;
    u8          Logorsvd:7;
#endif  //#if CONFIG_FCS_LOGO

    u8          Enable:1;
    u8          Idx:3;   //now user idx
    u8          RFEType:4;
    u8          TotalNum;
    u8          Period;
#if CONFIG_FCS_LOGO
    ChannelInfo Chifo[FCS_MAX_CHIFO_NUM];
#endif  //#if CONFIG_FCS_LOGO

#if CONFIG_MCC     
    MCC_CH_INFO MccChifo[FCS_MAX_CHIFO_NUM];
    u8          PreIdx;
//    u8          TxNullCnt;
    u8          StopFCS:1;    
    u8          C2hrpt:2;
    u8          NotFirstPeriod:1;
//    u8          NoChangeCH:1; //it means this period is keep in same channel due to txnull(1) fail.

#if CONFIG_MCC_DEBUG_NOTXNULL
    u8          DisTxNull:1;
    u8          rsvd:1;
#else
    u8          rsvd:2;
#endif

    u8          IQK_PathA:1;
    u8          IQK_PathB:1;

//    u8          NullDelayTime;//Null pkt use period
//    u16         Null1FailCnt;
//    u16         Null1LateCnt;
    
    // Scan Channel set format, support one set for scan
    u8          ChSetLen; // total channel number
    u8          ChidxSet[MCC_MAX_SCAN_CH_NUM];  
    u8          ChSetIdx[FCS_MAX_CHIFO_NUM]; //ChidxSet[ ChSetIdx[i] ]
    
//    u32         TIME0_TSFL;
//    u32         TIME0_TSFH;
//    u8          Interval;

    u8          MCCTSFSyncEnable:1;//from h2c
    u8          TSFoffset:7;
    u8          MCCStart;
    u8          MCCInterval;
    u8          MCCEarlySwitch;    
    u8          TSFSyncFirstDone:1;
    u8          TSFSyncCount:5;
    u8          b1Rsvd:1;
    u8          IQK_Ready:1;
    u8          MCCIntervalCnt;
    u16         NOAOffCnt;//use to count the switch times
//    u8          SwChTRXBcn;
    u8          StopTxNull;
    u8          NullSwRtyCnt;
    u8          MaxSwRetryCnt;
//    u8          MCCMode;//0:GO+STA, 1:GC+STA, 2:AP+STA

    u8          MCCUpdateEn:1;
    u8          MCCIdx0Duration:7;

    u8          MCCUpdateTSF:1;
    u8          MCCUpdateTSFOffset:7;

    u8          MCCUpdateStTime;
    u8          MCCUpdateDur0;
    u8          MCCUpdateDur1;
    u8          MCCUpdateIntvl;
#endif  //#if CONFIG_MCC     

#if CONFIG_ECSA
    ECSA_INFO   ECSAInfo;
#endif // #if CONFIG_ECSA
} FCSInfo, *PFCSInfo;

typedef struct _ReqChannel_ {
    u8 ChannelNum;
    u8 En_RFE;
    u8 RFEType;    
 } ReqChannel, *PReqChannel;

 
/*--------------------Export global variable----------------------------------*/

/*--------------------Function declaration-----------------------------------*/
extern void 
InitFCS(
    void
) ;

extern u8
FCSInitScanCHSet(
    IN u8  order
);

extern void
FCSGetDelayTime(
    u8  ind     // 0: start, 1:end
) ;

extern void
MCCTimeOutHDL(
    void
) ;

extern void
MCCBcnEarlyHDL(
    void
);

extern void
MCCStateHDL(
    void
);

extern void 
MCCTxNull1HDL(
    void
);

extern BOOLEAN
MCCDefaultSetting(
    u8 useridx
) ;//;

extern BOOLEAN
MCCMacidAction(
    u8  action,
    u8  useridx
) ;

extern 
void 
C2HFCSStatus(
    IN  u8  status
) ;

extern void
FastChannelSwitchTimeOutHDL(
    void
);

extern void
FCSTimerOut0HDL(
    void
) ;

extern void
MCCTimerOut0HDL(
    void
) ;

extern void
FCSDefaultSetting(
    u8 useridx
) ;//;

extern void
MACIDAddrNOffsetDecision(
    u8 value,
    u8 useridx,
    u8 idx
) ;//;

extern void
MACIDAction(
    u8  action,
    u8  useridx
) ;

extern void 
FCSTxNull1HDL(
    void
);

extern void 
MCCSwChForTRXBcn(
    IN u8   bcnerlychk
) ;

extern void 
H2CHDL_Switch_channel_offload(
    u8 *pbuf
) ;   

void 
InitDebugFCS(
    void
) ;

void
FCSDebug(
    IN u8 useridx
) ;

extern void
ECSATimeoutHDL(
    void
) ;

extern void
ECSANullHDL(
    BOOLEAN bPowerBit
) ;

#endif      //#if !IS_CHIP_8192F(CONFIG_CHIP_SEL)

#endif  //__HALCOM_FASTCHANNELSWITCH_RAM_H__

