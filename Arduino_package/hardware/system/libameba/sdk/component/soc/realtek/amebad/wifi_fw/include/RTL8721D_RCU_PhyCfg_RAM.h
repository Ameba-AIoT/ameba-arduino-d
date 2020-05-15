#ifndef __RTL8721D_RCU_PHYCFG_RAM_H__
#define __RTL8721D_RCU_PHYCFG_RAM_H__
/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct---------------------------------------*/

/*--------------------Export global variable-------------------------------*/
/*--------------------Function declaration---------------------------------*/
extern u32
PHY_QueryBBReg_8721D_RAM(
    IN  u16         RegAddr
    );      //access delay 20us

extern void
PHY_SetBBReg_8721D_RAM(
    IN  u16         RegAddr,
    IN  u32             Data
    );       //access delay 20us
    
extern void 
PHY_RFSerialWrite_8721D_RAM(
    IN  RF90_RADIO_PATH_E   eRFPath,
    IN  u8                  Offset,
    IN  u32                 Data
    );   

extern u32
PHY_RFSerialRead_8721D_RAM(
    IN  RF90_RADIO_PATH_E   eRFPath,
    IN  u8                  Offset
    ); 

extern void 
PHY_SetRFReg_8721D_RAM(
    IN  RF90_RADIO_PATH_E       eRFPath,
    IN  u8                      RegAddr,
    IN  u32                     Data
    );

extern u32
PHY_QueryRFReg_8721D_RAM(
    IN  RF90_RADIO_PATH_E   eRFPath,
    IN  u8                  RegAddr
    );   
#endif  //__RTL8721D_PHYCFG_RAM_H__

