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
    2. Module defines
    3. Data types
    4. Function prototypes

------------------------------------------------------------------------------*/

#ifndef __ENC_JPEG_MARKERS_H__
#define __ENC_JPEG_MARKERS_H__

/*------------------------------------------------------------------------------
    1. Include headers
------------------------------------------------------------------------------*/
#include "basetype.h"

/*------------------------------------------------------------------------------
    2. Module defines
------------------------------------------------------------------------------*/

/* JPEG markers, table B.1 page 32 */

enum
{
    SOI = 0xFFD8,   /* Start of Image                    */
    DQT = 0xFFDB,   /* Define Quantization Table(s)      */
    SOF0 = 0xFFC0,  /* Start of Frame                    */
    DRI = 0xFFDD,   /* Define Restart Interval           */
    RST0 = 0xFFD0,  /* Restart marker 0                  */
    RST1 = 0xFFD1,  /* Restart marker 1                  */
    RST2 = 0xFFD2,  /* Restart marker 2                  */
    RST3 = 0xFFD3,  /* Restart marker 3                  */
    RST4 = 0xFFD4,  /* Restart marker 4                  */
    RST5 = 0xFFD5,  /* Restart marker 5                  */
    RST6 = 0xFFD6,  /* Restart marker 6                  */
    RST7 = 0xFFD7,  /* Restart marker 7                  */
    DHT = 0xFFC4,   /* Define Huffman Table(s)           */
    SOS = 0xFFDA,   /* Start of Scan                     */
    EOI = 0xFFD9,   /* End of Image                      */
    APP0 = 0xFFE0,  /* APP0 Marker                       */
    COM = 0xFFFE    /* Comment marker                    */
};

/*------------------------------------------------------------------------------
    3. Data types
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------
    4. Function prototypes
------------------------------------------------------------------------------*/

#endif
