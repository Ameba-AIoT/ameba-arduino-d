#ifndef _FATFS_FLASH_API_H
#define _FATFS_FLASH_API_H
#include "ff.h"

typedef struct fatfs_flash_param_s{
	int drv_num;
	char drv[4];
	FATFS fs;
} fatfs_flash_params_t;

int fatfs_flash_init();
int fatfs_flash_close();
int fatfs_flash_get_param(fatfs_flash_params_t *param);

#endif //_FATFS_FLASH_API_H