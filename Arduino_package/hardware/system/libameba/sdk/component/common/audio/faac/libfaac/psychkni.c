
#include <stdio.h>
#include "platform_stdlib.h"
#include <math.h>

#include "psych.h"
#include "coder.h"
#include "fft.h"
#include "aac_util.h"
#include "frame.h"

typedef float psyfloat;

typedef struct
{
  /* bandwidth */
  int bandS;
  int lastband;

  /* SFB energy */
  psyfloat *fftEnrgS[8];
#if RING_BUFFER_EN
  psyfloat *fftEnrgNextS[8];
  psyfloat *fftEnrgNext2S[8];
  psyfloat *fftEnrgPrevS[8];
#endif
}
psydata_t;

static int start_time = 0, end_time = 0;
//extern volatile int OS_TimeMS; 

static void Hann(GlobalPsyInfo * gpsyInfo, float *inSamples, int size)
{
  int i;

  /* Applying Hann window */
  if (size == BLOCK_LEN_LONG * 2)
  {
    for (i = 0; i < size; i++)
      inSamples[i] *= gpsyInfo->hannWindow[i];
  }
  else
  {
#if !ARM_DSP_EN
    for (i = 0; i < size; i++)
      inSamples[i] *= gpsyInfo->hannWindowS[i];
#else
	arm_mult_f32(gpsyInfo->hannWindowS, inSamples, inSamples,size);
#endif
  }
}
float Q_rsqrt( float number )
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y  = number;
	i  = * ( long * ) &y;                       // evil floating point bit level hacking（?浮??的邪?位元hack）
	i  = 0x5f3759df - ( i >> 1 );               // what the fuck?（?他?的是怎么回事？）
	y  = * ( float * ) &i;
	y  = y * ( threehalfs - ( x2 * y * y ) );   // 1st iteration （第一次迭代）
//      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed（第二次迭代，可以?除）

	return y;
}
float sqrt2(const float x)
{
#ifdef AAC_EDIT
	float xux;
	union // get bits for floating value
	{
		float x;
		int i;
	} u;
	u.x = x;
	u.i = 0x5f3759df - (u.i >> 1);  // gives initial guess y0

	xux = x*u.x;

	return xux*(1.5f - .5f*xux*u.x);// Newton step, repeating increases accuracy 
#else
	return sqrt(x);
#endif
}

static void PsyCheckShort(PsyInfo * psyInfo)
{
  float totvol = 0.0;
  float totchg, totchg2;
  psydata_t *psydata = psyInfo->data;
  int lastband = psydata->lastband;
  int firstband = 1;
  int sfb;

  /* long/short block switch */
  totchg = totchg2 = 0.0;
  for (sfb = 0; sfb < lastband; sfb++)
  {
    int win;
	float volb[16]={0};
	float vavg[13]={0};
    float maxdif = 0.0;
    float totmaxdif = 0.0;
    float e, v;

#if RING_BUFFER_EN
    // previous frame
    for (win = 0; win < 4; win++)
    {
      e = psydata->fftEnrgPrevS[win + 4][sfb];

      volb[win] = sqrt2(e);
      totvol += e;
    }
#endif

    // current frame
    for (win = 0; win < 8; win++)
    {
      e = psydata->fftEnrgS[win][sfb];

      volb[win + 4] = sqrt2(e);
      totvol += e;
    }
#if RING_BUFFER_EN
    // next frame
    for (win = 0; win < 4; win++)
    {
      e = psydata->fftEnrgNextS[win][sfb];

      volb[win + 12] = sqrt2(e);
      totvol += e;
    }
#endif

    // ignore lowest SFBs
    if (sfb < firstband)
      continue;

    v = 0.0;
    for (win = 0; win < 4; win++)
    {
      v += volb[win];
    }
    vavg[0] = 0.25 * v;

    for (win = 1; win < 13; win++)
    {
      v -= volb[win - 1];
      v += volb[win + 3];
      vavg[win] = 0.25 * v;
    }

    for (win = 0; win < 8; win++)
    {
      int i;
      float mina, maxv;
      float voldif;
      float totvoldif;

      mina = vavg[win];
      for (i = 1; i < 5; i++)
        mina = min(mina, vavg[win + i]);

      maxv = volb[win + 2];
      for (i = 3; i < 6; i++)
        maxv = max(maxv, volb[win + i]);

      if (!maxv || !mina)
        continue;

      voldif = (maxv - mina) / mina;
      totvoldif = (maxv - mina) * (maxv - mina);

	if (voldif > maxdif)
	  maxdif = voldif;

	if (totvoldif > totmaxdif)
	  totmaxdif = totvoldif;
      }
    totchg += maxdif;
    totchg2 += totmaxdif;
  }

  totvol = sqrt2(totvol);

  totchg2 = sqrt2(totchg2);

  totchg = totchg / lastband;
  if (totvol)
    totchg2 /= totvol;
  else
    totchg2 = 0.0;

  psyInfo->block_type = ((totchg > 1.0) && (totchg2 > 0.04))
    ? ONLY_SHORT_WINDOW : ONLY_LONG_WINDOW;

#if 0
  {
    static int total = 0, shorts = 0;
    char *flash = "    ";

    total++;
    if (psyInfo->block_type == ONLY_SHORT_WINDOW)
    {
      flash = "****";
      shorts++;
    }

    //printf("totchg: %s %g %g\t%g\n", flash, totchg, totchg2,
	   (float)shorts/total);
  }
