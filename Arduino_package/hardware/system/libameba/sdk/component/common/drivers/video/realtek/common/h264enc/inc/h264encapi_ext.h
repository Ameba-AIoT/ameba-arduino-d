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
--  Abstract : Hantro 6250 H.264 Encoder Extended API
--
------------------------------------------------------------------------------*/

#ifndef __H264ENCAPI_EXT_H__
#define __H264ENCAPI_EXT_H__

#include "basetype.h"
#include "h264encapi.h"

#ifdef __cplusplus
extern "C"
{
#endif

    typedef struct
    {
        u32 disableDeblocking;
        i32 filterOffsetA;
        i32 filterOffsetB;
    } H264EncFilter;

    H264EncRet H264EncGetFilter(H264EncInst inst, H264EncFilter * pEncCfg);
    H264EncRet H264EncSetFilter(H264EncInst inst,
                                const H264EncFilter * pEncCfg);

    H264EncRet H264EncSetFilter(H264EncInst inst,
                                const H264EncFilter * pEncCfg);

    H264EncRet H264EncSetChromaQpIndexOffset(H264EncInst inst, i32 offset);

    H264EncRet H264EncSetHwBurstSize(H264EncInst inst, u32 burst);

    H264EncRet H264EncSetHwBurstType(H264EncInst inst, u32 burstType);

    H264EncRet H264EncTestInputLineBuf(H264EncInst inst);

    H264EncRet H264EncTestCropping(H264EncInst inst);

#ifdef __cplusplus
}
#endif

#endif /*__H264ENCAPI_EXT_H__*/
