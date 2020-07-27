
/* -----------------------------------------------------------------------------------------------------------
Software License for The Fraunhofer FDK AAC Codec Library for Android

� Copyright  1995 - 2013 Fraunhofer-Gesellschaft zur F�rderung der angewandten Forschung e.V.
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

/*****************************  MPEG-4 AAC Decoder  **************************

   Author(s):   Josef Hoepfl
   Description: long/short-block decoding

******************************************************************************/

#ifndef BLOCK_H
#define BLOCK_H

#include "common_fix.h"

#include "channelinfo.h"
#include "FDK_bitstream.h"

/* PNS (of block) */
void CPns_Read (CPnsData *pPnsData,
                HANDLE_FDK_BITSTREAM bs,
                const CodeBookDescription *hcb,
                SHORT *pScaleFactor,
                UCHAR global_gain,
                int band,
                int group);


void CPns_Apply (const CPnsData *pPnsData,
                 const CIcsInfo *pIcsInfo,
                 SPECTRAL_PTR pSpectrum,
                 const SHORT    *pSpecScale,
                 const SHORT    *pScaleFactor,
                 const SamplingRateInfo *pSamplingRateInfo,
                 const INT granuleLength,
                 const int channel);



/* TNS (of block) */
/*!
  \brief Read tns data-present flag from bitstream

  The function reads the data-present flag for tns from
  the bitstream.

  \return  none
*/
void CTns_ReadDataPresentFlag(HANDLE_FDK_BITSTREAM bs,
                              CTnsData *pTnsData);

void CTns_ReadDataPresentUsac(
        HANDLE_FDK_BITSTREAM hBs,
        CTnsData *pTnsData0,
        CTnsData *pTnsData1,
        const CIcsInfo *pIcsInfo,
        const UINT flags,
        const int fCommonWindow
        );

AAC_DECODER_ERROR CTns_Read(HANDLE_FDK_BITSTREAM bs,
                            CTnsData *pTnsData,
                            const CIcsInfo *pIcsInfo,
                            const UINT flags);

void CTns_Apply ( CTnsData *RESTRICT pTnsData, /*!< pointer to aac decoder info */
                  const CIcsInfo *pIcsInfo,
                  SPECTRAL_PTR pSpectralCoefficient,
                  const SamplingRateInfo *pSamplingRateInfo,
                  const INT granuleLength);

/* Block */

LONG CBlock_GetEscape(HANDLE_FDK_BITSTREAM bs, const LONG q);

/**
 * \brief Read scale factor data. See chapter 4.6.2.3.2 of ISO/IEC 14496-3.
 *        The SF_OFFSET = 100 value referenced in chapter 4.6.2.3.3 is already substracted
 *        from the scale factor values. Also includes PNS data reading.
 * \param bs bit stream handle data source
 * \param pAacDecoderChannelInfo channel context info were decoded data is stored into.
 * \param flags the decoder flags.
 */
AAC_DECODER_ERROR CBlock_ReadScaleFactorData(
        CAacDecoderChannelInfo *pAacDecoderChannelInfo,
        HANDLE_FDK_BITSTREAM bs,
        const UINT flags
        );

/**
 * \brief Read Huffman encoded spectral data.
 * \param pAacDecoderChannelInfo channel context info.
 * \param pSamplingRateInfo sampling rate info (sfb offsets).
 * \param flags syntax flags.
 */
AAC_DECODER_ERROR  CBlock_ReadSpectralData(HANDLE_FDK_BITSTREAM bs,
                                           CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                                           const SamplingRateInfo *pSamplingRateInfo,
                                           const UINT  flags);


AAC_DECODER_ERROR CBlock_ReadSectionData(HANDLE_FDK_BITSTREAM bs,
                                         CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                                         const SamplingRateInfo *pSamplingRateInfo,
                                         const UINT  flags);

/**
 * \brief find a common exponent (shift factor) for all sfb in each Spectral window, and store them into
 *        CAacDecoderChannelInfo::specScale.
 * \param pAacDecoderChannelInfo channel context info.
 * \param pSamplingRateInfo sampling rate info (sfb offsets).
 */
void CBlock_ScaleSpectralData(CAacDecoderChannelInfo *pAacDecoderChannelInfo, SamplingRateInfo *pSamplingRateInfo);

