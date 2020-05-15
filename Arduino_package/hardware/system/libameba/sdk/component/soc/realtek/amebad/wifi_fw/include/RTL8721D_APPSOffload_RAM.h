#ifndef __WLAN_APPSOffload_RAM_H__
#define __WLAN_APPSOffload_RAM_H__

#if IS_CHIP_8192F(CONFIG_CHIP_SEL)
/*--------------------Define --------------------------------------------*/
//#define DROPID_SHIFT 4
#define QInfo_MACIDMASK             0x7f
#define DROPID_MASK 0xF
#define GET_DROPID(x) ((x) & DROPID_MASK)
#define SET_DROPID(x) ((x) & DROPID_MASK)

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/
//#ifndef __ASSEMBLY__

typedef struct  _APPSSTA_PARM_ {
#if CONFIG_STA_AGING_FUNC
    u32 pauseTime;
#endif
    u8 SWPS_en:1;
    u8 dropid:2;
    u8 rsvd:5;
}APPSSTA_PARM, *PAPPSSTA_PARM;

typedef struct  _APPSBS_PARM_ {//bit status for sta
    u32                 bSTA_TxPause[MACID_NUM/32]; //1:pause
}APPSBS_PARM, *PAPPSBS_PARM;

typedef struct _HWDETC_PARM_ {
    u32                 bSTA_PwrState[MACID_NUM/32]; // 1:sleep 0:active 
}HWDETC_PARM, *PHWDETC_PARM;


typedef struct  _APPS_FUNC_ {
    APPSBS_PARM         ApPsBSParm; //bit status for sta
    APPSSTA_PARM        ApPsStaParm[MACID_NUM];
}APPS_FUNC, *PAPPS_FUNC;

typedef struct _HWDETECT_FUNC_ {
    HWDETC_PARM        HwDetcParm;
}HWDETECT_FUNC, *PHWDETECT_FUNC;

typedef struct _APPS_CTRL_ {
    HWDETECT_FUNC       HwDetectFunc;
    APPS_FUNC               ApPsFunc;
}APPS_CTRL, *PAPPS_CTRL;

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
extern void 
H2CHDL_AP_PS_OffloadSetting_8192F(
    IN  u8* pbuf
);

extern void 
H2CHDL_AP_PS_OffloadCtrl_8192F(
    IN  u8* pbuf
);

extern void
H2CHDL_APPS_MACIDPause_Aging_Func_Test_8192F( 
    IN  u8  *pbuf
);

extern void 
MACIDPause_Handler_8192F( 
    IN  u8 MACID_group,
    IN  u8 MACID_idx,
    IN  u8 isPause
);

extern void 
MACIDPause_Aging_Func_8192F( 
    void
);

extern void 
InitAPPSPara_8192F( 
    void
);

extern void
InitHWDETCPara_8192F(
    void
);

extern u8
ReadMACIDPause_8192F(
    u8 MACID_group,
    u8 MACID_idx
);

//#endif //#ifndef __ASSEMBLY__

#endif
#endif
