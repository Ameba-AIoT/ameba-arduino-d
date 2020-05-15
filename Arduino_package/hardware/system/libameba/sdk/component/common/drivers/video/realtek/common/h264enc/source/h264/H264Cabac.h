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
-  Description : H264 CABAC
-
------------------------------------------------------------------------------*/

#ifndef __H264CABAC_H__
#define __H264CABAC_H__

#include "basetype.h"

u32 H264CabacInit(u32 *contextTable, u32 cabac_init_idc);

#endif /* __H264CABAC_H__ */
