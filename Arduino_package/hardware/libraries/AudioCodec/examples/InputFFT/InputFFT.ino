/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-audio-inputfft/
 */

// AudioCodec is not supported on core RTL8720DN nor RTL8720DF.
#if !defined(CORE_RTL8720DF) && !defined(CORE_RTL8720DN)

#include "AudioCodec.h"
#include "FFT.h"

#define SAMPLERATE 16000
#define SAMPLECOUNT 128
#define GAIN_FACTOR 1.0  // Gain factor (can be adjusted), Cannot be 0 as it will mute the signal

int16_t audio_buffer[SAMPLECOUNT] = {0};
float fft_buffer[SAMPLECOUNT/2] = {0};
uint16_t freq_bins[SAMPLECOUNT/2] = {0};
int i = 0;
int audioBufferMaxValue = INT16_MAX;  // 16-bit signed audio data (Refer to int16_t audio_buffer[SAMPLECOUNT])

FFT fft;

void setup() {
    Serial.begin(2000000);

    fft.setWindow(HANN, SAMPLECOUNT);
    fft.getFrequencyBins(freq_bins, SAMPLECOUNT, SAMPLERATE);

    for (i = 0; i < (SAMPLECOUNT/2); i++) {
        Serial.print(freq_bins[i]);
        Serial.print(" Hz | ");
    }
    Serial.println();
    Codec.setSampleRate(SAMPLERATE);
    Codec.begin(TRUE, FALSE);
}

void loop() {
    if (Codec.readAvaliable()) {
        Codec.readDataPage(audio_buffer, SAMPLECOUNT);    // read latest received data from buffer
        for (int i = 0; i < SAMPLECOUNT; i++) {
            audio_buffer[i] *= constrain(audio_buffer[i] * GAIN_FACTOR, -audioBufferMaxValue, audioBufferMaxValue);  // Clamping to avoid overflow and amplify each sample
        }
        
        fft.calculate(audio_buffer, fft_buffer, SAMPLECOUNT);
        for (i = 0; i < (SAMPLECOUNT/2); i++) {
            if (fft_buffer[i] > 0.01) {
                Serial.print(fft_buffer[i],2);
                Serial.print(" | ");
            } else {
                Serial.print(" - |");
            }
        }
        Serial.println();
    }
    delay(1);
}

#else

void setup() {}

void loop() {}

#endif
