#ifndef __RTL8721D_INT_H__
#define __RTL8721D_INT_H__

#ifndef __ASSEMBLY__
/*--------------------Define --------------------------------------------*/
#define BIT_DBG_TRIGGER     BIT0
#define BIT_CD_TRIGGER      BIT5

#define CLKIs32K  1
#define CLKIs40M  0
/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/


extern void
ISR_RPWM2_8721D(
    void
);

extern void
UpdateISRUSB3_8721D(
    void
);

#if 0
extern void
UpdateISRSys_8721D(
    void
);
#endif

extern void
UpdateISRWlan_8721D(
    void
);

extern void
UpdateISRFT_8721D(
    void
);
/*
extern void
ISRSendSignal_8721D(
    u8  event
);

extern void
ISRSendSignalCommon(
    u8  event
);
*/

/*
extern void
ISRSendSignal_8721D1(
    u8  event
);
*/
extern void
ISR_TxClose_8721D(
    void
);

extern void
ISR_TXCCX_8721D(
    void
);

extern void
ISR_RxDone_8721D(
    void
);

extern void
ISR_H2CCMD_8721D(
    void
);

#if 1
extern void
ISR_HRCV_8721D(
    void
);
#endif

extern void
ISR_CTWEnd_8721D(
    void
);

extern void
ISR_BcnEarly_8721D(
    void
);



extern void
ISR_TXBCNERR_8721D(
    void
);

extern void
ISR_TXBCNOK_8721D(
    void
);

extern void
ISR_TxPKTIn_8721D(
    void
);

extern void
ISR_TxrptCntFull_8721D(
    void
);

extern void
ISR_TryDone_8721D(
    void
);

extern void
ISR_SoundDone_8721D(
    void
);

extern void
ISR_GTimer0_8721D(
    void
);

extern void
ISR_GTimer1_8721D(
    void
);

extern void
ISR_GTimer2_8721D(
    void
);

extern void
ISR_GTimer3_8721D(
    void
);

extern void
ISR_GTimer4_8721D(
    void
);

extern void 
ISR_TimerOut0_8721D(
    void
);
    
extern void
ISR_RPWM_8721D(
    void
);

extern void
ISR_EOSP_8721D(
    void
);

extern void
ISR_GTimer5_8721D(
    void
);

extern void
ISR_BCNERLY1_8721D(
    void
);

extern void
ISR_P2PRFOn_8721D(
    void
);

extern void
ISR_P2PRFOff_8721D(
    void
);

extern void
ISR_TRIGGER_PKT_8721D(
    void
);

extern void
ISR_TSF_BIT32_TOGGLE_8721D(
    void
);

extern void
ISR_RETRIVE_BUFFERED_8721D(
    void
);

extern void
ISR_CLR_PS_STATUS_8721D(
    void
);

extern void
ISR_BcnEarlyAGG_8721D(
    void
);

extern void
ISR_GPIO2En_8721D(
    void
);

extern void
ISR_GPIO3En_8721D(
    void
);

extern void
ISR_GPIO13En_8721D(
    void
);

extern void
ISR_PDN_8721D(
    void
);

extern void
ISR_WlanActOn_8721D(
    void
);

extern void
ISR_WlanActOff_8721D(
    void
);

extern void
ISR_TxNull0_8721D(
    void
);

extern void
ISR_TxNull1_8721D(
    void
);

void
ISR_GTimer6_8721D(
    void
);

void
ISR_SCO_OP_8721D(
    void
);

#endif  //#ifndef __ASSEMBLY__
#endif  //__RTL8721D_INT_H__

