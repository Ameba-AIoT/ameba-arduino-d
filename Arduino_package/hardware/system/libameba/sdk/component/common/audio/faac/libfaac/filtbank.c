/************************* MPEG-2 NBC Audio Decoder **************************
 *                                                                           *
"This software module was originally developed by
AT&T, Dolby Laboratories, Fraunhofer Gesellschaft IIS in the course of
development of the MPEG-2 NBC/MPEG-4 Audio standard ISO/IEC 13818-7,
14496-1,2 and 3. This software module is an implementation of a part of one or more
MPEG-2 NBC/MPEG-4 Audio tools as specified by the MPEG-2 NBC/MPEG-4
Audio standard. ISO/IEC  gives users of the MPEG-2 NBC/MPEG-4 Audio
standards free license to this software module or modifications thereof for use in
hardware or software products claiming conformance to the MPEG-2 NBC/MPEG-4
Audio  standards. Those intending to use this software module in hardware or
software products are advised that this use may infringe existing patents.
The original developer of this software module and his/her company, the subsequent
editors and their companies, and ISO/IEC have no liability for use of this software
module or modifications thereof in an implementation. Copyright is not released for
non MPEG-2 NBC/MPEG-4 Audio conforming products.The original developer
retains full right to use the code for his/her  own purpose, assign or donate the
code to a third party and to inhibit third party from using the code for non
MPEG-2 NBC/MPEG-4 Audio conforming products. This copyright notice must
be included in all copies or derivative works."
Copyright(c)1996.
 *                                                                           *
 ****************************************************************************/
/*
 * $Id: filtbank.c,v 1.13 2005/02/02 07:51:12 sur Exp $
 */

/*
 * CHANGES:
 *  2001/01/17: menno: Added frequency cut off filter.
 *
 */

#include <math.h>
#include <stdio.h>
#include "platform_stdlib.h"
#include <time.h>

#include "coder.h"
#include "filtbank.h"
#include "frame.h"
#include "fft.h"
#include "aac_util.h"

#define  TWOPI       2*M_PI


static float	Izero				( float x);
static void		MDCT				( FFT_Tables *fft_tables, float *data, int N );
static void		IMDCT				( FFT_Tables *fft_tables, float *data, int N );

static int start_time = 0, end_time = 0;
extern volatile int OS_TimeMS;

void FilterBankInit(faacEncHandle hEncoder)
{
    unsigned int i, channel;

    for (channel = 0; channel < hEncoder->numChannels; channel++) {
        hEncoder->freqBuff[channel] = (float*)AllocMemory(2*FRAME_LEN*sizeof(float));
        hEncoder->overlapBuff[channel] = (float*)AllocMemory(FRAME_LEN*sizeof(float));
        SetMemory(hEncoder->overlapBuff[channel], 0, FRAME_LEN*sizeof(float));
    }

    hEncoder->sin_window_long = (float*)AllocMemory(BLOCK_LEN_LONG*sizeof(float));
    hEncoder->sin_window_short = (float*)AllocMemory(BLOCK_LEN_SHORT*sizeof(float));

    for( i=0; i<BLOCK_LEN_LONG; i++ )
        hEncoder->sin_window_long[i] = sin((M_PI/(2*BLOCK_LEN_LONG)) * (i + 0.5));
    for( i=0; i<BLOCK_LEN_SHORT; i++ )
        hEncoder->sin_window_short[i] = sin((M_PI/(2*BLOCK_LEN_SHORT)) * (i + 0.5));
}

void FilterBankEnd(faacEncHandle hEncoder)
{
    unsigned int channel;

    for (channel = 0; channel < hEncoder->numChannels; channel++) {
        if (hEncoder->freqBuff[channel]) FreeMemory(hEncoder->freqBuff[channel]);
        if (hEncoder->overlapBuff[channel]) FreeMemory(hEncoder->overlapBuff[channel]);
    }

    if (hEncoder->sin_window_long) FreeMemory(hEncoder->sin_window_long);
    if (hEncoder->sin_window_short) FreeMemory(hEncoder->sin_window_short);
}
float* fTemp1;
float* fTemp2;
float* fTemp3;
#ifdef AAC_CYCLECOUNT
extern int g_adTempAccu[];
extern int adTemp[];
extern volatile uint32_t *DWT_CYCCNT0;
#endif

