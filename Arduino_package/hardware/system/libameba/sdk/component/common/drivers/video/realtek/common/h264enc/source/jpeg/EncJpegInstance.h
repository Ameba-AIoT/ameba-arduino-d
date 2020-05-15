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

#ifndef __ENC_JPEG_INSTANCE_H__
#define __ENC_JPEG_INSTANCE_H__

/*------------------------------------------------------------------------------
    1. Include headers
------------------------------------------------------------------------------*/
#include "enccommon.h"
#include "encpreprocess.h"
#include "encasiccontroller.h"

#include "EncJpeg.h"

/*------------------------------------------------------------------------------
    2. External compiler flags
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
    3. Module defines
------------------------------------------------------------------------------*/
enum JpegEncStatus
{
    ENCSTAT_INIT = 0xA1,
    ENCSTAT_ENCODE,
    ENCSTAT_ERROR
};

typedef struct
{
    u32 encStatus;
    stream_s stream;
    preProcess_s preProcess;
    jpegData_s jpeg;
    asicData_s asic;
    inputLineBuf_s inputLineBuf;
    const void *inst;     /* used as checksum */
    void *pAppData;     /* Pointer to application data. */
} jpegInstance_s;

#endif
