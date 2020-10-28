#ifndef __RTL8721D_HAL_RAM_H__
#define __RTL8721D_HAL_RAM_H__

#ifndef __ASSEMBLY__

//#include "RTL8721D_FastChannelSwitch_RAM.h"
#include "RTL8721D_Reg_RAM.h"
#include "RTL8721D_debug_RAM.h"
#include "RTL8721D_Init_RAM.h"
#include "RTL8721D_Util_RAM.h"
//#include "RTL8721D_Coex_RAM.h"
#include "RTL8721D_cmd_RAM.h"
#include "RTL8721D_PhyReg_RAM.h"
#include "RTL8721D_PhyCfg_RAM.h"
//#include "RTL8721D_Efuse_RAM.h"
#include "RTL8721D_Packet_RAM.h"
#include "RTL8721D_PowerCtrl_RAM.h"
#include "RTL8721D_TxReport_RAM.h"
#include "RTL8721D_INT_RAM.h"
#include "RTL8721D_RateAdaptive_RAM.h"
//#include "RTL8721D_RateAdaptive_table_RAM.h"
#include "RTL8721D_PSD_RAM.h"
#include "RTL8721D_LockChip_RAM.h"
#include "RTL8721D_LTECOEX_RAM.h"
#include "RTL8721D_BTConCurr_RAM.h"
#include "RTL8721D_NAN_RAM.h"
//#include "RTL8721D_INT_TEST_RAM.h"
#include "RTL8721D_Normal_Dbg.h"
#include "RTL8721D_CP_FT_TEST_RAM.h"
//#include "..\..\source_app2\8721D\app2.h" //include this for txbuf test





//1 Code Memory Map
//
// 0x0000 ~ 0x3FFF: IROM
// 0x4000 ~ 0xBFFF: code dwonload ram 32K
// 0xC000 ~ 0xFFFF: TX pkt buff high 16K


//1 Xdata Memory Map
//
// 0x0000 ~ 0x7FFF: MAC Register
// 0x0800 ~ 0x0FFF: BB Register
// 0x1000 ~ 0x1FFF: download ram space
// 0x2000 ~ 0x2FFF: RSVD
// 0x3000 ~ 0x3FFF: ext ptcl reg
// 0x4000 ~ 0x7FFF: RSVD
// 0x8000 ~ 0x9FFF: XData & TXRPT
// 0xEE00 ~ 0xEE47: USB descriptor RAM
// 0xEE48 ~ 0xFAFF: RSVD
// 0xF000 ~ 0xF0FF: usb3 register
// 0xFB00 ~ 0xFBFF: RxPktBuf
// 0xFC00 ~ 0xFCFF: TxPktBuf
// 0xFD00 ~ 0xFDFF: 8051 Internal
// 0xFE00 ~ 0xFEFF: USB Register
// 0xFF00 ~ 0xFFFF: RSVD




/*--------------------------Define -------------------------------------------*/
//Task List
#define TASK_INIT_8721D              0
#define TASK_IDLE_8721D              1
#define TASK_COMMON_8721D            2
#define TASK_PSDOffload_8721D        3
#define MAX_TASK_ID                  4


//3 Common Interface for each HAL
// Task List
#ifdef _MCU_8051_
#define TASK_INIT               TASK_INIT_8721D
#define TASK_IDLE               TASK_IDLE_8721D
#define TASK_COMMON             TASK_COMMON_8721D
#define TASK_PSDOffload         TASK_PSDOffload_8721D
#else
#define TASK_INIT               
#define TASK_IDLE               
#define TASK_COMMON             
#define TASK_PSDOffload         
#endif

#define RAM_VERSION             RTL8721D_FW_VERSION
#define RAM_SUBVERSION          RTL8721D_FW_SUBVERSION

//#define RAMInfo             RAMInfoRAM8721D

// HAL Structure
//#define HAL_DATA_TYPE           HAL_DATA_COMMON
//#define HAL_DATA_AS0_TYPE       HAL_DATA_AS0_COMMON

//brian sendsignal for os/isr sendsignal if a function both called in isrhdl or not, add to rom if possible
#define SendSignalCommon      SendSignalCommon_Ptr

// HAL Init
#define HALInit                 HALInit_8721D
#define InterfaceDecision       InterfaceDecision_8721D_ROM
#define InitInterrupt           InitInterrupt_8721D
#define MCUINTEnable            MCUINTEnable_8721D_ROM
//#define CheckOverallTaskIdle    CheckOverallTaskIdle_8721D

