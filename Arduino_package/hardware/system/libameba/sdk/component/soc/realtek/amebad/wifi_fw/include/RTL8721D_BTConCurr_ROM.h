#ifndef __RTL8721D_BTCONCURR_H__
#define __RTL8721D_BTCONCURR_H__

#ifndef __ASSEMBLY__
/*--------------------Define --------------------------------------------*/
#define DefaultTBTTOnPeriod 0xB
#define DefaultMedOnPeroid  0x8
#define MaxTBTTOnPeriod     0x2A   // 0X16
#define MaxMedOnPeroid      0x2A   // 0X16
#define MinTBTTOnPeriod     0x3
#define MinMedOnPeroid      0x3
#define NULL_EARLY          0x3
#define REDUCTIONPERIOD     0x5
#define DefaultRCount       0x4
#define DOWN                0
#define UP                  1
#define MAILBOX_MAX_LENGTH  7
#define EXTENSIONTHRESHOLD  65	//Alan 2013_0801
#define NORESPONSELIMIT     5
#define SLOT_WIFI           0
#define SLOT_BT             1
#define DEFAULTSETTING      3
//Alan 2013_0720
#define MAILBOX_STATUS	0

//Alan 2013_0923
#define SCO_eSCO_slot_unit	320	//320us
#define SCO_eSCO_timer_unit	32	//32us	
//#define SCO_WiFi_slot			0
//#define SCO_BT_slot			1
//#define BT_reduce_code_size	1

//Alan 
#define WLAN_ON             0
#define WLAN_OFF            1

typedef enum _MAILBOX_STATUS_ {
    BT_Active_OutReady_not_zero	= BIT0,
    BT_Non_Ative				= BIT1,
    BT_Patch_Timeout			=BIT2,	
    BT_Content_Error			=BIT3,
    BT_Content_Checksum_error	=BIT4,
} MAILBOX_STATUS_ENUM;

#if IS_CHIP_8192E(CONFIG_CHIP_SEL)
#define ANT_CTRL_BY_PTA     0x600600
#define ANT_CTRL_BY_BBSW    0x700700
#elif IS_CHIP_8195(CONFIG_CHIP_SEL)
#define ANT_CTRL_BY_PTA     0x66000000
#define ANT_CTRL_BY_BBSW    0x77000000
#else
#define ANT_CTRL_BY_PTA     0x66
#define ANT_CTRL_BY_BBSW    0x77
#endif

#define ANT_TOGGLE_WIFI     0x20000000
#define ANT_TOGGLE_BT       0x10000000

#define MAILBOX_RTY_LMT     4

#define RSP_WIFI_FW         0
#define RSP_WIFI_REQ        1
#define RSP_BT_CHANGE_STS   2

#define SEL_COEX_TABLE_1    0x03//0x6c0  bit[1:0]=2'b11
#define SEL_COEX_TABLE_2    0xFC//0x6c4  bit[1:0]=2'b00

#define ONBT                0
#define ONWIFI              1
#define NOCHANGE            2
#define ONBBSW              0
#define ONPTA               1

#define DACSWING_ADDR       0x880
#define DACSWING_SHF        25
#define DACSWING_MASK       0xC1ffffff

#define BTPATCH_STATUS      REG_BT_Cal_Ind  //0x49d[7:6]
#define BTNRDY              BIT6
#define BTRDY               BIT7

#define MAILBOXIVT          0xA


/*--------------------Define Enum---------------------------------------*/

typedef enum _MAILBOX_CMD_ {
    MBID_HW_CHANGE_INFO0      = 0x00,
    MBID_HW_CHANGE_INFO1      = 0x01,
    MBID_HW_CHANGE_INFO2      = 0x02,
    MBID_HW_CHANGE_INFO3      = 0x03,
    MBID_HW_CHANGE_INFO4      = 0x04,
    MBID_HW_CHANGE_INFO5      = 0x05,
    MBID_HW_CHANGE_INFO6      = 0x06,
    MBID_HW_CHANGE_INFO7      = 0x07,
    MBID_HW_CHANGE_INFO8      = 0x08,
    MBID_TBT                  = 0x0E,
    MBID_RPT_RSSI             = 0x0F,
    MBID_CFO                  = 0x15,
    MBID_PSD_CTRL             = 0x21,
    MBID_RPT_INFO             = 0x23,
    MBID_V_RPT_INFO           = 0x27,
    MBID_MP_RPT               = 0x30,
    MBID_BT_CAL               = 0x34,
} MAILBOX_CMD_ENUM;

/*--------------------Define MACRO--------------------------------------*/

#define MAILBOX_TIMERINT_MASK FTINT_GTINT6
#define MAILBOX_TIMER_MASK    REG_TC6_CTRL

/*--------------------Define Struct---------------------------------------*/

typedef struct _BTOption_Parm_ {
    u8              BTOptionMode;
} BTOption_Parm, *PBTOption_Parm;

typedef struct _PSD_Control_Parm_ {
    u8      enable;
}PSD_Control_Parm, *PPSD_Control_Parm;

