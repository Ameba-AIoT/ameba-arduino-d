#ifndef __AUTOCONF_8721D_TEST_H__
#define __AUTOCONF_8721D_TEST_H__

#define TEST_ON_FPGA           0
#if TEST_ON_FPGA
#define FPGA_CLK_40M        0       //0 means 80M
#endif

#define CONFIG_TEST_VERA        0
#define CONFIG_FLASH_DL          0
#define CONFIG_TEST                 1


#if CONFIG_TEST_VERA
#define CONFIG_INT_TEST             0
#define CONFIG_REG_TEST             0
#define CONFIG_BUF_TEST             0
#define CONFIG_DDMA_TEST            0
#define CONFIG_FLASH_TEST           0
#define CONFIG_FLASH_FT_TEST        0
#define CONFIG_EFUSE_TEST			0
#define CONFIG_IOPATH_TEST			0
#define CONFIG_ROM_TEST             0
#define CONFIG_LPS_TEST             0
#define CONFIG_SEC_ENG_TEST         0
#endif

#if CONFIG_FLASH_DL
#define CONFIG_INT_TEST             0
#define CONFIG_REG_TEST             0
#define CONFIG_BUF_TEST             0
#define CONFIG_DDMA_TEST            0
#define CONFIG_FLASH_TEST           1
#define CONFIG_FLASH_FT_TEST        0
#define CONFIG_EFUSE_TEST			0
#define CONFIG_IOPATH_TEST			0
#define CONFIG_ROM_TEST             0
#define CONFIG_LPS_TEST             0
#define CONFIG_SEC_ENG_TEST         0
#endif

#if CONFIG_TEST
#define CONFIG_INT_TEST             1
#define CONFIG_REG_TEST             1
#define CONFIG_BUF_TEST             1
#define CONFIG_DDMA_TEST            1
#define CONFIG_FLASH_TEST           0
#define CONFIG_FLASH_FT_TEST        0
#define CONFIG_EFUSE_TEST			0
#define CONFIG_IOPATH_TEST			1
#define CONFIG_ROM_TEST             0
#define CONFIG_LPS_TEST             1
#define CONFIG_SEC_ENG_TEST         1
#define CONFIG_FW_LBK_TEST          1
#endif

#if CONFIG_FLASH_TEST
#define CONFIG_FLASH_GD         1
#define CONFIG_FLASH_MXIC         1
#define CONFIG_FLASH_WB         1
#define CONFIG_FLASH_MICRON         1
#define CONFIG_FLASH_EON         1
#endif

#if CONFIG_FLASH_FT_TEST
#define CONFIG_FLASH_GD         0
#define CONFIG_FLASH_MXIC         0
#define CONFIG_FLASH_WB         0
#define CONFIG_FLASH_MICRON         0
#define CONFIG_FLASH_EON         1
#endif

#endif      //#ifndef __AUTOCONF_8721D_TEST_H__
