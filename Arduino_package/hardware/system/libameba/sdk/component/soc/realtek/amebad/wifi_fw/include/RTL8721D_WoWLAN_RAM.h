#ifndef __WLAN_WoWLAN_RAM_H__
#define __WLAN_WoWLAN_RAM_H__

/*--------------------Define --------------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/
#if IS_CATEGORY_NIC_IoT(CONFIG_CATEGORY_SEL)
    #define IOT_WAITHOST_TO     5
#endif
/*--------------------Define Struct---------------------------------------*/

typedef struct _SIDEBAND_WOWLAN_INFO_{
    u8 HST2DEV_GPIO;
    u8 DEV2HST_GPIO;
    u8 hst2dev_val;
}SIDEBAND_WOWLAN_INFO, *PSIDEBAND_WOWLAN_INFO;

typedef struct _CUSTOMER_WF_{
    u8          CustomerID:6; 
    u8          gpio_duration_ms_A:1;
    u8          gpio_duration_ms_B:1;
    u8          SpecialWakeUpReasonA;
    u8          gpio_duration_A:7;
    u8          gpio_pulse_infinite_en_A:1;
    u8          gpio_pulse_en_A:1;
    u8          gpio_pulse_count_A:7;
    u8          SpecialWakeUpReasonB;
    u8          gpio_duration_B:7;
    u8          gpio_pulse_infinite_en_B:1;
    u8          gpio_pulse_en_B:1;
    u8          gpio_pulse_count_B:7;    
}CUSTOMER_WF,*PCUSTOMER_WF;

#if CONFIG_MULTI_FUNCTION
typedef struct _Multi_Function_ {
    u8          RegBackup[8];
}Multi_Funtion, *pMulti_Funtion;
#endif

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
extern u8
CheckIV(
    u8* IVToCheck,
    u8 raaddr,
    BOOLEAN unicast_check
) ;

extern BOOLEAN
WaitRXDMAIdle(
    void
) ;

extern void 
UpdateRxFFReadPtr_DropPkt(
    u16 RPtr
) ;
#if 0
extern u16 
Page_Offset_Alignment(
    u16 PageSize_RX,
    u16 offset
) ;
#endif

extern void 
Set_HST2DEV_Attribute(
    u8 index, 
    u8 bINT_EN,
    u8 edge
);

extern void 
GPIO_WOL_HDL(
    void
);

extern void 
InitRemoteWakeUp(
    void
) ;

extern void 
IssueProbeRsp(
    IN u8 probeLoc,
    IN u8 SAAddr[]
)  ;

extern void 
RemoteWakeUp(
    void
) ;

extern void 
H2CHDL_RemoteWakeUp(
    u8 *pbuf
) ;

extern void 
H2CHDL_WoWLAN(
    u8 *pbuf
) ;

extern void 
H2CHDL_AOACGlobalInfo(
    u8 *pbuf
) ;

extern void 
H2CHDL_AOACRsvdpage1(
    u8 *pbuf
) ;

extern void 
H2CHDL_AOACRsvdpage2(
    u8 *pbuf
) ;

extern void 
H2CHDL_AOACRsvdpage3(
    u8 *pbuf
) ;

extern void 
RemoteWakeupCtrl_hdl( 
    u8 bFunEnable
);

extern void 
ARP_hdl( 
    u8 bFunEnable
);

extern void 
NDP_hdl( 
    u8 bFunEnable
);

extern void 
GTK_hdl( 
    u8 bFunEnable
);

extern void 
NLO_hdl( 
    u8 bFunEnable
);


extern void 
RealWoWLAN_hdl( 
    u8 bFunEnable
);

extern void 
RealWoWLANVer2_hdl( 
    u8 bFunEnable
);

extern void 
issue_ARP(
    u8* TargetIP,
    u8* TargetMAC,
    u8  retry_bit,
    u8  arpsel
);

extern BOOLEAN
OnARP(
    u16 addrl_frame_ctrl,
    u8 MacHeaderLen
) ; 

extern void
GetARPInfo(
u8 pagenum
);

void
InitSidebandWoWLAN(
    void
);

void 
GetNDPInfo(
    u8 pagenum
);

BOOLEAN
OnNS(
    u16 addrl_frame_ctrl,
    u8 MacHeaderLen
) ; 
void
IssueNA(
    u8* TargetIP,
    u8* TargetMAC,
    u8* OurIP,
    u8* OurMAC,
    u8  retry_bit,
    u8  ndpsel

);


void 
VerifyICMPV6Checksum(
void
);

void
FillIpv6PseudoHeader(
    u8* TargetIP,
    u8* OurIP,
    u32  PayLoadLength,
    u8  NextHeader
);


BOOLEAN
IsIPV6(
    u16 addrl_frame_ctrl,
    u8 MacHeaderLen
)  ;

u8
Ipv6IpType(
u8* ReceiveIpv6IpAddress,
u8* MyIpv6IpAddress
);


void 
PassSecurityInfoToDriver(
    u8 pagenum
) ;


void 
GetRemoteControlInfo(
    u8 pagenum
)
;

void 
EnableRemoteWakeUpAllFunc(
    void
)
; 

void 
DisableRemoteWakeUpAllFunc(
    void
)
; 

extern void
InitDisconnectDecision(
    void
) ;

extern void
DisconnectChk(
    void
) ;

extern void
DisconnectTxNullChk(
    IN u8 pwr
) ;

extern void 
H2CHDL_DisconnectDecision(
    u8 *pbuf
) ;

extern void
InitKeepAlive(
    void
) ;

extern void 
KeepAliveChk(
    void
) ;

#if 0
extern void
KeepAliveTxNullChk(
    IN u8 pwr
) ;
#endif

extern void 
H2CHDL_KeepAlive(
    u8 *pbuf
) ;

extern void 
WoWLAN_ANT_SWT(
    IN u8 value
) ;

extern u16
PageOffsetAlignment(
    IN u16 offset
) ;

void
H2CHDL_GPIO_WF_Customer(
    u8 *pbuf
) ;

//void UpdateIV(unsigned char *buf, unsigned char retry_bit);

#endif  //__WLAN_WoWLAN_RAM_H__