// InitCommon
#define InitCommon              InitCommon_8721D

// Interrupt handle function
//#define USB_INT_HDL_RAM         USB_INT_HDL_RAM_8721D
#define WLAN_INT_HDL_RAM        WLAN_INT_HDL_RAM_8721D
//#define UART_INT_HDL_RAM        UART_INT_HDL_RAM_8721D
#define FT_INT_HDL_RAM          FT_INT_HDL_RAM_8721D
//#define SYS_INT_HDL_RAM         SYS_INT_HDL_RAM_8721D

//Basic ISR function
//#define UpdateISRSys            UpdateISRSys_8721D
#define UpdateISRWlan           UpdateISRWlan_8721D
#define UpdateISRFT             UpdateISRFT_8721D

#define ClrAllFWUsedIMR         ClrAllFWUsedIMR_8721D_RAM
#define ClrAllFWUsedISR         ClrAllFWUsedISR_8721D_RAM
#define InitWlanIMR             InitWlanIMR_8721D
#define InitFTIMR               InitFTIMR_8721D
//#define InitFSIMR               InitFSIMR_8721D

//#define RWritePONRegByte        WritePONReg_8721D
//#define RReadPONRegByte         ReadPONReg_8721D

//Sub ISR function
//3 1.) SYSTEM ISR
//#define ISR_GPIO2En             ISR_GPIO2EnCommon
//#define ISR_GPIO3En             ISR_GPIO3EnCommon
//#define ISR_PDN                 ISR_PDNCommon


//3 2.) WLAN ISR
#define ISR_TxClose             ISR_TxClose_8721D
#define ISR_TXCCX               ISR_TXCCX_8721D
#define ISR_RxDone              ISR_RxDone_8721D
#define ISR_H2CCMD              ISR_H2CCMD_8721D
#define ISR_HRCV                ISR_HRCV_8721D
#define ISR_CTWEnd              ISR_CTWEnd_8721D
#define ISR_BcnEarly            ISR_BcnEarly_8721D
#define ISR_TBTT                ISR_TBTT_8721D
#define ISR_BCNRx               ISR_BCNRx_8721D
#define ISR_RxBMC_MD1           ISR_RxBMC_MD1_8721D
#define ISR_RxBMC_MD0           ISR_RxBMC_MD0_8721D
#define ISR_RxUnicast_MD1       ISR_RxUnicast_MD1_8721D
#define ISR_RxUnicast_MD0       ISR_RxUnicast_MD0_8721D
#define ISR_TXBCNERR            ISR_TXBCNERR_8721D
#define ISR_TXBCNOK             ISR_TXBCNOK_8721D
#define ISR_TxPKTIn             ISR_TxPKTIn_8721D
#define ISR_WlanActOn           ISR_WlanActOn_8721D
#define ISR_WlanActOff          ISR_WlanActOff_8721D
#define ISR_TxrptCntFull        ISR_TxrptCntFull_8721D
#define ISR_TryDone             ISR_TryDone_8721D
#define ISR_SoundDone           ISR_SoundDone_8721D

//3 2.1) WLAN2 ISR
#define ISR_EQTxOK              ISR_EQTxOK_8721D
#define ISR_SyncBcnRxOK         ISR_SyncBcnRxOK_8721D
#define ISR_DWEarly                 ISR_DWEarly_8721D
#define ISR_DWEnd                   ISR_DWEnd_8721D
#define ISR_EQEarly                 ISR_EQEarly_8721D
#define ISR_TBTT1               ISR_TBTT1_8721D

