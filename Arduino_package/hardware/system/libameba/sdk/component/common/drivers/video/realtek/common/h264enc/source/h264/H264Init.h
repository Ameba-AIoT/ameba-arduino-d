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
--  Abstract : Encoder initialization and setup
--
------------------------------------------------------------------------------*/

#ifndef __H264_INIT_H__
#define __H264_INIT_H__

/*------------------------------------------------------------------------------
    1. Include headers
------------------------------------------------------------------------------*/
#include "h264encapi.h"
#include "H264Instance.h"

/*------------------------------------------------------------------------------
    2. External compiler flags
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
    3. Module defines
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    4. Function prototypes
------------------------------------------------------------------------------*/

bool_e H264CheckCfg(const H264EncConfig * pEncCfg);
i32 H264GetAllowedWidth(i32 width, H264EncPictureType inputType);

H264EncRet H264Init(const H264EncConfig * pEncCfg, h264Instance_s ** instAddr);
    
void H264Shutdown(h264Instance_s *data);

#endif
