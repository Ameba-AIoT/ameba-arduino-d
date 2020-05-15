#ifndef _RTL8195A_SSI_MAIN_H_
#define _RTL8195A_SSI_MAIN_H_

/* Function Prototype */
extern void *
_memset( void *s, int c, SIZE_T n );

typedef struct _SSI_PARAMETER_ {
    u8  Para0;
    u8  Para1;
    u8  Para2;
    u8  Para3;
    u8  Para4;
    u8  Para5;
    u8  Para6;
    u8  Para7;
    u8  Para8;
    u8  Para9;
}SSI_PARAMETER, *PSSI_PARAMETER;

#if 0
/**
 * SPI Example Pin Map
 */
// SPI0 (TestChip: S2) (MPChip: S1)
#define SPI0_MOSI  PC_2
#define SPI0_MISO  PC_3
#define SPI0_SCLK  PC_1
#define SPI0_CS    PC_0

// SPI1 (S1)
#define SPI1_MOSI  PB_6
#define SPI1_MISO  PB_7
#define SPI1_SCLK  PB_5
#define SPI1_CS    PB_4

// SPI2 (S2)
#define SPI2_MOSI  PD_2
#define SPI2_MISO  PD_3
#define SPI2_SCLK  PD_1
#define SPI2_CS    PD_0
#endif

#if 1  // MP verify (SPI1, SPI0) //Fail
// SPI1 (S1)
/*
#define SPI1_MOSI  PB_6
#define SPI1_MISO  PB_7
#define SPI1_SCLK  PB_5
#define SPI1_CS    PB_4
*/
// SPI1 (S2)

#define SPI1_MOSI  PA_1
#define SPI1_MISO  PA_0
#define SPI1_SCLK  PA_2
#define SPI1_CS    PA_4


// SPI0 (S1)
#define SPI0_MOSI  PC_2
#define SPI0_MISO  PC_3
#define SPI0_SCLK  PC_1
#define SPI0_CS    PC_0

#endif

#if 0  // MP verify (SPI2, SPI0)
// SPI2 (S2)
#define SPI2_MOSI  PD_2
#define SPI2_MISO  PD_3
#define SPI2_SCLK  PD_1
#define SPI2_CS    PD_0

// SPI0 (S1)  // EEPROM
#define SPI0_MOSI  PC_2
#define SPI0_MISO  PC_3
#define SPI0_SCLK  PC_1
#define SPI0_CS    PC_0
#endif


#if 0  // MP verify (SPI2, SPI0)
// SPI2 (S2)
#define SPI2_MOSI  PD_2
#define SPI2_MISO  PD_3
#define SPI2_SCLK  PD_1
#define SPI2_CS    PD_0

// SPI0 (S0)  // J-Tag
#define SPI0_MOSI  PE_2
#define SPI0_MISO  PE_3
#define SPI0_SCLK  PE_1
#define SPI0_CS    PE_0
#endif


#endif

