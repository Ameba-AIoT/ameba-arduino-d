#ifndef __RTL8721D_SYSMIB_H__
#define __RTL8721D_SYSMIB_H__


/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/
//TaskBitMap0
#define RATEADAPTIVE        BIT0
#define H2CEVENT            BIT1
#define C2HEVENT            BIT2
#define RLPSEVENT           BIT3
#define REMOTEWAKEEVENT     BIT4
#define APOFFLOADEVENT      BIT5
#define MAILBOXEVENT        BIT6
#define SWTIMEREVENT        BIT7

//TaskBitMap1
#define BBNHMEVENT          BIT0
#define DBGPKTEVENT         BIT1
#define SIDEBANDWoWLAN      BIT2

/*--------------------Define Struct---------------------------------------*/

//3 1.) Xdata Section
typedef struct _mib_info_ {
    void (*intdis)(void);
    void (*inten)(void);

    //3 Task Bitmap
    //u8                      TaskBitMap;
    u8                      TaskBitMap1;
    u8                      Debug[2];
    
    u8                      TxPauseReasonCode;
    u8                      RxPauseReasonCode;	  //3 

    //3 HAL
    HAL_DATA_8721D          IntHdl;

    //3 Wlan
    //Packet Location
    RsvdPageLoc             RPL;
    H2C_EVT                 H2CEvt;
    C2H_EVT                 C2HEvt;
    u8                      H2C_rptr;

    //u16                     RXFF0_BNDY; 
    //u16                     PageSize_RX;

#if CONFIG_RATE_ADAPTIVE
    //3   ODM
    odm                     ODM;
#if CONFIG_H2C_PKT

    H2C_PKT_EVT     H2CPktEvt;

#endif  //#if CONFIG_H2C_PKT
#endif      //#if IS_CHIP_8192F(CONFIG_CHIP_SEL)


#if CONFIG_AP_OFFLOAD   //Alan
    STRUCT_AP_OFFLOAD       APOffloadCtrl;
    u8                      MBSSID_Num;
    u16                     MBSSID_count;
    hidden_parm             APHiddenCtrl;
    denyany_parm            APDenyAnyCtrl;
    BCN_rsvdpage_loc        AP_BCN_rsvdpage_loc;
    Probersp_rsvdpage_loc   AP_Probersp_rsvdpage_loc;
    Multi_WakeupPulse_Parm  AP_MultiWakeupPulse_Ctrl;
#endif //   CONFIG_AP_OFFLOAD

#if IS_CHIP_8192F(CONFIG_CHIP_SEL)
#if CONFIG_AP_PS_OFFLOAD
    APPS_CTRL             ApPsCtrl;
    H2C_APPS_PARM      H2CApPsParm;             
#endif    
#endif

#if CONFIG_NAN
    NAN_PARM     NAN_Parm;
#endif      //#if CONFIG_NAN

    //3 SECURITY
    IPSEC_FUNC              IpSec;


#if CONFIG_8051_INT_TESTING
    u8              FEISRBitMap[4];
    u8              FWISRBitMap[4];
    u8              FTISRBitMap[4];
    union{
        u32             TSFOnBcnErly1;
        u8              TSFOnBcnErly1Arr[4];
    }TSFOnBcnErly1Union;
    union{
        u32             TSFOnBcnErly2;
        u8              TSFOnBcnErly2Arr[4];
    }TSFOnBcnErly2Union;
    union{
        u32             TSFOnTBTT1;
        u8              TSFOnTBTT1Arr[4];
    }TSFOnTBTT1Union;
    union{
        u32             TSFOnTBTT2;
        u8              TSFOnTBTT2Arr[4];
    }TSFOnTBTT2Union;    
    union{
        u32             TSFOnDWEarly;
        u8              TSFOnDWEarlyArr[4];
    }TSFOnDWEarlyUnion;
    union{
        u32             TSFOnDWEnd;
        u8              TSFOnDWEndArr[4];
    }TSFOnDWEndUnion;     
    union{
        u32             TSFOnP2POn;
        u8              TSFOnP2POnArr[4];
    }TSFOnP2POnUnion;
    union{
        u32             TSFOnP2POff;
        u8              TSFOnP2POffArr[4];
    }TSFOnP2POffUnion;    
    u32             DeltaTSF1;
    u32             DeltaTSF2;
    u8              MbssidCnt;
    u8              ErlyAdjCnt;
    u8              TestItem;
#endif      //#if CONFIG_8051_INT_TESTING

#if !(IS_CHIP_8710B(CONFIG_CHIP_SEL) || IS_CHIP_8721D(CONFIG_CHIP_SEL))
#if CONFIG_BB_GAIN_RPT
	  u8                    BB_Gain_RPT_EN;

#endif	//#if CONFIG_BB_GAIN_RPT
#endif      //#if !IS_CHIP_8710B(CONFIG_CHIP_SEL)


#if CONFIG_D0_SCAN_OFFLOAD
    D0_Scan_offload_ctrl    ScanCtrl;
#endif //#if CONFIG_D0_SCAN_OFFLOAD

    //Media Status
    MEDIA_STATUS            MediaStatus;

    //PS

#if (CONFIG_P2P_PS || CONFIG_FAST_CHANNEL_SWITCH)
    //P2PPS
    P2PPS_Info              P2PPSInfo;
#endif  //#if (CONFIG_P2P_PS || CONFIG_FAST_CHANNEL_SWITCH)

#if CONFIG_POWERSAVING
    PS_PARM                 PSParm;
    PS_TUNING_PPARM         PSTParm;
    PS_BT_Parm              PSBTParm;    
    #if CONFIG_BCNEARLY_ADJUST_V1
    PS_BEAdj_V1            PsBEAdjV1;              
    #endif  //#if CONFIG_BCNEARLY_ADJUST_V1	
	#if CONFIG_BCNEARLY_ADJUST_V2
	BCNEARLY_ADJUST_PID   	BcnEarlyAdjustPID;
	#endif//#if CONFIG_BCNEARLY_ADJUST_V2
#endif  //CONFIG_POWERSAVING

#if CONFIG_SAPPS
    SAPPSParm               SAPPSCtrl;
#endif  //CONFIG_SAPPS

#if CONFIG_INACTIVE_PS
    INACTIVEPSParm          InactivePSCtrl;
#endif  //#if CONFIG_INACTIVE_PS

#if CONFIG_NOLINK_PS
    NOLINKPSParm          NoLinkPSCtrl;
#endif  //#if CONFIG_INACTIVE_PS

#if CONFIG_PARTIAL_OFF
    PG_PARM            PGParm;
#endif  //#if CONFIG_PARTIAL_OFF

#if 0//CONFIG_BTCOEX_CTRL
    //BT parameter recoqder
    BTCOEX_Param            BtCoexPara;

    //For 4 byte alignment
    u8                      CoexReserved[3];

    //DAC Swing update power
    u32                     DacSwingUpdatePower;
    
    //DAC Swing default power
    u32                     DacSwingDefaultPower;
#endif //#if CONFIG_BTCOEX_CTRL

#if CONFIG_BT_MAILBOX
    BT_B_TYPE_TDMA_Parm     BtTdmaParm;
    BT_MAILBOX_Parm         BtParm;
    WLAN_Opmode_Parm        WIFI_Info;           
   // BT_SCO_eSCO_operation_Parm		BT_SCO_eSCO_info;	
    BT_Page_Scan_Interval_Parm		BT_Page_Scan_Interval;
    BT_Init_Parm            BT_Init_Info;
#endif  //#if CONFIG_BT_MAILBOX

#if CONFIG_REMOTE_WAKEUP
    RemoteWakeup_Parm       RemoteWakeUpCtrl;
    AOAC_INFO               AOACInfo;
    AOAC_GLOBAL_INFO        AOACGlobalInfo;
    AOAC_RSVDPAGE1          AOACPage1;          
    AOAC_RSVDPAGE2          AOACPage2;       
    AOAC_RSVDPAGE3          AOACPage3;
#endif

#if !(IS_CHIP_8710B(CONFIG_CHIP_SEL) || IS_CHIP_8721D(CONFIG_CHIP_SEL))
#if CONFIG_SIDEBAND_WoWLAN
    SIDEBAND_WOWLAN_INFO    SidebandWoWlanInfo;
#endif //#if CONFIG_SIDEBAND_WoWLAN
#endif      //    #if !IS_CHIP_8710B(CONFIG_CHIP_SEL)

#if CONFIG_IPSEC_ENGINE_RAM
    IPSEC_INFO              IpsecInfo;
#endif  //CONFIG_IPSEC_ENGINE_RAM


#if CONFIG_TASK_IDLE_CHK
    u8                      MaxCreatTaskNum;
#endif

#if CONFIG_KEEP_ALIVE
    KeepAlive_Parm          KeepAliveParm;
#endif

#if CONFIG_DISCONNECT_DECISION
    Disconnect_Parm         DisconnectParm;
#endif  //CONFIG_DISCONNECT_DECISION

#if CONFIG_FAST_CHANNEL_SWITCH
    FCSInfo                 FCSCtrl;
#endif  //CONFIG_FAST_CHANNEL_SWITCH

#if CONFIG_TSF_RESET_PATCH
    u8                      TSF_RST_Port[2];
#endif  //#if CONFIG_TSF_RESET_PATCH

#if 0//CONFIG_DEBUGPRT_RA 
    u8                      RAcnt[32];
#endif

#if CONFIG_DEBUGPRT_BT
    //u8                      BTcnt[40];
#endif  

#if CONFIG_PSD_OFFLOAD
    PSD_PARM                PSDParm;
#endif  //CONFIG_PSD_OFFLOAD

#if CONFIG_RLPS
    RLPS_Parm               RLPSCtrl;
#endif  //CONFIG_RLPS

#if CONFIG_IQK_OFFLOAD
    IQK_OFFLOAD_PARM        IQKParm;
    //IQK_MATRIX_REGS_SETTING IQKResult;
#if CONFIG_DEBUGPRT_IQK    
    u8                      IQKdbgreg[2][34];//[path][idx]
//    u8                      IQKdbgreg1[2][16];//[path][idx]
//    u8                      IQKdbgreg2[2][16];//[path][idx]

#endif  //#if CONFIG_DEBUGPRT_IQK        
#endif  //#if CONFIG_IQK_OFFLOAD

#if 0//CONFIG_RF_JAGUAR
    u8                      RFReadConfig;
#endif

#if CONFIG_FW_SND
    TXBF_PARM       TXBFparm;
#endif

#if CONFIG_BB_NHM
    u8                      BBNHMEn;
#endif  //CONFIG_BB_NHM
#if CONFIG_TXBEACON_IGNORE_EDCCA
    u8                      BcnIgnoreEdccaEn;
#endif  //CONFIG_TXBEACON_IGNORE_EDCCA


#if CONFIG_MULTI_FUNCTION
    Multi_Funtion           Multi_Funtion;
#endif


    HW_Info		HWInfo;

    //debug mode
#if CONFIG_DBGMODE
    u8                      GlobalDebugLevel;
    u16                    DebugComponent;
#endif  //CONFIG_DBGMODE


#if (CONFIG_C2H_DEBUG & CONFIG_C2H_PKT)
    C2HEXT_EVT              C2HExtEvt;
#endif  //#if (CONFIG_C2H_DEBUG & CONFIG_C2H_PKT)


#if CONFIG_SNIFFER_DEBUG
    BOOLEAN                 FirstDbgPkt;
    u16                     SeqNum;
    DBGPKT_EVT              DbgPktEvt;    
#endif  //    #if CONFIG_SNIFFER_DEBUG

#if CONFIG_TX_PAUSE_DRV_INFO
	u8						TxPauseDRVInfo;
	u8						TxPauseFWInfo_0;
#endif // #if CONFIG_TX_PAUSE_DRV_INFO

#if CONFIG_MACID_PAUSE_DRV_INFO
	u8						MACIDPauseDRVInfo[16];
	u8						MACIDPauseFWInfo[16];
    u8 						MACIDPauseReasonCode;//3
#endif // #if CONFIG_MACID_PAUSE_DRV_INFO

#if CONFIG_REMOTE_WAKEUP
        u8                                  RxShiftEn:1;
        u8                                  DMAShiftMode:1;
        u8                                  rsvd:6;
#endif
    union{
        RF_DLPS_Parm    RfDlpsParm;
        RF_DLPS_Parm2   RfDlpsParm2;
    }RfDlpsParmUnion;
    u32                    RF_18;

}mib_info, *Pmib_info;