//3 3.) FT ISR
#define ISR_GTimer0             ISR_GTimer0_8721D
#define ISR_GTimer1             ISR_GTimer1_8721D
#define ISR_GTimer2             ISR_GTimer2_8721D
#define ISR_GTimer3             ISR_GTimer3_8721D
#define ISR_GTimer4             ISR_GTimer4_8721D
#define ISR_TimerOut0           ISR_TimerOut0_8721D
#define ISR_RPWM                ISR_RPWM_8721D
#define ISR_RPWM2               ISR_RPWM2Common
#define ISR_EOSP                ISR_EOSP_8721D
#define ISR_GTimer5             ISR_GTimer5_8721D
#define ISR_BCNERLY1            ISR_BCNERLY1_8721D
#define ISR_P2PRFOn             ISR_P2PRFOn_8721D
#define ISR_P2PRFOff            ISR_P2PRFOff_8721D
#define ISR_TRIGGER_PKT         ISR_TRIGGER_PKT_8721D
#define ISR_TSF_BIT32_TOGGLE    ISR_TSF_BIT32_TOGGLECommon
#define ISR_RETRIVE_BUFFERED    ISR_RETRIVE_BUFFERED_8721D
#define ISR_CLR_PS_STATUS       ISR_CLR_PS_STATUS_8721D
#define ISR_BcnEarlyAGG         ISR_BcnEarlyAGG_8721D
#define ISR_TxNull0             ISR_TxNull0_8721D
#define ISR_TxNull1             ISR_TxNull1_8721D
#define ISR_GTimer6             ISR_GTimer6_8721D
//#define ISR_SCO_OP              ISR_SCO_OP_8721D


//3 4.) FS ISR

// Interrupt IMR / ISR
//#define setwlanimr              setwlanimrCommon   
//#define clrwlanimr              clrwlanimrCommon
#define setwlanimr2              setwlanimr2_8721D_ROM
#define clrwlanimr2              clrwlanimr2_8721D_ROM
//#define setsysimr               setsysimrCommon
//#define clrsysimr               clrsysimrCommon
//#define setftimr                setftimrCommon
//#define clrftimr                clrftimrCommon

// H2C
#define CheckH2CMailbox         CheckH2CMailboxCommon
//#define H2CCmdFunc              H2CCmdFunc_8721D
//#define H2CInQueue              H2CInQueueCommon
#define H2CDeQueue              H2CDeQueue_Ptr

// C2H
#define InitC2H                     InitC2H_8721D_RAM
//#define C2HInQueue              C2HInQueueCommon

#define C2HDeQueue              C2HDeQueue_Ptr

#define C2HExtInQueue           C2HExtInQueueCommon
#define C2HExtDeQueue           C2HExtDeQueueCommon

//Media Status
#define MacidDropAction         MacidDropActionCommon
#define MediaConnection         MediaConnectionCommon
#define MediaDisconnection      MediaDisconnectionCommon
#define SetMediaStatus          SetMediaStatus_8721D_ROM
//#define GetMediaStatus          GetMediaStatus_8721D

// Packet
#define CheckCPUMGQEmpty                CheckCPUMGQEmpty_8721D_ROM
#define CheckTxPktBufOverFlow           CheckTxPktBufOverFlow_8721D_ROM
#define CheckRxPktBufOverFlow           CheckRxPktBufOverFlow_8721D_ROM
#define GetOccupyTxInd                  GetOccupyTxIndCommon

// RF ON/OFF
//#define PsOpenRF                        PsOpenRFCommon
//#define PsCloseRF                       PsCloseRFCommon

// 32K
//#define Init32K                         Init32K_8721D
//#define Init32kCAL                      Init32kCAL_8721D
//#define Check32KFDIsLocked              Check32KFDIsLocked_8721D

#if 0       //8721D fw cannot control 32k circuit, ben, 160505
// 32K
#define Init32K                         Init32KCommon
#define Init32kCAL                      Init32kCALCommon
#define Check32KFDIsLocked              Check32KFDIsLockedCommon
#endif

//PowerCtrl
//#define WaitTxStateMachineOk            WaitTxStateMachineOkCommon
#define GetRPWMValue                    GetRPWMValueCommon
#define GetRPWM2Value                   GetRPWMValue2Common
#define CheckTSFIsStable                CheckTSFIsStableCommon
#define WaitHWStateReady                WaitHWStateReadyCommon
#define SysClkDown                      SysClkDown_8721D
#define SysClkUp                          SysClkUp_8721D
#define SetTxPauseFWInfo            SetTxPauseFWInfo_8721D

//Testing
//#define IOTesting8051           IOTesting8051Common
//#define Reset8051Test           Reset8051TestCommon
//#define IODebug                 IODebugCommon
//#define TxBuffTesting           TxBuffTestingCommon
//#define  TXSRAMTEST		 TXSRAMTESTCommon




#define PHY_QueryRFRegRAM               PHY_QueryRFReg_8721D_RAM
#define PHY_SetRFRegRAM                 PHY_SetRFReg_8721D_RAM
    //Phycfg

