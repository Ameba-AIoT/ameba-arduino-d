#ifndef __WLAN_P2PPS_RAM_H__
#define __WLAN_P2PPS_RAM_H__


#if !CONFIG_ROM_32K

/*--------------------------Define -------------------------------------------*/

/*------------------------------Define Enum-----------------------------------*/

typedef enum _P2P_ROLE_ {
    P2P_CLIENT = 0,
    P2P_OWNER = 1
}P2P_ROLE, *PP2P_ROLE;


typedef enum _P2P_PS_OWNER_STATE_{
    P2PPS_OWNER_STATE_DISABLE,
    P2PPS_OWNER_STATE_ALLON,
    P2PPS_OWNER_STATE_ALLOFF,
    P2PPS_OWNER_STATE_RFON_TXPAUSE,
    P2PPS_OWNER_STATE_SCAN
}P2P_PS_OWNER_STATE, *PP2P_PS_OWNER_STATE;


/*--------------------------Define MACRO--------------------------------------*/

/*------------------------------Define Struct---------------------------------*/

typedef struct _P2P_PS_Offload_ {
    u8          Offload_En:1;
    u8          Role:1;         //typedef enum _P2P_ROLE_ {
    u8          CTWindow_En:1;
    u8          NoA0_En:1;
    u8          NoA1_En:1;
    u8          AllStaSleep:1;
    u8          Discovery:1;
    u8          rsvd:1;

}P2P_PS_Offload, *PP2P_PS_Offload;


typedef struct  _P2PPS_Info_ {
    //Common
    P2P_PS_Offload  P2PPSPara;
    BOOLEAN         bCTWPeriod;         //From Beacon Early to CTWEnd
    BOOLEAN         bSpecCTWPeriod; //From TBTT to CTWEnd

    //Owner
    u8      GOCurState;

    //Client
    u8      ClientCurState;

}P2PPS_Info, *PP2PPS_Info;

#endif //#if !CONFIG_ROM_32K
/*------------------------Export global variable------------------------------*/

/*------------------------------Function declaration--------------------------*/
#if CONFIG_P2P_PS
//-------------------------------------------------------
//
//      Common Part
//
//-------------------------------------------------------

extern void
InitP2PPS(
    void
);


extern BOOLEAN
CheckGOPSStateOn(
    void
);


extern BOOLEAN
CheckGOSpecPSStateOn(
    void
);


//-------------------------------------------------------
//
//      Group Owner Part
//
//-------------------------------------------------------

extern void
ChangeP2PPSOwnerState(
    IN  u8  RequestState
);


extern void
AutoChangeP2PPSOwnerState(
    void
);

extern BOOLEAN
ConditionMatchedForGO32K(
    void
);

//-------------------------------------------------------
//
//      Client Part
//
//-------------------------------------------------------
extern BOOLEAN
CheckAPStateToChangePSState(
    IN  u8  ps_state
) ;

extern void 
P2PPS_ClientCheckDeferOperation(
    void
) ;

extern void 
CheckLPSTxPktInInAPRFOn(
    void
) ;

extern void
P2PPS_RFOnForSmartPS(
    void
) ;

extern void 
RFOffBCNToHDL(
    void
) ;

extern void 
P2PPS_OnBeacon(
    void
) ;

#endif  //#if   CONFIG_P2P_PS

#endif  //__WLAN_P2PPS_RAM_H__