void FilterBank(faacEncHandle hEncoder,
                CoderInfo *coderInfo,
                float *p_in_data,
                float *p_out_mdct,
                float *p_overlap,
                int overlap_select)
{
    float *p_o_buf, *first_window, *second_window;
    float *transf_buf;
    int k, i;
    int block_type = coderInfo->block_type;
	fTemp1 = p_in_data;
	fTemp2 = p_out_mdct;

    transf_buf = (float*)AllocMemory(2*BLOCK_LEN_LONG*sizeof(float));
	fTemp3 = transf_buf;

#ifdef AAC_CYCLECOUNT
adTemp[33] = *DWT_CYCCNT0;
#endif
    /* create / shift old values */
    /* We use p_overlap here as buffer holding the last frame time signal*/
    if(overlap_select != MNON_OVERLAPPED) {
        memcpy(transf_buf, p_overlap, FRAME_LEN*sizeof(float));
        memcpy(transf_buf+BLOCK_LEN_LONG, p_in_data, FRAME_LEN*sizeof(float));
        memcpy(p_overlap, p_in_data, FRAME_LEN*sizeof(float));
    } else {
        memcpy(transf_buf, p_in_data, 2*FRAME_LEN*sizeof(float));
    }
#ifdef AAC_CYCLECOUNT
adTemp[34] = *DWT_CYCCNT0;
g_adTempAccu[33] += adTemp[34]-adTemp[33];
#endif

    /*  Window shape processing */
    if(overlap_select != MNON_OVERLAPPED) {
        switch (coderInfo->prev_window_shape) {
        case SINE_WINDOW:
            if ( (block_type == ONLY_LONG_WINDOW) || (block_type == LONG_SHORT_WINDOW))
                first_window = hEncoder->sin_window_long;
            else
                first_window = hEncoder->sin_window_short;
            break;
        }

        switch (coderInfo->window_shape){
        case SINE_WINDOW:
            if ( (block_type == ONLY_LONG_WINDOW) || (block_type == SHORT_LONG_WINDOW))
                second_window = hEncoder->sin_window_long;
            else
                second_window = hEncoder->sin_window_short;
            break;
        }
    } else {
        /* Always long block and sine window for LTP */
        first_window = hEncoder->sin_window_long;
        second_window = hEncoder->sin_window_long;
    }

    /* Set ptr to transf-Buffer */
    p_o_buf = transf_buf;

    /* Separate action for each Block Type */
    //start_time = OS_TimeMS;
    switch (block_type) {
    case ONLY_LONG_WINDOW :
#ifdef AAC_CYCLECOUNT
adTemp[35] = *DWT_CYCCNT0;
#endif
        for ( i = 0 ; i < BLOCK_LEN_LONG ; i++){
            p_out_mdct[i] = p_o_buf[i] * first_window[i];
            p_out_mdct[i+BLOCK_LEN_LONG] = p_o_buf[i+BLOCK_LEN_LONG] * second_window[BLOCK_LEN_LONG-i-1];
        }
#ifdef AAC_CYCLECOUNT
adTemp[36] = *DWT_CYCCNT0;
#endif
        MDCT( &hEncoder->fft_tables, p_out_mdct, 2*BLOCK_LEN_LONG );
#ifdef AAC_CYCLECOUNT
adTemp[37] = *DWT_CYCCNT0;
g_adTempAccu[35] += adTemp[36]-adTemp[35];
g_adTempAccu[36] += adTemp[37]-adTemp[36];
#endif
        break;

    case LONG_SHORT_WINDOW :
#ifdef AAC_CYCLECOUNT
adTemp[35] = *DWT_CYCCNT0;
#endif

#if !ARM_DSP_EN
        for ( i = 0 ; i < BLOCK_LEN_LONG ; i++)
            p_out_mdct[i] = p_o_buf[i] * first_window[i];
#else
		arm_mult_f32(p_o_buf, first_window, p_out_mdct, BLOCK_LEN_LONG);
#endif
        memcpy(p_out_mdct+BLOCK_LEN_LONG,p_o_buf+BLOCK_LEN_LONG,NFLAT_LS*sizeof(float));
        for ( i = 0 ; i < BLOCK_LEN_SHORT ; i++)
            p_out_mdct[i+BLOCK_LEN_LONG+NFLAT_LS] = p_o_buf[i+BLOCK_LEN_LONG+NFLAT_LS] * second_window[BLOCK_LEN_SHORT-i-1];
        SetMemory(p_out_mdct+BLOCK_LEN_LONG+NFLAT_LS+BLOCK_LEN_SHORT,0,NFLAT_LS*sizeof(float));
#ifdef AAC_CYCLECOUNT
adTemp[36] = *DWT_CYCCNT0;
#endif
        MDCT( &hEncoder->fft_tables, p_out_mdct, 2*BLOCK_LEN_LONG );
#ifdef AAC_CYCLECOUNT
adTemp[37] = *DWT_CYCCNT0;
g_adTempAccu[35] += adTemp[36]-adTemp[35];
g_adTempAccu[36] += adTemp[37]-adTemp[36];
#endif
        break;

    case SHORT_LONG_WINDOW :
#ifdef AAC_CYCLECOUNT
adTemp[35] = *DWT_CYCCNT0;
#endif
        SetMemory(p_out_mdct,0,NFLAT_LS*sizeof(float));
        for ( i = 0 ; i < BLOCK_LEN_SHORT ; i++)
            p_out_mdct[i+NFLAT_LS] = p_o_buf[i+NFLAT_LS] * first_window[i];
        memcpy(p_out_mdct+NFLAT_LS+BLOCK_LEN_SHORT,p_o_buf+NFLAT_LS+BLOCK_LEN_SHORT,NFLAT_LS*sizeof(float));
        for ( i = 0 ; i < BLOCK_LEN_LONG ; i++)
            p_out_mdct[i+BLOCK_LEN_LONG] = p_o_buf[i+BLOCK_LEN_LONG] * second_window[BLOCK_LEN_LONG-i-1];
#ifdef AAC_CYCLECOUNT
adTemp[36] = *DWT_CYCCNT0;
#endif
        MDCT( &hEncoder->fft_tables, p_out_mdct, 2*BLOCK_LEN_LONG );
#ifdef AAC_CYCLECOUNT
adTemp[37] = *DWT_CYCCNT0;
g_adTempAccu[35] += adTemp[36]-adTemp[35];
g_adTempAccu[36] += adTemp[37]-adTemp[36];
#endif
        break;

    case ONLY_SHORT_WINDOW :
        p_o_buf += NFLAT_LS;
        for ( k=0; k < MAX_SHORT_WINDOWS; k++ ) {
#ifdef AAC_CYCLECOUNT
adTemp[35] = *DWT_CYCCNT0;
#endif

//#if !ARM_DSP_EN
            for ( i = 0 ; i < BLOCK_LEN_SHORT ; i++ ){
                p_out_mdct[i] = p_o_buf[i] * first_window[i];//arm_mult_f32
                p_out_mdct[i+BLOCK_LEN_SHORT] = p_o_buf[i+BLOCK_LEN_SHORT] * second_window[BLOCK_LEN_SHORT-i-1];//arm_conv_f32
            }
//#else
//			arm_mult_f32(p_o_buf, hEncoder->sin_window_short_short, p_out_mdct, BLOCK_LEN_SHORT*2);
//#endif
			
#ifdef AAC_CYCLECOUNT
adTemp[36] = *DWT_CYCCNT0;
#endif
            MDCT( &hEncoder->fft_tables, p_out_mdct, 2*BLOCK_LEN_SHORT );
            p_out_mdct += BLOCK_LEN_SHORT;
            p_o_buf += BLOCK_LEN_SHORT;
            first_window = second_window;
#ifdef AAC_CYCLECOUNT
adTemp[37] = *DWT_CYCCNT0;
g_adTempAccu[35] += adTemp[36]-adTemp[35];
g_adTempAccu[36] += adTemp[37]-adTemp[36];
#endif
        }
        break;
    }
    //end_time = OS_TimeMS;
    //printf("MDCT\t%d\n", end_time - start_time);
    if (transf_buf) FreeMemory(transf_buf);
}

