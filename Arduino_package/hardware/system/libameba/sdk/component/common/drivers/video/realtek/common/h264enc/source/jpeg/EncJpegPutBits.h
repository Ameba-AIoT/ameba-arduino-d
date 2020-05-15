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
#ifndef __ENC_JPEG_PUT_BITS_H__
#define __ENC_JPEG_PUT_BITS_H__

/*------------------------------------------------------------------------------
    1. Include headers
------------------------------------------------------------------------------*/
#include "basetype.h"
#include "enccommon.h"

/*------------------------------------------------------------------------------
    2. External compiler flags
--------------------------------------------------------------------------------

--------------------------------------------------------------------------------
    3. Module defines
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    4. Function prototypes
------------------------------------------------------------------------------*/
void EncJpegPutBits(stream_s *, u32, u32);
void EncJpegByteAligned(stream_s * stream);
void EncJpegHeaderPutBits(stream_s *, u32, u32);
void EncJpegNextByteAligned(stream_s * stream);
bool_e EncJpegBufferStatus(stream_s * stream);
bool_e EncJpegSetBuffer(stream_s * buffer, u8 * stream, u32 size);

#endif
