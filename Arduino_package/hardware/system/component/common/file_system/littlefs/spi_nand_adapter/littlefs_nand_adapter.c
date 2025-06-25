#include "platform_opts.h"
#include "littlefs_nand_adapter.h"
#include "littlefs_nand_ftl.h"
#include "littlefs_spi_nand.h"


//lfs_t g_nand_flash_lfs;


/** @defgroup NAND_Page_Size_Main_Definitions
  * @{
  */
#define NAND_PAGE_SIZE_MAIN_BIT_EXP 		(g_nandflash_init_para.FLASH_pagesize_main_bit_exp)
#define NAND_PAGE_SIZE_MAIN 				(1U << NAND_PAGE_SIZE_MAIN_BIT_EXP)
#define NAND_ADDR_TO_PAGE_ADDR(addr) 		((addr) >> NAND_PAGE_SIZE_MAIN_BIT_EXP)
#define NAND_ADDR_TO_BYTE_ADDR(addr) 		((addr) & BIT_LEN_MASK_32(NAND_PAGE_SIZE_MAIN_BIT_EXP))
/** @} */


struct lfs_config g_nand_flash_lfs_cfg = {
	.read  = lfs_nand_flash_read,
	.prog  = lfs_nand_flash_prog,
	.erase = lfs_nand_flash_erase,
	.sync  = lfs_nand_flash_sync,

#ifdef LFS_THREADSAFE
	.lock = lfs_nand_flash_diskio_lock,
	.unlock = lfs_nand_flash_diskio_unlock,
#endif

	.read_size = 2048,
	.prog_size = 2048,
	.block_size = 2048 * 64,
	.lookahead_size = 8,
	.cache_size = 2048,
	.block_cycles = 100,
};

int lfs_nand_flash_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer, lfs_size_t size)
{
    LFS_NAND_DBG(LFS_NAND_INFO, "block=%d,off=%d,size=%d",block,off,size);
	if (size == 0) {
		return LFS_ERR_OK;
	}

	u32 WholeByteAddr, PageAddr;
	//device_mutex_lock(RT_DEV_LOCK_FLASH);
	//lfs_nand_flash_diskio_lock(c);

	WholeByteAddr = c->block_size * block + off;
	PageAddr = NAND_ADDR_TO_PAGE_ADDR(WholeByteAddr);
	if (NandFlash_FTL_ReadPage(PageAddr, (uint8_t *)buffer)) {
		//device_mutex_unlock(RT_DEV_LOCK_FLASH);
        //lfs_nand_flash_diskio_unlock(c);
        LFS_NAND_DBG(LFS_NAND_INFO, "LFS_ERR_CORRUPT");
		return LFS_ERR_CORRUPT;
	}
    
    /*LFS_NAND_DBG(LFS_NAND_DEBUG, "buffer:");
    for(int i=0;i<100;i++)
    {
        printf("%02X-",((uint8_t *)buffer)[i]);
    }
    printf("\n");*/

	//device_mutex_unlock(RT_DEV_LOCK_FLASH);	
    //lfs_nand_flash_diskio_unlock(c);

	return LFS_ERR_OK;
}

int lfs_nand_flash_prog(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer, lfs_size_t size)
{
    LFS_NAND_DBG(LFS_NAND_INFO, "block=%d,off=%d,size=%d",block,off,size);
	if (size == 0) {
		return LFS_ERR_OK;
	}

	if ((off + size) > c->block_size) {
		LFS_NAND_DBG(LFS_NAND_ERROR, "prog range exceed block size");
		return LFS_ERR_IO;
	}
    
    /*LFS_NAND_DBG(LFS_NAND_DEBUG, "buffer:");
    for(int i=0;i<100;i++)
    {
        printf("%02X-",((u8*)buffer)[i]);
    }
    printf("\n");*/
    
	u32 WholeByteAddr, PageAddr;

	//device_mutex_lock(RT_DEV_LOCK_FLASH);
	//lfs_nand_flash_diskio_lock(c);

	WholeByteAddr = c->block_size * block + off;
	PageAddr = NAND_ADDR_TO_PAGE_ADDR(WholeByteAddr);

	if (NandFlash_FTL_WritePage(PageAddr, (uint8_t *)buffer, 0)) {
		//device_mutex_unlock(RT_DEV_LOCK_FLASH);
        //lfs_nand_flash_diskio_unlock(c);
        LFS_NAND_DBG(LFS_NAND_INFO, "LFS_ERR_CORRUPT");
		return LFS_ERR_CORRUPT;
	}

	//device_mutex_unlock(RT_DEV_LOCK_FLASH);
    //lfs_nand_flash_diskio_unlock(c);

	return LFS_ERR_OK;
}

