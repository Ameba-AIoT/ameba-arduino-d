/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-audio-sinewave/
 */

#include "AudioCodec.h"

#define SAMPLERATE 16000
#define SAMPLECOUNT 512
#define FREQUENCY 1000

int16_t sine_buffer[SAMPLECOUNT] = {0};

void setup() {
    // generate sine wave and fill buffer with required samples
    double delta_t = 1.0/SAMPLERATE;
    double current_t = 0;
    for (int i = 0; i < SAMPLECOUNT; i++) {
        sine_buffer[i] = (int16_t)(sin(2 * PI  * FREQUENCY * current_t) * (pow(2, 15) - 1));
        current_t += delta_t;
    }

    Codec.setSampleRate(SAMPLERATE);
    Codec.begin(FALSE, TRUE);           // configure audio codec for output only
}

void loop() {
    if (Codec.writeAvaliable()) {
        Codec.writeDataPage(sine_buffer, SAMPLECOUNT);
    }
    delay(1);
}
