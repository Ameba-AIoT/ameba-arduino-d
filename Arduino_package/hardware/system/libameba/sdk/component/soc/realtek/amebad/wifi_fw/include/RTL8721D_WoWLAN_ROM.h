#ifndef __WLAN_WoWLAN_H__
#define __WLAN_WoWLAN_H__

/*--------------------Define --------------------------------------------*/
#define RXINDIRECTPAGESIZE  0xFF

#define DEFAULT_DISCONNECT_COUNT_LIMIT      8       //UNIT: beacon interval
#define DEFAULT_TRY_PKT_LIMIT               3
#define KeepAliveUnit                       10      //UNIT: beacon interval
#define DEFAULT_KEEP_ALIVE_COUNT_LIMIT      0x0A    //Unit: BCN interval
#define WAIT_RXDMA_CNT                      (2000)
#define CHECK_RXDMA_ONCE_TIME               (10)
#define SDD1RDY                             BIT3
#define MAGIC_WAKEUP                        BIT7
#define UNICAST_WAKEUP                      BIT6
#define PATTERN_WAKEUP                      BIT5
#define WAKEUPFEATURE                       BIT2
#define DMAIDLE                             BIT1
#define DMAPAUSE                            BIT2
#define ProtectedFrame                      BIT6

#define ARP_SEL_PASSIVE_RESPONSE            0
#define ARP_SEL_KEEPALIVE_UNICAST_GATEWAY   1

#define NDP_SEL_PASSIVE_RESPONSE            0
#define NDP_SEL_KEEPALIVE_UNICAST_GATEWAY   1


#define KEEPALIVE_TYPE_NULLDATA             0
#define KEEPALIVE_TYPE_ARP_RESPONSE         1

#define ARP_ACTION_ARP_RESPONSE             0
#define ARP_ACTION_WAKEUP_HOST              1

#define WAKEUP_HOST_FROM_AP_OFFLOAD         0
#define WAKEUP_HOST_FROM_WOWLAN             1

//WakeupHostDecision function Control
#define CHECK_WAKEUP_BY_DESC                BIT0
#define CHECK_WAKEUP_BY_INT                 BIT1
/*--------------------Define Enum---------------------------------------*/
enum WIFI_FRAME_TYPE {
    WIFI_MGT_TYPE  =    (0),
    WIFI_CTRL_TYPE =    (BIT2),
    WIFI_DATA_TYPE =    (BIT3),
};

enum WIFI_FRAME_SUBTYPE {

    // below is for mgt frame
    WIFI_ASSOCREQ       = (0 | WIFI_MGT_TYPE),
    WIFI_ASSOCRSP       = (BIT4 | WIFI_MGT_TYPE),
    WIFI_REASSOCREQ     = (BIT5 | WIFI_MGT_TYPE),
    WIFI_REASSOCRSP     = (BIT5 | BIT4 | WIFI_MGT_TYPE),
    WIFI_PROBEREQ       = (BIT6 | WIFI_MGT_TYPE),
    WIFI_PROBERSP       = (BIT6 | BIT4 | WIFI_MGT_TYPE),
    WIFI_BEACON         = (BIT7 | WIFI_MGT_TYPE),
    WIFI_ATIM           = (BIT7 | BIT4 | WIFI_MGT_TYPE),
    WIFI_DISASSOC       = (BIT7 | BIT5 | WIFI_MGT_TYPE),
    WIFI_AUTH           = (BIT7 | BIT5 | BIT4 | WIFI_MGT_TYPE),
    WIFI_DEAUTH         = (BIT7 | BIT6 | WIFI_MGT_TYPE),
    WIFI_ACTION         = (BIT7 | BIT6 | BIT4 | WIFI_MGT_TYPE),

