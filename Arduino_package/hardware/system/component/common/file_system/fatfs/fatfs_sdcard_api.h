#ifndef _FATFS_SDCARD_API_H
#define _FATFS_SDCARD_API_H
#include "ff.h"

typedef struct fatfs_sd_param_s{
	int drv_num;
	char drv[4];
	FATFS fs;
} fatfs_sd_params_t;

int fatfs_sd_is_inited(void);
int fatfs_sd_init(void);
int fatfs_sd_close(void);
int fatfs_sd_get_param(fatfs_sd_params_t *param);

int fatfs_sd_open_file(char* filename);
int fatfs_sd_close_file(void);
int fatfs_sd_create_write_buf(uint32_t buf_size);
void fatfs_sd_free_write_buf(void);
void fatfs_sd_write(char* buf, uint32_t len);
void fatfs_sd_flush_buf(void);
int fatfs_get_free_space(void);
long long int fatfs_get_free_space_byte(void);
void fatfs_list_files(void);

#endif //_FATFS_SDCARD_API_H
