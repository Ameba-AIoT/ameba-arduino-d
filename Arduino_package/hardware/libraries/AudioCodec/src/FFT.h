#ifndef _AUDIO_CODEC_FFT_H_
#define _AUDIO_CODEC_FFT_H_

#ifdef __cplusplus
extern "C" {
#endif

// Undef to resolve conflict with some arm_math.h variable names
#undef A0
#undef A1
#undef A2
#include "arm_math.h"

#ifdef __cplusplus
}
#endif

typedef enum {
    NONE = 0,
    HANN = 1,
    HAMMING = 2
} FFTWindow_t;

class FFT {
    public:
        FFT();
        ~FFT();
        
        // Set window function for FFT calculations
        void setWindow(FFTWindow_t window, uint16_t sampleCount);

        // Calculate FFT for an array of sampleCount samples in the input buffer, returns sampleCount/2 FFT results in outputBuf array
        void calculate(float* inputBuf, float* outputBuf, uint16_t sampleCount);
        void calculate(int16_t* inputBuf, float* outputBuf, uint16_t sampleCount);
        
        // Get the corresponding FFT frequency bins for a given sample rate and sample count
        void getFrequencyBins(uint16_t* outputBuf, uint16_t sampleCount, uint32_t sampleRate);
        uint16_t getFFTSize(uint16_t sampleCount);

    private:
        arm_rfft_fast_instance_f32 rfft;
        uint16_t _window = 0;
        float* _windowFunc = NULL;
};

#endif
