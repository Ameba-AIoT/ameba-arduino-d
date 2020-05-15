#ifndef __RTL8721D_RATE_ADAPTIVE_H__
#define __RTL8721D_RATE_ADAPTIVE_H__

#ifndef __ASSEMBLY__

#define RATESIZE        20
#define CONFIG_SGI      0
#define RATE_UNUSED 0xff
#define CLEAN_ENV_VAR_TH    2

#define RETRYSIZE       5
#define STEP_DROP      1
//#define CONFIG_SGI     0

//---RAInfo----------
#define MASK_RA_ULDL_STATE      BIT0
#define MASK_RA_STBC_STATE      BIT1
#define MASK_RA_NOISY_STATE    BIT2
#define MASK_RA_NO_USE_1          BIT3
#define MASK_RA_NO_USE_2          BIT4
#define MASK_RA_INIT_RATE_RSSI_STATE BIT5
#define MASK_RA_BF_STATE		BIT6
#define MASK_RA_NO_USE_3		BIT7

// RA mask
#define Rate_id_NUM         9 // 6 rate id from reg
#define ARFB_table_NUM      7

#define TRYING_DISABLE      0
#define TRYING_ENABLE        1

#define F_RATE_DECISION   BIT0
#define F_INIT_RA_TABLE   BIT1
#define F_RATE_UP               BIT2
#define F_RATE_DOWN_STEP   BIT3
#define F_TRY_DONE        BIT4
#define F_RA_H2C            BIT5
#define F_RATE_AP_RPT   BIT7

//---Init rate by RSSI----------
#define  RSSI_TH1 45
#define  RSSI_TH2 15

// H2C CMD OX40 ------------------------
//offset0
#define H2CID13_MACID       0x7F
//offset1
#define H2CID13_RATEID      0x1F
#define H2CID13_RSSI_LEVEL      (BIT6|BIT5)
#define H2CID13_SGI             BIT7
//offset2
#define H2CID13_BW              (BIT0|BIT1)
#define H2CID13_enldpc          BIT2
#define H2CID13_NOUPDATE    BIT3
#define H2CID13_VHT_EN      (BIT5|BIT4)
#define H2CID13_DISPT          BIT6
#define H2CID13_DISRA          BIT7

//---C2H DBG----------
#define RTY_PENALTY 1
#define N_HIGH 2
#define N_LOW 3
#define RA_DEBUG_MONITOR1   0xc
#define RA_DEBUG_MONITOR2   0xd
#define RA_DEBUG_MONITOR3   0xe
#define RA_DEBUG_MONITOR4   0xf

//#define RA_VERSION  RTL8721D_FW_VERSION

//H2C AP_Req_Tx_Rpt
#define H2CID43_RTY_OK_TOTAL BIT0
#define H2CID43_RTY_CNT_MACID   BIT1
    
typedef enum _RTL8195_RATEID_IDX_ {
    MODE_BGN_40M_2SS    = 0,
    MODE_BGN_40M_1SS    = 1,
    MODE_BGN_20M_2SS_BN = 2,
    MODE_BGN_20M_1SS_BN = 3,
    MODE_GN_N2SS        = 4,
    MODE_GN_N1SS        = 5,
    MODE_BG             = 6,
    MODE_G              = 7,
    MODE_B              = 8,
    MODE_ARFR0          = 9,
    MODE_ARFR1          = 10,
    MODE_ARFR2          = 11,
    MODE_ARFR3          = 12,
    MODE_ARFR4          = 13,
    MODE_ARFR5          = 14
} RTL8195_RATEID_IDX, *PRTL8195_RATEID_IDX;

#if 1
typedef struct  stainfo_rate_adaptive{

    u8   highest_rate;
    u8   lowest_rate;
    
    u8   RateID:5;
    u8   SGI:1;
    u8   fast_dec_sucess:2; 

    u8    NUM_SS:2; //number of max supported spatial stream
    u8    rsvd1:1;
    u8    trying_enable:1;
    u8    rate_UpDown:2; //0: up , 1:down
    u8    pre_bw:2;          //previous BW

    u8    multi_try_times:4;
    u8    consective_rate_up_fail_cnt: 4;
        
    u8   Initial_BW:2;
    u8   BW_setting:2;
    u8   VHT_EN:2;
    u8   DISPT:1;
    u8   DISRA:1;

    u8    pre_rate;
    u8    device_rssi;
    u8    ra_info;

    // Multi Try RA 
    u8    ra_up_waiting;
    u8    ra_up_fail_rssi;
    u8    ra_candidate_rate1;
    u8    ra_candidate_rate2;
    u8    ra_stage;
    u8    no_rate_up_cnt;
    u8    pcr_shift;
    
    //---------	
    u16 TOTAL;
    u8   DROP;
    u8   BUPDATE;
    u16 RTY[5];	

        //2 Power Trainning
    u8   TRAINING_RATE;
    u8   PRE_RSSI;
    u8   STOP_PT_COUNTER;
    u8   MODE_SS;
    u8   PT_smooth_factor;
    
    u8    pcr_updown_bitmap: 4; //up: 1  down: 0
    u8    rsvd2:4;
	
}STAINFO_RA, *PSTAINFO_RA;

