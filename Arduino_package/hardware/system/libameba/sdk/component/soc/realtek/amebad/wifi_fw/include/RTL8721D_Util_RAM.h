#ifndef __RTL8721D_UTIL_RAM_H__
#define __RTL8721D_UTIL_RAM_H__

/*--------------------Define --------------------------------------------*/

/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

/*--------------------Export global variable-------------------------------*/

/*--------------------Function declaration---------------------------------*/
extern void
WritePONReg_8721D_ROM(
    IN  u8  addr, 
    IN  u8  value
) ;

extern u8
ReadPONReg_8721D_ROM(
    IN  u8  addr
) ;

extern BOOLEAN
IsRXFFEmptyCommon(
    void
);



#endif  //__RTL8721D_UTIL_RAM_H__