#endif
}

static void PsyInit(GlobalPsyInfo * gpsyInfo, PsyInfo * psyInfo, unsigned int numChannels,
		    unsigned int sampleRate, int *cb_width_long, int num_cb_long,
		    int *cb_width_short, int num_cb_short)
{
  unsigned int channel;
  int i, j, size;

  gpsyInfo->hannWindow =
    (float *) AllocMemory(2 * BLOCK_LEN_LONG * sizeof(float));
  gpsyInfo->hannWindowS =
    (float *) AllocMemory(2 * BLOCK_LEN_SHORT * sizeof(float));

  for (i = 0; i < BLOCK_LEN_LONG * 2; i++)
    gpsyInfo->hannWindow[i] = 0.5 * (1 - cos(2.0 * M_PI * (i + 0.5) /
					     (BLOCK_LEN_LONG * 2)));
  for (i = 0; i < BLOCK_LEN_SHORT * 2; i++)
    gpsyInfo->hannWindowS[i] = 0.5 * (1 - cos(2.0 * M_PI * (i + 0.5) /
					      (BLOCK_LEN_SHORT * 2)));
  gpsyInfo->sampleRate = (float) sampleRate;

  for (channel = 0; channel < numChannels; channel++)
  {
    //psydata_t *psydata = pvPortMalloc(sizeof(psydata_t));
    psydata_t *psydata = AllocMemory(sizeof(psydata_t));
    psyInfo[channel].data = psydata;
  }

  size = BLOCK_LEN_LONG;
  for (channel = 0; channel < numChannels; channel++)
  {
    psyInfo[channel].size = size;

    psyInfo[channel].prevSamples =
      (float *) AllocMemory(size * sizeof(float));
    memset(psyInfo[channel].prevSamples, 0, size * sizeof(float));
  }

  size = BLOCK_LEN_SHORT;
  for (channel = 0; channel < numChannels; channel++)
  {
    psydata_t *psydata = psyInfo[channel].data;

    psyInfo[channel].sizeS = size;

    psyInfo[channel].prevSamplesS =
      (float *) AllocMemory(size * sizeof(float));
    memset(psyInfo[channel].prevSamplesS, 0, size * sizeof(float));

    for (j = 0; j < 8; j++)
    {
#if RING_BUFFER_EN
      psydata->fftEnrgPrevS[j] =
	(psyfloat *) AllocMemory(NSFB_SHORT * sizeof(psyfloat));
      memset(psydata->fftEnrgPrevS[j], 0, NSFB_SHORT * sizeof(psyfloat));
#endif
      psydata->fftEnrgS[j] =
	(psyfloat *) AllocMemory(NSFB_SHORT * sizeof(psyfloat));
      memset(psydata->fftEnrgS[j], 0, NSFB_SHORT * sizeof(psyfloat));
#if RING_BUFFER_EN
      psydata->fftEnrgNextS[j] =
	(psyfloat *) AllocMemory(NSFB_SHORT * sizeof(psyfloat));
      memset(psydata->fftEnrgNextS[j], 0, NSFB_SHORT * sizeof(psyfloat));
      psydata->fftEnrgNext2S[j] =
	(psyfloat *) AllocMemory(NSFB_SHORT * sizeof(psyfloat));
      memset(psydata->fftEnrgNext2S[j], 0, NSFB_SHORT * sizeof(psyfloat));
#endif
    }
  }
}

