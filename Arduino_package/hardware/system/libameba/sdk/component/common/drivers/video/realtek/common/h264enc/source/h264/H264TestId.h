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
--  Description :  Encoder setup according to a test vector
--
------------------------------------------------------------------------------*/

#ifndef __H264_TESTID_H__
#define __H264_TESTID_H__

/*------------------------------------------------------------------------------
    1. Include headers
------------------------------------------------------------------------------*/
#include "basetype.h"
#include "H264Instance.h"
#include "H264Slice.h"
#include "H264RateControl.h"

/*------------------------------------------------------------------------------
    2. External compiler flags
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
    3. Module defines
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    4. Function prototypes
------------------------------------------------------------------------------*/

void H264ConfigureTestBeforeFrame(h264Instance_s * inst);
void H264ConfigureTestPenalties(h264Instance_s * inst);
void H264InputLineBufDepthTest(h264Instance_s *inst);
void H264CroppingTest(h264Instance_s *inst);

#endif
