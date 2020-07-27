/*
 * buffered file I/O
 * Copyright (c) 2001 Fabrice Bellard
 *
 * This file is part of Libav.
 *
 * Libav is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * Libav is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with Libav; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

__attribute__((weak)) int errno = 0;

#include "libavutil/avstring.h"
#include "libavutil/internal.h"
#include "libavutil/opt.h"
#include "avformat.h"
#if !(defined(CONFIG_PLATFORM_8195A)||defined(CONFIG_PLATFORM_8721D))
#include <fcntl.h>
#endif
#if HAVE_IO_H
#include <io.h>
#endif
#if HAVE_UNISTD_H
#include <unistd.h>
#endif
#if !(defined(CONFIG_PLATFORM_8195A)||defined(CONFIG_PLATFORM_8721D))
#include <sys/stat.h>
#endif
#include <stdlib.h>
#include "os_support.h"
#include "url.h"

#if CONFIG_FILE_PROTOCOL
#if defined(CONFIG_PLATFORM_8195A)||defined(CONFIG_PLATFORM_8721D)
#include "ff.h"
#include <fatfs_ext/inc/ff_driver.h>
#include <disk_if/inc/sdcard.h>
char file_logical_drv[4]; //root diretor
static int file_drv_num;
static FATFS file_fs;
static char file_abs_path[32]; //Path to input file
static FIL file_fd;
static int avpriv_open(const char *filename, int flags, int mode)
{
    FRESULT res;
    file_drv_num = FATFS_RegisterDiskDriver(&SD_disk_Driver);
    if(file_drv_num < 0){
        printf("Rigester disk driver to FATFS fail.\n");
        return -1;
    }else{
        file_logical_drv[0] = file_drv_num + '0';
        file_logical_drv[1] = ':';
        file_logical_drv[2] = '/';
        file_logical_drv[3] = 0;
    }
    if(f_mount(&file_fs, file_logical_drv, 1) != FR_OK){
        printf("FATFS mount logical drive fail, please format DISK to FAT16/32.\n");
        return -1;
    }
    rtw_memset(file_abs_path, 0x00, sizeof(file_abs_path));
    strcpy(file_abs_path, file_logical_drv);
    sprintf(&file_abs_path[strlen(file_abs_path)],"%s", filename);
    res = f_open(&file_fd, file_abs_path, FA_OPEN_EXISTING | FA_READ); // open read only file
    if(res != FR_OK){
        printf("Open source file %s fail.\n", filename);
        return -1;
    } 
    //printf("%s: open file %s success\n", __func__, filename);
    return (int)&file_fd;
}
#endif
#endif
/* standard file protocol */

typedef struct FileContext {
    const AVClass *class;
    int fd;
    int trunc;
    int follow;
} FileContext;

static const AVOption file_options[] = {
    { "truncate", "Truncate existing files on write", offsetof(FileContext, trunc), AV_OPT_TYPE_INT, { .i64 = 1 }, 0, 1, AV_OPT_FLAG_ENCODING_PARAM },
    { "follow", "Follow a file as it is being written", offsetof(FileContext, follow), AV_OPT_TYPE_INT, { .i64 = 0 }, 0, 1, AV_OPT_FLAG_DECODING_PARAM },
    { NULL }
};

static const AVClass file_class = {
    .class_name = "file",
    .item_name  = av_default_item_name,
    .option     = file_options,
    .version    = LIBAVUTIL_VERSION_INT,
};

static int file_read(URLContext *h, unsigned char *buf, int size)
{
    FileContext *c = h->priv_data;
#if defined(CONFIG_PLATFORM_8195A)||defined(CONFIG_PLATFORM_8721D)
    int ret = 0;
    FIL *file = (FIL *)c->fd;
    if(file == NULL)
        return -1;
#if 0
    if(size > 0x800){
        printf("%s: file fd 0x%x size 0x%x fsize 0x%x fptr 0x%x\n", __func__, file, size, file->fsize, file->fptr);
        printf("###########TOO LARGE###########\n");
    }
    printf("%s: file fd 0x%x size 0x%x fsize 0x%x fptr 0x%x\n", __func__, file, size, file->fsize, file->fptr);
#endif
    FRESULT res = f_read(file, buf, size,(UINT*)&ret);
    if(res != FR_OK){
        printf("read source file fail.\n");
        ret = -1;
    }
    /*
    if(ret){
        __rtl_memDump_v1_00(buf, size, "FILE READ");
        f_lseek(file, file->fptr + size);
    }
    */
    return ret;
#else
    int ret = read(c->fd, buf, size);
    if (ret == 0 && c->follow)
        return AVERROR(EAGAIN);
    return (ret == -1) ? AVERROR(errno) : ret;
#endif
}

static int file_write(URLContext *h, const unsigned char *buf, int size)
{
    FileContext *c = h->priv_data;
#if defined(CONFIG_PLATFORM_8195A)||defined(CONFIG_PLATFORM_8721D)
    printf("%s: not implemented yet\n", __func__);
    int ret =  size;
#else
    int ret = write(c->fd, buf, size);
#endif
    return (ret == -1) ? AVERROR(errno) : ret;
}

static int file_get_handle(URLContext *h)
{
    FileContext *c = h->priv_data;
    return c->fd;
}

