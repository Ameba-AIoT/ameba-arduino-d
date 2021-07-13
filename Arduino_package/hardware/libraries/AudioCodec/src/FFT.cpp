#include "FFT.h"

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

#undef PI
#include <Arduino.h>

FFT::FFT() {
}

FFT::~FFT() {
    free(_windowFunc);
}

void FFT::setWindow(FFTWindow_t window, uint16_t sampleCount) {
    uint16_t i;
    if (window != NONE) {
        _windowFunc = (float*)realloc(_windowFunc, sampleCount * sizeof(float));
        if (_windowFunc == NULL) {
            printf("Window allocate buffer failed\n");
            return;
        }
    }
    switch(window) {
        case HANN: // Generate Hann window
            _window = 1;
            for (i = 0; i < sampleCount; i++) {
                _windowFunc[i] = 0.5 - 0.5 * arm_cos_f32((2 * PI * i) / (sampleCount - 1));
            }
            break;
        case HAMMING: // Generate Hamming window
            _window = 1;
            for (i = 0; i < sampleCount; i++) {
                _windowFunc[i] = 0.53836 - 0.46164 * arm_cos_f32((2 * PI * i) / (sampleCount - 1));
            }
            break;
        case NONE: // Default rectangular window
        default:
            _window = 0;
            free(_windowFunc);
            break;
    }
}

void FFT::calculate(float* inputBuf, float* outputBuf, uint16_t sampleCount) {
    if (arm_rfft_fast_init_f32(&rfft, sampleCount) == ARM_MATH_ARGUMENT_ERROR) {
        printf("Invalid sample count parameter for FFT\n");
        return;
    }
    if (_window != 0) {
        // Apply window function
        arm_mult_f32(inputBuf, _windowFunc,inputBuf, sampleCount);
    }
    float* _fftOutput = (float*)malloc(sampleCount * sizeof(float));
    uint16_t _fftSize = sampleCount>>1;
    arm_rfft_fast_f32(&rfft, inputBuf, _fftOutput, 0);
    // Handle special case
    float first_energy = sqrt(_fftOutput[0] * _fftOutput[0]);
    float last_energy =  sqrt(_fftOutput[1] * _fftOutput[1]);
    // Get magnitudes of complex FFT output
    arm_cmplx_mag_f32(_fftOutput, outputBuf, _fftSize);
    // Handle special case
    outputBuf[0] = first_energy;
    outputBuf[_fftSize] = last_energy;

    arm_scale_f32(outputBuf, (1.0/_fftSize), outputBuf, _fftSize);
    free(_fftOutput);
}

void FFT::calculate(int16_t* inputBuf, float* outputBuf, uint16_t sampleCount) {
    float* buffer = (float*)malloc(sampleCount * sizeof(float));
    if (buffer == NULL) {
        printf("FFT allocate buffer failed\n");
    }
    for(int i=0; i<sampleCount; i++) {
        buffer[i] = ((float)(inputBuf[i])) / (1<<15);
    }
    calculate(buffer, outputBuf, sampleCount);
    free(buffer);
}

void FFT::getFrequencyBins(uint16_t* outputBuf, uint16_t sampleCount, uint32_t sampleRate) {
    uint16_t i;
    for (i = 0; i < (sampleCount/2); i++) {
        outputBuf[i] = i * sampleRate / sampleCount;
    }
}

uint16_t FFT::getFFTSize(uint16_t sampleCount) {
    return (sampleCount/2);
}