void IFilterBank(faacEncHandle hEncoder,
                 CoderInfo *coderInfo,
                 float *p_in_data,
                 float *p_out_data,
                 float *p_overlap,
                 int overlap_select)
{
    float *o_buf, *transf_buf, *overlap_buf;
    float *first_window, *second_window;

    float  *fp;
    int k, i;
    int block_type = coderInfo->block_type;

    transf_buf = (float*)AllocMemory(2*BLOCK_LEN_LONG*sizeof(float));
    overlap_buf = (float*)AllocMemory(2*BLOCK_LEN_LONG*sizeof(float));

    /*  Window shape processing */
    if (overlap_select != MNON_OVERLAPPED) {

            if ( (block_type == ONLY_LONG_WINDOW) || (block_type == LONG_SHORT_WINDOW))
                first_window = hEncoder->sin_window_long;
            else
                first_window = hEncoder->sin_window_short;

            if ( (block_type == ONLY_LONG_WINDOW) || (block_type == SHORT_LONG_WINDOW))
                second_window = hEncoder->sin_window_long;
            else
                second_window = hEncoder->sin_window_short;

    } else {
        /* Always long block and sine window for LTP */
        first_window  = hEncoder->sin_window_long;
        second_window = hEncoder->sin_window_long;
    }

    /* Assemble overlap buffer */
    memcpy(overlap_buf,p_overlap,BLOCK_LEN_LONG*sizeof(float));
    o_buf = overlap_buf;

    /* Separate action for each Block Type */
    switch( block_type ) {
    case ONLY_LONG_WINDOW :
        memcpy(transf_buf, p_in_data,BLOCK_LEN_LONG*sizeof(float));
        IMDCT( &hEncoder->fft_tables, transf_buf, 2*BLOCK_LEN_LONG );
        for ( i = 0 ; i < BLOCK_LEN_LONG ; i++)
            transf_buf[i] *= first_window[i];
        if (overlap_select != MNON_OVERLAPPED) {
            for ( i = 0 ; i < BLOCK_LEN_LONG; i++ ){
                o_buf[i] += transf_buf[i];
                o_buf[i+BLOCK_LEN_LONG] = transf_buf[i+BLOCK_LEN_LONG] * second_window[BLOCK_LEN_LONG-i-1];
            }
        } else { /* overlap_select == NON_OVERLAPPED */
            for ( i = 0 ; i < BLOCK_LEN_LONG; i++ )
                transf_buf[i+BLOCK_LEN_LONG] *= second_window[BLOCK_LEN_LONG-i-1];
        }
        break;

    case LONG_SHORT_WINDOW :
        memcpy(transf_buf, p_in_data,BLOCK_LEN_LONG*sizeof(float));
        IMDCT( &hEncoder->fft_tables, transf_buf, 2*BLOCK_LEN_LONG );
        for ( i = 0 ; i < BLOCK_LEN_LONG ; i++)
            transf_buf[i] *= first_window[i];
        if (overlap_select != MNON_OVERLAPPED) {
            for ( i = 0 ; i < BLOCK_LEN_LONG; i++ )
                o_buf[i] += transf_buf[i];
            memcpy(o_buf+BLOCK_LEN_LONG,transf_buf+BLOCK_LEN_LONG,NFLAT_LS*sizeof(float));
            for ( i = 0 ; i < BLOCK_LEN_SHORT ; i++)
                o_buf[i+BLOCK_LEN_LONG+NFLAT_LS] = transf_buf[i+BLOCK_LEN_LONG+NFLAT_LS] * second_window[BLOCK_LEN_SHORT-i-1];
            SetMemory(o_buf+BLOCK_LEN_LONG+NFLAT_LS+BLOCK_LEN_SHORT,0,NFLAT_LS*sizeof(float));
        } else { /* overlap_select == NON_OVERLAPPED */
            for ( i = 0 ; i < BLOCK_LEN_SHORT ; i++)
                transf_buf[i+BLOCK_LEN_LONG+NFLAT_LS] *= second_window[BLOCK_LEN_SHORT-i-1];
            SetMemory(transf_buf+BLOCK_LEN_LONG+NFLAT_LS+BLOCK_LEN_SHORT,0,NFLAT_LS*sizeof(float));
        }
        break;

    case SHORT_LONG_WINDOW :
        memcpy(transf_buf, p_in_data,BLOCK_LEN_LONG*sizeof(float));
        IMDCT( &hEncoder->fft_tables, transf_buf, 2*BLOCK_LEN_LONG );
        for ( i = 0 ; i < BLOCK_LEN_SHORT ; i++)
            transf_buf[i+NFLAT_LS] *= first_window[i];
        if (overlap_select != MNON_OVERLAPPED) {
            for ( i = 0 ; i < BLOCK_LEN_SHORT; i++ )
                o_buf[i+NFLAT_LS] += transf_buf[i+NFLAT_LS];
            memcpy(o_buf+BLOCK_LEN_SHORT+NFLAT_LS,transf_buf+BLOCK_LEN_SHORT+NFLAT_LS,NFLAT_LS*sizeof(float));
            for ( i = 0 ; i < BLOCK_LEN_LONG ; i++)
                o_buf[i+BLOCK_LEN_LONG] = transf_buf[i+BLOCK_LEN_LONG] * second_window[BLOCK_LEN_LONG-i-1];
        } else { /* overlap_select == NON_OVERLAPPED */
            SetMemory(transf_buf,0,NFLAT_LS*sizeof(float));
            for ( i = 0 ; i < BLOCK_LEN_LONG ; i++)
                transf_buf[i+BLOCK_LEN_LONG] *= second_window[BLOCK_LEN_LONG-i-1];
        }
        break;

    case ONLY_SHORT_WINDOW :
        if (overlap_select != MNON_OVERLAPPED) {
            fp = o_buf + NFLAT_LS;
        } else { /* overlap_select == NON_OVERLAPPED */
            fp = transf_buf;
        }
        for ( k=0; k < MAX_SHORT_WINDOWS; k++ ) {
            memcpy(transf_buf,p_in_data,BLOCK_LEN_SHORT*sizeof(float));
            IMDCT( &hEncoder->fft_tables, transf_buf, 2*BLOCK_LEN_SHORT );
            p_in_data += BLOCK_LEN_SHORT;
            if (overlap_select != MNON_OVERLAPPED) {
                for ( i = 0 ; i < BLOCK_LEN_SHORT ; i++){
                    transf_buf[i] *= first_window[i];
                    fp[i] += transf_buf[i];
                    fp[i+BLOCK_LEN_SHORT] = transf_buf[i+BLOCK_LEN_SHORT] * second_window[BLOCK_LEN_SHORT-i-1];
                }
                fp += BLOCK_LEN_SHORT;
            } else { /* overlap_select == NON_OVERLAPPED */
                for ( i = 0 ; i < BLOCK_LEN_SHORT ; i++){
                    fp[i] *= first_window[i];
                    fp[i+BLOCK_LEN_SHORT] *= second_window[BLOCK_LEN_SHORT-i-1];
                }
                fp += 2*BLOCK_LEN_SHORT;
            }
            first_window = second_window;
        }
        SetMemory(o_buf+BLOCK_LEN_LONG+NFLAT_LS+BLOCK_LEN_SHORT,0,NFLAT_LS*sizeof(float));
        break;
    }

    if (overlap_select != MNON_OVERLAPPED)
        memcpy(p_out_data,o_buf,BLOCK_LEN_LONG*sizeof(float));
    else  /* overlap_select == NON_OVERLAPPED */
        memcpy(p_out_data,transf_buf,2*BLOCK_LEN_LONG*sizeof(float));

    /* save unused output data */
    memcpy(p_overlap,o_buf+BLOCK_LEN_LONG,BLOCK_LEN_LONG*sizeof(float));

    if (overlap_buf) FreeMemory(overlap_buf);
    if (transf_buf) FreeMemory(transf_buf);
}

