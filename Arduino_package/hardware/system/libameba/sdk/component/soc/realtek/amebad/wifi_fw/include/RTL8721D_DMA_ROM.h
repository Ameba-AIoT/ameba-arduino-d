#ifndef __RTL8721D_DMA_H__
#define __RTL8721D_DMA_H__

#ifndef __ASSEMBLY__
/*--------------------Define -------------------------------------------*/
#define REG_DDMA_CH0SA      0x00
#define REG_DDMA_CH0DA      0x04
#define REG_DDMA_CH0CTRL    0x08
#define REG_DDMA_CH1SA      0x10
#define REG_DDMA_CH1DA      0x14
#define REG_DDMA_CH1CTRL    0x18
#define REG_DDMA_CH2SA      0x20
#define REG_DDMA_CH3SA      0x30
#define REG_DDMA_CH4SA      0x40
#define REG_DDMA_CH5SA      0x50

#define REG_DDMA_INT_MSK       0xe0
#define REG_DDMA_INT_STATUS    0xe4
#define REG_DDMA_CHSTATUS      0xe8

#define DDMACH0_MSK       BIT0
#define DDMACH1_MSK       BIT1
#define DDMACH0_ISR_CLEAR       BIT0
#define DDMACH1_ISR_CLEAR       BIT1
#define DDMACH0_ISR       BIT0//8 
#define DDMACH1_ISR       BIT1//9 

#define BIT_MASK_DDMA_DLEN       0x3FFFF
#define BIT_DDMA_OWN             BIT31
#define BIT_DDMA_CHKSUM_EN       BIT29
#define BIT_DDMA_DDMA_MODE       BIT26
#define BIT_DDMA_SPICBURST_MODE  BIT21
#define BIT_DDMA_FLASHBURST_MODE BIT20


#define BIT_MASK_DDMA_ADDR       0xFFFFFFFF

#define BIT_DDMA_CHKSUM_STS      BIT3

#define DDMACH_DEST_OFSET       0x4
#define DDMACH_CTRL_OFSET       0x8


//#define DDMA_FLASH_BASEADDR         0x10000000
//#define DDMA_SPIC_BASEADDR          0x11000000
//#define DDMA_RXFF_BASEADDR          0x60000
//#define DDMA_RXFF_BLOCK0_BASEADDR   0x60000
//#define DDMA_RXFF_BLOCK1_BASEADDR   0x60400
#define DDMA_BB_BASEADDR            0x40083000 //2k size
#define DDMA_RFC_BASEADDR           0x40083800 //1k size
#define DDMA_TXFF_BASEADDR          0x40090000
#define DDMA_REG_BASEADDR           0x40080000
#define DDMA_EXT_PTCL_BASEADDR      0x40081400 //256byte
#define DDMA_BACAM_BASEADDR         0x40081800 //512 byte
#define DDMA_SECCAM_BASEADDR        0x40081A00 //1k size
/*--------------------Define Enum---------------------------------------*/

/*--------------------Define MACRO--------------------------------------*/

/*--------------------Define Struct-------------------------------------*/

/*--------------------Export global variable----------------------------*/

/*--------------------Function declaration------------------------------*/

extern void
WriteMACRegDWord(
    u16 addr,
    u32 value
)MEMMDL_LARGE;

extern u32
ReadMACRegDWord(
    u16 addr
)MEMMDL_LARGE;

extern BOOLEAN  
InitDDMA(
    IN  u8  ch_num, 
    IN  u32 saddr, 
    IN  u32 daddr, 
    IN  u32 dma_size, 
    IN  BOOLEAN chksum_en,
    IN  BOOLEAN blockcpu,
    IN  BOOLEAN spic_burst_mode,
    IN  BOOLEAN flash_burst_mode
) MEMMDL_LARGE;

extern BOOLEAN
WaitDDMARDY(
    u8 ch_num
) MEMMDL_LARGE;

extern void
DDMA0HPDone(
    void
) MEMMDL_LARGE;

extern void
DDMA0LPDone(
    void
) MEMMDL_LARGE;

extern BOOLEAN
ChkDDMAChksum(
    u8 ch_num
)MEMMDL_LARGE;

extern BOOLEAN
CheckDDMADone(
    IN u8 ch_num,
    IN u8 chk_chksum
)MEMMDL_LARGE;

#endif  //#ifndef __ASSEMBLY__
#endif  //#ifndef   __RTL8192F_DMA_H__
