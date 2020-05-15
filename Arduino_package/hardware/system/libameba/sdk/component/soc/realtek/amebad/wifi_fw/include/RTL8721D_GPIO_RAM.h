#ifndef __HALCOM_GPIO_RAM_H__
#define __HALCOM_GPIO_RAM_H__

#if (!IS_CHIP_8188F(CONFIG_CHIP_SEL))
/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/
#define GPIOWAKE_TIMER           TIMER_TC2_CTRL

/*--------------------Define Struct---------------------------------------*/
typedef struct _GPIO_CTRL_ {  
    u8 GPIOIdx:4;
    u8 C2HEn:1;
    u8 Pulse:1;
    u8 HighActive:1;
    u8 En:1;
    u8 Unitms:1;
    u8 HostStateEn:1;
    u8 RSVD:6;
    u8 Duration;
    u8 C2HDuration;
    u8 GPIOPulseCount; 
} GPIO_Ctrl, *PGPIO_Ctrl;

#endif //#if (!IS_CHIP_8188F(CONFIG_CHIP_SEL))

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
extern u8 
GetInputGPIO(
    IN u8 index
) ;

extern void 
SetOutPutGPIO(
    IN u8 index,
    IN u8 OutPutValue
) ;
extern void
InitGPIOCtrl(
    void
) ;

extern void
GPIOCtrl(
    IN u8 source
) ;

extern void 
GPIOWakeUpTimeOutHDL(
    void
) ;

extern void
GPIOWaveform_Haier(
    void
) ;

extern void
GPIOWaveformRealtek(
    IN u16 temp1,
    IN u8 Duration_ms,
    IN u8 PulseCount,
    IN u8 Enable,
    IN u8 InfiniteEn
) ;

#endif  //__HALCOM_GPIO_RAM_H__
