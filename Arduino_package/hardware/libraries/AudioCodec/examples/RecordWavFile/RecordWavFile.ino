#include "FatFs_SD.h"
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

// Callback function to read Microphone data from audio codec
void readCBFunc() {
    if (Codec.readAvaliable()) {
        Codec.readDataPage(buffer, BUFFERSIZE);
        recWav.writeAudioData(buffer, BUFFERSIZE);
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
    Codec.begin(TRUE, FALSE);
}

void loop() {
    if ((digitalRead(RECORDBTN) == HIGH) && (!recWav.fileOpened())) {
        Serial.println("Recording started");
        recWav.openFile(absolute_filename);
    } else if((digitalRead(RECORDBTN) == LOW) && (recWav.fileOpened())) {
        Serial.println("Recording stopped");
        recWav.closeFile();
    }
    delay(100);

}
