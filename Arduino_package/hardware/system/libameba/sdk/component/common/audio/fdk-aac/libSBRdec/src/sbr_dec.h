
/* -----------------------------------------------------------------------------------------------------------
Software License for The Fraunhofer FDK AAC Codec Library for Android

� Copyright  1995 - 2015 Fraunhofer-Gesellschaft zur F�rderung der angewandten Forschung e.V.
  All rights reserved.

 1.    INTRODUCTION
The Fraunhofer FDK AAC Codec Library for Android ("FDK AAC Codec") is software that implements
the MPEG Advanced Audio Coding ("AAC") encoding and decoding scheme for digital audio.
This FDK AAC Codec software is intended to be used on a wide variety of Android devices.

AAC's HE-AAC and HE-AAC v2 versions are regarded as today's most efficient general perceptual
audio codecs. AAC-ELD is considered the best-performing full-bandwidth communications codec by
independent studies and is widely deployed. AAC has been standardized by ISO and IEC as part
of the MPEG specifications.

Patent licenses for necessary patent claims for the FDK AAC Codec (including those of Fraunhofer)
may be obtained through Via Licensing (www.vialicensing.com) or through the respective patent owners
individually for the purpose of encoding or decoding bit streams in products that are compliant with
the ISO/IEC MPEG audio standards. Please note that most manufacturers of Android devices already license
these patent claims through Via Licensing or directly from the patent owners, and therefore FDK AAC Codec
software may already be covered under those patent licenses when it is used for those licensed purposes only.

Commercially-licensed AAC software libraries, including floating-point versions with enhanced sound quality,
are also available from Fraunhofer. Users are encouraged to check the Fraunhofer website for additional
applications information and documentation.

2.    COPYRIGHT LICENSE

Redistribution and use in source and binary forms, with or without modification, are permitted without
payment of copyright license fees provided that you satisfy the following conditions:

You must retain the complete text of this software license in redistributions of the FDK AAC Codec or
your modifications thereto in source code form.

You must retain the complete text of this software license in the documentation and/or other materials
provided with redistributions of the FDK AAC Codec or your modifications thereto in binary form.
You must make available free of charge copies of the complete source code of the FDK AAC Codec and your
modifications thereto to recipients of copies in binary form.

The name of Fraunhofer may not be used to endorse or promote products derived from this library without
prior written permission.

You may not charge copyright license fees for anyone to use, copy or distribute the FDK AAC Codec
software or your modifications thereto.

Your modified versions of the FDK AAC Codec must carry prominent notices stating that you changed the software
and the date of any change. For modified versions of the FDK AAC Codec, the term
"Fraunhofer FDK AAC Codec Library for Android" must be replaced by the term
"Third-Party Modified Version of the Fraunhofer FDK AAC Codec Library for Android."

3.    NO PATENT LICENSE

NO EXPRESS OR IMPLIED LICENSES TO ANY PATENT CLAIMS, including without limitation the patents of Fraunhofer,
ARE GRANTED BY THIS SOFTWARE LICENSE. Fraunhofer provides no warranty of patent non-infringement with
respect to this software.

You may use this FDK AAC Codec software or modifications thereto only for purposes that are authorized
by appropriate patent licenses.

4.    DISCLAIMER

This FDK AAC Codec software is provided by Fraunhofer on behalf of the copyright holders and contributors
"AS IS" and WITHOUT ANY EXPRESS OR IMPLIED WARRANTIES, including but not limited to the implied warranties
of merchantability and fitness for a particular purpose. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
CONTRIBUTORS BE LIABLE for any direct, indirect, incidental, special, exemplary, or consequential damages,
including but not limited to procurement of substitute goods or services; loss of use, data, or profits,
or business interruption, however caused and on any theory of liability, whether in contract, strict
liability, or tort (including negligence), arising in any way out of the use of this software, even if
advised of the possibility of such damage.

5.    CONTACT INFORMATION

Fraunhofer Institute for Integrated Circuits IIS
Attention: Audio and Multimedia Departments - FDK AAC LL
Am Wolfsmantel 33
91058 Erlangen, Germany

www.iis.fraunhofer.de/amm
amm-info@iis.fraunhofer.de
----------------------------------------------------------------------------------------------------------- */

/*!
  \file
  \brief  Sbr decoder  
*/
#ifndef __SBR_DEC_H
#define __SBR_DEC_H

#include "sbrdecoder.h"

#include "lpp_tran.h"
#include "qmf.h"
#include "env_calc.h"
#include "FDK_audio.h"


