#ifndef __WLAN_RCU_WoWLAN_RAM_H__
#define __WLAN_RCU_WoWLAN_RAM_H__

/*--------------------Define --------------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/
#if IS_CATEGORY_NIC_IoT(CONFIG_CATEGORY_SEL)
    #define IOT_WAITHOST_TO     5
#endif
#define WIFI_ACTION 0xD0
/*--------------------Define Struct---------------------------------------*/

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
extern void
RCU_WOW_Enable(u32 h2c_cmd);

extern void
RCU_WOW_Disable(void);

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
InitRemoteWakeUp(
    void
) ;

extern void 
RemoteWakeUp(
    void
) ;

extern void 
RemoteWakeupCtrl_hdl( 
    u8 bFunEnable
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
); 

extern u16
PageOffsetAlignment(
    IN u16 offset
);

void 
Parse_PKT(
    void
);

void 
ClearRxBuff(
    void
); 
#endif  
