
#include "FatFs_SD.h"
#include "Arduino.h"

#ifdef __cplusplus
extern "C" {
#include "ff.h"
#include <fatfs_ext/inc/ff_driver.h>
//#include "sdio_host.h"
#include <disk_if/inc/sdcard.h>
}
#endif

#define TEST_SIZE   (512)

char WRBuf[TEST_SIZE];
char RDBuf[TEST_SIZE];

int sdioInitErr = FR_OK;

FatFsSD::FatFsSD() {
    m_fs = NULL;
    drv_num = -1;
    logical_drv[0] = '0';
    logical_drv[1] = ':';
    logical_drv[2] = '/';
    logical_drv[3] = '\0';
//    if (sdio_init_host() != 0) {
//        printf("SDIO host init fail.\n");
//        sdioInitErr = FR_DISK_ERR;
//    }
}

FatFsSD::~FatFsSD() {
//    sdio_deinit_host();
}

int FatFsSD::begin() {
    FRESULT ret = FR_OK;

    do {
        m_fs = (FATFS *) malloc (sizeof(FATFS));
        if (m_fs == NULL) {
            ret = FR_INT_ERR;
            break;
        }

        if(sdioInitErr == FR_DISK_ERR)
            break;

        drv_num = FATFS_RegisterDiskDriver(&SD_disk_Driver);
        if (drv_num < 0) {
            printf("Rigester disk driver to FATFS fail.\n");
            ret = FR_DISK_ERR;
            break;
        }

        logical_drv[0] += drv_num;

        ret = f_mount((FATFS *)m_fs, logical_drv, 1);
        if (ret != FR_OK){
            printf("FATFS mount logical drive fail:%d\n", ret);
            break;
        }

    } while (0);

    if (ret != FR_OK) {
        drv_num = -1;
    }

    return (-(int)ret);
}

int FatFsSD::end() {
    FRESULT ret = FR_OK;

    ret = f_mount(NULL, logical_drv, 1);
    if (ret != FR_OK) {
        printf("FATFS unmount logical drive fail.\n");
    }

    if (FATFS_UnRegisterDiskDriver(drv_num)) {
        ret = FR_DISK_ERR;
        printf("Unregister disk driver from FATFS fail.\n");
    }

    if (m_fs != NULL) {
        free(m_fs);
        m_fs = NULL;
    }

    drv_num = -1;

    return (-ret);
}

char *FatFsSD::getRootPath() {
    if (drv_num < 0) {
        return NULL;
    } else {
        return logical_drv;
    }
}

int FatFsSD::readDir(char *path, char *result_buf, unsigned int bufsize) {
    FRESULT ret = FR_OK;
    FILINFO fno;
    DIR dir;

    char *fn;
    unsigned int fnlen;
    int bufidx = 0;

#if _USE_LFN
    char lfn[(_MAX_LFN + 1)];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    do {
        if (drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        ret = f_opendir(&dir, path);
        if (ret != FR_OK) {
            break;
        }

        memset(result_buf, 0, bufsize);

        while (1) {
            ret = f_readdir(&dir, &fno);
            if ((ret != FR_OK) || (fno.fname[0] == 0)) {
                break;
            }

#if _USE_LFN
            if (*fno.lfname)
            {
                fn = fno.lfname;
                fnlen = fno.lfsize;
            }
            else
#endif
            {
                fn = fno.fname;
                fnlen = strlen(fn);
            }

            if ((bufidx + fnlen + 1) < bufsize) {
                bufidx += sprintf((result_buf + bufidx), "%s", fn);
                bufidx++;
            }
        }
    } while (0);

    return (-ret);
}

int FatFsSD::mkdir(char *absolute_path) {
    FRESULT ret = FR_OK;

    do {
        if (drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        ret = f_mkdir(absolute_path);
        if (ret != FR_OK) {
            break;
        }
    } while (0);

    return ret;
}

int FatFsSD::rm(char *absolute_path) {
    FRESULT ret = FR_OK;

    do {
        if (drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        ret = f_unlink(absolute_path);
        if (ret != FR_OK) {
            break;
        }
    } while (0);

    return ret;
}

unsigned char FatFsSD::isDir(char *absolute_path) {
    unsigned char attr;
    if (getAttribute(absolute_path, &attr) >= 0) {
        if (attr & AM_DIR) {
            return 1;
        }
    }
    return 0;
}

unsigned char FatFsSD::isFile(char *absolute_path) {
    unsigned char attr;
    if (getAttribute(absolute_path, &attr) >= 0) {
        if (attr & AM_ARC) {
            return 1;
        }
    }
    return 0;
}

SdFatFile FatFsSD::open(char *absolute_path) {
    FRESULT ret = FR_OK;
    SdFatFile file;

    do {
        if (drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        file.m_file = (FIL *)malloc (sizeof(FIL));
        if (file.m_file == NULL) {
            ret = FR_INT_ERR;
            break;
        }

        ret = f_open((FIL *)file.m_file, absolute_path, FA_OPEN_ALWAYS | FA_READ | FA_WRITE);

        if (ret != FR_OK) {
            printf("open file (%s) fail. (ret=%d)\n", absolute_path, ret);
            break;
        }
    } while (0);

    if (ret != FR_OK) {
        if (file.m_file != NULL) {
            free(file.m_file);
            file.m_file = NULL;
        }
    }

    return file;
}

int FatFsSD::status() {
    //return sdio_sd_status() == 4;
    return 1;
}

int FatFsSD::getLastModTime(char *absolute_path, uint16_t *year, uint16_t *month, uint16_t *date, uint16_t *hour, uint16_t *minute, uint16_t *second) {
    FRESULT ret = FR_OK;

    FILINFO fno;
#if _USE_LFN
    char lfn[(_MAX_LFN + 1)];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    do {
        if (drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        ret = f_stat(absolute_path, &fno);
        if (ret != FR_OK) {
            break;
        }

        *year   = (fno.fdate >> 9) + 1980;
        *month  = (fno.fdate >> 5) & 0x0F;
        *date   = (fno.fdate & 0x1F);
        *hour   = (fno.ftime >> 11);
        *minute = (fno.ftime >> 5) & 0x3F;
        *second = 0;

    } while (0);

    return (-ret);
}

int FatFsSD::setLastModTime(char *absolute_path, uint16_t year, uint16_t month, uint16_t date, uint16_t hour, uint16_t minute, uint16_t second) {
    (void)second;
    FRESULT ret = FR_OK;
    FILINFO fno;
#if _USE_LFN
    char lfn[(_MAX_LFN + 1)];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    //int scan_count = 0;

    do {
        if (drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        fno.fdate = 0x0000 | ((year - 1980) <<  9) | ((month  & 0x0F) << 5) | (date & 0x1F);
        fno.ftime = 0x0000 | ((hour & 0x1F) << 11) | ((minute & 0x3F) << 5);
        ret = f_utime(absolute_path, &fno);
        if (ret != FR_OK) {
            break;
        }
    } while (0);

    return (-ret);
}

int FatFsSD::getAttribute(char *absolute_path, unsigned char *attr) {
    FRESULT ret = FR_OK;
    FILINFO fno;
#if _USE_LFN
    char lfn[(_MAX_LFN + 1)];
    fno.lfname = lfn;
    fno.lfsize = sizeof(lfn);
#endif

    do {
        if (drv_num < 0) {
            ret = FR_DISK_ERR;
            break;
        }

        ret = f_stat(absolute_path, &fno);
        if (ret != FR_OK) {
            break;
        }

        *attr = fno.fattrib;
    } while (0);

    return -ret;
}
