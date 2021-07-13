#include "platform_opts.h"
#if FATFS_DISK_FLASH
#include "platform_stdlib.h"
#include "ff.h"
#include <fatfs_ext/inc/ff_driver.h>
#include "fatfs_flash_api.h"
#include <disk_if/inc/flash_fatfs.h>

static fatfs_flash_params_t fatfs_flash_param;
static uint8_t fatfs_flash_init_done = 0;
static FIL     fatfs_flash_file;
static char* fatfs_flash_buf = NULL;
static uint32_t fatfs_flash_buf_size;
static uint32_t fatfs_flash_buf_pos;


int fatfs_flash_close()
{
	if (fatfs_flash_init_done) {
		if(f_mount(NULL,fatfs_flash_param.drv, 1) != FR_OK){
			printf("FATFS unmount flash logical drive fail.\n");
		}
		
		if(FATFS_UnRegisterDiskDriver(fatfs_flash_param.drv_num))
			printf("Unregister flash disk driver from FATFS fail.\n");

		fatfs_flash_init_done = 0;
	}
	return 0;
}

int fatfs_flash_init()
{
	int ret = 0;
	
	if (!fatfs_flash_init_done) {
		int Fatfs_ok = 0;
		FRESULT res1, res2;
		char path[64];
		char flash_test_fn[64] = "flash_drive_test.txt";
		
		// Register disk driver to Fatfs
		printf("Register flash disk driver to Fatfs.\n\r");
		fatfs_flash_param.drv_num = FATFS_RegisterDiskDriver(&FLASH_disk_Driver);

		if(fatfs_flash_param.drv_num < 0){
			printf("Register flash disk driver to FATFS fail.\n\r");
		}else{
			Fatfs_ok = 1;
			fatfs_flash_param.drv[0] = fatfs_flash_param.drv_num + '0';
			fatfs_flash_param.drv[1] = ':';
			fatfs_flash_param.drv[2] = '/';
			fatfs_flash_param.drv[3] = 0;

			printf("Flash drive path: %s\n", fatfs_flash_param.drv);
		}
		if(!Fatfs_ok) {
			ret = -1;
			goto fatfs_init_err;
		}
		res1 = f_mount(&fatfs_flash_param.fs, fatfs_flash_param.drv, 1);

		// test flash
        printf("Test flash drive (file: %s)\n\n",flash_test_fn);
        strcpy(path, fatfs_flash_param.drv);
		sprintf(&path[strlen(path)],"%s",flash_test_fn);		
		res2 = f_open(&fatfs_flash_file, path, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);
		if(res1 || res2){
			ret = f_mkfs(fatfs_flash_param.drv, 1, 4096);
			if(ret != FR_OK){
				printf("Create FAT volume on Flash fail. (%d)\n\r", ret);
				goto fatfs_init_err;
			}
			if(f_mount(&fatfs_flash_param.fs, fatfs_flash_param.drv, 0)!= FR_OK){
				printf("FATFS mount logical drive on Flash fail.\n\r");
				goto fatfs_init_err;
 			}
            printf("flash mkfs and mount OK\n\r");
		}
		else {
			printf("flash mount OK\n\r");
		}
		fatfs_flash_init_done = 1;
	}
	else {
		ret = -2;
	}
	
	return 0;
	
fatfs_init_err:
	fatfs_flash_close();
	return ret;
}

int fatfs_flash_get_param(fatfs_flash_params_t *param)
{
	if (fatfs_flash_init_done)	{
		memcpy(param, &fatfs_flash_param, sizeof(fatfs_flash_params_t));
		return 0;
	}
	else {
		memset(param, 0, sizeof(fatfs_flash_params_t));
		return -1;
	}
}

#endif //FATFS_DISK_FLASH