/*

 Example guide:
 https://www.amebaiot.com/en/amebad-arduino-audio-wav/
 */

// AudioCodec is not supported on core RTL8720DN nor RTL8720DF.
#if !defined(CORE_RTL8720DF) && !defined(CORE_RTL8720DN)

#include "FatFs_SD.h"
#include "RecordWav.h"
#include "AudioCodec.h"

#define RECORDBTN 9
#define SAMPLERATE 48000
#define MICTYPE PDMMIC // Analog mic: ANALOGMIC, Digital mic: PDMMIC

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
        Codec.amplifyReadData(buffer, BUFFERSIZE, 100); // to amplify the input audio (max: 100)
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

    if (MICTYPE) {
      Codec.setInputMicType(PDMMIC);
      Codec.setDMicBoost(3, 3);      
    }
    else {
      Codec.setInputMicType(ANALOGMIC);
      Codec.setAMicBoost(3, 3);      
    }
    Codec.setADCGain(127, 127);
}

void loop() {
    if ((digitalRead(RECORDBTN) == HIGH) && (!recWav.fileOpened())) {
        Serial.println("Recording started");
        recWav.openFile(absolute_filename);
        Codec.begin(TRUE, FALSE);
    } else if ((digitalRead(RECORDBTN) == LOW) && (recWav.fileOpened())) {
        Codec.end();
        recWav.closeFile();
        Serial.println("Recording stopped");
    }
    delay(100);
}

#else

void setup() {}

void loop() {}

#endif
