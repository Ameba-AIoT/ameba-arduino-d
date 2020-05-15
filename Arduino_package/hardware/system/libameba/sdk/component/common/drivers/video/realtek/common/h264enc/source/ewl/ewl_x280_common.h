/*------------------------------------------------------------------------------
--                                                                            --
--       This software is confidential and proprietary and may be used        --
--        only as expressly authorized by a licensing agreement from          --
--                                                                            --
--                            Hantro Products Oy.                             --
--                                                                            --
--                   (C) COPYRIGHT 2006 HANTRO PRODUCTS OY                    --
--                            ALL RIGHTS RESERVED                             --
--                                                                            --
--                 The entire notice above must be reproduced                 --
--                  on all copies and should not be removed.                  --
--                                                                            --
--------------------------------------------------------------------------------
--
--  Abstract : Hantro 6280/7280/8270/8290 Encoder Wrapper Layer for OS services
--
------------------------------------------------------------------------------*/
#ifndef __EWL_X280_COMMON_H__
#define __EWL_X280_COMMON_H__

#include <stdio.h>

/* Macro for debug printing */
#undef PTRACE
//#define TRACE_EWL
#ifdef TRACE_EWL
#   include "platform_stdlib.h"
#   define PTRACE(...) printf(__VA_ARGS__)
#else
#   define PTRACE(...)  /* no trace */
#endif

#define ASIC_STATUS_RFC_BUFF_OVERFLOW   0x800
#define ASIC_STATUS_LINE_BUFFER_DONE    0x400
#define ASIC_STATUS_SLICE_READY         0x100
#define ASIC_STATUS_TEST_COPY_RDY       0x80
#define ASIC_STATUS_TEST_IRQ            0x40
#define ASIC_STATUS_BUFF_FULL           0x20
#define ASIC_STATUS_RESET               0x10
#define ASIC_STATUS_ERROR               0x08
#define ASIC_STATUS_FRAME_READY         0x04
#define ASIC_IRQ_LINE                   0x01

#ifdef RT_LINE_BUF
#define ASIC_STATUS_ALL     (ASIC_STATUS_SLICE_READY |\
                             ASIC_STATUS_TEST_IRQ |\
                             ASIC_STATUS_TEST_COPY_RDY |\
                             ASIC_STATUS_FRAME_READY | \
                             ASIC_STATUS_BUFF_FULL | \
                             ASIC_STATUS_RESET | \
                             ASIC_STATUS_ERROR | \
                             ASIC_STATUS_LINE_BUFFER_DONE | \
                             ASIC_STATUS_RFC_BUFF_OVERFLOW)
#else
#define ASIC_STATUS_ALL     (ASIC_STATUS_SLICE_READY |\
                             ASIC_STATUS_TEST_IRQ |\
                             ASIC_STATUS_TEST_COPY_RDY |\
                             ASIC_STATUS_FRAME_READY | \
                             ASIC_STATUS_BUFF_FULL | \
                             ASIC_STATUS_RESET | \
                             ASIC_STATUS_ERROR | \
                             ASIC_STATUS_RFC_BUFF_OVERFLOW)
#endif

/* the encoder device driver nod */
#ifndef MEMALLOC_MODULE_PATH
#define MEMALLOC_MODULE_PATH        "/tmp/dev/memalloc"
#endif

#ifndef ENC_MODULE_PATH
#define ENC_MODULE_PATH             "/tmp/dev/hx280"
#endif

#ifndef SDRAM_LM_BASE
#define SDRAM_LM_BASE               0x00000000
#endif

/* EWL internal information for Linux */
typedef struct
{
    u32 clientType;
    int fd_mem;              /* /dev/mem */
    int fd_enc;              /* /dev/hx280 */
    int fd_memalloc;         /* /dev/memalloc */
    u32 regSize;             /* IO mem size */
    u32 regBase;
    volatile u32 *pRegBase;  /* IO mem base */
#ifdef PCIE_FPGA_VERIFICATION
    u32 linMemBase;          /* start address of linear memory  wxh added for pcie verification */
#endif
#ifdef PCIE_FPGA_VERI_LINEBUF
    u32 sram_base;
    u32 sram_size;
    volatile u32 *psrame;  /* srame mem base */ 
#endif
    int semid;
    int sigio_needed;
} hx280ewl_t;

void HandleSIGIO(hx280ewl_t * enc);

#endif /* __EWLX280_COMMON_H__ */
