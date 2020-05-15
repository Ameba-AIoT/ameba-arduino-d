#ifndef __WLAN_AP_Offload_RAM_H__
#define __WLAN_AP_Offload_RAM_H__

/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/
enum _STRUCT_RF_STATE_{
    RFState_ON = 0,
    RFState_OFF = 1
};
/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/
typedef struct _ap_offload_parm_ {
    u8      on:1;     // 1: on , 0: off
    u8      cfgMifiPlatform:1;
    u8      linked:1;	//1: sta linked, 0: idle
    u8      EnAutoWake:1;
    u8      wakeFlag:1;
    u8      GPIOwakeup:1;
    u8      OffH2C:1;
    u8      rsvd:1;    
    u8      hidden; // 1: hidden, 0:Broadcast
    u8      denyAny;    //1. deny SSID:any or ANY, 0: accept
    u8	waitTimerCnt;	//wait for host suspend, ms unit
    u8	wakeTimeout;	//auto wake duration, TBTT unit
    u8      lenIVPair;	//pair IV len, for ARP/DHCP parsing
    u8      lenIVGrp;	//group IV len
}ap_offload_parm,*Pap_offload_parm;


typedef struct _ap_offload_rfctrl_parm_ {
    u8  on;                 // 1: on , 0: off
    u16 OnTimeout;      //unit: ms, bcnearly is the start point
}ap_offload_rfctrl_parm,*Pap_offload_rfctrl_parm;

typedef struct _STRUCT_AP_OFFLOAD_{
    ap_offload_parm             BasicParm;
    ap_offload_rfctrl_parm      RFCtrlParm;
    u8                      RFState;
}STRUCT_AP_OFFLOAD,*PSTRUCT_AP_OFFLOAD;

typedef struct _hidden_parm_{
    u8                  Root;
    u8                  VAP1;
    u8                  VAP2;
    u8                  VAP3;
    u8                  VAP4;
    
}hidden_parm,*Phidden_parm;

typedef struct _denyany_parm_{
    u8                  Root;
    u8                  VAP1;
    u8                  VAP2;
    u8                  VAP3;
    u8                  VAP4;
}denyany_parm,*Pdenyany_parm;


typedef struct  _BCN_rsvdpage_loc_ {
    u8      loc_Root;
    u8      loc_VAP1;
    u8      loc_VAP2;
    u8      loc_VAP3;
    u8      loc_VAP4;
}BCN_rsvdpage_loc,*PBCN_rsvdpage_loc;


typedef struct  _Probersp_rsvdpage_loc_ {
    u8      loc_Root;
    u8      loc_VAP1;
    u8      loc_VAP2;
    u8      loc_VAP3;
    u8      loc_VAP4;
}Probersp_rsvdpage_loc,*PProbersp_rsvdpage_loc;
typedef struct _Multi_WakeupPulse_Parm_ {
    u8      En;
    u8      duration_8ms;
    u8      duration_2s;
}Multi_WakeupPulse_Parm,*PMulti_WakeupPulse_Parm;

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/

extern void 
InitAPOffload(
    void
) ;

extern void 
ap_offload_on_setting(
    void
) ;

extern void 
ap_offload_off_setting(
    void
) ;

extern void 
Change_Beacon_Loc_MBSSID(
    IN u8 BCN_loc
) ;

extern void 
Switch_BCN(
    void
) ;

extern void 
ap_offload_MBSSID(
    void
) ;

void
PreWakeUpHost(
    void
) ;


BOOLEAN
IsArpReq(
    IN u16 headaddr,
    IN u8  fctrlOffset
);


u8 
CalcMACHeaderLen(
    u8  macheader,
    u8  FrameCtrl,
    u8  raaddr
) ;

//ben, 150116
u8
CalcMACHeaderLenSec(
    IN u16 headaddr,
    IN u8  fctrlOffset
) ;

BOOLEAN
IsUnicastData(
    IN u16 headaddr,
    IN u8  fctrlOffset
);

BOOLEAN
IsDHCP(
    IN u16 headaddr,
    IN u8  fctrlOffset
);
extern void
APOFLDTBTTHDL(
    void
) ;
#endif  //__WLAN_AP_Offload_RAM_H__

