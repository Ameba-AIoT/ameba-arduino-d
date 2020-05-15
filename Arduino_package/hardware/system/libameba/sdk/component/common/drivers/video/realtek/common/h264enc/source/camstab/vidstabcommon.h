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
-
-  Description : ...
-
------------------------------------------------------------------------------*/
#ifndef __VIDSTABCOMMON_H__
#define __VIDSTABCOMMON_H__

#include "basetype.h"

#ifdef H8290_HAVE_VIDSTABDEBUG_H
#include "vidstabdebug.h"
#else
#ifndef ASSERT
#define ASSERT(expr)
#endif
#ifndef DEBUG_PRINT
#define DEBUG_PRINT(args)
#endif
#endif

typedef struct HWStabData_
{
    u32 rMotionSum;
    u32 rMotionMin;
    i32 rGmvX;
    i32 rGmvY;
    u32 rMatrixVal[9];
} HWStabData;

/* Stabilizer parameter and data structure */
typedef struct SwStbData_
{
    /* User defined parameters */
    i32 inputWidth; /* Input picture dimensions in pixels */
    i32 inputHeight;
    i32 stride; /* Input picture scanline length in bytes */
    i32 stabilizedWidth;    /* Stabilized picture dimensions in pixels, */
    i32 stabilizedHeight;   /*   multiple of 4, [8, inputHeight-8] */

    /* Stabilized picture position from top-left corner of input picture,
     * algorithm input/output, initialize to (inputWidth-stabilizedWidth)/2 */
    i32 stabOffsetX;    /* [0, inputWidth-stabilizedWidth] */
    i32 stabOffsetY;    /* [0, inputHeight-stabilizedHeight] */

    /* Algorithm internal data, MUST INITIALIZE TO ZERO */
    i32 qpMotionX;
    i32 qpMotionY;
    i32 filterPosX;
    i32 filterPosY;
    i32 filterX;
    i32 filterY;
    i32 filterLengthX;
    i32 filterLengthY;
    i32 filterSumMotX;
    i32 filterSumMotY;
    i32 filterSumStabX;
    i32 filterSumStabY;
    i32 filterErrorX;
    i32 filterErrorY;
    i32 filterErrorXp;
    i32 filterErrorYp;
    const u32 *motion;
    i32 half[5][5];
    i32 quarter[5][5];
    u32 prevMin;
    u32 prevMean;
    u32 sceneChange;
} SwStbData;

void VSAlgInit(SwStbData * data, u32 srcWidth, u32 srcHeight, u32 width,
               u32 height);
void VSAlgReset(SwStbData * data);
u32 VSAlgStabilize(SwStbData * data, const HWStabData * hwStabData);
void VSAlgGetResult(const SwStbData * data, u32 * xOff, u32 * yOff);
void VSReadStabData(const u32 * regMirror, HWStabData * hwStabData);

#endif /* __VIDSTABCOMMON_H__ */
