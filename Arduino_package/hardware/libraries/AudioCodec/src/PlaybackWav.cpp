#include "PlaybackWav.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "ff.h"

#ifdef __cplusplus
}
#endif

PlaybackWav::PlaybackWav() {
    _posSemaphore = xSemaphoreCreateBinary();
}

PlaybackWav::~PlaybackWav() {
    vSemaphoreDelete(_posSemaphore);
    closeFile();
}

/*
void printWavHeader(WavHeader *pwavHeader){
    u8 buf[5];
    printf("================wav header===============\n");

    memcpy(buf, &pwavHeader->chunk_id[0], 4);
    buf[4] = 0;
    printf(" chunk_id: %s\n", buf);
    printf(" chunk_size: 0x%08X\n",pwavHeader->chunk_size);

    memcpy(buf, &pwavHeader->format[0], 4);
    buf[4] = 0;
    printf(" format: %s\n", buf);

    memcpy(buf, &pwavHeader->fmtchunk_id[0], 4);
    buf[4] = 0;
    printf(" fmtchunk_id: %s\n", buf);
    printf(" fmtchunk_size: 0x%08X\n",pwavHeader->fmtchunk_size);
    printf(" audio_format: 0x%04X\n",pwavHeader->audio_format);
    printf(" num_channels: 0x%04X\n",pwavHeader->num_channels);
    printf(" sample_rate: 0x%08X\n",pwavHeader->sample_rate);
    printf(" byte_rate: 0x%08X\n",pwavHeader->byte_rate);
    printf(" block_align: 0x%04X\n",pwavHeader->block_align);
    printf(" bit_depth: 0x%04X\n",pwavHeader->bps);

    memcpy(buf, &pwavHeader->datachunk_id[0], 4);
    buf[4] = 0;
    printf(" datachunk_id: %s\n", buf);
    printf(" datachunk_size: 0x%08X\n",pwavHeader->datachunk_size);
    printf("==================end====================\n");
}*/

void PlaybackWav::openFile(const char* absFilepath) {
    FRESULT res;
    uint32_t read_length = 0;

    _fileReady = 0;
    memset(&_wavHeader, 0, sizeof(WavHeader));
    // Open file as read only
    res = f_open(&_file, absFilepath, FA_OPEN_EXISTING | FA_READ);
    if (res){
        printf("Open audio file (%s) failed \n", absFilepath);
        return;
    }

    // Read WAV file header
    res = f_read(&_file, &_wavHeader, sizeof(WavHeader), (UINT*)&read_length); 
    if (res || (read_length != sizeof(WavHeader))){
        printf("Read WAV header failed \n");
        closeFile();
        return;
    }

    //printWavHeader(&_wavHeader);

    // Check for correct WAV header contents
    if (strncmp(_wavHeader.chunk_id, "RIFF", 4) || strncmp(_wavHeader.format, "WAVE", 4)) {
        printf("Not a wav file\n");
        closeFile();
        return;
    }
    if ((_wavHeader.audio_format != 1) || (_wavHeader.fmtchunk_size != 16)) {
        printf("Compressed WAV file unsupported, only PCM encoding supported \n");
        closeFile();
        return;
    }
    // Check for supported channel count, sample rate, bit depth
    if ((_wavHeader.num_channels != 1) &&
        (_wavHeader.num_channels != 2)) {
        printf("Channel count unsupported by audio codec \n");
        closeFile();
        return;
    }
    if ((_wavHeader.sample_rate != 8000) &&
        (_wavHeader.sample_rate != 16000) &&
        (_wavHeader.sample_rate != 32000) &&
        (_wavHeader.sample_rate != 48000) &&
        (_wavHeader.sample_rate != 96000) &&
        (_wavHeader.sample_rate != 44100) &&
        (_wavHeader.sample_rate != 88200)) {
        printf("Sample rate unsupported by audio codec \n");
        closeFile();
        return;
    }
    if ((_wavHeader.bps != 8) && (_wavHeader.bps != 16)) {
        // 24bit audio will require additional processing to pad each sample with zeros to 32bits for the audio codec
        printf("Bit depth unsupported by audio codec \n");
        closeFile();
        return;
    }
    if (strncmp(_wavHeader.datachunk_id, "data", 4)) {
        printf("Failed to find valid data chunk \n");
        closeFile();
        return;
    }

    _wavOffset = f_tell(&_file);
    // Indicate file is ready for read
    _fileReady = 1;
}

