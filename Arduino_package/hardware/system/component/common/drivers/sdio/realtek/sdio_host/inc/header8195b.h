#ifndef _SDIO_HEADER_8195B 
#define _SDIO_HEADER_8195B 

#if defined(CONFIG_PLATFORM_8195A)

typedef void (*sdioh_card_detect)(void *para);
typedef void (*sdioh_task_yield)(void);
typedef uint8_t sdioh_pin_sel_t;
typedef uint32_t   hal_status_t;

#ifdef __cplusplus
  #define   __I     volatile             /*!< Defines 'read only' permissions */
#else
  #define   __I     volatile const       /*!< Defines 'read only' permissions */
#endif
#define     __O     volatile             /*!< Defines 'write only' permissions */
#define     __IO    volatile             /*!< Defines 'read / write' permissions */

#ifndef __IM                                    /*!< Fallback for older CMSIS versions                                         */
  #define __IM   __I
#endif
#ifndef __OM                                    /*!< Fallback for older CMSIS versions                                         */
  #define __OM   __O
#endif
#ifndef __IOM                                   /*!< Fallback for older CMSIS versions                                         */
  #define __IOM  __IO
#endif

#define __IM   __I

#ifndef SDIO_HOST_Type
typedef struct {                                /*!< (@ 0x40058000) SDIO_HOST Structure                                        */
  __IM  uint32_t  RESERVED[256];
  
  union {
    __IOM uint32_t sram_ctl;                    /*!< (@ 0x00000400) SRAM Control Register                                      */
    
    struct {
      __IOM uint32_t mem_region : 4;            /*!< [3..0] Set this bit to enable buffer SRAM direct access mode.             */
      __IOM uint32_t access_en  : 1;            /*!< [4..4] Set this bit to enable buffer SRAM direct access mode.             */
      __IOM uint32_t map_sel    : 1;            /*!< [5..5] 0: R-Bus = x01020304 => to flash 01, 02, 03, 04; 1: R-Bus
                                                     = x01020304 => to flash 04, 03, 02, 01                                    */
      __IOM uint32_t lx_burst_size : 2;         /*!< [7..6] Set the Lexra burst size. 0: 64 Bytes; 1: 128 Bytes;
                                                     2: 256 Bytes; 3: 128 Bytes                                                */
    } sram_ctl_b;
  } ;
  
  union {
    __IOM uint32_t dma_ctl1;                    /*!< (@ 0x00000404) DMA Control Register 1                                     */
    
    struct {
      __IOM uint32_t dram_sa    : 28;           /*!< [27..0] Dram start address for DMA transfer. This information
                                                     will be map to addcmd. (8 Bytes Unit, 1 means 8B)                         */
    } dma_ctl1_b;
  } ;
  
  union {
    __IOM uint32_t dma_ctl2;                    /*!< (@ 0x00000408) DMA Control Register 2                                     */
    
    struct {
      __IOM uint32_t dma_len    : 16;           /*!< [15..0] Transfer length for DMA transfer between DMA buffer
                                                     and DDR. (512B Unit, 1 means 512B)                                        */
    } dma_ctl2_b;
  } ;
  
  union {
    __IOM uint32_t dma_ctl3;                    /*!< (@ 0x0000040C) DMA Control Register 3                                     */
    
    struct {
      __IOM uint32_t dma_xfer   : 1;            /*!< [0..0] Set this bit to transfer data between DRAM and DMA buffer.
                                                     Direction must be set at next bit. The transfer length
                                                     is reference to DMA_CTL2[15:0]. This bit will be auto clear
                                                     when transfer done.                                                       */
      __IOM uint32_t ddr_wr     : 1;            /*!< [1..1] 1: Move data from DMA buffer to DDR; 0: Move data from
                                                     DDR to DMA Buffer.                                                        */
      __IM  uint32_t            : 2;
      __IOM uint32_t rsp17_sel  : 1;            /*!< [4..4] For response is R2 case, reponse will transfer by dma
                                                     and not store in register. 1: dma count to 16byte and send
                                                     sb1_req; 0: normal case.                                                  */
      __IOM uint32_t dat64_sel  : 1;            /*!< [5..5] For CMD6 case, read data length is 64byte (less than
                                                     256 byte) 1: dma count to 64byte and send sb1_req; 0: normal
                                                     case.                                                                     */
    } dma_ctl3_b;
  } ;
  
  union {
    __IOM uint32_t sys_low_pwr;                 /*!< (@ 0x00000410) Low Power Control Register                                 */
    
    struct {
      __IOM uint32_t dma_sram_rdy_num : 4;      /*!< [3..0] DMA SRAM ready cycle (leave sleep mode) (N+1) * clk_sys
                                                     period                                                                    */
      __IOM uint32_t dma_sram_lp_ena : 1;       /*!< [4..4] DMA SRAM low power enable                                          */
      __IM  uint32_t            : 1;
      __IOM uint32_t sys_clk_gate_ena : 1;      /*!< [6..6] DMA clk_sys gating enable                                          */
    } sys_low_pwr_b;
  } ;
  __IM  uint32_t  RESERVED1[4];
  
  union {
    __IOM uint32_t sd_isr;                      /*!< (@ 0x00000424) SD Interrupt Status Register                               */
    
    struct {
      __IOM uint32_t write_data : 1;            /*!< [0..0] 1 to set, 0 to clear bit with 1.                                   */
      __IOM uint32_t int1       : 1;            /*!< [1..1] SD Int1. Card End.                                                 */
      __IOM uint32_t int2       : 1;            /*!< [2..2] SD Int2. Card Error.                                               */
      __IM  uint32_t            : 1;
      __IOM uint32_t int4       : 1;            /*!< [4..4] SD Int4. DMA transfer done INT.                                    */
    } sd_isr_b;
  } ;
  
  union {
    __IOM uint32_t sd_isren;                    /*!< (@ 0x00000428) SD Interrupt Enable Register                               */
    
    struct {
      __IOM uint32_t write_data : 1;            /*!< [0..0] 1 to set, 0 to clear bit with 1.                                   */
      __IOM uint32_t int1en     : 1;            /*!< [1..1] SD Int1 Enable Card End INT Enable.                                */
      __IOM uint32_t int2en     : 1;            /*!< [2..2] SD Int2 Enable Card Error INT Enable.                              */
      __IM  uint32_t            : 1;
      __IOM uint32_t int4en     : 1;            /*!< [4..4] SD Int4 Enable dma_clr INT Enable.                                 */
    } sd_isren_b;
  } ;
  __IM  uint32_t  RESERVED2[18];
  
  union {
    __IOM uint32_t pad_ctl;                     /*!< (@ 0x00000474) Pad Control Register                                       */
    
    struct {
      __IOM uint32_t tune3318   : 1;            /*!< [0..0] Pad select 3.3v or 1.8v. 1: 3.3v; 0: 1.8v                          */
    } pad_ctl_b;
  } ;
  
  union {
    __IOM uint32_t ckgen_ctl;                   /*!< (@ 0x00000478) Clock Generation Control Register                          */
    
    struct {
      __IOM uint32_t clk_div    : 3;            /*!< [2..0] 000: div1; 001: div2; 010: div4; 011: div8                         */
      __IM  uint32_t            : 1;
      __IOM uint32_t crc_clk_src : 2;           /*!< [5..4] 00: ssc_clk; 01: ssc_clk_vp0; 10: ssc_clk_vp1                      */
      __IM  uint32_t            : 2;
      __IOM uint32_t sd30_push_clk_src : 2;     /*!< [9..8] 00: ssc_clk; 01: ssc_clk_vp0; 10: ssc_clk_vp1                      */
      __IM  uint32_t            : 2;
      __IOM uint32_t sd30_sample_clk_src : 2;   /*!< [13..12] 00: ssc_clk; 01: ssc_clk_vp0; 10: ssc_clk_vp1                    */
      __IM  uint32_t            : 2;
      __IOM uint32_t crc_clk_change : 1;        /*!< [16..16] 0: from crc_clk_src; 1: clk4M                                    */
      __IOM uint32_t sd30_push_change : 1;      /*!< [17..17] 0: from sd30_push_clk_src; 1: clk4M                              */
      __IOM uint32_t sd30_sample_change : 1;    /*!< [18..18] 0: from sd30_sample_clk_src; 1: clk4M                            */
    } ckgen_ctl_b;
  } ;
  __IM  uint32_t  RESERVED3[33];
  __IM  uint16_t  RESERVED4;
  
  union {
    __IOM uint8_t card_drive_sel;               /*!< (@ 0x00000502) Card Driving Selection Register                            */
    
    struct {
      __IOM uint8_t cf_drive    : 2;            /*!< [1..0] CF Interface Drive. 00: 4mA; 01: 8mA; 10: 12mA; 11: 12mA           */
      __IOM uint8_t xd_drive    : 2;            /*!< [3..2] XD Interface Drive. 00: 4mA; 01: 8mA; 10: 12mA; 11: 12mA           */
      __IOM uint8_t sd_drive    : 2;            /*!< [5..4] SD DAT7~4 Interface Drive. 00: 4mA; 01: 8mA; 10: 12mA;
                                                     11: 12mA                                                                  */
      __IOM uint8_t ms_drive    : 2;            /*!< [7..6] MS Interface Drive. 00: 4mA; 01: 8mA; 10: 12mA; 11: 12mA           */
    } card_drive_sel_b;
  } ;
  
  union {
    __IOM uint8_t card_stop;                    /*!< (@ 0x00000503) Stop Transfer Register                                     */
    
    struct {
      __IOM uint8_t cf_module   : 1;            /*!< [0..0] Target module is CF/MD card module. If set this bit the
                                                     transfer will be stop and the state machine of transfer
                                                     will return idle state.                                                   */
      __IOM uint8_t sm_module   : 1;            /*!< [1..1] Target module is SM/XD card module. If set this bit the
                                                     transfer will be stop and the state machine of transfer
                                                     will return idle state.                                                   */
      __IOM uint8_t sd_module   : 1;            /*!< [2..2] Target module is SD/MMC card module (SD). If set this
                                                     bit the transfer will be stop and the state machine of
                                                     transfer will return idle state.                                          */
      __IOM uint8_t ms_module   : 1;            /*!< [3..3] Target module is MS/MS-Pro/HG card module (MS). If set
                                                     this bit the transfer will be stop and the state machine
                                                     of transfer will return idle state.                                       */
      __IOM uint8_t sd2_module  : 1;            /*!< [4..4] Target module is SD/MMC card module (SD2). If set this
                                                     bit the transfer will be stop and the state machine of
                                                     transfer will return idle state.                                          */
      __IOM uint8_t ms2_module  : 1;            /*!< [5..5] Target module is MS/MS-Pro/HG card module (MS2). If set
                                                     this bit the transfer will be stop and the state machine
                                                     of transfer will return idle state.                                       */
    } card_stop_b;
  } ;
  __IM  uint32_t  RESERVED5[2];
  __IM  uint16_t  RESERVED6;
  
  union {
    __IOM uint8_t card_select;                  /*!< (@ 0x0000050E) Card Type Select Register                                  */
    
    struct {
      __IOM uint8_t card_sel    : 3;            /*!< [2..0] Card Select. Specify the current active card module.
                                                     000: CF module 001: XD module 010: SD module 011: MS module
                                                     100: I2C module 101: Reserved 110: SD2 module 111: MS2
                                                     module                                                                    */
    } card_select_b;
  } ;
  __IOM uint8_t   dummy1;                       /*!< (@ 0x0000050F) Dummy Register 1                                           */
  __IM  uint32_t  RESERVED7[3];
  __IM  uint16_t  RESERVED8;
  __IM  uint8_t   RESERVED9;
  
  union {
    __IOM uint8_t card_exist;                   /*!< (@ 0x0000051F) Card Detection Register                                    */
    
    struct {
      __IOM uint8_t cf_exist    : 1;            /*!< [0..0] CF Existence. If this bit is set it means CF/MD card
                                                     is in its socket.                                                         */
      __IOM uint8_t sm_exist    : 1;            /*!< [1..1] SM Existence. If this bit is set it means SM card is
                                                     in its socket.                                                            */
      __IOM uint8_t sd_exist    : 1;            /*!< [2..2] SD Existence. If this bit is set it means SD/MMC card
                                                     is in its socket.                                                         */
      __IOM uint8_t ms_exist    : 1;            /*!< [3..3] MS Existence. If this bit is set it means MS/MS-pro card
                                                     is in its socket.                                                         */
      __IOM uint8_t xd_exist    : 1;            /*!< [4..4] XD Existence. If this bit is set it means XD card is
                                                     in its socket.                                                            */
      __IOM uint8_t sd_wp       : 1;            /*!< [5..5] SD Write Protect. If this bit is set it means SD card
                                                     is mechanical write protected.                                            */
      __IOM uint8_t xd_wp       : 1;            /*!< [6..6] XD_WP Input. If the XD_WP pin is configure as input,
                                                     this bit reflect the level of XD_WP signal directly.                      */
    } card_exist_b;
  } ;
  
  union {
    __IOM uint8_t card_int_en;                  /*!< (@ 0x00000520) Card Interrupt Enable Register                             */
    
    struct {
      __IOM uint8_t cf_int_en   : 1;            /*!< [0..0] CF Card Interrupt Enable. If this bit is set it enables
                                                     the interrupt of CF card                                                  */
      __IOM uint8_t sm_int_en   : 1;            /*!< [1..1] SM Card Interrupt Enable. If this bit is set it enables
                                                     the interrupt of SM card                                                  */
      __IOM uint8_t sd_int_en   : 1;            /*!< [2..2] SD/MMC Interrupt Enable. If this bit is set it enables
                                                     the interrupt of SD/MMC card                                              */
      __IOM uint8_t ms_int_en   : 1;            /*!< [3..3] MS/MS-pro/HG card Interrupt Enable. If this bit is set
                                                     it enables the interrupt of MS/MS-pro card                                */
      __IOM uint8_t xd_int_en   : 1;            /*!< [4..4] XD Card Interrupt Enable. If this bit is set it enables
                                                     the interrupt of XD card                                                  */
      __IOM uint8_t sd2_int_en  : 1;            /*!< [5..5] SD/MMC (SD2) Interrupt Enable. If this bit is set it
                                                     enables the interrupt of SD/MMC card                                      */
      __IOM uint8_t ms2_int_en  : 1;            /*!< [6..6] MS/MS-pro/HG (MS2) card Interrupt Enable. If this bit
                                                     is set it enables the interrupt of MS/MS-pro card                         */
      __IOM uint8_t gpio_int_en : 1;            /*!< [7..7] GPIO[0] Interrupt Enable. If this bit is set it enables
                                                     the interrupt of GPIO0                                                    */
    } card_int_en_b;
  } ;
  
  union {
    __IOM uint8_t card_int_pend;                /*!< (@ 0x00000521) Card Interrupt Status Register                             */
    
    struct {
      __IOM uint8_t cf_int_pend : 1;            /*!< [0..0] CF Card Interrupt Pending. If this bit is set it means
                                                     the interrupt of CF card is pending and write '1' to this
                                                     bit to clear the interrupt flag to '0'                                    */
      __IOM uint8_t sm_int_pend : 1;            /*!< [1..1] SM Card Interrupt Pending. If this bit is set it means
                                                     the interrupt of SM card is pending and write '1' to this
                                                     bit to clear the interrupt flag to '0'                                    */
      __IOM uint8_t sd_int_pend : 1;            /*!< [2..2] SD/MMC Interrupt Pending. If this bit is set it means
                                                     the interrupt of SD/MMC card is pending and write '1' to
                                                     this bit to clear the interrupt flag to '0'                               */
      __IOM uint8_t ms_int_pend : 1;            /*!< [3..3] MS/MS-pro/HG card Interrupt Pending. If this bit is set
                                                     it means the interrupt of MS/MS-pro card is pending and
                                                     write '1' to this bit to clear the interrupt flag to '0'                  */
      __IOM uint8_t xd_int_pend : 1;            /*!< [4..4] XD Card Interrupt Pending. If this bit is set it means
                                                     the interrupt of XD card is pending and write '1' to this
                                                     bit to clear the interrupt flag to '0'                                    */
      __IOM uint8_t sd2_int_pend : 1;           /*!< [5..5] SD/MMC (SD2) Interrupt Pending. If this bit is set it
                                                     means the interrupt of SD/MMC card is pending and write
                                                     '1' to this bit to clear the interrupt flag to '0'                        */
      __IOM uint8_t ms2_int_pend : 1;           /*!< [6..6] MS/MS-pro/HG (MS2) card Interrupt Pending. If this bit
                                                     is set it means the interrupt of MS/MS-pro card is pending
                                                     and write '1' to this bit to clear the interrupt flag to
                                                     '0'                                                                       */
      __IOM uint8_t gpio_int_pend : 1;          /*!< [7..7] GPIO[0] Interrupt Pending. If this bit is set it means
                                                     the interrupt of GPIO0 is pending and write '1' to this
                                                     bit to clear the interrupt flag to '0'                                    */
    } card_int_pend_b;
  } ;
  __IM  uint16_t  RESERVED10;
  __IM  uint32_t  RESERVED11;
  __IM  uint8_t   RESERVED12;
  
  union {
    __IOM uint8_t card_clk_en_ctl;              /*!< (@ 0x00000529) Card Clock Enable Control Register                         */
    
    struct {
      __IOM uint8_t cf_clk_en   : 1;            /*!< [0..0] CF Card Module Clock Enable Control. Disable clock can
                                                     save power. 0: Disable; 1: Enable                                         */
      __IOM uint8_t xd_clk_en   : 1;            /*!< [1..1] XD Card Module Clock Enable Control. Disable clock can
                                                     save power. 0: Disable; 1: Enable                                         */
      __IOM uint8_t sd_clk_en   : 1;            /*!< [2..2] SD Card Module Clock Enable Control. Disable clock can
                                                     save power. 0: Disable; 1: Enable                                         */
      __IOM uint8_t ms_clk_en   : 1;            /*!< [3..3] MS Card Module Clock Enable Control. Disable clock can
                                                     save power. 0: Disable; 1: Enable                                         */
      __IOM uint8_t sd2_clk_en  : 1;            /*!< [4..4] SD2 Card Module Clock Enable Control. Disable clock can
                                                     save power. 0: Disable; 1: Enable                                         */
      __IOM uint8_t ms2_clk_en  : 1;            /*!< [5..5] MS2 Card Module Clock Enable Control. Disable clock can
                                                     save power. 0: Disable; 1: Enable                                         */
      __IOM uint8_t sw_clk_mode : 1;            /*!< [6..6] Switch Clock Mode. Clk_en use FW configuration of HW
                                                     setting. 0: Clk_en use FW configure 1: Clk_en use HW setting              */
      __IOM uint8_t auto_cbw    : 1;            /*!< [7..7] Auto CBW Mode. Used for card select option. 0: Card select
                                                     is configured by FW. 1: Card select is configured by HW.                  */
    } card_clk_en_ctl_b;
  } ;
  __IM  uint16_t  RESERVED13;
  __IM  uint32_t  RESERVED14;
  
  union {
    __IOM uint8_t clk_pad_drive;                /*!< (@ 0x00000530) Clock Pad Driving Register                                 */
    
    struct {
      __IOM uint8_t nmos_drive  : 3;            /*!< [2..0] SD_CLK PAD NMOS drive select 3V3: 1V8: 000: x 10.1 x
                                                     1.93 001: x 13.5 x 3.85 010,100: x 16.9 x 5.78 011,101:
                                                     x 20.2 x 7.73 110: x 23.5 x 9.65 111: x 26.8 x 11.5                       */
      __IOM uint8_t pmos_drive  : 3;            /*!< [5..3] SD_CLK PAD PMOS drive select 3V3: 1V8: 000: x 7,07 x
                                                     2.27 001: x 14.2 x 4.61 010,100: x 21 x 6.68 011,101: x
                                                     27.9 x 9.14 110: x 34.9 x 11.5 111: x 42.4 x 13.8                         */
    } clk_pad_drive_b;
  } ;
  
  union {
    __IOM uint8_t cmd_pad_drive;                /*!< (@ 0x00000531) Command Pad Driving Register                               */
    
    struct {
      __IOM uint8_t nmos_drive  : 3;            /*!< [2..0] SD_CMD PAD NMOS drive select 3V3: 1V8: 000: x 10.1 x
                                                     1.93 001: x 13.5 x 3.85 010,100: x 16.9 x 5.78 011,101:
                                                     x 20.2 x 7.73 110: x 23.5 x 9.65 111: x 26.8 x 11.5                       */
      __IOM uint8_t pmos_drive  : 3;            /*!< [5..3] SD_CMD PAD PMOS drive select 3V3: 1V8: 000: x 7,07 x
                                                     2.27 001: x 14.2 x 4.61 010,100: x 21 x 6.68 011,101: x
                                                     27.9 x 9.14 110: x 34.9 x 11.5 111: x 42.4 x 13.8                         */
    } cmd_pad_drive_b;
  } ;
  
  union {
    __IOM uint8_t dat_pad_drive;                /*!< (@ 0x00000532) Data Pad Driving Register                                  */
    
    struct {
      __IOM uint8_t nmos_drive  : 3;            /*!< [2..0] SD_DAT[3:0] PAD NMOS drive select 3V3: 1V8: 000: x 10.1
                                                     x 1.93 001: x 13.5 x 3.85 010,100: x 16.9 x 5.78 011,101:
                                                     x 20.2 x 7.73 110: x 23.5 x 9.65 111: x 26.8 x 11.5                       */
      __IOM uint8_t pmos_drive  : 3;            /*!< [5..3] SD_DAT[3:0] PAD PMOS drive select 3V3: 1V8: 000: x 7,07
                                                     x 2.27 001: x 14.2 x 4.61 010,100: x 21 x 6.68 011,101:
                                                     x 27.9 x 9.14 110: x 34.9 x 11.5 111: x 42.4 x 13.8                       */
    } dat_pad_drive_b;
  } ;
  __IM  uint8_t   RESERVED15;
  __IM  uint32_t  RESERVED16[19];
  
  union {
    __IOM uint8_t sd_config1;                   /*!< (@ 0x00000580) SD Configuration Register 1                                */
    
    struct {
      __IOM uint8_t bus_width   : 2;            /*!< [1..0] 00: 1-bit bus 01: 4-bit bus 10: 8-bit bus 11: Reserved             */
      __IOM uint8_t mode_sel    : 2;            /*!< [3..2] 00: SD20 mode (single data rate; internal clock frequency
                                                     is 2 times as SD_CLK) 01: DDR mode (double data rate; internal
                                                     clock frequency is 2 times as SD_CLK) 10: SD30 mode (single
                                                     data rate; internal clock frequency is the same as SD_CLK;
                                                     sampling point turning can only be used in this mode) 11:
                                                     Reserved Note: When access SDR Card that needs to do sampling
                                                     turning, we must select SD30_mode; when access SDR Card
                                                     that needn't sampling turning, both SD20_mode and SD30_mode
                                                     is available.                                                             */
      __IOM uint8_t sd30_async_fifo_rst : 1;    /*!< [4..4] When SD30_mode is asserted, the input CMD/DAT will be
                                                     latched by an asynchronous FIFO. The write clock of FIFO
                                                     is sample clock; the read clock of FIFO is internal clock.
                                                     Write this bit to 1'b0 to reset write/read pointer of FIFO
                                                     after the frequency/phase of sample clock or internal clock
                                                     is changed.                                                               */
      __IM  uint8_t             : 1;
      __IOM uint8_t clk_div     : 1;            /*!< [6..6] Clock Divider (only available when initial mode is set)
                                                     0: SDCLK is divided by 128 1: SDCLK is divided by 256                     */
      __IOM uint8_t initial_mode : 1;           /*!< [7..7] Initial Mode (can not be used in SD30 mode) This bit
                                                     is used to control whether the SD clock will be divided
                                                     by clock divider 0: The SD clock can not be divided 1:
                                                     The SD clock will de divided by the value of Clock Divider                */
    } sd_config1_b;
  } ;
  
  union {
    __IOM uint8_t sd_config2;                   /*!< (@ 0x00000581) SD Configuration Register 2                                */
    
    struct {
      __IOM uint8_t rsp_type    : 2;            /*!< [1..0] Response Type Configure. 00: No response 01: 6-byte response
                                                     10: 17-byte response 11: Reserved                                         */
      __IOM uint8_t crc7_chk    : 1;            /*!< [2..2] CRC7 Check Enable. 0: Check CRC7 1: Not check CRC7                 */
      __IOM uint8_t wait_busy_end : 1;          /*!< [3..3] Wait busy End Enable If this bit is set, hardware will
                                                     wait (with SD_CLK continually toggling) till SD card is
                                                     ready (SD_DAT0 is 1). This bit is only available when HW
                                                     executes SEND_CMD_GET_RSP command code 0: .Not wait busy
                                                     end; HW will stop toggle SD clock after the command/response
                                                     transfer is completed, not concern about whether the SD_DAT0
                                                     is 1b or not 1: Wait busy end; HW will continue to toggle
                                                     SD clock if SD_DAT0 is 0b                                                 */
      __IOM uint8_t ignore_crc_sts_err : 1;     /*!< [4..4] Ignore Write CRC Error Enable. Set this bit to ignore
                                                     the write error. 0: Check whether CRC status returned by
                                                     card is correct 1: Not check whether CRC status returned
                                                     by card is correct                                                        */
      __IOM uint8_t wait_crc_sts_timeout : 1;   /*!< [5..5] Wait Write CRC Status Time Out Enable If this bit is
                                                     set, HW will check whether the write CRC status is sent
                                                     by card in time.(In SD3.0 spec, card should send the CRC
                                                     status within 8 clocks, HW will wait 16 clocks actually)
                                                     0: HW will Check whether the write CRC status is time-out
                                                     1: HW will Not Check whether the write CRC status is time-out             */
      __IOM uint8_t crc16_chk   : 1;            /*!< [6..6] CRC16 Check Enable. 0: Check CRC16 1: Not check CRC16              */
      __IOM uint8_t crc7_cal    : 1;            /*!< [7..7] CRC7 Calculation Enable. 0: Calculate CRC7 1: No calculation       */
    } sd_config2_b;
  } ;
  
  union {
    __IOM uint8_t sd_config3;                   /*!< (@ 0x00000582) SD Configuration Register 3                                */
    
    struct {
      __IOM uint8_t rsp_timeout_en : 1;         /*!< [0..0] SD CMD Response Time Out Enable. If this bit is set,
                                                     HW will not check whether card's response to command is
                                                     time-out. It is considered to time-out if there is no SD
                                                     command response within 80 periods of SD_CLK after the
                                                     end bit of command is on the bus. 0: Not check whether
                                                     the response to command is time-out 1: Check whether the
                                                     response to command is time-out                                           */
      __IOM uint8_t addr_mode   : 1;            /*!< [1..1] Address mode. 0: sector address mode SD start data address
                                                     set in SD read/write command argument(CMD18/CMD25) equal
                                                     to (the start sector address) which is set in SCSI command
                                                     read10/write10; 1: byte mode SD start data address set
                                                     in SD read/write command argument(CMD18/CMD25) equal to
                                                     (the start sector address <<9)                                            */
      __IOM uint8_t rsp_chk     : 1;            /*!< [2..2] SD CMD Response Check Enable When in Random auto mode,
                                                     If this bit is set, HW will check whether card's response
                                                     is received correctly or not. When not in random auto mode,
                                                     this bit is useless. 0: No check whether the response of
                                                     command is correct 1: Check whether the response to command
                                                     is correct                                                                */
      __IOM uint8_t sd20_clk_stop : 1;          /*!< [3..3] SD20 Clock Stop After Data Transfer Over Enable. (Default
                                                     Disable) In order to conform to eMMC spec(NAC minimum value
                                                     is 2 SD Clock cycle), when this bit is 1'b1 and data transfer
                                                     is over(Sector Count is 0), SD clock(SD20 mode) is stopped
                                                     immediately. 0: Disable 1: Enable                                         */
      __IOM uint8_t sd30_clk_stop : 1;          /*!< [4..4] SD30 Clock Stop After Data Transfer Over Enable. (Default
                                                     Disable) In order to conform to eMMC spec(NAC minimum value
                                                     is 2 SD Clock cycle), when this bit is 1'b1 and data transfer
                                                     is over(Sector Count is 0), SD clock(SD30 SDR and DDR mode)
                                                     is stopped immediately. 0: Disable 1: Enable                              */
      __IOM uint8_t wait_card_idle : 1;         /*!< [5..5] Data Phase Wait Card busy Enable. When this bit is set
                                                     , wait until SD card not busy, then set sd_end. 0: No wait
                                                     SD card not busy, set sd_end immediately when data transfer
                                                     is over 1: Wait until SD card is not busy, then set sd_end                */
      __IOM uint8_t cmd_start_wait_card_idle : 1;/*!< [6..6] CMD Start Wait No Card busy. When this bit is set , no
                                                     wait card busy and send command immediately. 0: Wait until
                                                     card is not busy, then send the pending command 1: No wait
                                                     card busy and send command immediately                                    */
      __IOM uint8_t stop_cmd_start_wait_card_idle : 1;/*!< [7..7] STOP CMD Start No Wait Card busy. This bit is only valid
                                                     when the SD card controller is auto in random auto mode.
                                                     In random auto mode, SD card controller will auto send
                                                     stop cmd(CMD12) after read/write cmd(cmd18/cmd25), this
                                                     bit control whether HW wait busy end before send CMD12
                                                     When this bit is set , no wait card busy and send stop
                                                     command (CMD12) immediately. 0: Wait until card is not
                                                     busy, then send the stop command (CMD12) 1: No wait card
                                                     busy and send stop command immediately                                    */
    } sd_config3_b;
  } ;
  
  union {
    __IOM uint8_t sd_status1;                   /*!< (@ 0x00000583) SD Status Register 1                                       */
    
    struct {
      __IOM uint8_t tune_patrn_err : 1;         /*!< [0..0] SD Tuning Pattern Compare Error. This bit will be set
                                                     to 1'b1, if the tuning data sampled by HW is not correct.                 */
      __IOM uint8_t crc_sts_timeout_err : 1;    /*!< [1..1] Get Write CRC Status Time-out Error This bit will be
                                                     set to 1'b1, if SD card doesn't return write CRC status
                                                     bits to host within 8 SD_CLK cycles after the end bit of
                                                     data packet is on the bus (actually, HW will wait 16 clocks).             */
      __IOM uint8_t crc_sts_val : 3;            /*!< [4..2] Status Of Write CRC Status. This field reflects write
                                                     error bits returned from SD card. When card check CRC error,
                                                     it sends back CRC status (101); when card check CRC no
                                                     error, it sends back CRC status (010); when flash programming
                                                     error, CRC status read (111).                                             */
      __IOM uint8_t crc_sts_err : 1;            /*!< [5..5] Write CRC Error. This bit will be set to 1'b1, if the
                                                     write CRC status bits are not equal to 3'b010. (this means
                                                     Card detected CRC16 error in the data it received from
                                                     host).                                                                    */
      __IOM uint8_t crc16_err   : 1;            /*!< [6..6] CRC16 Error. This bit will be set to 1'b1, if there is
                                                     CRC16 check error in the data from SD card.                               */
      __IOM uint8_t crc7_err    : 1;            /*!< [7..7] CRC7 Error. This bit will be set to 1'b1, if there is
                                                     CRC7 check error in the response from SD card.                            */
    } sd_status1_b;
  } ;
  
  union {
    __IOM uint8_t sd_status2;                   /*!< (@ 0x00000584) SD Status Register 2                                       */
    
    struct {
      __IOM uint8_t rsp_timeout_err : 1;        /*!< [0..0] SD CMD Response Timeout Error. This bit will be set to
                                                     1'b1, if there is no response within 80 periods of SD_CLK
                                                     after the end bit of command is on the bus.                               */
      __IOM uint8_t rsp_invalid : 1;            /*!< [1..1] SD CMD Response Invalid. If SD_CONFIGURE3 bit[2] is set,
                                                     this bit will reflect when the response is correct or not.
                                                     If this bit is set, sd_end and sd_error (SD_TRANSFER bit[6]
                                                     and bit[4]) will also be set 0: The response of command
                                                     is correct 1: The response to command is not correct                      */
      __IOM uint8_t cbw_state   : 4;            /*!< [5..2] CBW State Machine.                                                 */
    } sd_status2_b;
  } ;
  
  union {
    __IOM uint8_t sd_bus_status;                /*!< (@ 0x00000585) SD Bus Status Register                                     */
    
    struct {
      __IOM uint8_t cmd         : 1;            /*!< [0..0] This bit reflect the level of pin SD_CMD.                          */
      __IOM uint8_t dat3_0      : 4;            /*!< [4..1] This bit reflect the level of pin SD_DAT3~SD_DAT0.                 */
      __IM  uint8_t             : 1;
      __IOM uint8_t stop_sdclk_when_no_xfer : 1;/*!< [6..6] Stop SD_CLK toggling when no cmd/data transfer (ignore
                                                     the level of SDDAT0)                                                      */
      __IOM uint8_t sdclk_toggle : 1;           /*!< [7..7] SD_CLK Toggle Enable. If this bit is set, host will generate
                                                     SDCLK toggle signal even when there is no signal transaction
                                                     on the CMD/DAT bus                                                        */
    } sd_bus_status_b;
  } ;
  __IM  uint16_t  RESERVED17;
  __IM  uint8_t   RESERVED18;
  
  union {
    __IOM uint8_t sd_cmd0;                      /*!< (@ 0x00000589) SD Command Register 0                                      */
    
    struct {
      __IOM uint8_t cmd         : 8;            /*!< [7..0] Command[47:40] or Response Data[47:40].                            */
    } sd_cmd0_b;
  } ;
  
  union {
    __IOM uint8_t sd_cmd1;                      /*!< (@ 0x0000058A) SD Command Register 1                                      */
    
    struct {
      __IOM uint8_t cmd         : 8;            /*!< [7..0] Command[39:32] or Response Data[39:32].                            */
    } sd_cmd1_b;
  } ;
  
  union {
    __IOM uint8_t sd_cmd2;                      /*!< (@ 0x0000058B) SD Command Register 2                                      */
    
    struct {
      __IOM uint8_t cmd         : 8;            /*!< [7..0] Command[31:24] or Response Data[31:24].                            */
    } sd_cmd2_b;
  } ;
  
  union {
    __IOM uint8_t sd_cmd3;                      /*!< (@ 0x0000058C) SD Command Register 3                                      */
    
    struct {
      __IOM uint8_t cmd         : 8;            /*!< [7..0] Command[23:16] or Response Data[23:16].                            */
    } sd_cmd3_b;
  } ;
  
  union {
    __IOM uint8_t sd_cmd4;                      /*!< (@ 0x0000058D) SD Command Register 4                                      */
    
    struct {
      __IOM uint8_t cmd         : 8;            /*!< [7..0] Command[15:8] or Response Data[15:8].                              */
    } sd_cmd4_b;
  } ;
  
  union {
    __IOM uint8_t sd_cmd5;                      /*!< (@ 0x0000058E) SD Command Register 5                                      */
    
    struct {
      __IOM uint8_t cmd         : 8;            /*!< [7..0] Command[7:0] or Response Data[7:0].                                */
    } sd_cmd5_b;
  } ;
  
  union {
    __IOM uint8_t sd_byte_cnt_l;                /*!< (@ 0x0000058F) Byte Count Register (Low Byte)                             */
    
    struct {
      __IOM uint8_t byte_cnt_l  : 8;            /*!< [7..0] Byte Count[7:0]. Byte count in one block transferred
                                                     from/to SD card.                                                          */
    } sd_byte_cnt_l_b;
  } ;
  
  union {
    __IOM uint8_t sd_byte_cnt_h;                /*!< (@ 0x00000590) Byte Count Register (High Byte)                            */
    
    struct {
      __IOM uint8_t byte_cnt_h  : 3;            /*!< [2..0] Byte Count[10:8]. Byte count in one block transferred
                                                     from/to SD card.                                                          */
    } sd_byte_cnt_h_b;
  } ;
  
  union {
    __IOM uint8_t sd_blk_cnt_l;                 /*!< (@ 0x00000591) Block Count Register (Low Byte)                            */
    
    struct {
      __IOM uint8_t blk_cnt_l   : 8;            /*!< [7..0] Block Count[7:0]. Block count transferred from/to SD
                                                     card.                                                                     */
    } sd_blk_cnt_l_b;
  } ;
  
  union {
    __IOM uint8_t sd_blk_cnt_h;                 /*!< (@ 0x00000592) Block Count Register (High Byte)                           */
    
    struct {
      __IOM uint8_t blk_cnt_h   : 7;            /*!< [6..0] Block Count[14:8]. Byte count transferred from/to SD
                                                     card.                                                                     */
    } sd_blk_cnt_h_b;
  } ;
  
  union {
    __IOM uint8_t sd_xfer;                      /*!< (@ 0x00000593) SD Transfer Control Register                               */
    
    struct {
      __IOM uint8_t cmd_code    : 4;            /*!< [3..0] Command Code. These bits encoded each working mode of
                                                     SD card control module. 0000(NORMAL_WRITE): Write 1 or
                                                     2 bytes to SD card and the content are put into SD_CMD2
                                                     & SD_CMD3 in advance. Hardware ignores write error returned
                                                     from SD card in this case. 0001(AUTO_WRITE3): Hardware
                                                     writes data from Ring buffer to SD card and the data length
                                                     is (SD_BYTE_CNT * SD_BLOCK_CNT). Hardware can only write
                                                     even bytes (2, 4, 6, ..., 512) data from SRAM2 buffer to
                                                     SD card if data source is consigned to SRAM2 buffer in
                                                     th                                                                        */
      __IOM uint8_t err         : 1;            /*!< [4..4] If this bit is set which means some error occurs detailed
                                                     error information is in register SD_STAT.                                 */
      __IOM uint8_t idle_ste    : 1;            /*!< [5..5] Status of SD card module state machine When this bit
                                                     is set 1, it means that the SD card module state machine
                                                     is in idle state.                                                         */
      __IOM uint8_t end         : 1;            /*!< [6..6] If transfer starts this bit will be clear automatically
                                                     by hardware. And if transfer completes this bit will be
                                                     set and keep 1 until the next Start is set.                               */
      __IOM uint8_t start       : 1;            /*!< [7..7] The transfer is launch if set this bit.                            */
    } sd_xfer_b;
  } ;
  __IM  uint8_t   RESERVED19;
  
  union {
    __IOM uint8_t sd_cmd_ste;                   /*!< (@ 0x00000595) SD Command State Register                                  */
    
    struct {
      __IOM uint8_t cmd_ste     : 4;            /*!< [3..0] CMD State Machine                                                  */
      __IM  uint8_t             : 3;
      __IOM uint8_t cmd_ste_is_idle : 1;        /*!< [7..7] CMD State Machine is idle When this bit is 1'b1, it indicates
                                                     IDLE state of CMD State Machine                                           */
    } sd_cmd_ste_b;
  } ;
  
  union {
    __IOM uint8_t sd_data_ste;                  /*!< (@ 0x00000596) SD Data State Register                                     */
    
    struct {
      __IOM uint8_t data_ste    : 5;            /*!< [4..0] DATA State Machine                                                 */
      __IM  uint8_t             : 2;
      __IOM uint8_t data_ste_is_idle : 1;       /*!< [7..7] DATA State Machine is idle When this bit is 1'b1, it
                                                     indicates IDLE state of DATA State Machine                                */
    } sd_data_ste_b;
  } ;
} SDIO_HOST_Type;  
#endif

typedef struct hal_sdio_host_adapter_s {
    SDIO_HOST_Type *base_addr;
    u32 xfer_int_sts;
    u8 csd[16];
    u16 rca;
    volatile u8 is_card_inserted;
    volatile u8 is_wp;
    u8 curr_sig_level;
    u8 is_sdhc_sdxc;
    u8 card_curr_ste;
    u8 sd_spec_ver;
    u8 curr_bus_spd;
    u8 pin_mux_sel;
    u8 card_support_spd_mdoe;
    u8 is_s18a;
    u8 voltage_mismatch;
    u8 rsvd[3];
    sdioh_card_detect card_insert_cb;
    sdioh_card_detect card_remove_cb;
    void *card_insert_cb_para;
    void *card_remove_cb_para;
    sdioh_task_yield task_yield;
    void (*dcache_invalidate_by_addr)(uint32_t *addr, int32_t dsize);   /*! callback function to do the D-cache invalidate  */
    void (*dcache_clean_by_addr) (uint32_t *addr, int32_t dsize);   /*! callback function to do the D-cache clean  */
} hal_sdio_host_adapter_t, *phal_sdio_host_adapter_t;

#endif

#endif