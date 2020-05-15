#ifndef __RTL8721D_RCU_INIT_RAM_H__
#define __RTL8721D_RCU_INIT_RAM_H__


/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/
#define NULL1_PKT_SIZE 64
#define RCU_TASK_PARSE_PKT BIT0
#define RCU_TASK_H2C_HDL BIT1
#define RCU_TASK_C2H_HDL BIT2

/*--------------------Define Struct---------------------------------------*/

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/

extern void 
Change_PS_State_8721D(
    IN  u8  request_ps_state,
    IN  u8  nulldata0Allow
) ;

extern BOOLEAN 
PS_S2_Condition_Match_8721D(
    void
) ;

extern BOOLEAN 
PS_S4_Condition_Match_8721D(
    void
) ;

extern void
SetBcnEarlyTime(
    void
);

extern void 
BcnEarlyAdjustByTSF_8721D(
    void
);

#if 0
extern void
EnlargeBcnEarlyAndTimeout_8721D(
    void
);

extern void 
ResetBcnEarlyAdjustParm_8721D(
IN	BOOLEAN OnJoin
);
extern void 
SetBcnEarlyAndTimeout_8721D(
    u8 BcnAheadShift,
    u8 BcnEarlyShift,
    u8 BcnTimeout
);
#endif

extern BOOLEAN
IssueNullData_8721D(
    IN  u8          bPowerBit,
    IN  u8          RtyLmt
) ;

extern void 
WakeUpHost_8721D(
    IN u8 source
) ;

extern unsigned char OnWpa_offload_8721D(  u16 mac_header_offset, u8 MacHeaderLen) ;
extern void wpa_supplicant_process_1_of_2_8721D( u16 version,u8 retry_bit) ;
extern void write_cam_wow_8721D( u8  entry, u8 encAlg, u8 *key) ;
extern void AesTkipIvFun_8721D(u8* pattrib_iv, u8 RetryBit) ;
extern void WepIvFun_8721D(u8* iv, u8 retry_bit) ;

extern u8
GetMACHeaderLen_8721D(
u8  macheader,
u8  FrameCtrl,
u8  raaddr
) ;

extern void 
UpdateIV_8721D(
    u8 *buf, 
    u8 retry_bit
);

#if (!IS_CUT_TEST(CONFIG_CHIP_SEL))
extern void
AppendTkipMIC_8721D(
    u8  pktLoc,
    u8  pyldOffset
);
#endif

void Null1_Rsvdpage_Init(void);

extern void
wifi_FW_init_ram(
    void
);

extern void 
TaskInit_8721D(
    void
);

extern void
TaskIdle_8721D(
    void
);

extern void
WlanInit(
    void
);

/*brian for function called both in isrhdl or not*/
extern _SendSignalCommon_Ptr               SendSignalCommon_Ptr;
extern xSemaphoreHandle	fw_lps_sema;
extern xSemaphoreHandle	fw_ipc_cmd_sema;
extern xSemaphoreHandle	fw_32k_sema;
extern xSemaphoreHandle	fw_common_sema;
extern u8 	RCU_ComTaskBitMap;
#endif  //__RTL8721D_RCU_INIT_RAM_H__

