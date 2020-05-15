#ifndef __RTL8721D_PHYCFG_H__
#define __RTL8721D_PHYCFG_H__

#ifndef __ASSEMBLY__


/*--------------------------Define -------------------------------------------*/
#define WAIT_BBRW_STABLE_BREAK_CNT      (5000)
#define WAIT_BBRW_STABLE_CNT            (100)
#define WAIT_BBRW_STABLE_ONCE_TIME      (20)
/*------------------------------Define Enum-----------------------------------*/
typedef enum _RF90_RADIO_PATH{
    RF90_PATH_A = 0,            //Radio Path A
    RF90_PATH_B = 1,            //Radio Path B
    RF90_PATH_C = 2,            //Radio Path C
    RF90_PATH_D = 3,            //Radio Path D
    //RF90_PATH_MAX             //Max RF number 90 support 
}RF90_RADIO_PATH_E, *PRF90_RADIO_PATH_E;


#define RF90_PATH_MAX           2


/* BB/RF related */
typedef enum _RF_TYPE_8190P{
    RF_TYPE_MIN   = 0,        // 0
    RF_8225       = 1,        // 1 11b/g RF for verification only
    RF_8256       = 2,        // 2 11b/g/n 
    RF_8258       = 3,        // 3 11a/b/g/n RF
    RF_6052       = 4,        // 4 11b/g/n RF
    //RF_6052=5,              // 4 11b/g/n RF
    // TODO: We sholud remove this psudo PHY RF after we get new RF.
    RF_PSEUDO_11N = 5,    // 5, It is a temporality RF. 
}RF_TYPE_8190P_E,*PRF_TYPE_8190P_E;


/*--------------------------Define MACRO--------------------------------------*/

/*------------------------------Define Struct---------------------------------*/

typedef struct _BB_REGISTER_DEFINITION{
#if 0
    u4Byte rfintfs;         // set software control: 
                            //      0x870~0x877[8 bytes]
                            
    u4Byte rfintfi;         // readback data: 
                            //      0x8e0~0x8e7[8 bytes]
                            
    u4Byte rfintfo;         // output data: 
                            //      0x860~0x86f [16 bytes]
                            
    u4Byte rfintfe;         // output enable: 
                            //      0x860~0x86f [16 bytes]
#endif
    u16 rf3wireOffset;      // LSSI data:
                            //      0x840~0x84f [16 bytes]
#if 0                           
    u4Byte rfLSSI_Select;   // BB Band Select: 
                            //      0x878~0x87f [8 bytes]
                            
    u4Byte rfTxGainStage;   // Tx gain stage: 
                            //      0x80c~0x80f [4 bytes]
                            
    u4Byte rfHSSIPara1;     // wire parameter control1 : 
                            //      0x820~0x823,0x828~0x82b, 0x830~0x833, 0x838~0x83b [16 bytes]
#endif

    u16 rfHSSIPara2;        // wire parameter control2 : 
                            //      0x824~0x827,0x82c~0x82f, 0x834~0x837, 0x83c~0x83f [16 bytes]
#if 0                               
    u4Byte rfSwitchControl; //Tx Rx antenna control : 
                            //      0x858~0x85f [16 bytes]
                                
    u4Byte rfAGCControl1;   //AGC parameter control1 : 
                            //      0xc50~0xc53,0xc58~0xc5b, 0xc60~0xc63, 0xc68~0xc6b [16 bytes] 
                                
    u4Byte rfAGCControl2;   //AGC parameter control2 : 
                            //      0xc54~0xc57,0xc5c~0xc5f, 0xc64~0xc67, 0xc6c~0xc6f [16 bytes] 
                            
    u4Byte rfRxIQImbalance; //OFDM Rx IQ imbalance matrix : 
                            //      0xc14~0xc17,0xc1c~0xc1f, 0xc24~0xc27, 0xc2c~0xc2f [16 bytes]
                            
    u4Byte rfRxAFE;         //Rx IQ DC ofset and Rx digital filter, Rx DC notch filter : 
                            //      0xc10~0xc13,0xc18~0xc1b, 0xc20~0xc23, 0xc28~0xc2b [16 bytes]
                            
    u4Byte rfTxIQImbalance; //OFDM Tx IQ imbalance matrix
                            //      0xc80~0xc83,0xc88~0xc8b, 0xc90~0xc93, 0xc98~0xc9b [16 bytes]
                            
    u4Byte rfTxAFE;         //Tx IQ DC Offset and Tx DFIR type
                            //      0xc84~0xc87,0xc8c~0xc8f, 0xc94~0xc97, 0xc9c~0xc9f [16 bytes]
#endif                              
    u16 rfLSSIReadBack;     //LSSI RF readback data SI mode
                            //      0x8a0~0x8af [16 bytes]


    u16 rfLSSIReadBackPi;   //LSSI RF readback data PI mode 0x8b8-8bc for Path A and B

}BB_REGISTER_DEFINITION_T, *PBB_REGISTER_DEFINITION_T;

/*------------------------Export global variable------------------------------*/

extern MEMTYPE_XDATA   BB_REGISTER_DEFINITION_T    PHYRegDef[RF90_PATH_MAX];   //Radio A/B/C/D



/*------------------------------Function declaration--------------------------*/
#if (!CONFIG_RF_JAGUAR)
extern void
PHY_InitBBRFRegDef_8721D(
    void
);
#else
void
PHY_InitBBRFRegDefJaguar_8721D(
    void
);
#endif

BOOLEAN
CheckBBRWIsStable_8721D(
    void
);

extern u32
PHY_QueryBBReg_8721D(
    IN  u16         RegAddr
    )   ;   //access delay 20us

extern void
PHY_SetBBReg_8721D(
    IN  u16         RegAddr,
    IN  u32         Data
    )   ;    //access delay 20us

extern u32
PHY_RFSerialReadJaguar_8721D(
    IN  RF90_RADIO_PATH_E   eRFPath,
    IN  u8                  Offset
    )   ;

extern void 
PHY_RFSerialWriteJaguar_8721D(
    IN  RF90_RADIO_PATH_E   eRFPath,
    IN  u8                  Offset,
    IN  u32                 Data
    )   ;

extern u32
PHY_QueryRFReg_8721D(
    IN  RF90_RADIO_PATH_E   eRFPath,
    IN  u8                  RegAddr
    )   ;

extern void
PHY_SetRFReg_8721D(
    IN  RF90_RADIO_PATH_E       eRFPath,
    IN  u8                      RegAddr,
    IN  u32                     Data
    )   ;

#endif  //#ifndef __ASSEMBLY__

#endif  //__RTL8721D_PHYCFG_H__

