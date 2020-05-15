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
--  Abstract  :   Encoder instance
--
------------------------------------------------------------------------------*/

#ifndef __H264_INSTANCE_H__
#define __H264_INSTANCE_H__

/*------------------------------------------------------------------------------
    1. Include headers
------------------------------------------------------------------------------*/
#include "enccommon.h"
#include "encpreprocess.h"
#include "encasiccontroller.h"

#include "h264encapi.h"     /* Callback type from API is reused */

#include "H264NalUnit.h"
#include "H264SequenceParameterSet.h"
#include "H264PictureParameterSet.h"
#include "H264PictureBuffer.h"
#include "H264Slice.h"
#include "H264RateControl.h"
#include "H264Mad.h"

#ifdef VIDEOSTAB_ENABLED
#include "vidstabcommon.h"
#endif

/*------------------------------------------------------------------------------
    2. External compiler flags
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
    3. Module defines
------------------------------------------------------------------------------*/
enum H264EncStatus
{
    H264ENCSTAT_INIT = 0xA1,
    H264ENCSTAT_START_STREAM,
    H264ENCSTAT_START_FRAME,
    H264ENCSTAT_ERROR
};

typedef struct
{
    u32 encStatus;
    u32 mbPerFrame;
    u32 mbPerRow;
    u32 mbPerCol;
    u32 interlaced;
    u32 frameCnt;
    u32 fillerNalSize;
    u32 testId;
    u32 numViews;
    u32 numRefBuffsLum;
    u32 numRefBuffsChr;
    u32 intraSliceMap[3];
    u32 idrHdr;
    stream_s stream;
    preProcess_s preProcess;
    sps_s seqParameterSet;
    pps_s picParameterSet;
    slice_s slice;
    mvc_s mvc;
    h264RateControl_s rateControl;
    madTable_s mad;
    asicData_s asic;
    i32 naluOffset;         /* Start offset for NAL unit size table */
    i32 numNalus;           /* Number of NAL units created */
    H264EncSliceReadyCallBackFunc sliceReadyCbFunc;
    u32 *pOutBuf;           /* User given stream output buffer */
    void *pAppData;         /* User given application specific data */
    const void *inst;
    picBuffer picBuffer;
#ifdef VIDEOSTAB_ENABLED
    HWStabData vsHwData;
    SwStbData vsSwData;
#endif
    i32 gdrEnabled;
    i32 gdrStart;
    i32 gdrDuration;
    i32 gdrCount;
    i32 gdrAverageMBRows;
    i32 gdrMBLeft;
    i32 gdrFirstIntraFrame;
    inputLineBuf_s inputLineBuf;
    i32 rfcBufOverflow;
} h264Instance_s;

#endif
