#include "RecordWav.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ff.h"

#ifdef __cplusplus
}
#endif

RecordWav::RecordWav() {
    strcpy(_wavHeader.chunk_id, "RIFF");
    strcpy(_wavHeader.format, "WAVE");
    strcpy(_wavHeader.fmtchunk_id, "fmt ");
    _wavHeader.fmtchunk_size = 16;
    _wavHeader.audio_format = 1;
    strcpy(_wavHeader.datachunk_id, "data");
    // Assume default audio codec parameters
    _wavHeader.num_channels = 1;
    _wavHeader.sample_rate = 48000;
    _wavHeader.bps = 16;
}

RecordWav::~RecordWav() {
    closeFile();
}

void RecordWav::openFile(const char* absFilepath) {
    FRESULT res;
    uint32_t write_length = 0;

    _fileReady = 0;
    // Open file, new file will be created if none exist, existing file will be truncated and overwritten
    res = f_open(&_file, absFilepath, FA_OPEN_ALWAYS | FA_READ | FA_WRITE | FA_CREATE_ALWAYS);
    if (res){
        printf("Open audio file (%s) failed \n", absFilepath);
        return;
    }

    // Create WAV file header
    _wavHeader.block_align = _wavHeader.num_channels * ((_wavHeader.bps)>>3);
    _wavHeader.byte_rate = _wavHeader.num_channels * _wavHeader.sample_rate;
    _wavHeader.datachunk_size = 0;
    _wavHeader.chunk_size = 4 + (8 + _wavHeader.fmtchunk_size) + (8 + _wavHeader.datachunk_size);

    res = f_write(&_file, &_wavHeader, sizeof(WavHeader), (UINT*)&write_length);
    if (res || (write_length != sizeof(WavHeader))){
        f_lseek(&_file, 0); 
        printf("Header write error\n");
        closeFile();
        return;
    }
    _wavOffset = f_tell(&_file);
    // Indicate file is ready for write
    _fileReady = 1;
}

void RecordWav::updateWavHeader() {
    FRESULT res;
    uint32_t current_fptr = f_tell(&_file);
    uint32_t write_length = 0;

    // Recalculate parameters and update wave file header
    _wavHeader.block_align = _wavHeader.num_channels * ((_wavHeader.bps)>>3);
    _wavHeader.byte_rate = _wavHeader.num_channels * _wavHeader.sample_rate;
    _wavHeader.chunk_size = 4 + (8 + _wavHeader.fmtchunk_size) + (8 + _wavHeader.datachunk_size);

    // Move to start of file
    res = f_lseek(&_file, 0);
    // Write updated wave file header
    res = f_write(&_file, &_wavHeader, sizeof(WavHeader), (UINT*)&write_length);
    if (res || (write_length != sizeof(WavHeader))) {
        f_lseek(&_file, 0); 
        printf("Header write error\n");
        return;
    }
    // Return to original file position
    res = f_lseek(&_file, current_fptr);
}

void RecordWav::closeFile() {
    if (_fileReady) {
        _fileReady = 0;
        updateWavHeader();
        FRESULT res;
        // Close source file
        res = f_close(&_file);
        if (res) {
            printf("Close audio file failed : FatFs code %d \n", res);
        }
    }
}

bool RecordWav::fileOpened() {
    return _fileReady;
}

void RecordWav::setSampleRate(uint32_t sampleRate) {
   _wavHeader.sample_rate = sampleRate;
}

void RecordWav::setChannelCount(uint16_t channelCount) {
    _wavHeader.num_channels = channelCount;
}

void RecordWav::setBitDepth(uint16_t bitDepth) {
    _wavHeader.bps = bitDepth;
}

uint32_t RecordWav::getLengthMillis() {
    if (!_fileReady) {
        return 0;
    }
    return bytesToMillis(_wavHeader.datachunk_size);
}

uint32_t RecordWav::millisToBytes(uint32_t ms) {
    uint32_t samples_per_ms = (_wavHeader.sample_rate) / 1000;
    uint32_t bytes_per_sample = ((_wavHeader.num_channels) * ((_wavHeader.bps)>>3));
    uint32_t num_samples =  ms * samples_per_ms;
    return (num_samples * bytes_per_sample);
}

uint32_t RecordWav::bytesToMillis(uint32_t bytes) {
    uint32_t samples_per_ms = (_wavHeader.sample_rate) / 1000;
    uint32_t bytes_per_sample = ((_wavHeader.num_channels) * ((_wavHeader.bps)>>3));
    uint32_t num_samples = bytes / bytes_per_sample;
    return (num_samples / samples_per_ms);
}

uint32_t RecordWav::writeAudioData(int8_t* src, uint32_t len) {
    if (!_fileReady) {
        return 0;
    }
    FRESULT res;
    uint32_t write_length = 0;
    res = f_write(&_file, src, len, (UINT*)&write_length);
    if (res || (write_length != len)) {
        printf("File write error : %d\n", res);
    }
    _wavHeader.datachunk_size += write_length;
    return write_length;
}

uint32_t RecordWav::writeAudioData(int16_t* src, uint32_t len) {
    return (writeAudioData((int8_t*)src, (len * 2)) / 2);
}