#define PHY_RFSerialReadRAM         PHY_RFSerialRead_8721D_RAM
#define PHY_RFSerialWriteRAM        PHY_RFSerialWrite_8721D_RAM

#undef PHY_QueryBBReg
#undef PHY_SetBBReg
#define PHY_QueryBBReg                  PHY_QueryBBReg_8721D_RAM
#define PHY_SetBBReg                    PHY_SetBBReg_8721D_RAM

#undef PHY_RFSerialRead
#undef PHY_RFSerialWrite
#define PHY_RFSerialRead                PHY_RFSerialRead_8721D_RAM
#define PHY_RFSerialWrite               PHY_RFSerialWrite_8721D_RAM

#undef PHY_QueryRFReg
#define PHY_QueryRFReg                  PHY_QueryRFReg_8721D_RAM
#define PHY_SetRFReg                    PHY_SetRFReg_8721D_RAM


//3 RA
#define InitRAInfo					InitRAInfo_8721D_RAM
#define InitRATable                                InitRATable_8721D_RAM
#define RateDownTrying                  RateDownTrying_8721D_ROM
#define RateDownStep                    RateDownStep_8721D_RAM
#define RateUp                              RateUp_8721D_RAM
#define Count_Ratio                         Count_Ratio_8721D_ROM
#define TryDone                             TryDone_8721D_RAM
#if 0   //IS_CUT_A(CONFIG_CHIP_SEL)
#define GetTxrptStatistic				GetTxrptStatistic_8721D_RAM  //brian (+=) -> (=) 20170322
#else
#define GetTxrptStatistic				GetTxrptStatistic_8721D_ROM
#endif
#define C2H_RA_Rpt                      C2H_RA_Rpt_8721D_RAM
#define RateDecision					RateDecision_8721D_RAM
#define H2CHDL_Set_MACID_Config	H2CHDL_Set_MACID_Config_8721D_RAM
#define H2CHDL_SetRssiSingle		H2CHDL_SetRssiSingle_8721D_ROM
#define H2CHDL_Set_FW_Trace_en      H2CHDL_Set_FW_Trace_en_8721D_ROM
//#define C2H_RA_Dbg_code_en                    C2H_RA_Dbg_code_en_8721D_ROM
#if (IS_CATEGORY_AP(CONFIG_CATEGORY_SEL))
#define H2CHDL_APReqTxrpt           H2CHDL_APReqTxrpt_8721D_ROM
#define H2CHDL_InitRateCollect       H2CHDL_InitRateCollect_8721D_ROM
#endif
#define PT_Mode_Sel                            PT_Mode_Sel_8721D_RAM
#define PT_update                               PT_update_8721D_ROM
#define Pkt_Num_shift                           Pkt_Num_shift_8721D_ROM

#if (CONFIG_C2H_RA_DEBUG_CODE == 1)
    #define C2H_RA_Dbg_code     C2H_RA_Dbg_code_en_8721D_RAM
#else
    #define C2H_RA_Dbg_code
#endif
//#define RateDown_search_RateMask        RateDown_search_RateMask_8721D_ROM    //patch for endlessloop caused by "i is u8" and "i>=0,i--". Brian_zhang

//#define RateDownStep                RateDownStepRAM_8721D
#define b64QamRate                  b64QamRate_8721D_ROM
#define BTC_Modi_RtyPnty    BTC_Modi_RtyPnty_8721D_ROM

//#define RateUp                      RateUp_8721D

#define NSC_Calculate                   NSC_Calculate_8721D_ROM
    



//#define FillMailbox		FillMailbox_8721D
//#define WL2BTMailbox		WL2BTMailbox_8721D

#define H2CHDL_BTPSTDMA		H2CHDL_BTPSTDMA_8721D

