/*
 * FAAC - Freeware Advanced Audio Coder
 * Copyright (C) 2001 Menno Bakker
 *
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.

 * You should have received a copy of the GNU Lesser General Public
 * License along with this library; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
 *
 * $Id: util.h,v 1.8 2003/12/20 04:32:48 stux Exp $
 */

#ifndef AAC_UTIL_H
#define AAC_UTIL_H

#ifdef __cplusplus
extern "C" {
#endif /* __cplusplus */

#define AMEBA   0 // 1 runs on Ameba, 0 runs on STM32
#include "diag.h"
#if AMEBA
#include "FreeRTOS.h"
#include "task.h"
#include "diag.h"
#include "main.h"
#include "platform_stdlib.h"
#include "math.h"
#else
#include <stdlib.h>
#include <stdio.h>
#endif // end of #AMEBA
#include <string.h>
// for CMSIS-DSP library, wf, 0415
#include "arm_math.h"
#include <arm_const_structs.h>
//#include "../../../../../component/soc/realtek/8195b/cmsis/cmsis-dsp/include/arm_math.h"
//#include "arm_const_structs.h"
#include "section_config.h"

/* amebad use psram */
#if defined(CONFIG_PLATFORM_8721D)
#include <psram_reserve.h>
#define FAAC_USE_PSRAM
#endif

#ifndef max
#define max(a, b) (((a) > (b)) ? (a) : (b))
#endif
#ifndef min
#define min(a, b) (((a) < (b)) ? (a) : (b))
#endif

#ifndef M_PI
#define M_PI        3.14159265358979323846
#endif

#define AAC_TNS_EN      0
#define AAC_LTP_EN      0
#define ARM_DSP_EN      1
#define READ_FILE       0
#define DEBUG_TIME      0
#define RING_BUFFER_EN  0

#define FLOATING_POINT_ENABLE 0

#define afPOW_ACCU 8
#define xr_pow_ACCU 10
#define xr_pow_ACCU_N 16
#define xmin_ACCU 10
#define fixstep_ACCU 16
#define adj43_ACCU 8
#define pow43_ACCU 8
#define sfacfix_ACCU 30
#define log_ifqstep_ACCU 10


#define AAC_RAM_TYPE SDRAM_DATA_SECTION//static or SDRAM_DATA_SECTION

#define AAC_EDIT
#define AAC_ACCURACY_EDIT

//#define AAC_CYCLECOUNT
#define AAC_CYCLECOUNT_ALL
	
//#define SAMPLE01//pcm_test
//#define SAMPLE02//pcm0808m
//#define SAMPLE03//bubbs
//#define SAMPLE04//Viola
//#define SAMPLE05//WF
#define SAMPLE06// 16-bit

/* Memory functions */

void MemEst(void* a_pAddr, int a_dSize);

// be careful here, wf, 0414
//#define float float //wf, to save memory
//#define AllocMemory(size) /*malloc(size)*//*pvPortMalloc(size)*/(pvPortMalloc(size))
void* AllocMemory(int size);
#ifdef FAAC_USE_PSRAM
#define FreeMemory(block) Psram_reserve_free(block)
#else
#define FreeMemory(block) vPortFree(block)
#endif
#define SetMemory(block, value, size) memset(block, value, size)
void *pvPortMalloc( size_t xWantedSize );
void vPortFree( void *pv );
int GetSRIndex(unsigned int sampleRate);
int GetMaxPredSfb(int samplingRateIdx);
unsigned int MaxBitrate(unsigned long sampleRate);
unsigned int MinBitrate();
unsigned int MaxBitresSize(unsigned long bitRate, unsigned long sampleRate);
unsigned int BitAllocation(float pe, int short_block);
int getTick();

void reset_timer();
void start_timer();
void stop_timer();
unsigned int getCycles();


#ifdef __cplusplus
}
#endif /* __cplusplus */

#endif /* AAC_UTIL_H */