#if 1//CONFIG_RATE_ADAPTIVE
typedef struct rate_adaptive_general{

        u8 RA_version;
	 #if 0//(CONFIG_HALcom_RA_2 ==0)
        u8 RateTypeStart[RATE_TYPE_NUM];
        u8 RateTypeLength[RATE_TYPE_NUM];
	 #endif

        #if 1//(CONFIG_PER_CNT == 1)
        u8 PCR_VAR[RATESIZE];
        u8 PCR_MA[RATESIZE];
        #else
        u8 RETRY_PENALTY_IDX_0[RATESIZE];
        u8 RETRY_PENALTY_IDX_1[RATESIZE];
        #endif
        u8 ra_counter;

}PHYDM_RA_T,*PPHYDM_RA_T;
#endif

typedef struct phydm_debug{

        u8  fw_trace_en;
        //u8  monitor_mode;
        //u8  macid;       

}PHYDM_DEBUG_T,*PPHYDM_DEBUG_T;

typedef struct _odm_{
    STAINFO_RA    stainfo_ra[MACID_NUM];
    PHYDM_DEBUG_T DEBUG;
    PHYDM_RA_T    RA;
    u16           PASS_C2H[MACID_NUM];
    u16           DROP_C2H[MACID_NUM];
    u32           RTY_C2H;
    u16           RTY_CNT_C2H[MACID_NUM];
    u16           TOTAL_CNT_C2H[MACID_NUM];
}odm,*Podm;

#endif


extern void
Set_arfr_8721D_ROM(
    IN u8  e0,
    IN u8  e1,
    IN u8  e2,
    IN u8  e3,
    IN u8  e4,
    IN u8  e5,
    IN u8  e6,
    IN u8  e7
) ;

extern void 
InitRAInfo_8721D_ROM(
    void
);

#if 0
extern void
C2H_RA_Rpt_CallPtr(
    u8  macid,
    u8  rate,
    u8  content_0,
    u8  content_1
) ;
#endif

extern void
C2H_RA_Rpt_8721D_ROM(
    u8  macid,
    u8  rate,
    u8  content_0,
    u8  content_1
);

extern u8
check_rate_8721D_ROM(
    IN u8 rate_idx,
    IN u8 macid
) ;

extern void
RateUp_search_RateMask_8721D_ROM(    
    IN u8		init_rate_idx,
    IN u8		highest_rate,
    IN u8		macid
) ;

extern void
RateDown_search_RateMask_8721D_ROM(    
    IN u8		init_rate_idx,
    IN u8		lowest_rate,
    IN u8		macid,
    IN u8		mod_step
) ;

extern void 
RateDownTrying_8721D_ROM(
    IN u8 macid
);

u16
Count_Ratio_8721D_ROM(
    IN u16  cnt,
    IN u16 total_cnt
);

extern void
ArfrRefresh_8721D_ROM(
    IN u8 macid
) ;

extern void
InitialRateByRssi_8721D_ROM(
    IN u8 macid,
    IN u8 start_rssi
);

#if 0
extern void
InitialRateUpdate_CallPtr(
    IN u8 macid, 
    IN u8 rate, 
    IN u8 trybit,
    IN u8 BW
);
#endif

extern void
H2CHDL_Set_FW_Trace_en_8721D_ROM(
    u8  *pbuf
);

extern void 
H2CHDL_Set_MACID_Config_8721D_ROM(
    u8 *pbuf
) ;

extern void    
H2CHDL_SetRssiSingle_8721D_ROM(
    u8  *pbuf
) ;

extern void
ODM_H2C_RADebugMonitor_8721D_ROM(
    IN      u8      macid,
    IN      u8      mode
);

/*extern void
H2CHDL_Set_FW_Trace_en_8721D_ROM(
    u8  *pbuf
);*/

extern void
C2H_RA_Dbg_code_en_8721D_ROM(
    u8  function,
    u8  dbg_num,    
    u16  content_0,
    u16  content_1,
    u16  content_2,
    u16  content_3,
    u16  content_4   
) ;

extern void
H2CHDL_APReqTxrpt_8721D_ROM(
    u8  *pbuf
);

extern void
H2CHDL_InitRateCollect_8721D_ROM(
    u8 *pbuf
);


extern u8
Pkt_Num_shift_8721D_ROM(
    IN u16  total
) ;

extern void
PT_update_8721D_ROM(
    IN u8 macid,
    IN u8 stage
) ;

#endif
#endif
