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
-  Description : Video Stabilization Standalone API
-
------------------------------------------------------------------------------*/
#ifndef __VIDSTBAPI_H__
#define __VIDSTBAPI_H__

#ifdef __cplusplus
extern "C"
{
#endif

#include "basetype.h"

/* Function return values */
    typedef enum VideoStbRet_
    {
        VIDEOSTB_OK = 0,
        VIDEOSTB_ERROR = -1,
        VIDEOSTB_NULL_ARGUMENT = -2,
        VIDEOSTB_INVALID_ARGUMENT = -3,
        VIDEOSTB_MEMORY_ERROR = -4,
        VIDEOSTB_EWL_ERROR = -5,
        VIDEOSTB_EWL_MEMORY_ERROR = -6,
        VIDEOSTB_HW_BUS_ERROR = -9,
        VIDEOSTB_HW_TIMEOUT = -11,
        VIDEOSTB_HW_RESERVED = -12,
        VIDEOSTB_SYSTEM_ERROR = -13,
        VIDEOSTB_INSTANCE_ERROR = -14,
        VIDEOSTB_HW_RESET = -16
    } VideoStbRet;

/* YUV type for initialization */
    typedef enum VideoStbInputFormat_
    {
        VIDEOSTB_YUV420_PLANAR = 0,             /* YYYY... UUUU... VVVV */
        VIDEOSTB_YUV420_SEMIPLANAR = 1,         /* YYYY... UVUVUV...    */
        VIDEOSTB_YUV422_INTERLEAVED_YUYV = 2,   /* YUYVYUYV...          */
        VIDEOSTB_YUV422_INTERLEAVED_UYVY = 3,   /* UYVYUYVY...          */
        VIDEOSTB_RGB565 = 4,                    /* 16-bit RGB           */
        VIDEOSTB_BGR565 = 5,                    /* 16-bit RGB           */
        VIDEOSTB_RGB555 = 6,                    /* 15-bit RGB           */
        VIDEOSTB_BGR555 = 7,                    /* 15-bit RGB           */
        VIDEOSTB_RGB444 = 8,                    /* 12-bit RGB           */
        VIDEOSTB_BGR444 = 9,                    /* 12-bit RGB           */
        VIDEOSTB_RGB888 = 10,                   /* 24-bit RGB           */
        VIDEOSTB_BGR888 = 11,                   /* 24-bit RGB           */
        VIDEOSTB_RGB101010 = 12,                /* 30-bit RGB           */
        VIDEOSTB_BGR101010 = 13                 /* 30-bit RGB           */
    } VideoStbInputFormat;

    typedef const void *VideoStbInst;

    typedef struct VideoStbParam_
    {
        u32 inputWidth;
        u32 inputHeight;
        u32 stride;
        u32 stabilizedWidth;
        u32 stabilizedHeight;
        VideoStbInputFormat format;
    } VideoStbParam;

    typedef struct VideoStbResult_
    {
        u32 stabOffsetX;
        u32 stabOffsetY;
    } VideoStbResult;

/* Version information */
    typedef struct
    {
        u32 major;           /* API major version */
        u32 minor;           /* API minor version */
    } VideoStbApiVersion;

    typedef struct
    {
        u32 swBuild;         /* Software build ID */
        u32 hwBuild;         /* Hardware build ID */
    } VideoStbBuild;

/*------------------------------------------------------------------------------
    API prototypes
------------------------------------------------------------------------------*/

/* Version information */
    VideoStbApiVersion VideoStbGetApiVersion(void);
    VideoStbBuild VideoStbGetBuild(void);

/* Initialization & release */
    VideoStbRet VideoStbInit(VideoStbInst * instAddr,
                             const VideoStbParam * param);
    VideoStbRet VideoStbReset(VideoStbInst vidStab,
                              const VideoStbParam * param);
    VideoStbRet VideoStbRelease(VideoStbInst vidStab);

/* Stabilize next image based on the current one */
    VideoStbRet VideoStbStabilize(VideoStbInst vidStab, VideoStbResult * result,
                                  u32 referenceFrameLum, u32 stabilizedFameLum);

/* API tracing callback function */
    void VideoStb_Trace(const char *str);

#ifdef __cplusplus
}
#endif

#endif                       /* __VIDSTBAPI_H__ */
