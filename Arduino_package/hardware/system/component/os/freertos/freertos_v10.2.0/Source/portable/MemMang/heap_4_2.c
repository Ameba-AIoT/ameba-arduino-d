/*
 * FreeRTOS Kernel V10.0.1
 * Copyright (C) 2017 Amazon.com, Inc. or its affiliates.  All Rights Reserved.
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * http://www.FreeRTOS.org
 * http://aws.amazon.com/freertos
 *
 * 1 tab == 4 spaces!
*/

/*
 * A sample implementation of pvPortMalloc() and vPortFree() that combines
 * (coalescences) adjacent memory blocks as they are freed, and in so doing
 * limits memory fragmentation.
 *
 * See heap_1.c, heap_2.c and heap_3.c for alternative implementations, and the
 * memory management pages of http://www.FreeRTOS.org for more information.
 */
#include <stdlib.h>

/* Defining MPU_WRAPPERS_INCLUDED_FROM_API_FILE prevents task.h from redefining
all the API functions to use the MPU wrappers.  That should only be done when
task.h is included from an application file. */
#define MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#include "FreeRTOS.h"
#include "task.h"

#undef MPU_WRAPPERS_INCLUDED_FROM_API_FILE

#if( configSUPPORT_DYNAMIC_ALLOCATION == 0 )
	#error This file must not be used if configSUPPORT_DYNAMIC_ALLOCATION is 0
#endif

#if !defined( configSUPPORT_REALLOC )
	// default using realloc version 1 
	#define configSUPPORT_REALLOC 1
#endif

/* Block sizes must not get too small. */
#define heapMINIMUM_BLOCK_SIZE	( ( size_t ) ( xHeapStructSize << 1 ) )

/* Assumes 8bit bytes! */
#define heapBITS_PER_BYTE		( ( size_t ) 8 )

/* Allocate the memory for the heap. */
//uint8_t *ucHeap = NULL;

#if defined(__ICCARM__)
extern uint8_t SramEnd[];
extern uint8_t EramEnd[];
#pragma section = "RAM_BSS"
#pragma section = "ERAM_BSS"

#undef configTOTAL_HEAP_SIZE 
#undef configTOTAL_HEAP_ext_SIZE

#define configTOTAL_HEAP1_SIZE ((uint32_t)SramEnd - ((uint32_t)__section_end("RAM_BSS")))
#define HEAP1_START (uint8_t*)__section_end("RAM_BSS")

#define configTOTAL_HEAP0_SIZE ((uint32_t)EramEnd - ((uint32_t)__section_end("ERAM_BSS")))
#define HEAP0_START (uint8_t*)__section_end("ERAM_BSS")

#elif defined(__GNUC__)

#undef configTOTAL_HEAP_SIZE 
#undef configTOTAL_HEAP_ext_SIZE

extern uint8_t __sram_end__[];
extern uint8_t __eram_end__[];
extern uint8_t __bss_end__[];
extern uint8_t __eram_bss_end__[];

#define configTOTAL_HEAP1_SIZE ((uint32_t)__sram_end__ - ((uint32_t)__bss_end__))
#define HEAP1_START (uint8_t*)__bss_end__

#define configTOTAL_HEAP0_SIZE ((uint32_t)__eram_end__ - ((uint32_t)__eram_bss_end__))
#define HEAP0_START (uint8_t*)__eram_bss_end__

#endif

#if( configAPPLICATION_ALLOCATED_HEAP == 1 )
	/* The application writer has already defined the array used for the RTOS
	heap - probably so it can be placed in a special segment or address. */
	extern uint8_t ucHeap1[ configTOTAL_HEAP_SIZE ];
    extern uint8_t ucHeap2[ configTOTAL_HEAP_ext_SIZE ];
#endif /* configAPPLICATION_ALLOCATED_HEAP */
    
/* Define the linked list structure.  This is used to link free blocks in order
of their memory address. */
typedef struct A_BLOCK_LINK
{
	struct A_BLOCK_LINK *pxNextFreeBlock;	/*<< The next free block in the list. */
	size_t xBlockSize;						/*<< The size of the free block. */
} BlockLink_t;

