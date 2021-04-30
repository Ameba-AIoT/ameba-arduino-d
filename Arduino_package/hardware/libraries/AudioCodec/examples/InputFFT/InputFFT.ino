#include "AudioCodec.h"
#include "FFT.h"

#define SAMPLERATE 16000
#define SAMPLECOUNT 128

int16_t audio_buffer[SAMPLECOUNT] = {0};
float fft_buffer[SAMPLECOUNT/2] = {0};
uint16_t freq_bins[SAMPLECOUNT/2] = {0};
int i = 0;

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
    if(Codec.readAvaliable()) {
        Codec.readDataPage(audio_buffer, SAMPLECOUNT);    // read latest received data from buffer
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
