/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-audio-wav/
 */

// AudioCodec is not supported on core RTL8720DN nor RTL8720DF.
#if !defined(CORE_RTL8720DF) && !defined(CORE_RTL8720DN)

#include "FatFs_SD.h"
#include "PlaybackWav.h"
#include "RecordWav.h"
#include "AudioCodec.h"

#define RECORDBTN 9
#define SAMPLERATE 48000

#define BUFFERSIZE 512
int16_t buffer[BUFFERSIZE] = {0};

char filename[] = "Test_Recording_48khz_16bit_mono.wav";
char absolute_filename[128];

FatFsSD fs;
RecordWav recWav;
PlaybackWav playWav;

void readCBFunc() {
    if (Codec.readAvaliable() && recWav.fileOpened()) {
        Codec.readDataPage(buffer, BUFFERSIZE);
        recWav.writeAudioData(buffer, BUFFERSIZE);
    }
}

void writeCBFunc() {
    if (Codec.writeAvaliable() && playWav.fileOpened()) {
        playWav.readAudioData(buffer, BUFFERSIZE);
        Codec.writeDataPage(buffer, BUFFERSIZE);
    }
}

void setup() {
    Serial.begin(115200);

    fs.begin();
    sprintf(absolute_filename, "%s%s", fs.getRootPath(), filename);

    pinMode(RECORDBTN, INPUT);

    Codec.setSampleRate(SAMPLERATE);
    recWav.setSampleRate(SAMPLERATE);
    Codec.setReadCallback(readCBFunc);
    Codec.setWriteCallback(writeCBFunc);
    Codec.begin(TRUE, TRUE);
}

void loop() {
    if ((digitalRead(RECORDBTN) == HIGH) && (!recWav.fileOpened())) {
        playWav.closeFile();
        Serial.println("Replay stopped, Recording started");
        recWav.openFile(absolute_filename);
    } else if((digitalRead(RECORDBTN) == LOW) && (recWav.fileOpened())) {
        recWav.closeFile();
        Serial.println("Recording stopped, Replay starting");
        playWav.openFile(absolute_filename);
    }
    if (playWav.fileOpened()) {
        if (playWav.getPositionMillis() == playWav.getLengthMillis()) {
            playWav.setPositionMillis(0);
        }
    }
    delay(100);
}

#endif
