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
--  Abstract  : 
--
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------

    Table of contents

    1. Include headers
    2. External compiler flags
    3. Module defines
    4. Function prototypes

------------------------------------------------------------------------------*/
#ifndef __ENC_JPEG_CODE_VOP_H__
#define __ENC_JPEG_CODE_VOP_H__

/*------------------------------------------------------------------------------
    1. Include headers
------------------------------------------------------------------------------*/
#include "basetype.h"
#include "EncJpeg.h"
#include "EncJpegInstance.h"

#include "enccommon.h"
#include "encasiccontroller.h"
#include "EncJpegPutBits.h"

/*------------------------------------------------------------------------------
    2. External compiler flags
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
    3. Module defines
------------------------------------------------------------------------------*/

typedef enum
{
    JPEGENCODE_OK = 0,
    JPEGENCODE_TIMEOUT = 1,
    JPEGENCODE_DATA_ERROR = 2,
    JPEGENCODE_HW_ERROR = 3,
    JPEGENCODE_SYSTEM_ERROR = 4,
    JPEGENCODE_HW_RESET = 5
} jpegEncodeFrame_e;

/*------------------------------------------------------------------------------
    4. Function prototypes
------------------------------------------------------------------------------*/

jpegEncodeFrame_e EncJpegCodeFrame(jpegInstance_s * inst);

#endif