static void PsyEnd(GlobalPsyInfo * gpsyInfo, PsyInfo * psyInfo, unsigned int numChannels)
{
  unsigned int channel;
  int j;

  if (gpsyInfo->hannWindow)
    FreeMemory(gpsyInfo->hannWindow);
  if (gpsyInfo->hannWindowS)
    FreeMemory(gpsyInfo->hannWindowS);

  for (channel = 0; channel < numChannels; channel++)
  {
    if (psyInfo[channel].prevSamples)
      FreeMemory(psyInfo[channel].prevSamples);
  }

  for (channel = 0; channel < numChannels; channel++)
  {
    psydata_t *psydata = psyInfo[channel].data;

    if (psyInfo[channel].prevSamplesS)
      FreeMemory(psyInfo[channel].prevSamplesS);
    for (j = 0; j < 8; j++)
    {
#if RING_BUFFER_EN
      if (psydata->fftEnrgPrevS[j])
	FreeMemory(psydata->fftEnrgPrevS[j]);
#endif
      if (psydata->fftEnrgS[j])
	FreeMemory(psydata->fftEnrgS[j]);
#if RING_BUFFER_EN
      if (psydata->fftEnrgNextS[j])
	FreeMemory(psydata->fftEnrgNextS[j]);
      if (psydata->fftEnrgNext2S[j])
	FreeMemory(psydata->fftEnrgNext2S[j]);
#endif
    }
  }

  for (channel = 0; channel < numChannels; channel++)
  {
    if (psyInfo[channel].data)
      FreeMemory(psyInfo[channel].data);
  }
}

/* Do psychoacoustical analysis */
static void PsyCalculate(ChannelInfo * channelInfo, GlobalPsyInfo * gpsyInfo,
			 PsyInfo * psyInfo, int *cb_width_long, int
			 num_cb_long, int *cb_width_short,
			 int num_cb_short, unsigned int numChannels)
{
  unsigned int channel;

  for (channel = 0; channel < numChannels; channel++)
  {
    if (channelInfo[channel].present)
    {

      if (channelInfo[channel].cpe && channelInfo[channel].ch_is_left)
      {				/* CPE */

	      int leftChan = channel;
	      int rightChan = channelInfo[channel].paired_ch;
        
	      PsyCheckShort(&psyInfo[leftChan]);
	      PsyCheckShort(&psyInfo[rightChan]);
      }
      else if (!channelInfo[channel].cpe && channelInfo[channel].lfe)
      {				/* LFE */
        // Only set block type and it should be OK
	      psyInfo[channel].block_type = ONLY_LONG_WINDOW;
      }
      else if (!channelInfo[channel].cpe)
      {				/* SCE */
	      PsyCheckShort(&psyInfo[channel]);
      }
    }
  }
}
#ifdef AAC_CYCLECOUNT
extern int g_adTempAccu[];
extern int adTemp[];
extern volatile uint32_t *DWT_CYCCNT0;
#endif

