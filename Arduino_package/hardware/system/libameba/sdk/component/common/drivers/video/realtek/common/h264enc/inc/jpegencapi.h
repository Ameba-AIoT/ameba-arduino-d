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
--  Abstract : Hantro H1 JPEG Encoder API
--
------------------------------------------------------------------------------*/

/*------------------------------------------------------------------------------

    Table of contents

    1. Include headers
    2. Module defines
    3. Data types
    4. Function prototypes

------------------------------------------------------------------------------*/

#ifndef __JPEGENCAPI_H__
#define __JPEGENCAPI_H__

#ifdef __cplusplus
extern "C"
{
#endif

/*------------------------------------------------------------------------------
    1. Include headers
------------------------------------------------------------------------------*/

#include "basetype.h"
#include "enccommon.h"

/*------------------------------------------------------------------------------
    2. Module defines
------------------------------------------------------------------------------*/

    typedef const void *JpegEncInst;

    typedef enum
    {
        JPEGENC_FRAME_READY = 1,
        JPEGENC_RESTART_INTERVAL = 2,
        JPEGENC_OK = 0,
        JPEGENC_ERROR = -1,
        JPEGENC_NULL_ARGUMENT = -2,
        JPEGENC_INVALID_ARGUMENT = -3,
        JPEGENC_MEMORY_ERROR = -4,
        JPEGENC_INVALID_STATUS = -5,
        JPEGENC_OUTPUT_BUFFER_OVERFLOW = -6,
        JPEGENC_EWL_ERROR = -7,
        JPEGENC_EWL_MEMORY_ERROR = -8,
        JPEGENC_HW_BUS_ERROR = -9,
        JPEGENC_HW_DATA_ERROR = -10,
        JPEGENC_HW_TIMEOUT = -11,
        JPEGENC_HW_RESERVED = -12,
        JPEGENC_SYSTEM_ERROR = -13,
        JPEGENC_INSTANCE_ERROR = -14,
        JPEGENC_HW_RESET = -15
    } JpegEncRet;

/* Picture YUV type for initialization */
    typedef enum
    {
        JPEGENC_YUV420_PLANAR = 0,              /* YYYY... UUUU... VVVV...  */
        JPEGENC_YUV420_SEMIPLANAR,              /* YYYY... UVUVUV...        */
        JPEGENC_YUV420_SEMIPLANAR_VU,           /* YYYY... VUVUVU...        */
        JPEGENC_YUV422_INTERLEAVED_YUYV,        /* YUYVYUYV...              */
        JPEGENC_YUV422_INTERLEAVED_UYVY,        /* UYVYUYVY...              */
        JPEGENC_RGB565,                         /* 16-bit RGB 16bpp         */
        JPEGENC_BGR565,                         /* 16-bit RGB 16bpp         */
        JPEGENC_RGB555,                         /* 15-bit RGB 16bpp         */
        JPEGENC_BGR555,                         /* 15-bit RGB 16bpp         */
        JPEGENC_RGB444,                         /* 12-bit RGB 16bpp         */
        JPEGENC_BGR444,                         /* 12-bit RGB 16bpp         */
        JPEGENC_RGB888,                         /* 24-bit RGB 32bpp         */
        JPEGENC_BGR888,                         /* 24-bit RGB 32bpp         */
        JPEGENC_RGB101010,                      /* 30-bit RGB 32bpp         */
        JPEGENC_BGR101010                       /* 30-bit RGB 32bpp         */
    } JpegEncFrameType;

/* Picture rotation for initialization */
    typedef enum
    {
        JPEGENC_ROTATE_0 = 0,
        JPEGENC_ROTATE_90R = 1, /* Rotate 90 degrees clockwise */
        JPEGENC_ROTATE_90L = 2  /* Rotate 90 degrees counter-clockwise */
    } JpegEncPictureRotation;

/* Picture color space conversion for RGB input */
    typedef enum
    {
        JPEGENC_RGBTOYUV_BT601 = 0, /* Color conversion according to BT.601 */
        JPEGENC_RGBTOYUV_BT709 = 1, /* Color conversion according to BT.709 */
        JPEGENC_RGBTOYUV_USER_DEFINED = 2   /* User defined color conversion */
    } JpegEncColorConversionType;

    typedef enum
    {
        JPEGENC_WHOLE_FRAME = 0,    /* The whole frame is stored in linear memory */
        JPEGENC_SLICED_FRAME    /* The frame is sliced into restart intervals;
                                 * Input address is given for each slice */
    } JpegEncCodingType;

    typedef enum
    {
        JPEGENC_420_MODE = 0,   /* Encoding in YUV 4:2:0 mode */
        JPEGENC_422_MODE    /* Encoding in YUV 4:2:2 mode */
    } JpegEncCodingMode;

    typedef enum
    {
        JPEGENC_NO_UNITS = 0,   /* No units,
                                 * X and Y specify the pixel aspect ratio */
        JPEGENC_DOTS_PER_INCH = 1,  /* X and Y are dots per inch */
        JPEGENC_DOTS_PER_CM = 2 /* X and Y are dots per cm   */
    } JpegEncAppUnitsType;

    typedef enum
    {
        JPEGENC_SINGLE_MARKER = 0,  /* Luma/Chroma tables are written behind
                                     * one marker */
        JPEGENC_MULTI_MARKER    /* Luma/Chroma tables are written behind
                                 * one marker/component */
    } JpegEncTableMarkerType;

    typedef enum
    {
        JPEGENC_THUMB_JPEG = 0x10,  /* Thumbnail coded using JPEG  */
        JPEGENC_THUMB_PALETTE_RGB8 = 0x11,  /* Thumbnail stored using 1 byte/pixel */
        JPEGENC_THUMB_RGB24 = 0x13  /* Thumbnail stored using 3 bytes/pixel */
    } JpegEncThumbFormat;

/*------------------------------------------------------------------------------
    3. Data types
------------------------------------------------------------------------------*/
/* Version information */
    typedef struct
    {
        u32 major;           /* Encoder API major version                    */
        u32 minor;           /* Encoder API minor version                    */
    } JpegEncApiVersion;
    typedef struct
    {
        u32 swBuild;         /* Software build ID */
        u32 hwBuild;         /* Hardware build ID */
    } JpegEncBuild;

/* thumbnail info */
    typedef struct
    {
        JpegEncThumbFormat format;  /* Format of the thumbnail */
        u8 width;            /* Width in pixels of thumbnail */
        u8 height;           /* Height in pixels of thumbnail */
        const void *data;    /* Thumbnail data */
        u16 dataLength;      /* Data amount in bytes */
    } JpegEncThumb;

/* RGB input to YUV color conversion */
    typedef struct
    {
        JpegEncColorConversionType type;
        u16 coeffA;          /* User defined color conversion coefficient  */
        u16 coeffB;          /* User defined color conversion coefficient  */
        u16 coeffC;          /* User defined color conversion coefficient  */
        u16 coeffE;          /* User defined color conversion coefficient  */
        u16 coeffF;          /* User defined color conversion coefficient  */
    } JpegEncColorConversion;

/* Encoder configuration */
    typedef struct
    {
        u32 inputWidth;      /* Number of pixels/line in input image         */
        u32 inputHeight;     /* Number of lines in input image               */
        u32 xOffset;         /* Pixels from top-left corner of input image   */
        u32 yOffset;         /*   to top-left corner of encoded image        */
        u32 codingWidth;     /* Width of encoded image                       */
        u32 codingHeight;    /* Height of encoded image                      */
        u32 restartInterval; /* Restart interval (MCU lines)                 */
        u32 qLevel;          /* Quantization level (0 - 9)                   */
        const u8 *qTableLuma;   /* Quantization table for luminance [64],
                                 * overrides quantization level, zigzag order   */
        const u8 *qTableChroma; /* Quantization table for chrominance [64],
                                 * overrides quantization level, zigzag order   */
        JpegEncFrameType frameType; /* Input frame YUV / RGB format     */
        JpegEncColorConversion colorConversion; /* RGB to YUV conversion    */
        JpegEncPictureRotation rotation;    /* rotation off/-90/+90             */
        JpegEncCodingType codingType;   /* Whole frame / restart interval   */
        JpegEncCodingMode codingMode;   /* 4:2:0 / 4:2:2 coding             */
        JpegEncAppUnitsType unitsType;  /* Units for X & Y density in APP0  */
        JpegEncTableMarkerType markerType;  /* Table marker type            */
        u32 xDensity;        /* Horizontal pixel density                     */
        u32 yDensity;        /* Vertical pixel density                       */
        u32 comLength;       /* Length of COM header                         */
        const u8 *pCom;      /* Comment header pointer                       */
        u32 inputLineBufEn;            /* enable input image control signals */
        u32 inputLineBufLoopBackEn;    /* input buffer loopback mode enable */
        u32 inputLineBufDepth;         /* input buffer depth in mb lines */
        u32 inputLineBufHwModeEn;      /* hw handshake*/
    } JpegEncCfg;

/* Input info */
    typedef struct
    {
        u32 frameHeader;     /* Enable/disable creation of frame headers     */
        u32 busLum;          /* Bus address of luminance input   (Y)         */
        u32 busCb;           /* Bus address of chrominance input (Cb)        */
        u32 busCr;           /* Bus address of chrominance input (Cr)        */
        const u8 *pLum;      /* Pointer to luminance input   (Y)             */
        const u8 *pCb;       /* Pointer to chrominance input (Cb)            */
        const u8 *pCr;       /* Pointer to chrominance input (Cr)            */
        u8 *pOutBuf;         /* Pointer to output buffer                     */
        u32 busOutBuf;       /* Bus address of output stream buffer          */
        u32 outBufSize;      /* Size of output buffer (bytes)                */
        u32 lineBufWrCnt;    /* The number of MB lines already in input MB line buffer */
    } JpegEncIn;

/* Output info */
    typedef struct
    {
        u32 jfifSize;        /* Encoded JFIF size (bytes)                    */
    } JpegEncOut;

/*------------------------------------------------------------------------------
    4. Function prototypes
------------------------------------------------------------------------------*/

/* Version information */
    JpegEncApiVersion JpegEncGetApiVersion(void);

/* Build information */
    JpegEncBuild JpegEncGetBuild(void);

/* Helper for input format bit-depths */
    u32 JpegEncGetBitsPerPixel(JpegEncFrameType type);

/* Initialization & release */
    JpegEncRet JpegEncInit(const JpegEncCfg * pEncCfg, JpegEncInst * instAddr);
    JpegEncRet JpegEncRelease(JpegEncInst inst);

/* Encoding configuration */
    JpegEncRet JpegEncSetPictureSize(JpegEncInst inst,
                                     const JpegEncCfg * pEncCfg);

    JpegEncRet JpegEncSetThumbnail(JpegEncInst inst,
                                   const JpegEncThumb * JpegThumb);

/* Jfif generation */
    JpegEncRet JpegEncEncode(JpegEncInst inst,
                                   const JpegEncIn * pEncIn,
                                   JpegEncOut * pEncOut,
                                   EncInputMBLineBufCallBackFunc lineBufCbFunc,
                                   void * pAppData);

    /* Set valid input MB lines for encoder to work */
    u32 JpegEncGetEncodedMbLines(JpegEncInst inst);

    /* Get encoded lines information from encoder */
    JpegEncRet JpegEncSetInputMBLines(JpegEncInst inst, u32 lines);

/*------------------------------------------------------------------------------
    5. Encoder API tracing callback function
------------------------------------------------------------------------------*/

    void JpegEnc_Trace(const char *msg);

#ifdef __cplusplus
}
#endif

#endif
