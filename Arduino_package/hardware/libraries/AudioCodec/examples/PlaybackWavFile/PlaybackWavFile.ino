/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-audio-wav/
 */

// AudioCodec is not supported on core RTL8720DN nor RTL8720DF.
#if !defined(CORE_RTL8720DF) && !defined(CORE_RTL8720DN)

#include "FatFs_SD.h"
#include "PlaybackWav.h"
#include "AudioCodec.h"

char filename[] = "Test_Audio_48khz_16bit_stereo.wav";

#define BUFFERSIZE 512
int16_t buffer[BUFFERSIZE] = {0};

FatFsSD fs;
PlaybackWav playWav;

// Callback function to feed audio codec with additional data
void writeCBFunc() {
    if(Codec.writeAvaliable()) {
        playWav.readAudioData(buffer, BUFFERSIZE);
        Codec.writeDataPage(buffer, BUFFERSIZE);
    }
}

void setup() {
    Serial.begin(115200);
    char absolute_filename[128];

    fs.begin();

    sprintf(absolute_filename, "%s%s", fs.getRootPath(), filename);
    playWav.openFile(absolute_filename);

    Codec.setSampleRate(playWav.getSampleRate());
    Codec.setChannelCount(playWav.getChannelCount());
    Codec.setBitDepth(playWav.getBitDepth());
    Codec.setWriteCallback(writeCBFunc);
    Codec.begin(FALSE, TRUE);
}

void loop() {
    // If playback has reached end of file, jump to 35s
    if (playWav.getPositionMillis() == playWav.getLengthMillis()) {
        playWav.setPositionMillis(35000);
    }
    delay(100);

}

#else

void setup() {}

void loop() {}

#endif
