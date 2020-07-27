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
 * $Id: util.c,v 1.10 2005/02/02 07:56:33 sur Exp $
 */

#include <math.h>
#include <stdio.h>
#include <string.h>

#include "aac_util.h"
#include "coder.h"  // FRAME_LEN

#define rt_printf printf
/* Returns the sample rate index */
int aac_mem = 0;
extern int aac_mem_record;
// this is only for STM32 board, if you are using another test board, please implement your own time function
extern volatile int OS_TimeMS;

int g_dSizeALL = 0;
int g_dSizeAva = 0;
void* AllocMemory(int size)
{
	int abc;
#ifdef FAAC_USE_PSRAM
	void* addr = Psram_reserve_malloc(size);
	// printf("*******used Psram_reserve_malloc, addr:0x%x*******\r\n", addr);
#else
	void* addr = pvPortMalloc(size);
	//printf("*******used pvPortMalloc, addr:0x%x*******\r\n", addr);
#endif
	g_dSizeALL += size;
	if(addr != 0)
        {
		//rt_printf("Memory Allocation Success! (%d)\r\n", size);
		g_dSizeAva += size;
        }
        else
        {
		rt_printf("Memory Allocation Fail! (%d)\r\n", size);
        }
	return addr;
}

void MemEst(void* a_pAddr, int a_dSize)
{
	g_dSizeALL += a_dSize;
	if(a_pAddr != 0)
		g_dSizeAva += a_dSize;
}

int getTick()
{
   return OS_TimeMS;
}

int GetSRIndex(unsigned int sampleRate)
{
    if (92017 <= sampleRate) return 0;
    if (75132 <= sampleRate) return 1;
    if (55426 <= sampleRate) return 2;
    if (46009 <= sampleRate) return 3;
    if (37566 <= sampleRate) return 4;
    if (27713 <= sampleRate) return 5;
    if (23004 <= sampleRate) return 6;
    if (18783 <= sampleRate) return 7;
    if (13856 <= sampleRate) return 8;
    if (11502 <= sampleRate) return 9;
    if (9391 <= sampleRate) return 10;

    return 11;
}

/* Returns the maximum bitrate per channel for that sampling frequency */
unsigned int MaxBitrate(unsigned long sampleRate)
{
    /*
     *  Maximum of 6144 bit for a channel
     */
    return (unsigned int)(6144.0 * (float)sampleRate/(float)FRAME_LEN + .5);
}

/* Returns the minimum bitrate per channel for that sampling frequency */
unsigned int MinBitrate()
{
    return 8000;
}


/* Max prediction band for backward predictionas function of fs index */
const int MaxPredSfb[] = { 33, 33, 38, 40, 40, 40, 41, 41, 37, 37, 37, 34, 0 };

int GetMaxPredSfb(int samplingRateIdx)
{
    return MaxPredSfb[samplingRateIdx];
}



/* Calculate bit_allocation based on PE */
unsigned int BitAllocation(float pe, int short_block)
{
    float pew1;
    float pew2;
    float bit_allocation;

    if (short_block) {
        pew1 = 0.6;
        pew2 = 24.0;
    } else {
        pew1 = 0.3;
        pew2 = 6.0;
    }
    bit_allocation = pew1 * pe + pew2 * sqrt(pe);
    bit_allocation = min(max(0.0, bit_allocation), 6144.0);

    return (unsigned int)(bit_allocation+0.5);
}

/* Returns the maximum bit reservoir size */
unsigned int MaxBitresSize(unsigned long bitRate, unsigned long sampleRate)
{
    return 6144 - (unsigned int)((float)bitRate/(float)sampleRate*(float)FRAME_LEN);
}
#ifdef AAC_CYCLECOUNT_ALL
volatile uint32_t *DWT_CONTROL0 = (uint32_t *) 0xE0001000;
volatile uint32_t *DWT_CYCCNT0 = (uint32_t *) 0xE0001004;
volatile uint32_t *DEMCR0 = (uint32_t *) 0xE000EDFC;
volatile uint32_t *LAR0  = (uint32_t *) 0xE0001FB0;

volatile unsigned int *DWT_CYCCNT  ;
volatile unsigned int *DWT_CONTROL ;
volatile unsigned int *SCB_DEMCR   ;

void reset_timer(){
	*DEMCR0 = *DEMCR0 | 0x01000000;     // enable trace
	*LAR0 = 0xC5ACCE55;                // <-- added unlock access to DWT (ITM, etc.)registers 
	*DWT_CYCCNT = 0;                  // clear DWT cycle counter
	*DWT_CONTROL = *DWT_CONTROL | 1;  // enable DWT cycle counter
    DWT_CYCCNT   = (int *)0xE0001004; //address of the register
    DWT_CONTROL  = (int *)0xE0001000; //address of the register
    SCB_DEMCR    = (int *)0xE000EDFC; //address of the register
    *SCB_DEMCR   = *SCB_DEMCR | 0x01000000;
    *DWT_CYCCNT  = 0; // reset the counter
    *DWT_CONTROL = 0; 
}

void start_timer(){
    *DWT_CONTROL = *DWT_CONTROL | 1 ; // enable the counter
}

void stop_timer(){
    *DWT_CONTROL = *DWT_CONTROL | 0 ; // disable the counter    
}

unsigned int getCycles(){
    return *DWT_CYCCNT;
}
#endif
