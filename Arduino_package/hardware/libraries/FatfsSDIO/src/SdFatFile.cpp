
#include "SdFatFile.h"
#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
//#include "sdio_host.h"
#include <disk_if/inc/sdcard.h>
}
#endif

SdFatFile::SdFatFile() {
    m_file = NULL;
}

SdFatFile::~SdFatFile() {
#if 0
    if (m_file != NULL) {
        free(m_file);
        m_file = NULL;
    }
#endif
}

size_t SdFatFile::write(uint8_t c) {
    return write(&c, 1);
}

size_t SdFatFile::write(const uint8_t *buf, size_t size) {
    FRESULT ret = FR_OK;
    unsigned int writesize = 0;

    ret = f_write((FIL *)m_file, (const void *)buf, size, &writesize);
    if (ret != FR_OK) {
        printf("File function error. \r\n");
    }

    return writesize;
}

int SdFatFile::read() {
    FRESULT ret = FR_OK;
    char c = 0;
    unsigned int readsize = 0;

    ret = f_read((FIL *)m_file, &c, 1, &readsize);
    if (ret != FR_OK) {
        printf("File function error. \r\n");
    }

    return c;
}

int SdFatFile::peek() {
    char c = 0;
    uint32_t pos = 0;

    pos = ((FIL *)m_file)->fptr;
    c = read();
    f_lseek((FIL *)m_file, pos);

    return c;
}

uint32_t SdFatFile::cursor_pos () {
    uint32_t pos = 0;

    pos = ((FIL *)m_file)->fptr;

    return pos;
}

uint32_t SdFatFile::file_size () {
    uint32_t size = 0;

    size = ((FIL *)m_file)->fsize;

    return size;
}

int SdFatFile::available() {
    return !(f_eof((FIL *)m_file));
}

void SdFatFile::flush() {
    /* nop */
}

SdFatFile::operator bool() {
    return m_file != NULL;
}

int SdFatFile::read(void *buf, uint16_t nbyte) {
    FRESULT ret = FR_OK;
    unsigned int readsize = 0;

    ret = f_read((FIL *)m_file, buf, nbyte, &readsize);

    if (ret != FR_OK) {
        printf("File function error. \r\n");
    }

    return readsize;
}

int SdFatFile::seek(uint32_t pos) {
    FRESULT ret = FR_OK;

    ret = f_lseek((FIL *)m_file, pos);

    return (-ret);
}

int SdFatFile::close() {
    FRESULT ret = FR_OK;

    ret = f_close((FIL *)m_file);
    free(m_file);
    m_file = NULL;

    return (-ret);
}