    // below is for control frame
    WIFI_PSPOLL         = (BIT7 | BIT5 | WIFI_CTRL_TYPE),
    WIFI_RTS            = (BIT7 | BIT5 | BIT4 | WIFI_CTRL_TYPE),
    WIFI_CTS            = (BIT7 | BIT6 | WIFI_CTRL_TYPE),
    WIFI_ACK            = (BIT7 | BIT6 | BIT4 | WIFI_CTRL_TYPE),
    WIFI_CFEND          = (BIT7 | BIT6 | BIT5 | WIFI_CTRL_TYPE),
    WIFI_CFEND_CFACK    = (BIT7 | BIT6 | BIT5 | BIT4 | WIFI_CTRL_TYPE),

    // below is for data frame
    WIFI_DATA           = (0 | WIFI_DATA_TYPE),
    WIFI_DATA_CFACK     = (BIT4 | WIFI_DATA_TYPE),
    WIFI_DATA_CFPOLL    = (BIT5 | WIFI_DATA_TYPE),
    WIFI_DATA_CFACKPOLL = (BIT5 | BIT4 | WIFI_DATA_TYPE),
    WIFI_DATA_NULL      = (BIT6 | WIFI_DATA_TYPE),
    WIFI_CF_ACK         = (BIT6 | BIT4 | WIFI_DATA_TYPE),
    WIFI_CF_POLL        = (BIT6 | BIT5 | WIFI_DATA_TYPE),
    WIFI_CF_ACKPOLL     = (BIT6 | BIT5 | BIT4 | WIFI_DATA_TYPE),
    WIFI_QOS_DATA       = (BIT7 | WIFI_DATA_TYPE),
    WIFI_QOS_NULL       = (BIT7 | BIT6 | WIFI_DATA_TYPE),
};

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/
#if 0
typedef struct _SIDEBAND_WOWLAN_INFO_{
    u8 HST2DEV_GPIO;
    u8 DEV2HST_GPIO;
    u8 hst2dev_val;
}SIDEBAND_WOWLAN_INFO, *PSIDEBAND_WOWLAN_INFO;
#endif

#if 0
typedef struct _AOAC_REPORT_{
    u8   IV[8];
    u8   ReplayCounterOfEapolKey[8];
	u8   GroupKey[32];
    u8   KeyIndex;
    u8   scurityType;            //per received packet
//    u8   WowPatternIndex;        //per received packet
//    u8   Rsvd;    
}AOAC_REPORT, *PAOAC_REPORT;
#endif

#define AOAC_REPORT_VERSION      0x1

typedef struct _AOAC_REPORT_ {
    u8   pPTKTXIV[8];    
    u8   pReplayCntEapolKey[8];
    u8   pGroupKey[32];          //the last updated GTK data
    u8   u1KeyIdx;               //the last updated GTK index
    u8   u1SecurityType;         //per received packet
    u8   u1WowPatternIdx;        //per received packet
    u8   u1RptFormatVer;         //In old FW, this is a 1-byte rsvd, FW clear this byte certainly
    u8   u1Rsvd[4];              //In old FW, these 4 bytes are not include in AOAC rpt. Don't use these 4 bytes. Just for 8-byte alignment
    u8   u4RxPTKIV[8];           //Unicast IV 
    u8   u4RxGTKIV_0[8];         //Broadcast/Mulicast IV, 4 GTK index
    u8   u4RxGTKIV_1[8];
    u8   u4RxGTKIV_2[8];
    u8   u4RxGTKIV_3[8];
} AOAC_REPORT, *PAOAC_REPORT;

typedef struct _SECURITY_INFO_{
    //u8   IV[8];
    //u8   GroupKey[16];	
    //u8   KeyIndex;
    //u8   scurityType;            //per received packet
    //union pn48      iv_counter;
    H2C_WPA_TWO_WAY_PARA wpa_two_way_para;
        //WPA_LOC wpa_loc;
    H2C_EAPOL_KEY_STRUCT gtk_req;
    wpa_sm  wpa_sm_data;
}SECURITY_INFO,*PSECURITY_INFO;

