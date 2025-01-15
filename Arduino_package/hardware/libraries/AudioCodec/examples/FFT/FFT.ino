/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-audio-fft/
 */

// AudioCodec is not supported on core RTL8720DN nor RTL8720DF.
#if !defined(CORE_RTL8720DF) && !defined(CORE_RTL8720DN)

#include "FFT.h"

#define SAMPLERATE 16000
#define SAMPLECOUNT 1024
#define FREQUENCY1 1000
#define FREQUENCY2 4096

float sine_buffer[SAMPLECOUNT] = {0};
float fft_buffer[SAMPLECOUNT/2] = {0};
uint16_t freq_bins[SAMPLECOUNT/2] = {0};

FFT testfft;

void setup() {
    Serial.begin(115200);
    int i = 0;

    // generate sine wave and fill buffer with required samples
    double delta_t = 1.0/SAMPLERATE;
    double current_t = 0;
    for(int i = 0; i < SAMPLECOUNT; i++){
        sine_buffer[i] = (sin(2 * PI  * FREQUENCY1 * current_t)) + (sin(2 * PI  * FREQUENCY2 * current_t));
        current_t += delta_t;
    }

    testfft.setWindow(HANN, SAMPLECOUNT);
    testfft.calculate(sine_buffer, fft_buffer, SAMPLECOUNT);
    testfft.getFrequencyBins(freq_bins, SAMPLECOUNT, SAMPLERATE);

    for (i = 0; i < (SAMPLECOUNT/2); i++) {
        Serial.print(freq_bins[i]);
        Serial.print(" Hz : ");
        Serial.println(fft_buffer[i], 6);
    }
}

void loop() {
    delay(1);
}

##else

void setup() {}

void loop() {}

#endif

