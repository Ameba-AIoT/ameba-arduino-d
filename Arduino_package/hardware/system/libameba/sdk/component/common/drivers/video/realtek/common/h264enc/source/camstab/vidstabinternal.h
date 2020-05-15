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
-  Description : Standalone stabilization internal stuff
-
------------------------------------------------------------------------------*/
#ifndef __VIDSTBINTERNAL_H__
#define __VIDSTBINTERNAL_H__

#include "basetype.h"
#include "vidstabcommon.h"
#include "vidstbapi.h"
#include "ewl.h"

/* HW status register bits */
#define ASIC_STATUS_SLICE_READY         0x100
#define ASIC_STATUS_HW_TIMEOUT          0x040
#define ASIC_STATUS_BUFF_FULL           0x020
#define ASIC_STATUS_HW_RESET            0x010
#define ASIC_STATUS_ERROR               0x008
#define ASIC_STATUS_FRAME_READY         0x004

#define ASIC_STATUS_ALL                 0x1FD

#define ASIC_IRQ_LINE                   0x001
#define ASIC_STATUS_ENABLE              0x001

#define ASIC_VS_MODE_OFF                0x00
#define ASIC_VS_MODE_ALONE              0x01
#define ASIC_VS_MODE_ENCODER            0x02

#define ASIC_INPUT_YUV420PLANAR         0x00
#define ASIC_INPUT_YUV420SEMIPLANAR     0x01
#define ASIC_INPUT_YUYV422INTERLEAVED   0x02
#define ASIC_INPUT_UYVY422INTERLEAVED   0x03
#define ASIC_INPUT_RGB565               0x04
#define ASIC_INPUT_RGB555               0x05
#define ASIC_INPUT_RGB444               0x06
#define ASIC_INPUT_RGB888               0x07
#define ASIC_INPUT_RGB101010            0x08

typedef struct RegValues_
{
    u32 irqDisable;
    u32 mbsInCol;
    u32 mbsInRow;
    u32 pixelsOnRow;
    u32 xFill;
    u32 yFill;
    u32 inputImageFormat;
    u32 inputLumBase;
    u32 inputLumaBaseOffset;

    u32 rwNextLumaBase;
    u32 rwStabMode;

    u32 rMaskMsb;
    u32 gMaskMsb;
    u32 bMaskMsb;
    u32 colorConversionCoeffA;
    u32 colorConversionCoeffB;
    u32 colorConversionCoeffC;
    u32 colorConversionCoeffE;
    u32 colorConversionCoeffF;

    HWStabData hwStabData;

    u32 asicCfgReg;

#ifdef ASIC_WAVE_TRACE_TRIGGER
    u32 vop_count;
#endif
} RegValues;

typedef u32 SwStbMotionType;

typedef struct VideoStb_
{
    const void *ewl;
    u32 regMirror[64];
    const void *checksum;
    SwStbData data;
    RegValues regval;
    u32 stride;
    VideoStbInputFormat yuvFormat;
} VideoStb;

void VSSetCropping(VideoStb * pVidStab, u32 currentPictBus, u32 nextPictBus);

void VSInitAsicCtrl(VideoStb * pVidStab);
i32 VSCheckInput(const VideoStbParam * param);
void VSSetupAsicAll(VideoStb * pVidStab);
i32 VSWaitAsicReady(VideoStb * pVidStab);

#endif /* __VIDSTBINTERNAL_H__ */
