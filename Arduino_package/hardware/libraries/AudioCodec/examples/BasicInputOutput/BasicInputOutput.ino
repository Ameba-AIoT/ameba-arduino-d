/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-audio-basic/
 */

#include "AudioCodec.h"

#define SAMPLECOUNT 512

int16_t buffer[SAMPLECOUNT] = {0};
unsigned int vol = 0;

void setup() {
    Codec.setSampleRate(16000);
    Codec.begin(TRUE, TRUE);
}

void loop() {
    // if received data is available and transmit data buffer is available for write
    if(Codec.writeAvaliable() && Codec.readAvaliable()) {
        Codec.readDataPage(buffer, SAMPLECOUNT);    // read latest received data from buffer
        Codec.writeDataPage(buffer, SAMPLECOUNT);   // write latest data into transmit data buffer
    }
    vol = analogRead(A0);
    vol = map(vol, 0, 1024, 0, 100);
    Codec.setOutputVolume(vol, vol);
}