typedef struct _ARP_INFO_{
    //u8       BSSID[6];
    //u8       MACAddr[6];
    u8       A3Addr[6];//gateway's address
    u8       IPV4[4];
    u8       THA[6];
    u8       TPA[4];
}ARP_INFO,*PARP_INFO;


typedef struct _NDP_INFO_{

    u8 Enable:1;
    u8 bCheckRemoveIP:1;   // Need to Check Sender IP or not 
    u8 Rsvd:6;   // Need to Check Sender IP or not 
    u8 NumberOfTargetIP; // Number of Check IP which NA query IP
    u8 TagetLinkAddress[6];  // Maybe support change MAC address !!
    u8 RemoteIPv6Address[16]; // Just respond IP
    u8 TargetIP[2][16]; //  target IP 

}NDP_INFO,*PNDP_INFO;

typedef struct _H2C_PROTOCOL_OFFLOAD_INFO{

        NDP_INFO NDPInfo[2];
        //NDP_INFO NDPInfoExt; 

} H2C_PROTOCOL_OFFLOAD_INFO, *PH2C_PROTOCOL_OFFLOAD_INFO;

typedef struct _IPV6_PSEUDO_HEADER_INFO{

    u8 SourceAddress[16];
    u8 DestinationAddress[16];
    u32 PayPloadLength;
    u8 rsvd1[3];
    u8 NextHeader;

} IPV6_PSEUDO_HEADER_INFO, *PIPV6_PSEUDO_HEADER_INFO;


typedef struct _AOAC_INFO_{

    ARP_INFO    ARPInfo;
    NDP_INFO    NDPInfo[2];
    IPV6_PSEUDO_HEADER_INFO Ipv6PseudoHeaderInfo;
    //NDP_INFO    NDPInfoExt;
    SECURITY_INFO SecurityInfo;
    AOAC_REPORT AOACReport;
#if 1//CONFIG_TEREDO_REALWOWL    
    TEREDO_INFO         teredo_info;  
#endif //#if CONFIG_TEREDO_REALWOWL    
#if 1//CONFIG_REALWOW_V2
    REALWOW_VER2_INFO   RWV2;
#endif

#if 1//(CONFIG_NLO_COMPLEX_OFFLOAD||CONFIG_NLO_SIMPLE_OFFLOAD)
    NLO_OFFLOAD_NETWORK_LIST_INFO NetworkListInfo;
#endif //(CONFIG_NLO_COMPLEX_OFFLOAD||CONFIG_NLO_SIMPLE_OFFLOAD)
}AOAC_INFO,*PAOAC_INFO;

#if 0
typedef struct _CUSTOMER_WF_{

    u8          CustomerID; 
}CUSTOMER_WF,*PCUSTOMER_WF;
#endif

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/

#if 0
extern BOOLEAN
WaitRXDMAIdle_8721D_ROM(
    void
) ;

