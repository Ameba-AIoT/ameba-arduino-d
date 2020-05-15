#ifndef __RTL8721D_RCU_SYSMIB_H__
#define __RTL8721D_RCU_SYSMIB_H__


/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/
typedef struct _HW_Info_ {
    u8 single_chip:1;

} HW_Info, *PHW_Info;

typedef struct _RF_State_ {
    u8 rf_state_now;
    u8 rf_state_request;
    u8 wait_null1_txing;
    u8 tx_null1_result;
    //u8 rsvd:3;
} RF_State, *PRF_State;

typedef struct _WOW_Timer_Flag_ {
    u8 set_closeRF:1;
    u8 set_openRF:1;
    u8 rsvd:6;
} WOW_Timer_Flag, *PWOW_Timer_Flag;

typedef struct _RCU_LPS_Info_ {
    RF_State rf_state;
    WOW_Timer_Flag wow_timer_flag;
    u8 Enter32KHzPermission:1;
    u8 IsGoingTo32K:1;
    //u8 ClkRequestEnable:1;
    u8 IsRsvdpage_init:1;
    u8 rsvd:5;
} RCU_LPS_Info, *PRCU_LPS_Info;

typedef struct _RCU_H2C_Parms_ {
    u8 lps_func_en:1;
    u8 wow_func_en:1;
    u8 rsvd:6;
    u16 CloseRF_TO_ms;
    u16 wow_IVL_100ms;
    u8 wow_OpenRF_TO_ms;
    u8 h2c_index;
} RCU_H2C_Parms, *PRCU_H2C_Parms;

typedef struct _RCU_WOW_Parms_ {
        u8                                  RxShiftEn:1;
        u8                                  DMAShiftMode:1;
        u8                                  wakeup:1;
        u8                                  rsvd:5;
} RCU_WOW_Parms, *PRCU_WOW_Parms;

//3 1.) Xdata Section
typedef struct _rcu_mib_info_ {
    void (*intdis)(void);
    void (*inten)(void);

    //3 HAL
    HAL_DATA_8721D          IntHdl;

    //Media Status
    //MEDIA_STATUS            MediaStatus;

#if CONFIG_PARTIAL_OFF
    PG_PARM            PGParm;
#endif  //#if CONFIG_PARTIAL_OFF

    HW_Info		HWInfo;

    union{
        RF_DLPS_Parm    RfDlpsParm;
        RF_DLPS_Parm2   RfDlpsParm2;
    }RfDlpsParmUnion;
    u32                    RF_18;

    u8 skip_intmask_set;

    RCU_LPS_Info      rcu_lps_Info;
    RCU_H2C_Parms      rcu_h2c_parms;
    RCU_WOW_Parms    rcu_wow_parms;
    C2H_EVT_HDR c2h_hdr;
}rcu_mib_info, *Prcu_mib_info;

#ifdef __RTL8721D_RCU_INIT_RAM_C__

FW_RAM_BSS_SECTION    rcu_mib_info         RcuSysMib;

#else 

extern                rcu_mib_info         RcuSysMib;

#endif

/*------------------------Export global variable------------------------------*/
/*------------------------------Function declaration--------------------------*/
#endif  //__RTL8721D_SYSMIB_H__