/*-----------------------------------------------------------*/

/*
 * Inserts a block of memory that is being freed into the correct position in
 * the list of free memory blocks.  The block being freed will be merged with
 * the block in front it and/or the block behind it if the memory blocks are
 * adjacent to each other.
 */
static void prvInsertBlockIntoFreeList( BlockLink_t *pxBlockToInsert, int idx );

/*
 * Called automatically to setup the required heap structures the first time
 * pvPortMalloc() is called.
 */
static void prvHeapInit( void );

/*-----------------------------------------------------------*/

/* The size of the structure placed at the beginning of each allocated memory
block must by correctly byte aligned. */
static const size_t xHeapStructSize	= ( sizeof( BlockLink_t ) + ( ( size_t ) ( portBYTE_ALIGNMENT - 1 ) ) ) & ~( ( size_t ) portBYTE_ALIGNMENT_MASK );

#if 0
/* Create a couple of list links to mark the start and end of the list. */
static BlockLink_t xStart, *pxEnd = NULL;

/* Keeps track of the number of free bytes remaining, but says nothing about
fragmentation. */
static size_t xFreeBytesRemaining = 0U;
static size_t xMinimumEverFreeBytesRemaining = 0U;

/* Gets set to the top bit of an size_t type.  When this bit in the xBlockSize
member of an BlockLink_t structure is set then the block belongs to the
application.  When the bit is free the block is still part of the free heap
space. */
static size_t xBlockAllocatedBit = 0;
#endif

typedef size_t (*GetInfo_t)(void);

struct __heap_info{
    BlockLink_t xStart, *pxEnd;
    size_t xFreeBytesRemaining;
    size_t xMinimumEverFreeBytesRemaining;
    size_t xBlockAllocatedBit;
    size_t xTotalHeapSize;
    uint8_t *ucHeap;
    uint8_t *ucHeapTop;
    
    GetInfo_t fGetHeapSize;
    GetInfo_t fGetHeapStart;
};

size_t xGetHeap1Size(void)
{
    return configTOTAL_HEAP1_SIZE;
}

size_t xGetHeap0Size(void)
{
    return configTOTAL_HEAP0_SIZE;
}

size_t xGetHeap1Start(void)
{
    return (size_t)HEAP1_START;
}

size_t xGetHeap0Start(void)
{
    return (size_t)HEAP0_START;
}


struct __heap_info xHeapInfo[2] = {
    {.fGetHeapStart = xGetHeap0Start, .fGetHeapSize = xGetHeap0Size},
    {.fGetHeapStart = xGetHeap1Start, .fGetHeapSize = xGetHeap1Size}
};

#if 0 //defined(SECURE)
void vTaskSuspendAll() {}
BaseType_t xTaskResumeAll( void ) {return 0;}
#endif

static unsigned int ulCinitMode = 0;

void vPortSetCinitMode(void)
{
	ulCinitMode	 = 1;
}

void vPortClrCinitMode(void)
{
	ulCinitMode	 = 0;
}

void vPortLock(void)
{
	if(ulCinitMode == 0)
		vTaskSuspendAll();
}

void vPortUnlock(void)
{
	if(ulCinitMode == 0)
		( void ) xTaskResumeAll();
}

#include "diag.h"
/*-----------------------------------------------------------*/
int xPortGetHeapIndex( void *pv)
{
    //rt_printf("heap count %d\n\r", sizeof(xHeapInfo)/sizeof(xHeapInfo[0]));
    for(int i=0;i<sizeof(xHeapInfo)/sizeof(xHeapInfo[0]);i++){
        //rt_printf("top %x, size %x, free %x\n\r", xHeapInfo[i].ucHeapTop, xHeapInfo[i].xTotalHeapSize, pv);
        if( ((size_t)xHeapInfo[i].ucHeapTop <= (size_t)pv) && (((size_t)xHeapInfo[i].ucHeapTop+xHeapInfo[i].xTotalHeapSize) > (size_t)pv))
           return i;
    }
    dbg_printf("heap count %d\n\r", sizeof(xHeapInfo)/sizeof(xHeapInfo[0]));
    dbg_printf("top %x, size %x, free %x\n\r", xHeapInfo[0].ucHeapTop, xHeapInfo[0].xTotalHeapSize, pv);
    dbg_printf("top %x, size %x, free %x\n\r", xHeapInfo[1].ucHeapTop, xHeapInfo[1].xTotalHeapSize, pv);
    return -1;
}
              