//  float transBuff[2 * BLOCK_LEN_LONG];
 // float transBuffS[2 * BLOCK_LEN_SHORT];
// cost 29ms, using FFT here, wf, 0420
static void PsyBufferUpdate( FFT_Tables *fft_tables, GlobalPsyInfo * gpsyInfo, PsyInfo * psyInfo,
			    float *newSamples, unsigned int bandwidth,
			    int *cb_width_short, int num_cb_short)
{
  int win;
  psydata_t *psydata = psyInfo->data;
  psyfloat *tmp;
  int sfb;
// whether to use DSP
#ifdef AAC_CYCLECOUNT
adTemp[27] = *DWT_CYCCNT0;
#endif

	float *transBuff = (float*)AllocMemory(2 * BLOCK_LEN_LONG*sizeof(float));
	float *transBuffS = (float*)AllocMemory(2 * BLOCK_LEN_SHORT*sizeof(float));

#if ARM_DSP_EN  
  arm_rfft_fast_instance_f32 S;
  //float transBuffS_FFT_res[2 * BLOCK_LEN_SHORT], transBuffS_FFT_in[2 * BLOCK_LEN_SHORT];
	float *transBuffS_FFT_res = (float*)AllocMemory(2 * BLOCK_LEN_SHORT*sizeof(float));
	float *transBuffS_FFT_in  = (float*)AllocMemory(2 * BLOCK_LEN_SHORT*sizeof(float));
 arm_rfft_fast_init_f32(&S, 256);
#endif    
#ifdef AAC_CYCLECOUNT
adTemp[28] = *DWT_CYCCNT0;
#endif
  psydata->bandS = psyInfo->sizeS * bandwidth * 2 / gpsyInfo->sampleRate;

  memcpy(transBuff, psyInfo->prevSamples, psyInfo->size * sizeof(float));
  memcpy(transBuff + psyInfo->size, newSamples, psyInfo->size * sizeof(float));
  
//printf("oscar.%d.debug.PsyBufferUpdate.01\r\n", psyInfo->size);
  
  for (win = 0; win < 8; win++)
  {
    int first = 0;
    int last = 0;

#ifdef AAC_CYCLECOUNT
adTemp[29] = *DWT_CYCCNT0;
#endif
    memcpy(transBuffS, transBuff + (win * BLOCK_LEN_SHORT) + (BLOCK_LEN_LONG - BLOCK_LEN_SHORT) / 2,
	   2 * psyInfo->sizeS * sizeof(float));

    Hann(gpsyInfo, transBuffS, 2 * psyInfo->sizeS);
#ifdef AAC_CYCLECOUNT
adTemp[30] = *DWT_CYCCNT0;
#endif
    
#if ARM_DSP_EN
    //arm_cfft_f32(&arm_cfft_sR_f32_len64, x_complex, 0, 1);
    //start_time = OS_TimeMS;
    {
        int i = 0;
        for(i = 0; i < 2 * BLOCK_LEN_SHORT; i++)
            transBuffS_FFT_in[i] = transBuffS[i];
        arm_rfft_fast_f32(&S, transBuffS_FFT_in, transBuffS_FFT_res, 0);        
    }
#ifdef AAC_CYCLECOUNT
adTemp[31] = *DWT_CYCCNT0;
#endif
    //end_time = OS_TimeMS;
    //printf("arm_rfft\t%d\n", end_time - start_time);     
#else
    //start_time = OS_TimeMS;
    rfft( fft_tables, transBuffS, 8);
    //end_time = OS_TimeMS;
    //printf("rfft\t%d\n", end_time - start_time);
   
#endif    

    // shift bufs
#if RING_BUFFER_EN
    tmp = psydata->fftEnrgPrevS[win];
    psydata->fftEnrgPrevS[win] = psydata->fftEnrgS[win];
    psydata->fftEnrgS[win] = psydata->fftEnrgNextS[win];
    psydata->fftEnrgNextS[win] = psydata->fftEnrgNext2S[win];
    psydata->fftEnrgNext2S[win] = tmp;
#endif

    for (sfb = 0; sfb < num_cb_short; sfb++)
    {
      float e;
      int l;

      first = last;
      last = first + cb_width_short[sfb];

      if (first < 1)
	      first = 1;

      //if (last > psydata->bandS) // band out of range
      if (first >= psydata->bandS) // band out of range
	      break;
      e = 0.0;
      
#if ARM_DSP_EN      
      for (l = first; l < last; l++)
      {
	      float a = transBuffS_FFT_res[l];
	      float b = transBuffS_FFT_res[l + psyInfo->sizeS];
        
	      e += a * a + b * b;
      }
#else
      for (l = first; l < last; l++)
      {
	      float a = transBuffS[l];
	      float b = transBuffS[l + psyInfo->sizeS];
        
	      e += a * a + b * b;
      }      

#endif      
#if RING_BUFFER_EN
      psydata->fftEnrgNext2S[win][sfb] = e;
#else
	psydata->fftEnrgS[win][sfb] = (float)e;
#endif
    }
#ifdef AAC_CYCLECOUNT
 adTemp[32] = *DWT_CYCCNT0;
#endif
   psydata->lastband = sfb;
    for (; sfb < num_cb_short; sfb++)
    {
#if RING_BUFFER_EN
      psydata->fftEnrgNext2S[win][sfb] = 0;
#else
		psydata->fftEnrgS[win][sfb] = 0;
#endif
    }
#ifdef AAC_CYCLECOUNT
g_adTempAccu[27] += adTemp[28]-adTemp[27];
g_adTempAccu[29] += adTemp[30]-adTemp[29];
g_adTempAccu[30] += adTemp[31]-adTemp[30];
g_adTempAccu[31] += adTemp[32]-adTemp[31];
#endif
}
  memcpy(psyInfo->prevSamples, newSamples, psyInfo->size * sizeof(float));
  FreeMemory(transBuff);
  FreeMemory(transBuffS);
#if ARM_DSP_EN  
  FreeMemory(transBuffS_FFT_res);
  FreeMemory(transBuffS_FFT_in);
#endif    

}

