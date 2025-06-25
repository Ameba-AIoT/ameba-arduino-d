#include "platform_opts.h"
#include "littlefs_adapter.h"

lfs_t g_lfs;

struct lfs_config g_lfs_cfg = {
	.read  = lfs_diskio_read,
	.prog  = lfs_diskio_prog,
	.erase = lfs_diskio_erase,
	.sync  = lfs_diskio_sync,

#ifdef LFS_THREADSAFE
	.lock = lfs_diskio_lock,
	.unlock = lfs_diskio_unlock,
#endif

	.read_size = 1,
	.prog_size = 1,
	.block_size = 4096,
#ifndef CONFIG_USE_FLASHCFG
	.block_count = LFS_DEVICE_SIZE / 4096,
	.lookahead_size = (LFS_DEVICE_SIZE / 4096 / 8) * 8,
#endif
	.cache_size = 256,
	.block_cycles = 100,
};


int lfs_diskio_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
	if (size == 0) {
		return LFS_ERR_OK;
	}

	flash_t flash;
	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_stream_read(&flash, LFS_FLASH_BASE_ADDR + c->block_size * block + off, size, (uint8_t *)buffer);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	return LFS_ERR_OK;
}

int lfs_diskio_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
	if (size == 0) {
		return LFS_ERR_OK;
	}

	if ((off + size) > c->block_size) {
		printf("[ERR] %s prog range exceed block size\n", __FUNCTION__);
		return LFS_ERR_IO;
	}

	flash_t flash;

	//printf("[lfs_diskio_prog] : block is %d, off is %d, size is %d ======= \r\n", block, off, size);
	//printf("[lfs_diskio_prog] : buffer is %s ======== \r\n", buffer);
	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_stream_write(&flash, LFS_FLASH_BASE_ADDR + c->block_size * block + off, size, (uint8_t *)buffer);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	return LFS_ERR_OK;
}

int lfs_diskio_erase(const struct lfs_config *c, lfs_block_t block)
{
	if (c->block_size != 0x1000) {
		printf("[ERR] %s block size config wrong\n", __FUNCTION__);
		return LFS_ERR_IO;
	}

	flash_t flash;
	device_mutex_lock(RT_DEV_LOCK_FLASH);
	flash_erase_sector(&flash, LFS_FLASH_BASE_ADDR + c->block_size * block);
	device_mutex_unlock(RT_DEV_LOCK_FLASH);

	return LFS_ERR_OK;
}

int lfs_diskio_sync(const struct lfs_config *c)
{
	(void) c;
	return LFS_ERR_OK;
}

#ifdef LFS_THREADSAFE
_mutex lfs_op_mux = NULL;
int lfs_diskio_lock(const struct lfs_config *c)
{
	(void) c;
	if (lfs_op_mux == NULL) {
		rtw_mutex_init(&lfs_op_mux);
	}
	rtw_mutex_get(&lfs_op_mux);
	return LFS_ERR_OK;
}

int lfs_diskio_unlock(const struct lfs_config *c)
{
	(void) c;
	if (lfs_op_mux == NULL) {
		rtw_mutex_init(&lfs_op_mux);
	}
	rtw_mutex_put(&lfs_op_mux);
	return LFS_ERR_OK;
}
#endif