/*-----------------------------------------------------------*/
void *pvPortMallocExt( size_t xWantedSize, int idx )
{
BlockLink_t *pxBlock, *pxPreviousBlock, *pxNewBlockLink;
void *pvReturn = NULL;

	//vTaskSuspendAll();
	vPortLock();
	{
		/* If this is the first call to malloc then the heap will require
		initialisation to setup the list of free blocks. */
		if( xHeapInfo[idx].pxEnd == NULL )
		{
			prvHeapInit();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}

		/* Check the requested block size is not so large that the top bit is
		set.  The top bit of the block size member of the BlockLink_t structure
		is used to determine who owns the block - the application or the
		kernel, so it must be free. */
		if( ( xWantedSize & xHeapInfo[idx].xBlockAllocatedBit ) == 0 )
		{
			/* The wanted size is increased so it can contain a BlockLink_t
			structure in addition to the requested amount of bytes. */
			if( xWantedSize > 0 )
			{
				xWantedSize += xHeapStructSize;

				/* Ensure that blocks are always aligned to the required number
				of bytes. */
				if( ( xWantedSize & portBYTE_ALIGNMENT_MASK ) != 0x00 )
				{
					/* Byte alignment required. */
					xWantedSize += ( portBYTE_ALIGNMENT - ( xWantedSize & portBYTE_ALIGNMENT_MASK ) );
					configASSERT( ( xWantedSize & portBYTE_ALIGNMENT_MASK ) == 0 );
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}

			if( ( xWantedSize > 0 ) && ( xWantedSize <= xHeapInfo[idx].xFreeBytesRemaining ) )
			{
				/* Traverse the list from the start	(lowest address) block until
				one	of adequate size is found. */
				pxPreviousBlock = &xHeapInfo[idx].xStart;
				pxBlock = xHeapInfo[idx].xStart.pxNextFreeBlock;
				while( ( pxBlock->xBlockSize < xWantedSize ) && ( pxBlock->pxNextFreeBlock != NULL ) )
				{
					pxPreviousBlock = pxBlock;
					pxBlock = pxBlock->pxNextFreeBlock;
				}

				/* If the end marker was reached then a block of adequate size
				was	not found. */
				if( pxBlock != xHeapInfo[idx].pxEnd )
				{
					/* Return the memory space pointed to - jumping over the
					BlockLink_t structure at its start. */
					pvReturn = ( void * ) ( ( ( uint8_t * ) pxPreviousBlock->pxNextFreeBlock ) + xHeapStructSize );

					/* This block is being returned for use so must be taken out
					of the list of free blocks. */
					pxPreviousBlock->pxNextFreeBlock = pxBlock->pxNextFreeBlock;

					/* If the block is larger than required it can be split into
					two. */
					if( ( pxBlock->xBlockSize - xWantedSize ) > heapMINIMUM_BLOCK_SIZE )
					{
						/* This block is to be split into two.  Create a new
						block following the number of bytes requested. The void
						cast is used to prevent byte alignment warnings from the
						compiler. */
						pxNewBlockLink = ( void * ) ( ( ( uint8_t * ) pxBlock ) + xWantedSize );
						configASSERT( ( ( ( size_t ) pxNewBlockLink ) & portBYTE_ALIGNMENT_MASK ) == 0 );

						/* Calculate the sizes of two blocks split from the
						single block. */
						pxNewBlockLink->xBlockSize = pxBlock->xBlockSize - xWantedSize;
						pxBlock->xBlockSize = xWantedSize;

						/* Insert the new block into the list of free blocks. */
						prvInsertBlockIntoFreeList( pxNewBlockLink , idx);
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}

					xHeapInfo[idx].xFreeBytesRemaining -= pxBlock->xBlockSize;

					if( xHeapInfo[idx].xFreeBytesRemaining < xHeapInfo[idx].xMinimumEverFreeBytesRemaining )
					{
						xHeapInfo[idx].xMinimumEverFreeBytesRemaining = xHeapInfo[idx].xFreeBytesRemaining;
					}
					else
					{
						mtCOVERAGE_TEST_MARKER();
					}

					/* The block is being returned - it is allocated and owned
					by the application and has no "next" block. */
					pxBlock->xBlockSize |= xHeapInfo[idx].xBlockAllocatedBit;
					pxBlock->pxNextFreeBlock = NULL;
				}
				else
				{
					mtCOVERAGE_TEST_MARKER();
				}
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}

		traceMALLOC( pvReturn, xWantedSize );
	}
	//( void ) xTaskResumeAll();
	vPortUnlock();

	#if( configUSE_MALLOC_FAILED_HOOK == 1 )
	{
		if( pvReturn == NULL )
		{
			extern void vApplicationMallocFailedHook( void );
			vApplicationMallocFailedHook();
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}
	#endif

	configASSERT( ( ( ( size_t ) pvReturn ) & ( size_t ) portBYTE_ALIGNMENT_MASK ) == 0 );
	return pvReturn;
}
              
/*-----------------------------------------------------------*/
void *pvPortMalloc( size_t xWantedSize )
{
    // use heap[0]
    return pvPortMallocExt(xWantedSize, 0);
}
              
/*-----------------------------------------------------------*/
void vPortFree( void *pv )
{
uint8_t *puc = ( uint8_t * ) pv;
BlockLink_t *pxLink;

    if( pv == NULL) return;
    int idx = xPortGetHeapIndex(pv);
    
    //rt_printf("Index is %d\n\r", idx);
    
	if( pv != NULL )
	{
		/* The memory being freed will have an BlockLink_t structure immediately
		before it. */
		puc -= xHeapStructSize;

		/* This casting is to keep the compiler from issuing warnings. */
		pxLink = ( void * ) puc;

		/* Check the block is actually allocated. */
		configASSERT( ( pxLink->xBlockSize & xHeapInfo[idx].xBlockAllocatedBit ) != 0 );
		configASSERT( pxLink->pxNextFreeBlock == NULL );

		if( ( pxLink->xBlockSize & xHeapInfo[idx].xBlockAllocatedBit ) != 0 )
		{
			if( pxLink->pxNextFreeBlock == NULL )
			{
				/* The block is being returned to the heap - it is no longer
				allocated. */
				pxLink->xBlockSize &= ~xHeapInfo[idx].xBlockAllocatedBit;

				//vTaskSuspendAll();
				vPortLock();
				{
					/* Add this block to the list of free blocks. */
					xHeapInfo[idx].xFreeBytesRemaining += pxLink->xBlockSize;
					traceFREE( pv, pxLink->xBlockSize );
					prvInsertBlockIntoFreeList( ( ( BlockLink_t * ) pxLink ) , idx);
				}
				//( void ) xTaskResumeAll();
				vPortUnlock();
			}
			else
			{
				mtCOVERAGE_TEST_MARKER();
			}
		}
		else
		{
			mtCOVERAGE_TEST_MARKER();
		}
	}
}
/*-----------------------------------------------------------*/

size_t xPortGetFreeHeapSizeExt( int idx )
{
	return xHeapInfo[idx].xFreeBytesRemaining;
}

size_t xPortGetFreeHeapSize( void )
{
	return xPortGetFreeHeapSizeExt(0);
}
/*-----------------------------------------------------------*/

size_t xPortGetMinimumEverFreeHeapSizeExt( int idx )
{
	return xHeapInfo[idx].xMinimumEverFreeBytesRemaining;
}

size_t xPortGetMinimumEverFreeHeapSize( void )
{
	return xPortGetMinimumEverFreeHeapSizeExt(0);
}
/*-----------------------------------------------------------*/

void vPortInitialiseBlocks( void )
{
	/* This just exists to keep the linker quiet. */
}

/*-----------------------------------------------------------*/

static void prvHeapInitExt( int idx )
{
BlockLink_t *pxFirstFreeBlock;
uint8_t *pucAlignedHeap;
size_t uxAddress;
size_t xTotalHeapSize = 0;//configTOTAL_HEAP_SIZE;

    xHeapInfo[idx].ucHeap = (uint8_t *)xHeapInfo[idx].fGetHeapStart();
    xTotalHeapSize = xHeapInfo[idx].fGetHeapSize();
/*
    if(idx == 0){
        xHeapInfo[idx].ucHeap = HEAP0_START;
        xTotalHeapSize = configTOTAL_HEAP0_SIZE;
    }else if(idx==1){
        xHeapInfo[idx].ucHeap = HEAP1_START;
        xTotalHeapSize = configTOTAL_HEAP1_SIZE;
    }
*/
	/* Ensure the heap starts on a correctly aligned boundary. */
	uxAddress = ( size_t ) xHeapInfo[idx].ucHeap;

    //(SramSize- ((uint32_t)__section_end("RAM_BSS")-SramStart))
    //rt_printf("\n\rSramEnd %x, bss end %x\n\r", (uint32_t)SramEnd, (uint32_t)__section_end("RAM_BSS"));
    //rt_printf("Heap %x Size %x\n\r", xHeapInfo[idx].ucHeap, xTotalHeapSize);
    
	if( ( uxAddress & portBYTE_ALIGNMENT_MASK ) != 0 )
	{
		uxAddress += ( portBYTE_ALIGNMENT - 1 );
		uxAddress &= ~( ( size_t ) portBYTE_ALIGNMENT_MASK );
		xTotalHeapSize -= uxAddress - ( size_t ) xHeapInfo[idx].ucHeap;
	}

    xHeapInfo[idx].xTotalHeapSize = xTotalHeapSize;
    xHeapInfo[idx].ucHeapTop = (uint8_t *)uxAddress;
    //rt_printf("Aligned Addr %x Size %x\n\r", uxAddress, xTotalHeapSize);
    
	pucAlignedHeap = ( uint8_t * ) uxAddress;

	/* xStart is used to hold a pointer to the first item in the list of free
	blocks.  The void cast is used to prevent compiler warnings. */
	xHeapInfo[idx].xStart.pxNextFreeBlock = ( void * ) pucAlignedHeap;
	xHeapInfo[idx].xStart.xBlockSize = ( size_t ) 0;

	/* pxEnd is used to mark the end of the list of free blocks and is inserted
	at the end of the heap space. */
	uxAddress = ( ( size_t ) pucAlignedHeap ) + xTotalHeapSize;
	uxAddress -= xHeapStructSize;
	uxAddress &= ~( ( size_t ) portBYTE_ALIGNMENT_MASK );
	xHeapInfo[idx].pxEnd = ( void * ) uxAddress;
	xHeapInfo[idx].pxEnd->xBlockSize = 0;
	xHeapInfo[idx].pxEnd->pxNextFreeBlock = NULL;

	/* To start with there is a single free block that is sized to take up the
	entire heap space, minus the space taken by pxEnd. */
	pxFirstFreeBlock = ( void * ) pucAlignedHeap;
	pxFirstFreeBlock->xBlockSize = uxAddress - ( size_t ) pxFirstFreeBlock;
	pxFirstFreeBlock->pxNextFreeBlock = xHeapInfo[idx].pxEnd;

	/* Only one block exists - and it covers the entire usable heap space. */
	xHeapInfo[idx].xMinimumEverFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;
	xHeapInfo[idx].xFreeBytesRemaining = pxFirstFreeBlock->xBlockSize;

	/* Work out the position of the top bit in a size_t variable. */
	xHeapInfo[idx].xBlockAllocatedBit = ( ( size_t ) 1 ) << ( ( sizeof( size_t ) * heapBITS_PER_BYTE ) - 1 );
}

/*-----------------------------------------------------------*/
static void prvHeapInit( void )
{
    prvHeapInitExt(0);
    prvHeapInitExt(1);
}

/*-----------------------------------------------------------*/
    
static void prvInsertBlockIntoFreeList( BlockLink_t *pxBlockToInsert, int idx)
{
BlockLink_t *pxIterator;
uint8_t *puc;

	/* Iterate through the list until a block is found that has a higher address
	than the block being inserted. */
	for( pxIterator = &xHeapInfo[idx].xStart; pxIterator->pxNextFreeBlock < pxBlockToInsert; pxIterator = pxIterator->pxNextFreeBlock )
	{
		/* Nothing to do here, just iterate to the right position. */
	}

	/* Do the block being inserted, and the block it is being inserted after
	make a contiguous block of memory? */
	puc = ( uint8_t * ) pxIterator;
	if( ( puc + pxIterator->xBlockSize ) == ( uint8_t * ) pxBlockToInsert )
	{
		pxIterator->xBlockSize += pxBlockToInsert->xBlockSize;
		pxBlockToInsert = pxIterator;
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}

	/* Do the block being inserted, and the block it is being inserted before
	make a contiguous block of memory? */
	puc = ( uint8_t * ) pxBlockToInsert;
	if( ( puc + pxBlockToInsert->xBlockSize ) == ( uint8_t * ) pxIterator->pxNextFreeBlock )
	{
		if( pxIterator->pxNextFreeBlock != xHeapInfo[idx].pxEnd )
		{
			/* Form one big block from the two blocks. */
			pxBlockToInsert->xBlockSize += pxIterator->pxNextFreeBlock->xBlockSize;
			pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock->pxNextFreeBlock;
		}
		else
		{
			pxBlockToInsert->pxNextFreeBlock = xHeapInfo[idx].pxEnd;
		}
	}
	else
	{
		pxBlockToInsert->pxNextFreeBlock = pxIterator->pxNextFreeBlock;
	}

	/* If the block being inserted plugged a gab, so was merged with the block
	before and the block after, then it's pxNextFreeBlock pointer will have
	already been set, and should not be set here as that would make it point
	to itself. */
	if( pxIterator != pxBlockToInsert )
	{
		pxIterator->pxNextFreeBlock = pxBlockToInsert;
	}
	else
	{
		mtCOVERAGE_TEST_MARKER();
	}
}

#if configSUPPORT_REALLOC==1
void* pvPortReAlloc( void *pv,  size_t xWantedSize )
{
	BlockLink_t *pxLink;

	unsigned char *puc = ( unsigned char * ) pv;
	
	int idx = xPortGetHeapIndex(pv);
	configASSERT( idx >= 0 );
	
	if( pv )
	{
		if( !xWantedSize )
		{
			vPortFree( pv );
			return NULL;
		}

		void *newArea = pvPortMallocExt( xWantedSize, idx );
		if( newArea )
		{
			/* The memory being freed will have an xBlockLink structure immediately
				before it. */
			puc -= xHeapStructSize;

			/* This casting is to keep the compiler from issuing warnings. */
			pxLink = ( void * ) puc;

			int oldSize =  (pxLink->xBlockSize & ~xHeapInfo[idx].xBlockAllocatedBit) - xHeapStructSize;
			int copySize = ( oldSize < xWantedSize ) ? oldSize : xWantedSize;
			memcpy( newArea, pv, copySize );

			vPortFree( pv );
			return newArea;
		}
	}
	
	return NULL;
}

#elif configSUPPORT_REALLOC==2

static BlockLink_t* pvPortGetPrevBlock( void *pv, int idx )
{
	BlockLink_t *pxIterator = NULL;

	/* Iterate through the list until a block is found that has a higher address
	than the block being inserted. */
	for( pxIterator = &xHeapInfo[idx].xStart; pxIterator->pxNextFreeBlock < pv; pxIterator = pxIterator->pxNextFreeBlock )
	{
		/* Nothing to do here, just iterate to the right position. */
	}	
	
	return pxIterator;
}

void* pvPortReAlloc( void *pv,  size_t xWantedSize )
{
	BlockLink_t *pxCurrBlock;

	unsigned char *puc = ( unsigned char * ) pv;
	
	int idx = xPortGetHeapIndex(pv);
	configASSERT( idx >= 0 );
	
	if( pv )
	{
		if( !xWantedSize )
		{
			vPortFree( pv );
			return NULL;
		}

		/* The memory being freed will have an xBlockLink structure immediately
			before it. */
		puc -= xHeapStructSize;

		/* This casting is to keep the compiler from issuing warnings. */
		pxCurrBlock = ( void * ) puc;

		int oldSize =  (pxCurrBlock->xBlockSize & ~xHeapInfo[idx].xBlockAllocatedBit) - xHeapStructSize;
		
		
		// check size
		if( oldSize == xWantedSize)
			return pv;
		
		if( oldSize > xWantedSize ){
			//printf("shrink size\n\r");
			
			xWantedSize += xHeapStructSize;
			if( ( xWantedSize & portBYTE_ALIGNMENT_MASK ) != 0x00 )
			{
				/* Byte alignment required. */
				xWantedSize += ( portBYTE_ALIGNMENT - ( xWantedSize & portBYTE_ALIGNMENT_MASK ) );
			}			
			
			/* If the block is larger than required it can be split into
					two. */
			if( ((oldSize + xHeapStructSize) - xWantedSize) > heapMINIMUM_BLOCK_SIZE ){
				BlockLink_t *pxNextBlock;
				vPortLock();
				
				/* get next block */
				pxNextBlock = ( void * ) ( ( ( uint8_t * ) pxCurrBlock ) + oldSize + xHeapStructSize );
				
				/* modify current block */
				pxCurrBlock->xBlockSize = xWantedSize | xHeapInfo[idx].xBlockAllocatedBit;
				
				/* create new free space */
				BlockLink_t *pxNewBlockLink = ( void * ) ( ( ( uint8_t * ) pxCurrBlock ) + xWantedSize );

				/* Calculate the sizes of two blocks split from the
				single block. and combine if next block is free*/
				if( (pxNextBlock->xBlockSize & xHeapInfo[idx].xBlockAllocatedBit) == 0 ){
					BlockLink_t* pxPrevFreeBlock = pvPortGetPrevBlock(pv, idx);
					/* merge next free block */
					pxNewBlockLink->xBlockSize = pxNextBlock->xBlockSize + oldSize + xHeapStructSize - xWantedSize;
					pxNewBlockLink->pxNextFreeBlock = pxNextBlock->pxNextFreeBlock;
					
					/* update previous free block */
					pxPrevFreeBlock->pxNextFreeBlock = pxNewBlockLink;
					
					/* update free byte remaining */
					xHeapInfo[idx].xFreeBytesRemaining += (oldSize + xHeapStructSize - xWantedSize);
				}else{
					pxNewBlockLink->xBlockSize = oldSize + xHeapStructSize - xWantedSize;
					pxCurrBlock->xBlockSize = xWantedSize;
					/* Insert the new block into the list of free blocks. */
					prvInsertBlockIntoFreeList( pxNewBlockLink , idx);
					/* update free byte remaining */
					xHeapInfo[idx].xFreeBytesRemaining += pxNewBlockLink->xBlockSize;
				}
				
				//printf("after shrink, create new free node %x, %x, %x\n\r", pxNewBlockLink, pxNewBlockLink->xBlockSize, pxNewBlockLink->pxNextFreeBlock);
				//printf("after shrink, current node%x , %x, %x\n\r", pxCurrBlock, pxCurrBlock->xBlockSize, pxCurrBlock->pxNextFreeBlock);
				
				vPortUnlock();
			}
			return pv;
		}else{
			//printf("enlarge size\n\r");
			/* find previous free block */
			vPortLock();
			BlockLink_t* pxPrevFreeBlock = pvPortGetPrevBlock(pv, idx);
			/* find next free block */
			//BlockLink_t* pxNextFreeBlock = pxPrevFreeBlock->pxNextFreeBlock;
			/* find next block */
			BlockLink_t* pxNextBlock =  ( void * ) ( ( ( uint8_t * ) pxCurrBlock ) + oldSize + xHeapStructSize );

			/* next block is free block */
			if( (pxNextBlock->xBlockSize & xHeapInfo[idx].xBlockAllocatedBit) == 0 ){
				int blockFreeSize = pxNextBlock->xBlockSize;
				if( (xWantedSize - oldSize) < blockFreeSize ){
					
					BlockLink_t* pxNextFreeBlock = pxPrevFreeBlock->pxNextFreeBlock;
					//printf("before enlarge, prev free node %x, %x, %x\n\r", pxPrevFreeBlock, pxPrevFreeBlock->xBlockSize, pxPrevFreeBlock->pxNextFreeBlock);
					//printf("before enlarge, next free node %x, %x, %x\n\r", pxNextFreeBlock, pxNextFreeBlock->xBlockSize, pxNextFreeBlock->pxNextFreeBlock);
					//printf("before enlarge, current node %x, %x, %x\n\r", pxCurrBlock, pxCurrBlock->xBlockSize, pxCurrBlock->pxNextFreeBlock);
					
					/* modify current size */
					xWantedSize += xHeapStructSize;
					if( ( xWantedSize & portBYTE_ALIGNMENT_MASK ) != 0x00 )
					{
						/* Byte alignment required. */
						xWantedSize += ( portBYTE_ALIGNMENT - ( xWantedSize & portBYTE_ALIGNMENT_MASK ) );
					}
					size_t oldBlockSize = pxCurrBlock->xBlockSize & (~xHeapInfo[idx].xBlockAllocatedBit);
					pxCurrBlock->xBlockSize = xWantedSize | xHeapInfo[idx].xBlockAllocatedBit;
					
					/* create new next block structure */
					BlockLink_t* pxNewNextBlock =  ( void * ) ( ( ( uint8_t * ) pxCurrBlock ) + xWantedSize );
					pxNewNextBlock->pxNextFreeBlock = pxNextBlock->pxNextFreeBlock;
					pxNewNextBlock->xBlockSize = pxNextBlock->xBlockSize - (xWantedSize - oldBlockSize);
					
					/* connect to previous block */
					pxPrevFreeBlock->pxNextFreeBlock = pxNewNextBlock;
					
					//printf("after enlarge, prev free node %x, %x, %x\n\r", pxPrevFreeBlock, pxPrevFreeBlock->xBlockSize, pxPrevFreeBlock->pxNextFreeBlock);
					//printf("after enlarge, next free node %x , %x, %x\n\r", pxNewNextBlock, pxNewNextBlock->xBlockSize, pxNewNextBlock->pxNextFreeBlock);
					//printf("after enlarge, current node %x, %x, %x\n\r", pxCurrBlock, pxCurrBlock->xBlockSize, pxCurrBlock->pxNextFreeBlock);
					
					/* update free byte remaining */
					xHeapInfo[idx].xFreeBytesRemaining -= (xWantedSize - oldBlockSize);
					
					if( xHeapInfo[idx].xFreeBytesRemaining < xHeapInfo[idx].xMinimumEverFreeBytesRemaining )
					{
						xHeapInfo[idx].xMinimumEverFreeBytesRemaining = xHeapInfo[idx].xFreeBytesRemaining;
					}
					
					vPortUnlock();
					return pv;
				}
			}
			vPortUnlock();
			
			void *newArea = pvPortMallocExt( xWantedSize, idx );
			if( newArea )
			{
				int copySize = ( oldSize < xWantedSize ) ? oldSize : xWantedSize;
				memcpy( newArea, pv, copySize );

				vPortFree( pv );
				return newArea;
			}
		}
	}
	
	return NULL;
}
#else
	#error NOT SUPPORT configSUPPORT_REALLOC > 2
#endif


