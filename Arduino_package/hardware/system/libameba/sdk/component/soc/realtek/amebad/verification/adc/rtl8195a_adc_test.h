/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */

#ifndef _RTL8195A_ADC_TEST_H_
#define _RTL8195A_ADC_TEST_H_

typedef enum _ADC_VERI_PROC_CMD_ {
        ADC_TEST_DEINIT     = 0,
        ADC_TEST_INIT       = 1,
        ADC_TEST_DUMP       = 2,
}ADC_VERI_PROC_CMD, *PADC_VERI_PROC_CMD;

typedef enum _ADC_VERI_ITEM_ {
        ADC_TEST_S_NINT     = 1,     //single rw without DMA without INT
        ADC_TEST_S_RINT     = 2,     //single rw without DMA with INT
        ADC_TEST_D_INT      = 3,     //single rw without DMA with INT
        ADC_TEST_DTR_INT    = 4,     //single rw using DMA with INT
#if TASK_SCHEDULER_DISABLED
        ADC_TEST_S_GCINT    = 7
#else
        ADC_TEST_S_GCINT    = 7,
        ADC_TEST_N_DEINT    = 10
#endif
}ADC_VERI_ITEM, *PADC_VERI_ITEM;


typedef struct _ADC_VERI_PARA_ {
        u32         VeriProcCmd;
        u32         VeriItem;
        u32         VeriLoop;
        u32         VeriAudio;
        u32         VeriEndian;
}ADC_VERI_PARA,*PADC_VERI_PARA;

#endif
