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
--  Description : Basic type definitions.
--
------------------------------------------------------------------------------*/

#ifndef BASETYPE_H_INCLUDED
#define BASETYPE_H_INCLUDED

#include <stdint.h>
#include "basic_types.h"
#define VOLATILE    volatile

#ifdef __linux__    /* typedefs for Linux */

#include <stddef.h> /* for size_t, NULL, etc. */

typedef unsigned char u8;
typedef signed char i8;
typedef unsigned short u16;
typedef signed short i16;
typedef unsigned int u32;
typedef signed int i32;
typedef unsigned long long u64;
typedef int64_t   i64;

#ifndef __cplusplus
typedef enum {
        false   = 0,
        true    = 1
} bool;
#endif

#else /* __symbian__ or __win__ or whatever, customize it to suit well */

//#ifndef _SIZE_T_DEFINED
//typedef unsigned int size_t;
//#define _SIZE_T_DEFINED
//#endif

#ifndef NULL
#ifdef  __cplusplus
#define NULL    0
#else /*  */
#define NULL    ((void *)0)
#endif /*  */
#endif

#ifndef u8
typedef unsigned char u8;
#endif
#ifndef i8
typedef signed char i8;
#endif
#ifndef u16
typedef unsigned short u16;
#endif
#ifndef i16
typedef signed short i16;
#endif
#ifndef u32
typedef unsigned int u32;
#endif
#ifndef i32
typedef signed int i32;
#endif
#ifndef u64
typedef unsigned long long u64;
#endif
#ifndef i64
typedef int64_t   i64;
#endif

#ifndef bool
//typedef unsigned char bool;
#define true  (1)
#define false (0)
#endif

#endif

#if defined(VC1SWDEC_16BIT) || defined(MP4ENC_ARM11)
typedef unsigned short u16x;
typedef signed short i16x;
#else
typedef unsigned int u16x;
typedef signed int i16x;
#endif

#endif /* BASETYPE_H_INCLUDED */
