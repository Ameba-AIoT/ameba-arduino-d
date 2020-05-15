#ifndef __RTL8721D_PHYCFG_RAM_H__
#define __RTL8721D_PHYCFG_RAM_H__

/*--------------------Define --------------------------------------------*/

//========= RF Config     =======================================
// BW and sideband setting
#define     HAL_PRIME_CHNL_OFFSET_DONT_CARE     0
#define     HAL_PRIME_CHNL_OFFSET_LOWER         1
#define     HAL_PRIME_CHNL_OFFSET_UPPER         2


#define    BIT_BW_20MHZ    BIT2


/*--------------------Define Enum---------------------------------------*/

typedef enum _WIRELESS_MODE {
    WIRELESS_MODE_UNKNOWN   = 0x00,
    WIRELESS_MODE_A         = 0x01,
    WIRELESS_MODE_B         = 0x02,
    WIRELESS_MODE_G         = 0x04,
    WIRELESS_MODE_AUTO      = 0x08,
    WIRELESS_MODE_N_24G     = 0x10,
    WIRELESS_MODE_N_5G      = 0x20,
    WIRELESS_MODE_AC_5G     = 0x40
} WIRELESS_MODE;

typedef enum _BAND_TYPE{
    BAND_ON_2_4G = 0,
    BAND_ON_5G,
    BAND_ON_BOTH,
    BANDMAX
}BAND_TYPE,*PBAND_TYPE;

//
// Represent Channel Width in HT Capabilities
//
typedef enum _HT_CHANNEL_WIDTH{
    HT_CHANNEL_WIDTH_20 = 0,
    HT_CHANNEL_WIDTH_40 = 1,
    HT_CHANNEL_WIDTH_80 = 2,
    HT_CHANNEL_WIDTH_MAX = 3,
}HT_CHANNEL_WIDTH, *PHT_CHANNEL_WIDTH;



typedef enum _VHT_DATA_SC{
    VHT_DATA_SC_DONOT_CARE          = 0,
    VHT_DATA_SC_20_UPPER_OF_80MHZ   = 1,
    VHT_DATA_SC_20_LOWER_OF_80MHZ   = 2,
    VHT_DATA_SC_20_UPPERST_OF_80MHZ = 3,
    VHT_DATA_SC_20_LOWEST_OF_80MHZ  = 4,
    VHT_DATA_SC_20_RECV1            = 5,
    VHT_DATA_SC_20_RECV2            = 6,
    VHT_DATA_SC_20_RECV3            = 7,
    VHT_DATA_SC_20_RECV4            = 8,
    VHT_DATA_SC_40_UPPER_OF_80MHZ   = 9,
    VHT_DATA_SC_40_LOWER_OF_80MHZ   = 10,
}VHT_DATA_SC, *PVHT_DATA_SC_E;



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
    
extern u8 
GetSecondaryChnl(
    IN  u8      CurrentChannelBW,
    IN  u8      nCur80MhzPrimeSC,
    IN  u8      nCur40MhzPrimeSC
);
 
void
SwitchWirelessBand(
    IN  u8     RequestBand,
    IN  u8     RFEType
);


void
CheckBand(
    IN  u8     RequestChannel,
    IN  u8     RFEType
);
 
#if !IS_CHIP_8192F(CONFIG_CHIP_SEL)
void 
SelectChannel(
    IN u8 channel,
    IN u8 CurrentChannelBW,    
    IN u8 channel_offset,
    IN u8 RFEType    
);
#endif      //#if !IS_CHIP_8192F(CONFIG_CHIP_SEL)
 
extern void 
PutRFRegSetting(
    IN u8 Path,
    IN u16 Addr, 
    IN u32 BitMask, 
    IN u32 UserValue
);
 

void 
PutBBRegSetting(
    IN u16 Addr, 
    IN u32 BitMask, 
    IN u32 UserValue
);
 

void 
SetBWMode_AC(
    IN  u8      CurrentChannelBW,
    IN  u8      nCur80MhzPrimeSC,
    IN  u8      nCur40MhzPrimeSC
);
 
   
void
SetBWRFReg(
    IN  u8    CurrentChannelBW
);
       

void 
SetBWBBReg(
    IN u8 CurrentBW,
    IN u8 SubChnlNum
);
 

void 
SetBWMACReg(
    IN u8 CurrentBW
);

u8
GetCurrentBand(
    void
);


// N mode function
#if !IS_CHIP_8192F(CONFIG_CHIP_SEL)
void 
SetBWMode_N(
    IN  u8  CurrentChannelBW,
    IN  u8  channel_offset
);
#endif      //#if !IS_CHIP_8192F(CONFIG_CHIP_SEL)

s8
CCKRSSI(
    IN  u1Byte  cck_agc_rpt
);

u8
QueryRxPwrPercentage(
    IN  s8  AntPower
);


void
PHY_InitBBRFRegDefRAM8721D(
    void
) ;

void 
PHY_SetRFRegRAM8721D(
    IN  RF90_RADIO_PATH_E       eRFPath,
    IN  u8                      RegAddr,
    IN  u32                     Data
)   ;

u32
PHY_QueryRFRegRAM8721D(
    IN  RF90_RADIO_PATH_E   eRFPath,
    IN  u8                  RegAddr
)   ;
#endif  //__RTL8721D_PHYCFG_RAM_H__