/**
 * \brief Apply TNS and PNS tools.
 */
void ApplyTools ( CAacDecoderChannelInfo *pAacDecoderChannelInfo[],
                  const SamplingRateInfo *pSamplingRateInfo,
                  const UINT flags,
                  const int channel );

/**
 * \brief Transform MDCT spectral data into time domain
 */
void CBlock_FrequencyToTime(CAacDecoderStaticChannelInfo *pAacDecoderStaticChannelInfo,
                            CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                            INT_PCM outSamples[],
                            const SHORT frameLen,
                            const int stride,
                            const int frameOk,
                            FIXP_DBL *pWorkBuffer1);

/**
 * \brief Transform double lapped MDCT (AAC-ELD) spectral data into time domain.
 */
void CBlock_FrequencyToTimeLowDelay(CAacDecoderStaticChannelInfo *pAacDecoderStaticChannelInfo,
                                    CAacDecoderChannelInfo *pAacDecoderChannelInfo,
                                    INT_PCM outSamples[],
                                    const short frameLen,
                                    const char stride);

AAC_DECODER_ERROR CBlock_InverseQuantizeSpectralData(CAacDecoderChannelInfo *pAacDecoderChannelInfo, SamplingRateInfo *pSamplingRateInfo);

/**
 * \brief Calculate 2^(lsb/4) * value^(4/3)
 * \param pValue pointer to quantized value. The inverse quantized result is stored back here.
 * \param lsb 2 LSBs of the scale factor (scaleFactor % 4) applied as power 2 factor to the
 *        resulting inverse quantized value.
 * \return the exponent of the result (mantissa) stored into *pValue.
 */
FDK_INLINE
int EvaluatePower43 ( FIXP_DBL *pValue,
                      UINT lsb )
{
  FIXP_DBL value;
  UINT freeBits;
  UINT exponent;

  value = *pValue;
  freeBits = fNormz (value) ;
  exponent = DFRACT_BITS - freeBits ;
  FDK_ASSERT (exponent < 14);

  UINT x = (((int)value << freeBits) >> 19) ;
  UINT tableIndex = (x & 0x0FFF) >> 4 ;
  FIXP_DBL invQVal ;

  x = x & 0x0F;

  UINT r0=(LONG)InverseQuantTable [tableIndex+0];
  UINT r1=(LONG)InverseQuantTable [tableIndex+1];
  USHORT nx=16-x;
  UINT temp=(r0)*nx+(r1)*x;
  invQVal = (FIXP_DBL)temp;

  FDK_ASSERT(lsb < 4);
  *pValue = fMultDiv2 (invQVal, MantissaTable [lsb][exponent]) ;

  /* + 1 compensates fMultDiv2(). */
  return ExponentTable [lsb][exponent] + 1;
}

/**
 * \brief determine the required shift scale for the given quantized value and scale (factor % 4) value.
 */
FDK_INLINE int GetScaleFromValue (FIXP_DBL value, unsigned int lsb)
{
  if (value!=(FIXP_DBL)0)
  {
    int scale = EvaluatePower43 (&value, lsb) ;
    return CntLeadingZeros (value) - scale - 2 ;
  }
  else
    return 0; /* Return zero, because its useless to scale a zero value, saves workload and avoids scaling overshifts. */
}


//#ifdef AACDEC_HUFFMANDECODER_ENABLE

/*!
  \brief Read huffman codeword

  The function reads the huffman codeword from the bitstream and
  returns the index value.

  \return  index value
*/
inline int CBlock_DecodeHuffmanWord( HANDLE_FDK_BITSTREAM bs,                           /*!< pointer to bitstream */
                                     const CodeBookDescription *hcb )                   /*!< pointer to codebook description */
{
  UINT val;
  UINT index = 0;
  const USHORT (*CodeBook) [HuffmanEntries] = hcb->CodeBook;

  while (1)
  {
    val = CodeBook[index][FDKreadBits(bs,HuffmanBits)]; /* Expensive memory access */

    if ((val & 1) == 0)
    {
      index = val>>2;
      continue;
    }
    else
    {
      if (val & 2)
      {
        FDKpushBackCache(bs,1);
      }

      val >>= 2;
      break;
    }
  }

  return val;
}

//#endif /* AACDEC_HUFFMANDECODER_ENABLE */

#endif /* #ifndef BLOCK_H */
