#ifndef _RECORD_WAV_H_
#define _RECORD_WAV_H_

#include <Arduino.h>
#include "WavFileHeader.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ff.h"

#ifdef __cplusplus
}
#endif

class RecordWav {
    public:
        RecordWav();
        ~RecordWav();
        void openFile(const char* absFilepath);
        void closeFile();
        bool fileOpened();

        void setSampleRate(uint32_t sampleRate);
        void setChannelCount(uint16_t channelCount);
        void setBitDepth(uint16_t bitDepth);

        uint32_t getLengthMillis();
        uint32_t millisToBytes(uint32_t ms);
        uint32_t bytesToMillis(uint32_t bytes);

        uint32_t writeAudioData(int8_t* src, uint32_t len);
        uint32_t writeAudioData(int16_t* src, uint32_t len);

    private:
        void updateWavHeader();
        FIL _file;
        WavHeader _wavHeader; // Pointer to a valid wav header after file is open, nullptr after file is closed
        uint32_t _wavOffset = 0;    // Offset of start of audio data from file beginning
        bool _fileReady = 0;
};

#endif