typedef struct _PTA_Parm_ {
    u8      data0;
    u8      data1;
    u8      data2;
    u8      data3;
    u8      data4;
    u8      data5;
}PTA_Parm, *PPTA_Parm;

typedef struct _Report_Parm_ {
    u8      data0;
    u8      data1;
    u8      data2;
    u8      data3;
    u8      data4;
    u8      data5;
    u8      data6;
}Report_Parm, *PReport_Parm;

typedef struct _BTTxStatus_Parm_ {
    u8      StatusWLOpmode:1;
    u8      StatusWLOpmodeTx:1;
    u8      StatusBTInfo:1;
    u8      StatusBTInfoTx:1;
    u8      StatusIgnoreWLAct:1;
    u8      StatusIgnoreWLActTx:1;
    u8      StatusBTMP:1;
    u8      StatusBTMPTx:1;
    u8      StatusAutoRPT:1;
    u8      StatusAutoRPTTx:1;
    u8      rsvd:6;
    u8      BTInfo;
    u8      IgnoreWLAct;
    u8      AutoRPT;
    u8      BTMPContent[5];
}BTTxStatus_Parm, *PBTTxStatus_Parm;

typedef struct _BT_B_TYPE_TDMA_Parm_ {
    //B0
    u8      Enable                  :1;
    u8      ValueofReg870InBT       :1;     //1:0x870 = 0x300, 0:0x870 = 0x310
#if 0   //IS_CUT_A(CONFIG_CHIP_SEL)
    u8      TxPspollIntheMedium     :1;
#else   //#elif IS_CUT_B(CONFIG_CHIP_SEL)
    u8      HIDSlotToggle     :1;
#endif
    u8      ValueOfReg870           :1;     // when disable 1:0x870 = 0x310, 0:0x870 = 0x300  
                                            //when enable 1: 0x860 = 0x110, 0:0x860 = 0x210
    u8      AutoWakeUp              :1;		//when set 1, use null(0) and null(1) packet
    u8      NoPS                    :1;			//when set 1, in BT slot will pause WiFi Tx
    u8      AllowBTHighPriority     :1;
    u8      ValueofReg870InWIFI     :1;     //1:0x870 = 0x300, 0:0x870 = 0x310
    //B1
    u8      TBTTOnPeriod;
    //B2
    u8      MedPeriod;
    //B3              
    u8      NoTxPause               :1;
    u8      Valueof778InWIFI        :1;
    u8      Valueof778InWIFIOnSCO   :1;
    u8      ValueofReg778           :1;     //when disable 1: 778=3, 0: 778=1
                                            //when enable 1:allow 32k, 0:not allow 32k
    u8      SCOOption               :1;
    u8      ValueofReg860InWIFI     :1;
    u8      TwoAntenna              :1;
    u8      ReduceWIFIPower         :1;
    //B4
    u8      ExtraDecisonOf778             :1;
    	//u8      RxExtension             :1; // no use : Alan 2013_0805
    u8      CCK_Priority_toggle   :1;
    //u8      NoPSRFCtrl              :1;		//When set 1. in BT slot need to close WiFi RF 
    u8      WifiAutoSlot            :1;
    //u8      ChangeCoexTable2        :1;	//no use: Alan 2013_0805
    u8		CTS2Self				  :1;	
    u8      mChangeCoexTable         :1;             //name modified by jackie liu, ChangeCoexTable->mChangeCoexTable
    u8      LongNAV                 :1;			// use large NAV to protect BT slot
    u8      FlexibleExtension       :1;
    u8      ValueofReg778B1InBT     :1;        // 1:778[1]=1, 0:778[1]=0 in bt

    //DHCP
    u8      DtimExt;
    u8      DHCPOn                  :1;
    u8      NoRxBCNCondition        :1;
    u8      Write860                :1;
    u8      Value860                :1;
    
    u8      DTIMFlag                :1;
    u8      WifiActive              :1;
    u8      rsvd2                   :2;    

    u8      BTTxAbort               :1;
    u8      TxNull1                 :1;
    u8      TxNull1ok               :1;
    u8      rsvd3                   :5;
    
    u8      WLANPeriodTemp;
    u8      ExtensionThreshold1;
    u8      ExtensionThreshold2;
    u8      ExtensionThreshold3;
    u8      HalfTBTTOnPeriod;
    u8      HalfMedPeriod;
    u8      RNum;
    u8      RCount;
    u8      HalfBCNIvl;
    u8      BCNIvl;			//Alan 2013_0911
    u8      WLANOnPeriod;
    u8      WLANPeriodCount;
    u8      InitialBTPower;
    u8      ExtensionCount;
    u8      NullExtensionCount;
    u8      NAVTemp;
    u8      TxCounter;
    u8      BTConnect;
    u8      BTRptConnect;
    u8      BTChangeType:1;
    u8      BTCTCount:4;
    u8      BTCTCTh:3;
    u32     ValueBackup;
    u8      ValueBackup2;
    u8      ValueBackup3;
    u8      ValueBackup4;
    u8      MailboxRdy;
    u8      BTDisable;
    u8      RxExtension             :1;				// Alan 2013_0905
    u8      WLCalibrationEn         :1;
    u8      GNT_BT_Val              :1;                        //Alan 2014_0226
    u8      BTOnlyTest              :1;     
    u8      DynamicSlotBackup       :1;
    u8      PanProtection           :1;
    u8      rsvd4:2;
    u16     Reg948Backup;
    BTTxStatus_Parm BTTxStatus;

    u8      extensionthreshold;                     //alan 20150108

    #if 1   //IS_CUT_B(CONFIG_CHIP_SEL)
    u32      HIDPTATable;
    u32      HIDPTATableBackUp;
    u8      HIDPTATableEn;
    u8      HIDSlotCnt;
    u8      HIDPTATableInterval;
    #endif

} BT_B_TYPE_TDMA_Parm, *PBT_B_TYPE_TDMA_Parm;
/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
extern void 
Write778_8721D_ROM(
    IN u8 value
) ;

