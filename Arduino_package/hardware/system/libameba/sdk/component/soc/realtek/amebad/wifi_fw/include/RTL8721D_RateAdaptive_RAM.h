#ifndef __RTL8721D_RATE_ADAPTIVE_RAM_H__
#define __RTL8721D_RATE_ADAPTIVE_RAM_H__

#define NO_MULTI_TRY    0xFF


#define RATE_HOLD 0
#define RATE_UP 1
#define RATE_DOWN 2

#define RESET_TX_RTY BIT0
#define RESET_TX_TOTAL BIT1
#define RESET_TX_DROP BIT2

#define SUPER_RA_NUM 5

//---PER RA----------
#define PCR_VAR_SIZE      10
#define DIFF2VAR_SHIFT  10
#define PCR_MA_NUM        8 //packet correct rate moving average number
#define PCR_VAR_MAX      15
#define MA_RATIO             3 //1/2^N (new) + (2^N-1)/2^N (old)
#define COMMON_RATE_DOWN_OFFSET 3
#define SGI_DOWN_TO_LGI_OFFSET 8   // N-SGI  -> N_LGI    //  ratio = (3.2/3.6) = 0.88888
    
//---others----------
#define DIFF(A,B) ((A>=B) ? (A-B):(B-A))

// Power Training
#define PT_mask				0x0000ff00
#define SS_PT_TH_High   66
#define SS_PT_TH_low    57
#define SS_PT_off       48

extern void 
InitRAInfo_rsvd_8721D_RAM(
    void
);

extern void
InitRATable_8721D_RAM(
    void
);

#if 0//CONFIG_11AC
extern u8 
VHT_Rate_shiftRAM(
	IN u8 VHT_HT_swich,
	IN u8 rate_idx,
	IN u8 dir
) ;
#endif

#if 0//CONFIG_2SS
extern void
Dynamic_ARFB_Table_Select(
    IN u8 macid,
    IN u8 rate_idx
) ;
#endif

extern void
C2H_RA_Rpt_8721D_RAM(
    u8  macid,
    u8  rate,
    u8  content_0,
    u8  content_1
);

extern void 
InitialRateUpdate_8721D_RAM(
    IN u8 macid, 
    IN u8 rate, 
    IN u8 trybit,
    IN u8 BW
) ;

extern void 
InitRAInfo_8721D_RAM(
    void
);

#if 0//(CONFIG_2SS)
extern u8
b1ssRateFirst(
	IN u8 macid,
	IN u8 rate1,
	IN u8 rate2
) ;

extern u8
MultiSearchRateRAM(
	IN u8 macid,
	IN u8 rate_idx,
	IN u8 up_down
) ;
#endif

extern void
TryDone_8721D_RAM(
    void
);

extern void 
RateUp_8721D_RAM(
    IN u8 macid,
    IN u8 trybit
);

#if 0//CONFIG_2SS
extern u8
check_lowest_rate(
    IN u8 rate_idx,
    IN u8 lowest_rate,
    IN u8 n_ss
);
#endif

extern void
RateDownStep_8721D_RAM(
    IN u8 macid, 
    IN u8 step,
    IN u8 trybit
);

extern void 
RateDecision_8721D_RAM(
    void
);

extern void
C2H_RA_Dbg_code_en_8721D_RAM(
    u8  function,
    u8  dbg_num,    
    u16  content_0,
    u16  content_1,
    u16  content_2,
    u16  content_3,
    u16  content_4   
) ;

#if CONFIG_POWER_TRAINING_WIL

extern u8
PT_Mode_Sel_8721D_RAM(
    IN u8  macid,
    IN u8  rate_idx
) ;


#endif  //#if CONFIG_POWER_TRAINING_WIL

#endif