static void BlockSwitch(CoderInfo * coderInfo, PsyInfo * psyInfo, unsigned int numChannels)
{
  unsigned int channel;
  int desire = ONLY_LONG_WINDOW;

  /* Use the same block type for all channels
     If there is 1 channel that wants a short block,
     use a short block on all channels.
   */
  for (channel = 0; channel < numChannels; channel++)
  {
    if (psyInfo[channel].block_type == ONLY_SHORT_WINDOW)
      desire = ONLY_SHORT_WINDOW;
  }

  for (channel = 0; channel < numChannels; channel++)
  {
    int lasttype = coderInfo[channel].block_type;

    if (desire == ONLY_SHORT_WINDOW
	|| coderInfo[channel].desired_block_type == ONLY_SHORT_WINDOW)
    {
      if (lasttype == ONLY_LONG_WINDOW || lasttype == SHORT_LONG_WINDOW)
	      coderInfo[channel].block_type = LONG_SHORT_WINDOW;
      else
	      coderInfo[channel].block_type = ONLY_SHORT_WINDOW;
    }
    else
    {
      if (lasttype == ONLY_SHORT_WINDOW || lasttype == LONG_SHORT_WINDOW)
	      coderInfo[channel].block_type = SHORT_LONG_WINDOW;
      else
	      coderInfo[channel].block_type = ONLY_LONG_WINDOW;
    }
    coderInfo[channel].desired_block_type = desire;
  }
}

psymodel_t psymodel2 =
{
  PsyInit,
  PsyEnd,
  PsyCalculate,
  PsyBufferUpdate,
  BlockSwitch
};
