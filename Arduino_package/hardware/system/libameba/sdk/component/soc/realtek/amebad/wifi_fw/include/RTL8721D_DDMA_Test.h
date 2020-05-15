#ifndef __RTL8192F_DDMA_TEST_H__
#define __RTL8192F_DDMA_TEST_H__


#define DDMAVER_PATTERN1_SIZE   32
#define DDMAVER_PATTERN2_SIZE   32// 256 //1024 //4096
        
#define DDMAVER_DA_TXFF_b0      (0x7F00|DDMA_TXFF_BASEADDR) //DDMA tx buffer address need set to high 8k
//#define DDMAVER_DA_TXFF_b1      (0x4080|DDMA_TXFF_BASEADDR)
//#define DDMAVER_DA_TXFF_b2      (0x7fe0|DDMA_TXFF_BASEADDR)

#define DDMAVER_DA_BBREG        (0x0000|DDMA_BB_BASEADDR)
#define DDMAVER_DA_RFCREG       (0x0018|DDMA_RFC_BASEADDR)
#define DDMAVER_DA_BACAM        (0x0080|DDMA_BACAM_BASEADDR)
#define DDMAVER_DA_SECCAM       (0x0000|DDMA_SECCAM_BASEADDR)
#define DDMAVER_DA_EXTPTCL      (0x0080|DDMA_EXT_PTCL_BASEADDR)

//#define DDMAVER_DA_RXFF_b0      (0x0000|DDMA_RXFF_BASEADDR)
//#define DDMAVER_DA_RXFF_b1      (0x2008|DDMA_RXFF_BASEADDR)
//#define DDMAVER_DA_RXFF_b2      (0x3fe0|DDMA_RXFF_BASEADDR)


#define DDMAVER_DA_MACREG       (0x0484|DDMA_REG_BASEADDR)//offset 0x0484~0x04A3
#define DDMAVER_DA_PONREG       (0x88|DDMA_REG_BASEADDR)//offset 0x88~0x8B
//#define DDMAVER_DA_FLASH_b0     (0x20000|DDMA_FLASH_BASEADDR)
//#define DDMAVER_DA_FLASH_b1     (0xffffe0|DDMA_FLASH_BASEADDR)

#define DDMAVER_SA               0x7000 //DDMA tx buffer address need set to high 8k
#define DDMAVER_DA               0x6000

#define VERA_DDMA_REF2SA_FAIL      BIT0
#define VERA_DDMA_SA2DA_FAIL       BIT1
#define VERA_DDMA_DA2OUT_FAIL      BIT2
#define VERA_DDMA_REF_OUT_MIS      BIT3

#define REG_VERA_DDMA_FLAG         0xff

extern void 
DDMAPathTest(
    void
)MEMMDL_LARGE;

extern u8
VeraDDMAPathTest(
    u8 ch,
    u32 sa,
    u32 da,
    u32 len
)MEMMDL_LARGE;

#endif      //#ifndef __RTL8192F_DDMA_TEST_H__