#ifdef __RTL8721D_INIT_RAM_C__

FW_RAM_BSS_SECTION    mib_info         SysMib;

#else 

extern                mib_info         SysMib;

#endif





#if CONFIG_BCNEARLY_ADJUST_V1 /*|| IS_CUT_A(CONFIG_CHIP_SEL)*/ || (IS_CATEGORY_NIC_IoT(CONFIG_CATEGORY_SEL))
/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/
typedef struct _H2C_S1_null1_ctrl_parm_{
        u8 S1_null1_ctrl_en:1;
        u8 S1_null1_fail_limit:7;
        u8 S1_null1_interval;
}H2C_S1_null1_ctrl_parm,*PH2C_S1_null1_ctrl_parm;

typedef struct _S1_null1_parm_{
        u32 S1_null1_unlock_time;
        u8 S1_null1_fail_cnt;
        u8 S1_null1_delay_flag:1;
        u8 rsvd:7;
}S1_null1_parm,*PS1_null1_parm;

typedef struct _S1_null1_ivl_patch_{
        S1_null1_parm s1_null1_parm;
        H2C_S1_null1_ctrl_parm s1_null1_ctrl_parm;
}S1_null1_ivl_patch, *PS1_null1_ivl_patch;

typedef struct _BT_B_TYPE_TDMA_Parm_8721D_ {

    //20200116
    u8      TdmaLeapyApModeDisable  :1;
    u8      LeapyAPCount            :7;
    u8      LeapyAPRxCount;
    u8      TDMASlotToggleCount;
    u8      LeapyAPCountForObservation;

    u8      NullTypeTDMA;
    u8      Null_P_PacketSend;
} BT_B_TYPE_TDMA_Parm_8721D, *PBT_B_TYPE_TDMA_Parm_8721D;


typedef struct _mib_info_8721D_ {
    S1_null1_ivl_patch s1_null1_ivl_patch;

    #if CONFIG_BT_MAILBOX
    BT_B_TYPE_TDMA_Parm_8721D   BtTdmaParm8721D;
    #endif
}mib_info_8721D, *Pmib_info_8721D;


/*------------------------Export global variable------------------------------*/
#ifdef  __RTL8721D_INIT_RAM_C__

FW_RAM_BSS_SECTION   mib_info_8721D                SysMib8721D;

#else

extern               mib_info_8721D                SysMib8721D;

#endif  //#ifdef    __HALCOM_INIT_RAM_C__


#endif //#if CONFIG_BCNEARLY_ADJUST_V1 || IS_CUT_A(CONFIG_CHIP_SEL)


/*------------------------Export global variable------------------------------*/
/*------------------------------Function declaration--------------------------*/
#endif  //__RTL8721D_SYSMIB_H__

