#ifndef __WLAN_D0_SCAN_OFFLOAD_H__
#define __WLAN_D0_SCAN_OFFLOAD_H__



/*--------------------Define --------------------------------------------*/

#define MAX_SUPPORT_CHANNEL     40


//  Note, SW Timer unit is 10ms
//  Timeout value 10 = 10*10ms= 100ms
//  Timeout value 100 = 100*10ms= 1000ms = 1sec
#define FIRST_SCAN_TIMEOUT  10000 // 5secs
#define SWITCH_CHANNEL_TIMEOUT  10 // 100ms


#define SSID_MAX_LEN        32
#define SSID_NUM_LIMIT      3



/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/


typedef struct _CHANNEL_PATTERN_
{
    u8  Channel;
    u8  TXPower;
    u8  TimeOut;
    u8  bActive;
}CHANNEL_PATTERN, *PCHANNEL_PATTERN;


typedef struct _DOT11_SSID {
  u8  SsidLength;
  u8  Ssid[SSID_MAX_LEN];
} DOT11_SSID, *PDOT11_SSID;


typedef	enum _OUI_SUBTYPE{
	OUI_SUB_DONT_CARE,
	OUI_SUB_WPA,
} OUI_TYPE,*POUI_TYPE;


typedef struct _SS_DrvCtrl_
{   
    u8  NumOfchannels;  
    u8  OriginalChl;    
    u8  OriginalBW;        
    u8  Originalch40offset;
    u8  Originalch80offset;    
    u8  bPriodScan;    
    u8  PriodScanTime;        
    u8  En_RFE;    
    u8  RFEType;        
    u8  Rsvd[7];            
    CHANNEL_PATTERN ChannelPattern;
}SS_DrvCtrl, *PSS_DrvCtrl;

typedef struct _SITESURVEY_PARAMETER_ {
    u8   curChannel;
    u8   chnlPatternIdx;
    u8   orig_init_gain0;
    u8   orig_init_gain1;
    SS_DrvCtrl      scanChnlInfo;    
    u8   orig_RCR;
    u8   orig_PS_STATE;            
    u16  origFilterMap;
    u8   bEnableDynamicRXDMAMachine;
}SITESURVEY_PARAMETER, *PSITESURVEY_PARAMETER;



/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/


extern void 
H2CHDL_D0ScanOffloadCtrl(
    IN  u8 *pbuf
);

void 
getScanCmdInfo(
    void
) ;

void 
surveyCmdHdl(
    void
) ;

void 
surveyTimerHdl(
    void
) ;

void 
issueProbeRsq(
    u8 probeLoc
) ;

void 
checkSwitchChannelPara(
    u8 channel
) ;

void 
checkSwitchChannelPara_N(
    void
) ;

extern void 
checkRXFIFOStatus(
    void
) ;

void 
SetProbeRateTo5G(
    void
);

VOID
SetRFEReg(
	IN u1Byte		Band,
	IN u1Byte		RFEType
);

void 
RestartScanTimer(
	void
) ;

#if CONFIG_RTD3_SCAN_OFFLOAD

void 
getSSIDInfo(
    void
) ;

#endif // CONFIG_RTD3_SCAN_OFFLOAD

#if (CONFIG_RTD3_SCAN_OFFLOAD || CONFIG_NLO_COMPLEX_OFFLOAD || CONFIG_NLO_SIMPLE_OFFLOAD)
void 
onBeaconOrProbeRsp(
    IN u16 addrl_frame_ctrl,
    IN u16 packet_len
) ;

void 
compareInfoWithNetworkList(
    IN u16 addrl_frame_ctrl,
    IN u16 packet_len
) ;

u16 getElementOffset(
    IN u16 offset,
    IN OUI_TYPE type,
    IN u16 index,
    IN u16 limit
) ;

u8 compareOUI(
    IN u16 offset,
    IN OUI_TYPE type
) ;

#else
#if CONFIG_RUNTIME_PM_OFFLOAD

void 
onBeacon(
    IN u16 addr_rx_status
) ;

void 
GetRSSI(
    IN u16 addr_rx_status
) ;

#endif //#if CONFIG_RUNTIME_PM_OFFLOAD

#endif //#if (CONFIG_RTD3_SCAN_OFFLOAD || CONFIG_NLO_COMPLEX_OFFLOAD || CONFIG_NLO_SIMPLE_OFFLOAD)

/*--------------------Define extern variable -------------------------------*/

#if (CONFIG_D0_SCAN_OFFLOAD || CONFIG_NLO_COMPLEX_OFFLOAD || CONFIG_NLO_SIMPLE_OFFLOAD)
extern MEMTYPE_XDATA   u8 bScanStart;
extern MEMTYPE_XDATA SITESURVEY_PARAMETER surveyPara;
#endif //#if CONFIG_D0_SCAN_OFFLOAD

#endif // #ifndef __WLAN_D0_SCAN_OFFLOAD_H__