#define	H2CHDL_BTInfo		H2CHDL_BTInfo_8721D_ROM
#define	H2CHDL_ForceBTTxpwr		H2CHDL_ForceBTTxpwr_8721D_ROM
#define H2CHDL_BTIgnoreWlanAct		H2CHDL_BTIgnoreWlanAct_8721D_ROM
#define H2CHDL_DACSwingValue		H2CHDL_DACSwingValue_8721D_ROM
#define H2CHDL_AntSelReverse		H2CHDL_AntSelReverse_8721D_ROM
#define H2CHDL_WLOpmode			H2CHDL_WLOpmode_8721D_ROM
#define H2CHDL_BTMpH2C			H2CHDL_BTMpH2C_8721D_ROM
#define H2CHDL_BTControl		H2CHDL_BTControl_8721D_ROM
#define H2CHDL_BTWifiCtrl		H2CHDL_BTWifiCtrl_8721D
//#define H2CHDL_BT_SCO_eSCO_Operation		H2CHDL_BT_SCO_eSCO_Operation_8721D_ROM
#define H2CHDL_BT_Page_Scan_Interval		H2CHDL_BT_Page_Scan_Interval_8721D_ROM
#define H2CHDL_WLCalibration		H2CHDL_WLCalibration_8721D_ROM
#define H2CHDL_GNT_BT_CTRL		H2CHDL_GNT_BT_CTRL_8721D_ROM
#define H2CHDL_BT_ONLY_TEST		H2CHDL_BT_ONLY_TEST_8721D_ROM
#define H2CHDL_BT_Init_Param      H2CHDL_BT_Init_Param_8721D_ROM
//#define BT_TDMA_Dynamic_Slot    BT_TDMA_Dynamic_Slot_8721D_ROM


//3 6.)power save

//#define SetSmartPSTimer			SetSmartPSTimer_8721D
//#define	SetPwrStateReg			SetPwrStateReg_8721D
//#define	ResetPSParm				ResetPSParm_8721D
#define	ChkTxQueueIsEmpty		ChkTxQueueIsEmpty_8721D_ROM
//#define	Legacy_PS_Setting		Legacy_PS_Setting_8721D
//#define PSModeSetting			PSModeSetting_8721D
//#define	ConfigListenBeaconPeriod		ConfigListenBeaconPeriod_8721D

//PSSetMode only callded in App code; jackie;
#define PSSetMode			PSSetMode_8721D_ROM

#define PS_S2ToS3State		PS_S2ToS3State_8721D_ROM
//#define PS_S2ToS0State		PS_S2ToS0State_8721D
#define	PS_S3ToS2orS0State	PS_S3ToS2orS0State_8721D_ROM
#define	PS_S0ToS1State		PS_S0ToS1State_8721D_ROM
#define	PS_S1ToS0orS2State	PS_S1ToS0orS2State_8721D_RAM
#define	PS_S2ToS4State		PS_S2ToS4State_8721D_ROM
#define PS_S2ToS5State		        PS_S2ToS5State_8721D_ROM
#define PS_S5ToS2State		        PS_S5ToS2State_8721D_ROM
#define	PS_S4ToS2State		PS_S4ToS2State_8721D_ROM
#define	SmartPS2InitTimerAndToGetRxPkt	SmartPS2InitTimerAndToGetRxPkt_8721D_ROM
//#define SetBcnEarlyAndTimeout	SetBcnEarlyAndTimeout_8721D

#define BcnEarlyAdjustByTSF		BcnEarlyAdjustByTSF_8721D

//#define	EnlargeBcnEarlyAndTimeout	EnlargeBcnEarlyAndTimeout_Ptr
//#define ResetBcnEarlyAdjustParm		ResetBcnEarlyAdjustParm_8721D
#define PS_S0ToS6State		PS_S0ToS6State_8721D_ROM
#define PS_S6ToS0State		PS_S6ToS0State_8721D_ROM

#define TSF_After(a,b)  ((s32)(b)-(s32)(a)<0)

#define PS_S2_Condition_Match   PS_S2_Condition_Match_Ptr
#define PS_S4_Condition_Match   PS_S4_Condition_Match_Ptr
/*------------------------------Define Enum-----------------------------------*/


/*--------------------------Define MACRO--------------------------------------*/


/*------------------------------Define Struct---------------------------------*/
#if 0

typedef struct _HAL_DATA_COMMON {

    u8  HCISel;


}HAL_DATA_COMMON, *PHAL_DATA_COMMON;

#endif

typedef struct _HAL_DATA_AS0_COMMON {

    //IMR & ISR
    u8  sysimr[4];
    u8  sysisr[4];
    
    u8  wlanimr[4];
    u8  wlanisr[4];
    
    u8  ftimr[4];
    u8  ftisr[4];

}HAL_DATA_AS0_COMMON, *PHAL_DATA_AS0_COMMON;



/*------------------------Export global variable------------------------------*/

/*------------------------------Funciton declaration--------------------------*/


#endif  // __ASSEMBLY__
#endif  //__RTL8721D_HAL_RAM_H__

