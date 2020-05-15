#ifndef __RTL8721D_UTIL_H__
#define __RTL8721D_UTIL_H__

/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/

#ifndef __ASSEMBLY__

//-------------------------------------------------------
//
//      I/O Function
//
//-------------------------------------------------------

extern u8 
PollingReg_8721D_ROM(
    u16 RegOffset,u8 PollingBit,u8 PollingResult,u16 PollingLimit, u16 *PollingCount
);

extern void
WritePONReg_8721D_ROM(
    IN  u8  addr, 
    IN  u8  value
) ;

extern u8
ReadPONReg_8721D_ROM(
    IN  u8  addr
) ;

//-------------------------------------------------------
//
//      Delay Function
//
//-------------------------------------------------------
extern void 
DelayUS_8721D_ROM(
    IN  u16 us
);

extern void 
DelayMS_8721D_ROM(
    IN  u16 ms
);


//-------------------------------------------------------
//
//      Timer Function
//
//-------------------------------------------------------
#if 0
extern void 
InitGTimer32us_CallPtr(
    IN  u16     timer_addr,
    IN  u8      _mode,
    IN  u8      Unit2s,
    IN  u8      Unit8ms,
    IN  u8      Unit32us
);  //mode=0: counter, 1: timer
#endif

extern void 
InitGTimer32us_8721D_ROM(
    IN  u16     timer_addr,
    IN  u8      _mode,
    IN  u8      Unit2s,
    IN  u8      Unit8ms,
    IN  u8      Unit32us
);  //mode=0: counter, 1: timer



//==== set the timer/counter mode, enable bit, and period =========
//
//
// 1 ms:
//  duration_H unit: 1 ms<<16 = 65536 ms
//  duration_M unit: 1 ms<<8 = 256 ms
//  duration_L unit:  1 ms
//
#if 0
extern void 
InitGTimer1ms_CallPtr(
    IN  u16     timer_addr,
    IN  u8      _mode,
    IN  u8      Unit65s, 
    IN  u8      Unit256ms, 
    IN  u8      Unit1ms
);  //mode=0: counter, 1: timer
#endif

extern void 
InitGTimer1ms_8721D_ROM(
    IN  u16     timer_addr,
    IN  u8      _mode,
    IN  u8      Unit65s, 
    IN  u8      Unit256ms, 
    IN  u8      Unit1ms
);  //mode=0: counter, 1: timer

#if 0
extern void    
InitGTimer1ms_8721D_ROM(
    IN  u16     timer_addr,
    IN  u8      _mode,
    IN  u8      Unit65s,           
    IN  u8      Unit256ms, 
    IN  u8      Unit1ms
);
#endif 

//-------------------------------------------------------
//
//      Interrupt Util Function
//
//-------------------------------------------------------
extern void
setwlanimr_8721D_ROM(
    IN  u8  index, 
    IN  u8  bitmask
);


extern void 
clrwlanimr_8721D_ROM(
    IN  u8  index, 
    IN  u8  bitmask
);

extern void
setwlanimr2_8721D_ROM(
    IN  u8  index,
    IN  u8 bitmask
);


extern void 
clrwlanimr2_8721D_ROM(
    IN  u8  index,
    IN  u8  bitmask
);

extern void
setftimr_8721D_ROM(
    IN  u8  index, 
    IN  u8  bitmask
);


extern void 
clrftimr_8721D_ROM(
    IN  u8  index, 
    IN  u8  bitmask
);

//-------------------------------------------------------
//
//      MAC Util Function
//
//-------------------------------------------------------
#endif  //#ifndef __ASSEMBLY__

#endif  //__RTL8721D_UTIL_H__

