#ifndef __RTL8721D_RCU_UTIL_RAM_H__
#define __RTL8721D_RCU_UTIL_RAM_H__

/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
extern void    
StartGTimer(
    u16     timer_addr,
    u8      _mode,
    u32     timeout_1ms
);   //mode=0: counter, 1: timer

void RCU_GPIOInit(u32 GPIO_Pin, u8 Direction, u8 PuPd);
extern void OSSendSignal_RCU_32K(void);

extern void
OSSendSignal_RCU_LPS(void);

extern void
ISRSendSignal_RCU_LPS(void);

extern void
OSSendSignal_RCU_Common(u8 event);

extern void
ISRSendSignal_RCU_Common(u8 event);

#endif  //__RTL8721D_UTIL_RAM_H__

