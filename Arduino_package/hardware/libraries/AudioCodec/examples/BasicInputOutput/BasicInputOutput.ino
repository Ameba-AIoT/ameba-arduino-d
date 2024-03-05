/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-audio-basic/
 */

#include "AudioCodec.h"

//State the analog pin that you want to read
#ifdef BOARD_AITHINKER_BW16
int analogChannel = A2;  // on the BW16 only A2 is available
#elif BOARD_AMB25 || BOARD_AMB26
int analogChannel = A4;  // on the AMB25/AMB26 only A4 A5 A6 are available
#else
int analogChannel = A0;  // Default using A0
#endif

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
    vol = analogRead(analogChannel);
    vol = map(vol, 0, 1024, 0, 100);
    Codec.setOutputVolume(vol, vol);
}