void specFilter(float *freqBuff,
                int sampleRate,
                int lowpassFreq,
                int specLen
                )
{
    int lowpass,xlowpass;

    /* calculate the last line which is not zero */
    lowpass = (lowpassFreq * specLen) / (sampleRate>>1) + 1;
    xlowpass = (lowpass < specLen) ? lowpass : specLen ;

    SetMemory(freqBuff+xlowpass,0,(specLen-xlowpass)*sizeof(float));
}

static float Izero(float x)
{
    const float IzeroEPSILON = 1E-41;  /* Max error acceptable in Izero */
    float sum, u, halfx, temp;
    int n;

    sum = u = n = 1;
    halfx = x/2.0;
    do {
        temp = halfx/(float)n;
        n += 1;
        temp *= temp;
        u *= temp;
        sum += u;
    } while (u >= IzeroEPSILON*sum);

    return(sum);
}

// N; 512
static void MDCT( FFT_Tables *fft_tables, float *data, int N )
{
#ifdef AAC_CYCLECOUNT
adTemp[38] = *DWT_CYCCNT0;
#endif
#if !ARM_DSP_EN
    float *xi, *xr;
#else    
    float *x_complex;
#endif    
    float tempr, tempi, c, s, cold, cfreq, sfreq; /* temps for pre and post twiddle */
    float freq = TWOPI / N;
    float cosfreq8, sinfreq8;
    int i, n;

#if !ARM_DSP_EN
    xi = (float*)AllocMemory((N >> 2)*sizeof(float));
    xr = (float*)AllocMemory((N >> 2)*sizeof(float));
#else
    x_complex = (float*)AllocMemory((N >> 1)*sizeof(float));
#endif    

#ifdef AAC_CYCLECOUNT
adTemp[39] = *DWT_CYCCNT0;
#endif
    /* prepare for recurrence relation in pre-twiddle */
#ifdef AAC_EDIT
	if(N==2048)
	{
		cfreq = 9.9999529122E-1;//   9.99698817E-1
		sfreq = 3.0679567711E-3;//   2.4541229E-2
		cosfreq8 = 9.9999994033E-1;//   9.9999529122E-1
		sinfreq8 = 3.83495207E-4;//   3.0679567711E-3
	}
	if(N==256)
	{
		cfreq = 9.99698817E-1;
		sfreq = 2.4541229E-2;
		cosfreq8 = 9.9999529122E-1;
		sinfreq8 = 3.0679567711E-3;
	}
	else
	{
		cfreq = cos (freq);//9.9999529122E-1   9.99698817E-1
		sfreq = sin (freq);//3.0679567711E-3   2.4541229E-2
		cosfreq8 = cos (freq * 0.125);//9.9999994033E-1   9.9999529122E-1
		sinfreq8 = sin (freq * 0.125);//3.83495207E-4   3.0679567711E-3
	}
#else
    cfreq = cos (freq);//9.9999529122E-1   9.99698817E-1
    sfreq = sin (freq);//3.0679567711E-3   2.4541229E-2
    cosfreq8 = cos (freq * 0.125);//9.9999994033E-1   9.9999529122E-1
    sinfreq8 = sin (freq * 0.125);//3.83495207E-4   3.0679567711E-3
#endif

    c = cosfreq8;
    s = sinfreq8;

#ifdef AAC_CYCLECOUNT
adTemp[40] = *DWT_CYCCNT0;
#endif
    for (i = 0; i < (N >> 2); i++) {
        /* calculate real and imaginary parts of g(n) or G(p) */
        n = (N >> 1) - 1 - 2 * i;

        if (i < (N >> 3))
            tempr = data [(N >> 2) + n] + data [N + (N >> 2) - 1 - n]; /* use second form of e(n) for n = N / 2 - 1 - 2i */
        else
            tempr = data [(N >> 2) + n] - data [(N >> 2) - 1 - n]; /* use first form of e(n) for n = N / 2 - 1 - 2i */

        n = 2 * i;
        if (i < (N >> 3))
            tempi = data [(N >> 2) + n] - data [(N >> 2) - 1 - n]; /* use first form of e(n) for n=2i */
        else
            tempi = data [(N >> 2) + n] + data [N + (N >> 2) - 1 - n]; /* use second form of e(n) for n=2i*/

        /* calculate pre-twiddled FFT input */
#if !ARM_DSP_EN        
        xr[i] = tempr * c + tempi * s;
        xi[i] = tempi * c - tempr * s;
#else        
        x_complex[i << 1] = tempr * c + tempi * s;       // real part, wf, 0415
        x_complex[(i << 1) + 1] = tempi * c - tempr * s; // imag part
#endif        

        /* use recurrence to prepare cosine and sine for next value of i */
        cold = c;
        c = c * cfreq - s * sfreq;
        s = s * cfreq + cold * sfreq;
    }
#ifdef AAC_CYCLECOUNT
adTemp[41] = *DWT_CYCCNT0;
#endif
    /* Perform in-place complex FFT of length N/4 */
    switch (N) {
      case BLOCK_LEN_SHORT * 2:
#if !ARM_DSP_EN      
          fft( fft_tables, xr, xi, 6);
#else          
          arm_cfft_f32(&arm_cfft_sR_f32_len64, x_complex, 0, 1);
#endif
          break;
      case BLOCK_LEN_LONG * 2:
#if !ARM_DSP_EN          
          fft( fft_tables, xr, xi, 9);
#else          
          arm_cfft_f32(&arm_cfft_sR_f32_len512, x_complex, 0, 1);
#endif    
    }

    /* prepare for recurrence relations in post-twiddle */
    c = cosfreq8;
    s = sinfreq8;

#ifdef AAC_CYCLECOUNT
adTemp[42] = *DWT_CYCCNT0;
#endif
    /* post-twiddle FFT output and then get output data */
    for (i = 0; i < (N >> 2); i++) {
        /* get post-twiddled FFT output  */
#if !ARM_DSP_EN        
        tempr = 2. * (xr[i] * c + xi[i] * s);
        tempi = 2. * (xi[i] * c - xr[i] * s);
#else        
        tempr = 2. * (x_complex[i << 1] * c + x_complex[(i << 1) + 1] * s);
        tempi = 2. * (x_complex[(i << 1) + 1] * c - x_complex[i << 1] * s);
#endif        

        /* fill in output values */
        data [2 * i] = -tempr;   /* first half even */
        data [(N >> 1) - 1 - 2 * i] = tempi;  /* first half odd */
        data [(N >> 1) + 2 * i] = -tempi;  /* second half even */
        data [N - 1 - 2 * i] = tempr;  /* second half odd */

        /* use recurrence to prepare cosine and sine for next value of i */
        cold = c;
        c = c * cfreq - s * sfreq;
        s = s * cfreq + cold * sfreq;
    }
#ifdef AAC_CYCLECOUNT
adTemp[43] = *DWT_CYCCNT0;
#endif
#if !ARM_DSP_EN
    if (xr) FreeMemory(xr);
    if (xi) FreeMemory(xi);
#else
    if (x_complex) FreeMemory(x_complex);
#endif    
	
#ifdef AAC_CYCLECOUNT
g_adTempAccu[38] += adTemp[39]-adTemp[38];
g_adTempAccu[39] += adTemp[40]-adTemp[39];
g_adTempAccu[40] += adTemp[41]-adTemp[40];
g_adTempAccu[41] += adTemp[42]-adTemp[41];
g_adTempAccu[42] += adTemp[43]-adTemp[42];
#endif
}

