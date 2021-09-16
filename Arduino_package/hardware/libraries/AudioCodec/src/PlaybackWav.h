#ifndef _PLAYBACK_WAV_H_
#define _PLAYBACK_WAV_H_

#include <Arduino.h>
#include "WavFileHeader.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ff.h"

#ifdef __cplusplus
}
#endif

class PlaybackWav {
    public:
        PlaybackWav();
        ~PlaybackWav();
        void openFile(const char* absFilepath);
        void closeFile();
        bool fileOpened();

        uint32_t getSampleRate();
        uint16_t getChannelCount();
        uint16_t getBitDepth();

        uint32_t getLengthMillis();
        uint32_t getPositionMillis();
        void setPositionMillis(uint32_t pos);
        uint32_t millisToBytes(uint32_t ms);
        uint32_t bytesToMillis(uint32_t bytes);

        uint32_t readAudioData(int8_t* dst, uint32_t len);
        uint32_t readAudioData(int16_t* dst, uint32_t len);

    private:
        FIL _file;
        WavHeader _wavHeader;
        uint32_t _wavOffset = 0;    // Offset of start of audio data from file beginning
        uint32_t _posOffset = 0;
        SemaphoreHandle_t _posSemaphore;
        bool _fileReady = 0;
};

#endif
