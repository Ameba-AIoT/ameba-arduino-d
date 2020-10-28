#ifndef __RTL8721D_INT_RAM_H__
#define __RTL8721D_INT_RAM_H__

/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
extern void    
WLAN_INT_HDL_RAM_8721D(
        void
); 

extern void    
FT_INT_HDL_RAM_8721D(
    void
);

extern void
ISRSendSignalCommon_8721D(
    u8  event
);


extern void
ISR_TBTT_8721D(
    void
);

extern void
ISR_BCNRx_8721D(
    void
);

extern  void
ISR_RxBMC_MD1_8721D(
    void
);
 
extern void
ISR_RxBMC_MD0_8721D(
    void
);

extern void
ISR_RxUnicast_MD1_8721D(
    void
);

extern void
ISR_RxUnicast_MD0_8721D(
    void
);

extern void
ISR_TxPKTIn_8721D(
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
ISR_TRIGGER_PKT_8721D(
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
ISR_GTimer6_8721D(
    void
);

#if 0
extern void
ISR_SCO_OP_8721D(
    void
);
#endif

extern void
ISR_TxNull0_8721D(
    void
);

extern void
ISR_TxNull1_8721D(
    void
);



extern void
ISR_SyncBcnRxOK_8721D(
    void
);

extern void
ISR_DWEarly_8721D(
    void
);

extern void
ISR_DWEnd_8721D(
    void
);

extern void
ISR_EQEarly_8721D(
    void
);

extern void
ISR_EQTxOK_8721D(
    void
);

extern void
ISR_TBTT1_8721D(
    void
);    

extern void
ISR_TSF_BIT32_TOGGLECommon(
    void
);


#endif  //__RTL8721D_INT_RAM_H__