#include "sbrdec_drc.h"

#define SACDEC_ALIGNMENT_FIX

typedef struct
{
  QMF_FILTER_BANK     AnalysiscQMF;
  QMF_FILTER_BANK     SynthesisQMF;

  SBR_CALCULATE_ENVELOPE  SbrCalculateEnvelope;
  SBR_LPP_TRANS           LppTrans;

  QMF_SCALE_FACTOR sbrScaleFactor;
  QMF_SCALE_FACTOR sbrScaleFactorRight;

  /*! Delayed spectral data needed for the dynamic framing of SBR. Not required in case of CLDFB */
  FIXP_DBL * pSbrOverlapBuffer;

  /* References to workbuffers */
  FIXP_DBL * WorkBuffer1;
  FIXP_DBL * WorkBuffer2;

  /* Delayed time input signal needed to align CLDFD with LD-MPS QMF. */
  INT_PCM    coreDelayBuf[(96)];

  /* QMF filter states */
  FIXP_QAS   anaQmfStates[(320)];
  FIXP_QSS * pSynQmfStates;

  /* Reference pointer arrays for QMF time slots,
     mixed among overlap and current slots. */
  FIXP_DBL * QmfBufferReal[(((1024)/(32))+(6))];
  FIXP_DBL * QmfBufferImag[(((1024)/(32))+(6))];
  int useLP;

  /* QMF domain extension time slot reference pointer array */

  SBRDEC_DRC_CHANNEL  sbrDrcChannel;

} SBR_DEC;

typedef SBR_DEC *HANDLE_SBR_DEC;


typedef struct
{
  SBR_FRAME_DATA      frameData[(1)+1];
  SBR_PREV_FRAME_DATA prevFrameData;
  SBR_DEC SbrDec;
}
SBR_CHANNEL;

typedef SBR_CHANNEL *HANDLE_SBR_CHANNEL;

void
SbrDecodeAndProcess (HANDLE_SBR_DEC hSbrDec,
                     INT_PCM *timeIn,
                     HANDLE_SBR_HEADER_DATA hHeaderData,
                     HANDLE_SBR_FRAME_DATA hFrameData,
                     HANDLE_SBR_PREV_FRAME_DATA hPrevFrameData,
                     int applyProcessing,
                     int channelNr
                     , UCHAR useLP
                     );


void
SbrConstructTimeOutput (HANDLE_SBR_DEC hSbrDec,            /*!< handle to Decoder channel */
                        INT_PCM *timeOut,                    /*!< pointer to output time signal */
                        HANDLE_SBR_HEADER_DATA hHeaderData,/*!< Static control data */
                        HANDLE_SBR_PREV_FRAME_DATA hPrevFrameData,  /*!< Some control data of last frame */
                        int channelNr
                       ,UCHAR useLP
                        );


void
sbr_dec (HANDLE_SBR_DEC hSbrDec,            /*!< handle to Decoder channel */
         INT_PCM *timeIn,                   /*!< pointer to input time signal */
         INT_PCM *timeOut,                  /*!< pointer to output time signal */
         HANDLE_SBR_DEC hSbrDecRight,       /*!< handle to Decoder channel right */
         INT_PCM *timeOutRight,             /*!< pointer to output time signal */
         const int strideIn,                /*!< Time data traversal strideIn */
         const int strideOut,               /*!< Time data traversal strideOut */
         HANDLE_SBR_HEADER_DATA hHeaderData,/*!< Static control data */
         HANDLE_SBR_FRAME_DATA hFrameData,  /*!< Control data of current frame */
         HANDLE_SBR_PREV_FRAME_DATA hPrevFrameData,  /*!< Some control data of last frame */
         const int applyProcessing,         /*!< Flag for SBR operation */
         HANDLE_PS_DEC h_ps_d,
         const UINT flags,
         const int codecFrameSize
        );



SBR_ERROR
createSbrDec (SBR_CHANNEL * hSbrChannel,
              HANDLE_SBR_HEADER_DATA hHeaderData,
              TRANSPOSER_SETTINGS *pSettings,
              const int downsampleFac,
              const UINT qmfFlags,
              const UINT flags,
              const int overlap,
              int chan);

int
deleteSbrDec (SBR_CHANNEL * hSbrChannel);

SBR_ERROR
resetSbrDec (HANDLE_SBR_DEC hSbrDec,
             HANDLE_SBR_HEADER_DATA hHeaderData,
             HANDLE_SBR_PREV_FRAME_DATA hPrevFrameData,
             const int useLP,
             const int downsampleFac);

#endif
