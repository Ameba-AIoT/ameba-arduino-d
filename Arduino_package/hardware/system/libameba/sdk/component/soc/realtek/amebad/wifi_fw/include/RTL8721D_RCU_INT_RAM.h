#ifndef __RTL8721D_RCU_INT_RAM_H__
#define __RTL8721D_RCU_INT_RAM_H__

/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/
#define RCU_CLOSE_RF 0x00
#define RCU_JOIN_STATUS 0x01
#define RCU_LPS_DIS 0x02
#define RCU_LPS_EN 0x03
#define RCU_WOW_DIS 0x04
#define RCU_WOW_EN 0x05
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
ISRSendSignal_RCU_LPS(
    void
);

extern void
OSSendSignal_RCU_LPS(
    void
);

extern void
ISR_TxPKTIn_8721D(
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
ISR_RPWM_8721D(
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

#endif  //__RTL8721D_INT_RAM_H__

