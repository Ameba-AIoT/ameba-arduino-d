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
--  Description : Rate control structures and function prototypes
--
------------------------------------------------------------------------------*/

#ifndef H264_RATE_CONTROL_H
#define H264_RATE_CONTROL_H

#include "enccommon.h"
#include "H264Sei.h"

enum
{ H264RC_OVERFLOW = -1 };

#define RC_CBR_HRD  0   /* 1 = Constant bit rate model. Must use filler
                         * data to conform */

#define CTRL_LEVELS          7  /* DO NOT CHANGE THIS */
#define CHECK_POINTS_MAX    10  /* DO NOT CHANGE THIS */
#define RC_TABLE_LENGTH     10  /* DO NOT CHANGE THIS */

typedef struct {
    i32  a1;               /* model parameter */
    i32  a2;               /* model parameter */
    i32  qp_prev;          /* previous QP */
    i32  qs[RC_TABLE_LENGTH+1];  /* quantization step size */
    i32  bits[RC_TABLE_LENGTH+1];/* Number of bits needed to code residual */
    i32  pos;              /* current position */
    i32  len;              /* current lenght */
    i32  zero_div;         /* a1 divisor is 0 */
} linReg_s;

typedef struct
{
    i32 wordError[CTRL_LEVELS]; /* Check point error bit */
    i32 qpChange[CTRL_LEVELS];  /* Check point qp difference */
    i32 wordCntTarget[CHECK_POINTS_MAX];    /* Required bit count */
    i32 wordCntPrev[CHECK_POINTS_MAX];  /* Real bit count */
    i32 checkPointDistance;
    i32 checkPoints;
} h264QpCtrl_s;

/* Virtual buffer */
typedef struct
{
    i32 bufferSize;          /* size of the virtual buffer */
    i32 bitRate;             /* input bit rate per second */
    i32 bitPerPic;           /* average number of bits per picture */
    i32 picTimeInc;          /* timeInc since last coded picture */
    i32 timeScale;           /* input frame rate numerator */
    i32 unitsInTic;          /* input frame rate denominator */
    i32 virtualBitCnt;       /* virtual (channel) bit count */
    i32 realBitCnt;          /* real bit count */
    i32 bufferOccupancy;     /* number of bits in the buffer */
    i32 skipFrameTarget;     /* how many frames should be skipped in a row */
    i32 skippedFrames;       /* how many frames have been skipped in a row */
    i32 nonZeroTarget;
    i32 bucketFullness;      /* Leaky Bucket fullness */
    i32 bucketLevel;         /* Leaky Bucket fullness + virtualBitCnt */
    /* new rate control */
    i32 windowRem;
    i32 seconds;             /* Full seconds elapsed */
    i32 averageBitRate;      /* This buffer average bitrate for full seconds */
} h264VirtualBuffer_s;

typedef struct
{
    true_e picRc;
    true_e mbRc;             /* Mb header qp can vary, check point rc */
    true_e picSkip;          /* Frame Skip enable */
    true_e hrd;              /* HRD restrictions followed or not */
    u32 fillerIdx;
    i32 mbPerPic;            /* Number of macroblock per picture */
    i32 mbRows;              /* MB rows in picture */
    i32 coeffCntMax;         /* Number of coeff per picture */
    i32 nonZeroCnt;
    i32 srcPrm;              /* Source parameter */
    i32 qpSum;               /* Qp sum counter */
    u32 sliceTypeCur;
    u32 sliceTypePrev;
    true_e frameCoded;       /* Pic coded information */
    i32 fixedQp;             /* Pic header qp when fixed */
    i32 qpHdr;               /* Pic header qp of current voded picture */
    i32 qpMin;               /* Pic header minimum qp, user set */
    i32 qpMax;               /* Pic header maximum qp, user set */
    i32 qpHdrPrev;           /* Pic header qp of previous coded picture */
    i32 qpLastCoded;         /* Quantization parameter of last coded mb */
    i32 qpTarget;            /* Target quantrization parameter */
    u32 estTimeInc;
    i32 outRateNum;
    i32 outRateDenom;
    i32 gDelaySum;
    i32 gInitialDelay;
    i32 gInitialDoffs;
    h264QpCtrl_s qpCtrl;
    h264VirtualBuffer_s virtualBuffer;
    sei_s sei;
    i32 gBufferMin, gBufferMax;
   /* new rate control */
    linReg_s linReg;       /* Data for R-Q model for inter frames */
    linReg_s rError;       /* Rate prediction error for inter frames(bits) */
    linReg_s intra;        /* Data for intra frames */
    linReg_s intraError;   /* Prediction error for intra frames */
    linReg_s gop;          /* Data for GOP */
    i32 targetPicSize;
    i32 frameBitCnt;
    i32 sumQp;
    i32 sumBitrateError;
    i32 sumFrameError;
   /* for gop rate control */
    i32 gopQpSum;           /* Sum of current GOP inter frame QPs */
    i32 gopQpDiv;
    i32 gopBitCnt;          /* Current GOP bit count so far */
    i32 gopAvgBitCnt;       /* Previous GOP average bit count */
    u32 frameCnt;
    i32 gopLen;
    i32 windowLen;          /* Bitrate window which tries to match target */
    i32 intraInterval;      /* Distance between two previous I-frames */
    i32 intraIntervalCtr;
    i32 intraQpDelta;
    u32 fixedIntraQp;
    i32 mbQpAdjustment[3];     /* QP delta for MAD macroblock QP adjustment */
    i32 longTermPicRate;
    i32 mbQpAutoBoost;
	/* for adaptive QP (can be lower or higher) */
	i32 rcErrorRst;				/* reset rc error when rate change */
	i32 adaptQpEn;				/* enable = 1 */
	i32 qpPrevDesired;			/* store unbounded QP (not limited by qpMax and qpMin) */
	i32 qpInitMax;				/* init value of qpMax */
	i32 qpInitMin;				/* init value of qpMin */
	
} h264RateControl_s;

/*------------------------------------------------------------------------------
    Function prototypes
------------------------------------------------------------------------------*/
bool_e H264InitRc(h264RateControl_s * rc, u32 newStream);
void H264BeforePicRc(h264RateControl_s * rc, u32 timeInc, u32 sliceType);
i32 H264AfterPicRc(h264RateControl_s * rc, u32 nonZeroCnt, u32 byteCnt,
        u32 qpSum);
u32 H264FillerRc(h264RateControl_s * rc, u32 frameCnt);
i32 H264Calculate(i32 a, i32 b, i32 c);

#endif /* H264_RATE_CONTROL_H */