extern void 
DacSwing_8721D_ROM(
    IN u8 action
) ;

extern void 
InitBTMailbox_8721D_ROM(
    void
) ;

extern void 
BTMailboxRetry_8721D_ROM(
    void
) ;



extern void 
C2HBTInfo_8721D_ROM (
    IN  u8  option
) ;

extern void 
C2HBTLoopback_8721D_ROM (
    IN  u8  *pbuf
) ;

extern void 
C2HBTBasicInfo(
    IN  u8  type
) ;

extern void 
C2HBTMpRpt_8721D_ROM(
    IN  u8  idx, 
    IN  u8  status, 
    IN  u8  req_num
) ;

void 
FlexibleExtensionDecision_8721D_ROM(
    void
) ;

#if 0
void 
BTNullSetting_CallPtr(
    IN  u8  bPowerBit, 
    IN  u16 offset, 
    IN  u8  lsb, 
    IN  u8  msb
) ;
#endif

void 
BTNullSetting_8721D_ROM(
    IN  u8  bPowerBit, 
    IN  u16 offset, 
    IN  u8  lsb, 
    IN  u8  msb
) ;

#if 0
extern void 
C2HBTOptionMode (
    IN  u8  op
) ;
#endif
extern void 
IssueBTQNull_8721D_ROM(
    IN  u8  bPowerBit, 
    IN  u8  lsb, 
    IN  u8  msb
) ;

extern void
WlanActCtrl_8721D_ROM(
    void
) ;

extern void 
ChangeCoexTable_8721D_ROM(
    IN  u8  op
) ;

#if 0
extern void
BT_TDMA_Dynamic_Slot_8721D_ROM(
    void
 ) ;


extern void
InitBTypeTDMA_8721D_ROM(
    void
) ;
#endif

extern void
InitBTypeTDMA2_8721D_ROM(
    void
) ;

extern void
BTypeTDMABCNEarly_8721D_ROM(
    void
) ;

extern void
BTypeTDMATBTThdl_8721D_ROM(
    void
) ;

extern void
BTypeTDMATimeOuthdl_8721D_ROM(
    void
) ;



extern void 
MailboxINTHDL_8721D_ROM(
    void
) ;

extern void
BTRoleChangeTBTTHDL_8721D_ROM(
    void
) ;

extern void
BTRoleChangeTimerHDL_8721D_ROM(
    void
) ;

extern void 
BtFwDownload(
    u16 len
) ;

#if 0
extern void 
C2HBTMailBoxStatus_8721D_ROM(
    u8 status
) ;
#endif

//Alan 2013_09_04
extern u8
BTPatchContentCheckSum(
    IN u16 length 
) ;


extern void 
InitBTCoexTimer_8721D_ROM(
    IN  u8 duration
);

#if 0
extern void
InitSCOTimer_8721D_ROM(
    IN  u16 duration
);

extern void
BTSCOeSCOIssueNullHDL_8721D_ROM(
    void
) ;

void
SCO_eSCO_IssueCtrlData_8721D_ROM(
	IN	u8	ctrl_pkt_type,
	IN	u8	slot,
	IN	u8	power_bit
)	;


void
AOAC_ANT_SWT_CallPtr(
        IN  u8  TwoAnt,
        IN  u8  value,
        IN  u8  AntSwitchOutside,
        IN  u8  AntSelReverse
);
#endif


void
BT_TDMA_Slot_Operation_0and3_8721D_ROM(
    IN	u8	slot_num
 ) ;

void
BT_TDMA_Slot_Operation_1and4_8721D_ROM(
    IN	u8	slot_num
 ) ;

void
BT_TDMA_Slot_Operation_2and5_8721D_ROM(
    IN	u8	slot_num
 ) ;



#if 0
void
BTSCOeSCOAntSetup_8721D_ROM(
    IN	u8	slot
) ;
#endif



#if 0
extern void 
C2HBTOptionMode (
    IN  u8  op
) ;
#endif


#endif   //#ifndef __ASSEMBLY__
#endif  //__RTL8721D_BTCONCURR_H__