static int file_check(URLContext *h, int mask)
{
#if defined(CONFIG_PLATFORM_8195A)||defined(CONFIG_PLATFORM_8721D)
    printf("%s: not implemented yet\n", __func__);
    return 0;
#else
    const char *filename = h->filename;
    struct stat st;
    int ret;

    av_strstart(filename, "file:", &filename);

    ret = stat(filename, &st);
    if (ret < 0)
        return AVERROR(errno);

    ret |= st.st_mode&S_IRUSR ? mask&AVIO_FLAG_READ  : 0;
    ret |= st.st_mode&S_IWUSR ? mask&AVIO_FLAG_WRITE : 0;

    return ret;
#endif
}

#if CONFIG_FILE_PROTOCOL

static int file_open(URLContext *h, const char *filename, int flags)
{
    FileContext *c = h->priv_data;
    int access;
    int fd;

    av_strstart(filename, "file:", &filename);
#if !(defined(CONFIG_PLATFORM_8195A)||defined(CONFIG_PLATFORM_8721D))
    if (flags & AVIO_FLAG_WRITE && flags & AVIO_FLAG_READ) {
        access = O_CREAT | O_RDWR;
        if (c->trunc)
            access |= O_TRUNC;
    } else if (flags & AVIO_FLAG_WRITE) {
        access = O_CREAT | O_WRONLY;
        if (c->trunc)
            access |= O_TRUNC;
    } else {
        access = O_RDONLY;
    }
#endif
#ifdef O_BINARY
    access |= O_BINARY;
#endif
    fd = avpriv_open(filename, access, 0666);
    if (fd == -1)
        return AVERROR(errno);
    c->fd = fd;
    return 0;
}

/* XXX: use llseek */
static int64_t file_seek(URLContext *h, int64_t pos, int whence)
{
    FileContext *c = h->priv_data;
    int64_t ret;
#if defined(CONFIG_PLATFORM_8195A)||defined(CONFIG_PLATFORM_8721D)
    if(!c || !c->fd)
        return -1;
    if (whence == AVSEEK_SIZE) {
        //printf("%s: AVSEEK_SIZE get file size %d\n", __func__, ((FIL *)c->fd)->fsize);
        return ((FIL *)c->fd)->fsize;
    }
    if(pos){
        //printf("%s: c->fd = 0x%x, pos = 0x%x, whence = 0x%x\n", __func__, c->fd, (int32_t)pos, whence);
        if(f_lseek((FIL *)c->fd, pos) != FR_OK){
            printf("seek file failed 0x%x %d\n", c->fd, (int32_t)pos);
            return -1;
        }
    }
    return ((FIL *)c->fd)->fptr;
#else

    if (whence == AVSEEK_SIZE) {
        struct stat st;

        ret = fstat(c->fd, &st);
        return ret < 0 ? AVERROR(errno) : st.st_size;
    }

    ret = lseek(c->fd, pos, whence);

    return ret < 0 ? AVERROR(errno) : ret;
#endif
}

static int file_close(URLContext *h)
{
    FileContext *c = h->priv_data;
#if defined(CONFIG_PLATFORM_8195A)||defined(CONFIG_PLATFORM_8721D)
    if(c->fd){
        FRESULT res = f_close((FIL *)c->fd);
        if(res){
            printf("close file fail.\n");
        }
        c->fd = -1;
    }
    if(f_mount(NULL, file_logical_drv, 1) != FR_OK){
        printf("FATFS unmount logical drive fail.\n");
    }
    if(FATFS_UnRegisterDiskDriver(file_drv_num))	
        printf("Unregister disk driver from FATFS fail.\n");
    return 0;
#else
    return close(c->fd);
#endif
}

const URLProtocol ff_file_protocol = {
    .name                = "file",
    .url_open            = file_open,
    .url_read            = file_read,
    .url_write           = file_write,
    .url_seek            = file_seek,
    .url_close           = file_close,
    .url_get_file_handle = file_get_handle,
    .url_check           = file_check,
    .priv_data_size      = sizeof(FileContext),
    .priv_data_class     = &file_class,
};

#endif /* CONFIG_FILE_PROTOCOL */

#if CONFIG_PIPE_PROTOCOL

static int pipe_open(URLContext *h, const char *filename, int flags)
{
    FileContext *c = h->priv_data;
    int fd;
    char *final;
    av_strstart(filename, "pipe:", &filename);

    fd = strtol(filename, &final, 10);
    if((filename == final) || *final ) {/* No digits found, or something like 10ab */
        if (flags & AVIO_FLAG_WRITE) {
            fd = 1;
        } else {
            fd = 0;
        }
    }
#if HAVE_SETMODE
    setmode(fd, O_BINARY);
#endif
    c->fd = fd;
    h->is_streamed = 1;
    return 0;
}

const URLProtocol ff_pipe_protocol = {
    .name                = "pipe",
    .url_open            = pipe_open,
    .url_read            = file_read,
    .url_write           = file_write,
    .url_get_file_handle = file_get_handle,
    .url_check           = file_check,
    .priv_data_size      = sizeof(FileContext),
};

#endif /* CONFIG_PIPE_PROTOCOL */