extern void 
UpdateRxFFReadPtr_DropPkt_8721D_ROM(
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
H2CHDL_AOACGlobalInfo_8721D_ROM(
    u8 *pbuf
) ;

extern void 
H2CHDL_AOACRsvdpage1_8721D_ROM(
    u8 *pbuf
) ;

extern void 
H2CHDL_AOACRsvdpage2_8721D_ROM(
    u8 *pbuf
) ;

extern void 
H2CHDL_AOACRsvdpage3_8721D_ROM(
    u8 *pbuf
) ;


extern void 
ARP_hdl_8721D_ROM( 
    u8 bFunEnable
);

extern void 
NDP_hdl_8721D_ROM( 
    u8 bFunEnable
);

extern void 
GTK_hdl_8721D_ROM( 
    u8 bFunEnable
);



extern void 
issue_ARP_8721D_ROM(
    u8* TargetIP,
    u8* TargetMAC,
    u8  retry_bit,
    u8  arpsel
);

extern BOOLEAN
OnARP_8721D_ROM(
    u16 addrl_frame_ctrl,
    u8 MacHeaderLen
) ; 

extern void
GetARPInfo_8721D_ROM(
u8 pagenum
);


extern void 
GetNDPInfo_8721D_ROM(
    u8 pagenum
);

extern BOOLEAN
OnNS_8721D_ROM(
    u16 addrl_frame_ctrl,
    u8 MacHeaderLen
) ; 

extern void
IssueNA_8721D_ROM(
    u8* TargetIP,
    u8* TargetMAC,
    u8* OurIP,
    u8* OurMAC,
    u8  retry_bit,
    u8  ndpsel

);


extern void
FillIpv6PseudoHeader_8721D_ROM(
    u8* TargetIP,
    u8* OurIP,
    u32  PayLoadLength,
    u8  NextHeader
);


extern BOOLEAN
IsIPV6_8721D_ROM(
    u16 addrl_frame_ctrl,
    u8 MacHeaderLen
)  ;
#endif

extern void 
GetIV(
    u16 addrl_frame_ctrl,
    u8 MacHeaderLen,
    u8 IvLen,
    u8* IV
);

extern u8
GetIVLenAndSecurityType_8721D_ROM(
    u8 raaddr
);

extern u8
GetMACHeaderLen_8721D_ROM(
u8  macheader,
u8  FrameCtrl,
u8  raaddr
) ;
    
extern void 
UpdateIV_8721D_ROM(
    u8 *buf, 
    u8 retry_bit
);

extern void 
updateCheckSumVer2_8721D_ROM(
    IN u8 mode,
    IN u16 offset,
    IN u8 loc_rsvd
) ;

#if 0
extern void 
PassSecurityInfoToDriver_8721D_ROM(
    u8 pagenum
) ;

extern void 
GetRemoteControlInfo_8721D_ROM(
    u8 pagenum
)
;



extern void
InitDisconnectDecision_8721D_ROM(
    void
) ;

extern void
DisconnectChk_8721D_ROM(
    void
) ;

extern void
DisconnectTxNullChk_8721D_ROM(
    IN u8 pwr
) ;

extern void 
H2CHDL_DisconnectDecision_8721D_ROM(
    u8 *pbuf
) ;

extern void
InitKeepAlive_8721D_ROM(
    void
) ;

extern void 
KeepAliveChk_8721D_ROM(
    void
) ;

#if 0
extern void
KeepAliveTxNullChk(
    IN u8 pwr
) ;
#endif

extern void 
H2CHDL_KeepAlive_8721D_ROM(
    u8 *pbuf
) ;

void 
WakeUpHost_8721D_ROM(
    IN u8 source
) ;


extern u16
PageOffsetAlignment_8721D_ROM(
    IN u16 offset
) ;

extern BOOLEAN
DestinationMatch_8721D_ROM(
    u16 addrl_frame_ctrl
);

extern void 
FwDisConnectWakeUpHost_8721D_ROM(
    IN u8 type
);

extern BOOLEAN 
FwReKeyWakeUpHost_8721D_ROM(
    IN  u16 addrl_frame_ctrl, 
    IN  u8  macHeaderLen,
    IN  u8  LLC
);

extern BOOLEAN 
MatchQQ_8721D_ROM(
    u16 addrl_frame_ctrl, 
    u8 macHeaderLen
) ;

extern BOOLEAN
WakeUpHostDecision_8721D_ROM(
    u8 RxDescWakeReason,
    u8 CheckMode
) ;

extern void
EnableWoWLAN_8721D_ROM(
    void
) ;

extern u8
DropCHK_8721D_ROM(
    void
) ;

extern void
GetAddr_8721D_ROM(
    void
) ;

extern BOOLEAN
DropDecision_8721D_ROM(
    void
) ;
#endif

//void UpdateIV(unsigned char *buf, unsigned char retry_bit);

#endif  //__WLAN_WoWLAN_H__