// N = 2*BLOCK_LEN_LONG
static void IMDCT( FFT_Tables *fft_tables, float *data, int N)
{
    float *xi, *xr;
    float tempr, tempi, c, s, cold, cfreq, sfreq; /* temps for pre and post twiddle */
    float freq = 2.0 * M_PI / N;
    float fac, cosfreq8, sinfreq8;
    int i;

    xi = (float*)AllocMemory((N >> 2)*sizeof(float));
    xr = (float*)AllocMemory((N >> 2)*sizeof(float));

    /* Choosing to allocate 2/N factor to Inverse Xform! */
    fac = 2. / N; /* remaining 2/N from 4/N IFFT factor */

    /* prepare for recurrence relation in pre-twiddle */
    cfreq = cos (freq);
    sfreq = sin (freq);
    cosfreq8 = cos (freq * 0.125);
    sinfreq8 = sin (freq * 0.125);
    c = cosfreq8;
    s = sinfreq8;

    for (i = 0; i < (N >> 2); i++) {
        /* calculate real and imaginary parts of g(n) or G(p) */
        tempr = -data[2 * i];
        tempi = data[(N >> 1) - 1 - 2 * i];

        /* calculate pre-twiddled FFT input */
        xr[i] = tempr * c - tempi * s;
        xi[i] = tempi * c + tempr * s;

        /* use recurrence to prepare cosine and sine for next value of i */
        cold = c;
        c = c * cfreq - s * sfreq;
        s = s * cfreq + cold * sfreq;
    }

    /* Perform in-place complex IFFT of length N/4 */
    switch (N) {
    case BLOCK_LEN_SHORT * 2:
#if 0
        arm_cfft_f32(&arm_cfft_sR_f32_len64, x_complex, 1, 1);
#else    
        ffti( fft_tables, xr, xi, 6);
#endif        
        break;
    case BLOCK_LEN_LONG * 2:
#if 0
        arm_cfft_f32(&arm_cfft_sR_f32_len512, x_complex, 1, 1);
#else    
        ffti( fft_tables, xr, xi, 9);
#endif    
    }

    /* prepare for recurrence relations in post-twiddle */
    c = cosfreq8;
    s = sinfreq8;

    /* post-twiddle FFT output and then get output data */
    for (i = 0; i < (N >> 2); i++) {

        /* get post-twiddled FFT output  */
#if 0
        tempr = fac * (x_complex[(i << 1)] * c - x_complex[(i << 1) + 1] * s);
        tempi = fac * (x_complex[(i << 1) + 1] * c + x_complex[(i << 1)] * s);
#else        
        tempr = fac * (xr[i] * c - xi[i] * s);
        tempi = fac * (xi[i] * c + xr[i] * s);
#endif
        /* fill in output values */
        data [(N >> 1) + (N >> 2) - 1 - 2 * i] = tempr;
        if (i < (N >> 3))
            data [(N >> 1) + (N >> 2) + 2 * i] = tempr;
        else
            data [2 * i - (N >> 2)] = -tempr;

        data [(N >> 2) + 2 * i] = tempi;
        if (i < (N >> 3))
            data [(N >> 2) - 1 - 2 * i] = -tempi;
        else
            data [(N >> 2) + N - 1 - 2*i] = tempi;

        /* use recurrence to prepare cosine and sine for next value of i */
        cold = c;
        c = c * cfreq - s * sfreq;
        s = s * cfreq + cold * sfreq;
    }
#if 0
    if (x_complex) FreeMemory(x_complex);
#else
    if (xr) FreeMemory(xr);
    if (xi) FreeMemory(xi);
#endif    
}
