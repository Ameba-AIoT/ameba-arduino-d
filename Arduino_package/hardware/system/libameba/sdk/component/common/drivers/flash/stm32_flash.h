#ifndef __STM32_FLASH_H__
#define __STM32_FLASH_H__

#include <stdint.h>

uint32_t flash_SectorAddress(uint16_t sector_nb);
#if defined STM32F2XX || STM32F4XX 
int flash_EraseSector(uint16_t sector_nb);
#elif defined STM32f1xx
int flash_EraseSector(uint32_t Page_Address);
#endif
int flash_Wrtie(uint32_t address, char* buf, uint32_t len);
int flash_Read(uint32_t address, char* buf, uint32_t len);

#endif // __STM32_FLASH_H__