int lfs_nand_flash_erase(const struct lfs_config *c, lfs_block_t block)
{
    LFS_NAND_DBG(LFS_NAND_INFO, "block=%d",block);
	if (c->block_size != 0x20000) {
		LFS_NAND_DBG(LFS_NAND_ERROR, "block size config wrong");
		return LFS_ERR_IO;
	}

	u32 WholeByteAddr, PageAddr;
	//device_mutex_lock(RT_DEV_LOCK_FLASH);
	//lfs_nand_flash_diskio_lock(c);

	WholeByteAddr = c->block_size * block;
	PageAddr = NAND_ADDR_TO_PAGE_ADDR(WholeByteAddr);

	if (NandFlash_FTL_EraseBlock(PageAddr, 0)) {
		//device_mutex_unlock(RT_DEV_LOCK_FLASH);
        //lfs_nand_flash_diskio_unlock(c);
        LFS_NAND_DBG(LFS_NAND_INFO, "LFS_ERR_CORRUPT");
		return LFS_ERR_CORRUPT;
	}

	//device_mutex_unlock(RT_DEV_LOCK_FLASH);
    //lfs_nand_flash_diskio_unlock(c);

	return LFS_ERR_OK;
}


int lfs_nand_flash_sync(const struct lfs_config *c)
{
	(void) c;
	return LFS_ERR_OK;
}

#ifdef LFS_THREADSAFE

static _mutex lfs_nand_op_mux = NULL;
int lfs_nand_flash_diskio_lock(const struct lfs_config *c)
{
	(void) c;
    
	//LFS_NAND_DBG(LFS_NAND_INFO, "lfs_nand_op_mux=%08X", lfs_nand_op_mux);    
	if (lfs_nand_op_mux == NULL) {
		rtw_mutex_init(&lfs_nand_op_mux);
	}
	//LFS_NAND_DBG(LFS_NAND_INFO, "lfs_nand_op_mux=%08X", lfs_nand_op_mux);
    
	rtw_mutex_get(&lfs_nand_op_mux);
	return LFS_ERR_OK;
}

int lfs_nand_flash_diskio_unlock(const struct lfs_config *c)
{
	(void) c;
	//LFS_NAND_DBG(LFS_NAND_INFO, "lfs_nand_op_mux=%08X", lfs_nand_op_mux);
	if (lfs_nand_op_mux == NULL) {
		rtw_mutex_init(&lfs_nand_op_mux);
	}
	rtw_mutex_put(&lfs_nand_op_mux);
	//LFS_NAND_DBG(LFS_NAND_INFO, "lfs_nand_op_mux=%08X", lfs_nand_op_mux);
	return LFS_ERR_OK;
}
#endif

extern NAND_FTL_DeviceTypeDef NandFlash_Device;

int lfs_nand_init(lfs_t *lfs)
{
	struct lfs_config *lfs_cfg;
	int ret = 0;

    printf("\n");
	LFS_NAND_DBG(LFS_NAND_INFO, "init nand lfs cfg");
    
	ret = NandFlash_FTL_Init();
    if(ret){        
        LFS_NAND_DBG(LFS_NAND_ERROR, ": FAILED");
        return ret;        
    }
    
	g_nand_flash_lfs_cfg.block_count = NandFlash_Device.MemInfo.BlocksPerLun*NandFlash_Device.MemInfo.LunsPerTarget*NandFlash_Device.MemInfo.Targets;
	lfs_cfg = &g_nand_flash_lfs_cfg;
    LFS_NAND_DBG(LFS_NAND_INFO, "BlocksPerLun=%d, LunsPerTarget=%d, Targets=%d", NandFlash_Device.MemInfo.BlocksPerLun, NandFlash_Device.MemInfo.LunsPerTarget, NandFlash_Device.MemInfo.Targets);

    //lfs_nand_flash_erase(lfs_cfg, 0);
    //lfs_nand_flash_erase(lfs_cfg, 1);
	ret = lfs_mount(lfs, lfs_cfg);
	if (ret) {
        LFS_NAND_DBG(LFS_NAND_ERROR, "lfs_mount fail %d", ret);
		ret = lfs_format(lfs, lfs_cfg);
		if (ret) {
			LFS_NAND_DBG(LFS_NAND_ERROR, "lfs_format fail %d", ret);
			return ret;
		}
		ret = lfs_mount(lfs, lfs_cfg);
		if (ret) {
			LFS_NAND_DBG(LFS_NAND_ERROR, "lfs_mount fail %d", ret);
			return ret;
		}
	}

	return ret;

}