void PlaybackWav::closeFile() {
    _fileReady = 0;
    FRESULT res;
    // Close source file
    res = f_close(&_file);
    if (res) {
        printf("Close audio file failed : FatFs code %d \n", res);
    }
}

bool PlaybackWav::fileOpened() {
    return _fileReady;
}

uint32_t PlaybackWav::getSampleRate() {
    if (!_fileReady) {
        return 0;
    }
    return (_wavHeader.sample_rate);
}

uint16_t PlaybackWav::getChannelCount() {
    if (!_fileReady) {
        return 0;
    }
    return (_wavHeader.num_channels);
}

uint16_t PlaybackWav::getBitDepth() {
    if (!_fileReady) {
        return 0;
    }
    return (_wavHeader.bps);
}

uint32_t PlaybackWav::getLengthMillis() {
    if (!_fileReady) {
        return 0;
    }
    return bytesToMillis(_wavHeader.datachunk_size);
}

uint32_t PlaybackWav::getPositionMillis() {
    if (!_fileReady) {
        return 0;
    }
    return bytesToMillis(f_tell(&_file) - _wavOffset);
}

void PlaybackWav::setPositionMillis(uint32_t pos) {
    if (!_fileReady) {
        return;
    }
    if (pos > getLengthMillis()) {
        pos = getLengthMillis();
    }
    _posOffset = _wavOffset + millisToBytes(pos);
    xSemaphoreGive(_posSemaphore);     // Indicate pending file position change
}

uint32_t PlaybackWav::millisToBytes(uint32_t ms) {
    if (!_fileReady) {
        return 0;
    }
    uint32_t samples_per_ms = (_wavHeader.sample_rate) / 1000;
    uint32_t bytes_per_sample = ((_wavHeader.num_channels) * ((_wavHeader.bps)>>3));
    uint32_t num_samples =  ms * samples_per_ms;
    return (num_samples * bytes_per_sample);
}

uint32_t PlaybackWav::bytesToMillis(uint32_t bytes) {
    if (!_fileReady) {
        return 0;
    }
    uint32_t samples_per_ms = (_wavHeader.sample_rate) / 1000;
    uint32_t bytes_per_sample = ((_wavHeader.num_channels) * ((_wavHeader.bps)>>3));
    uint32_t num_samples = bytes / bytes_per_sample;
    return (num_samples / samples_per_ms);
}

uint32_t PlaybackWav::readAudioData(int8_t* dst, uint32_t len) {
    if (!_fileReady) {
        return 0;
    }
    FRESULT res;
    uint32_t read_length = 0;
    // Process any pending file position change
    if (xSemaphoreTake(_posSemaphore, 0) == pdTRUE) {
        res = f_lseek(&_file, _posOffset);
        if (res || (f_tell(&_file) != _posOffset)) {
            printf("Position change failed\n");
        }
    }
    // Check if at end of file, return zeroed buffer if no more data to read
    if (f_eof(&_file)) {
        memset(dst, 0, len);
        return 0;
    }
    // Check if requested data is less than remaining data
    if (len > (f_size(&_file) - f_tell(&_file))) {
        memset(dst, 0, len);
    }
    res = f_read(&_file, dst, len, (UINT*)&read_length);
    if (res) {
        printf("File read error : %d\n", res);
    }
    return read_length;
}

uint32_t PlaybackWav::readAudioData(int16_t* dst, uint32_t len) {
    return (readAudioData((int8_t*)dst, (len * 2)) / 2);
}

