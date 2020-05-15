#ifndef __WLAN_P2POFFLOAD_RAM_H__
#define __WLAN_P2POFFLOAD_RAM_H__

/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/
//H2C Index: 0x8A
typedef struct _P2P_RSVDPAGE_ {
    u8  loc_bcn;
    u8  loc_p2p_ProbeRsp;
    u8  loc_p2p_NegoRsp;
    u8  loc_p2p_InvitationRsp;
    u8  loc_p2p_PDRsp;
}P2P_RSVDPAGE,*PP2P_RSVDPAGE;

//H2C Index: 0x8B
typedef struct _P2P_OFFLOAD_ {
    u8  Disconnect_WakeDriv:1;
    u8  Role:2;
    u8  rsvd:5;
    
    u8  WPS_config[2];
}P2P_OFFLOAD,*PP2P_OFFLOAD;

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/

extern void
P2POffload_hdl(
    IN u8 bFunEnable
);

#endif  //__WLAN_P2POFFLOAD_RAM_H__

