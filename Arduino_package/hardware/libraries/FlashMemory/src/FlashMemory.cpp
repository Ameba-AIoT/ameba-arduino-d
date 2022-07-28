#include "FlashMemory.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "flash_api.h"
#include "cmsis_os.h"

#ifdef __cplusplus
}
#endif

flash_t flash_obj;

FlashMemoryClass::FlashMemoryClass(unsigned int _base_address, unsigned int _buf_size) {
    base_address = _base_address;
    buf_size = _buf_size;

    buf = (unsigned char *) malloc ( FLASH_SECTOR_SIZE );

    pFlash = (void *)(&flash_obj);
}

FlashMemoryClass::~FlashMemoryClass() {
    if (buf != NULL) {
        free(buf);
        buf = NULL;
    }
}

void FlashMemoryClass::begin(unsigned int _base_address, unsigned int _buf_size) {
    base_address = _base_address;
    buf_size = _buf_size;
}

void FlashMemoryClass::read() {
    flash_stream_read((flash_t *)pFlash, base_address, FLASH_SECTOR_SIZE, buf);
}

void FlashMemoryClass::update(bool erase) {
    if (erase) {
        flash_erase_sector((flash_t *)pFlash, base_address);
    }
    flash_stream_write((flash_t *)pFlash, base_address, FLASH_SECTOR_SIZE, buf);
}

unsigned int FlashMemoryClass::readWord(unsigned int offset) {
    unsigned int value;

    flash_read_word((flash_t *)pFlash, base_address + offset, (uint32_t *)&value);

    return value;
}

void FlashMemoryClass::writeWord(unsigned int offset, unsigned int data) {
    unsigned int value;
    unsigned char *tmpbuf;
    unsigned int *tmpvalue;
    unsigned int sector;

    flash_write_word((flash_t *)pFlash, base_address + offset, data);

    flash_read_word((flash_t *)pFlash, base_address + offset, (uint32_t *)&value);
    if (value != data) {
        sector = ((base_address + offset) / FLASH_SECTOR_SIZE ) * FLASH_SECTOR_SIZE;
        tmpbuf = (unsigned char *) malloc (FLASH_SECTOR_SIZE);

        flash_stream_read((flash_t *)pFlash, sector, FLASH_SECTOR_SIZE, tmpbuf);

        flash_erase_sector((flash_t *)pFlash, sector);

        tmpvalue = (unsigned int *) (tmpbuf + base_address + offset - sector);
        *tmpvalue = data;

        flash_stream_write((flash_t *)pFlash, sector, FLASH_SECTOR_SIZE, tmpbuf);

        free(tmpbuf);
    }
}

FlashMemoryClass FlashMemory = FlashMemoryClass(FLASH_MEMORY_APP_BASE, FLASH_SECTOR_SIZE);
