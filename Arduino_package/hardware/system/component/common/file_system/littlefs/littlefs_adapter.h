#include "osdep_service.h"
#include "device_lock.h"
#include "flash_api.h"
#include "lfs.h"

#ifndef CONFIG_USE_FLASHCFG
#define LFS_FLASH_BASE_ADDR 0x200000
#define LFS_DEVICE_SIZE 0x100000
#endif

extern struct lfs_config g_lfs_cfg;
extern lfs_t g_lfs;

/**
 * lfs与底层flash读数据接口
 * @param  c
 * @param  block  块编号
 * @param  off    块内偏移地址
 * @param  buffer 用于存储读取到的数据
 * @param  size   要读取的字节数
 * @return
 */
int lfs_diskio_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size);

/**
 * lfs与底层flash写数据接口
 * @param  c
 * @param  block  块编号
 * @param  off    块内偏移地址
 * @param  buffer 待写入的数据
 * @param  size   待写入数据的大小
 * @return
 */
int lfs_diskio_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size);

/**
 * lfs与底层flash擦除接口
 * @param  c
 * @param  block 块编号
 * @return
 */
int lfs_diskio_erase(const struct lfs_config *c, lfs_block_t block);

int lfs_diskio_sync(const struct lfs_config *c);

#ifdef LFS_THREADSAFE
int lfs_diskio_lock(const struct lfs_config *c);
int lfs_diskio_unlock(const struct lfs_config *c);
#endif