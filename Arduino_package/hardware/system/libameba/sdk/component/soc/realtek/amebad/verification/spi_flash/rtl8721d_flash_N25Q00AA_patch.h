#ifndef _RTL8195A_FLASH_N25Q00AA_PATCH_H_
#define _RTL8195A_FLASH_N25Q00AA_PATCH_H_

#include "device.h"

typedef struct flash_s flash_t;

/**
  * global data structure
  */   
extern flash_t	        flash;

void FLASH_ReadFlagStatusReg(void);
void FLASH_SelectSegment(IN u8 SegNum);
void FLASH_EraseDie(u32 Address);

void N25Q00AA_erase_sector(flash_t *obj, u32 address);
void N25Q00AA_erase_block(flash_t *obj, u32 address);
void N25Q00AA_erase_chip(flash_t *obj);
u32  N25Q00AA_read_word(flash_t *obj, u32 address, u32 * data);
u32  N25Q00AA_write_word(flash_t *obj, u32 address, u32 data);
u32  N25Q00AA_stream_read(flash_t *obj, u32 address, u32 len, u8 * data);
u32  N25Q00AA_stream_write(flash_t *obj, u32 address, u32 len, u8 * data);

#endif
