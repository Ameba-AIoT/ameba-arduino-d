/**
  ******************************************************************************
  * @file    patch_header_check.h
  * @author
  * @version V0.0.1
  * @date    2017-09-01
  * @brief   This file contains all the functions regarding patch header check.
  ******************************************************************************
  * @attention
  *
  * This module is a confidential and proprietary property of RealTek and
  * possession or use of this module requires written permission of RealTek.
  *
  * Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
  ******************************************************************************
  */

#ifndef _PATCH_HEADER_CHECK_H_
#define _PATCH_HEADER_CHECK_H_
#include <stdbool.h>
#include <stdint.h>
//#include "rtl876x.h"
//#include "flash_device.h"

/** @addtogroup  FLASH_DEVICE    Flash Device
    * @{
    */
#define   OTA_HEADER_SIZE               0x1000
#define   OTA_HEADER_DEFAULT_VALUE      0xFFFFFFFF

#define AUTO_ENTER_DFU_MODE_PATTERN     0xC58E
#define USE_SINGLE_BANK_OTA_PATTERN     0x7369

#define   IMG_HEADER_SIZE               1024

/*============================================================================*
  *                                   Types
  *============================================================================*/
/** @defgroup FLASH_DEVICE_Exported_Types Flash Device Exported Types
  * @brief
  * @{
  */

typedef enum
{
    SCCD        = 0x278D,
    OCCD        = 0x278E,
    FactoryCode = 0x278F,
    OTA         = 0x2790, /**< OTA header */
    SecureBoot  = 0x2791,
    RomPatch    = 0x2792,
    AppPatch    = 0x2793,
    AppData1    = 0x2794,
    AppData2    = 0x2795,
    AppData3    = 0x2796,
    AppData4    = 0x2797,
    AppData5    = 0x2798,
    AppData6    = 0x2799,
    IMAGE_MAX   = 0x279a,
    IMAGE_USER_DATA = 0xFFFE,  /**<the image only support unsafe single bank ota*/
} T_IMG_ID;

typedef enum
{
    ENC_KEY_SCEK = 0,
    ENC_KEY_SCEK_WITH_RTKCONST,
    ENC_KEY_OCEK,
    ENC_KEY_OCEK_WITH_OEMCONST,
    ENC_KEY_ON_FLASH,
    ENC_KEY_MAX,
} T_ENC_KEY_SELECT;

typedef struct
{
    uint8_t N[256];
    uint8_t E[4];
} T_RSA_PUBLIC_KEY;

typedef struct
{
    uint8_t ic_type; //!< indicate different ic, bee2 is 5
    uint8_t secure_version; //!< secure version info
    union
    {
        uint16_t value;
        struct
        {
            uint16_t xip: 1; //!< payload is executed on flash
            uint16_t enc: 1; //!< all the payload is encrypted
            uint16_t load_when_boot: 1; //!< load image when boot
            uint16_t enc_load: 1; //!< encrypt load part or not
            uint16_t enc_key_select: 3; //!< referenced to ENC_KEY_SELECT
            uint16_t not_ready : 1; //!< for copy image in ota
            uint16_t not_obsolete : 1; //!< for copy image in ota
            uint16_t integrity_check_en_in_boot : 1; //!< enable image integrity check in boot flow
            uint16_t rsvd: 6;
        } flag_value;
    } ctrl_flag;
    uint16_t image_id; //!< indicate different image
    uint16_t crc16; //!<when enable image integrity check in boot flow, crc16 check image integrity if crc16 is not zero, otherwise SHA256 check.
    uint32_t payload_len; //!< image length in bytes exclude 1KB image header
} T_IMG_CTRL_HEADER_FORMAT;

typedef struct
{
    union
    {
        uint32_t version;
        struct
        {
            uint32_t _version_reserve: 8;   //!< reserved
            uint32_t _version_revision: 8; //!< revision version
            uint32_t _version_minor: 8;     //!< minor version
            uint32_t _version_major: 8;     //!< major version
        } header_sub_version; //!< ota header sub version
        struct
        {
            uint32_t _version_major: 4;     //!< major version
            uint32_t _version_minor: 8;     //!< minor version
            uint32_t _version_revision: 15; //!< revision version
            uint32_t _version_reserve: 5;   //!< reserved
        } img_sub_version; //!< other image sub version including patch, app, app data1-6

    } ver_info;
} T_IMAGE_VERSION;

typedef struct
{
    union
    {
        uint32_t version;
        struct
        {
            uint32_t _version_major: 4;     //!< major version
            uint32_t _version_minor: 8;     //!< minor version
            uint32_t _version_revision: 15; //!< revision version
            uint32_t _version_reserve: 5;   //!< reserved
        } sub_version;
    } ver_info;
    uint32_t _version_commitid;     //!< git commit id
    uint8_t _customer_name[8];      //!< branch name for customer patch
} T_VERSION_FORMAT;

typedef struct
{
    T_IMG_CTRL_HEADER_FORMAT ctrl_header;
    uint8_t uuid[16];
    uint32_t exe_base;
    uint32_t load_base;
    uint32_t load_len;
    uint32_t img_base;
    uint8_t rsvd0[4];
    uint32_t magic_pattern;
    uint8_t dec_key[16];
    uint8_t rsvd1[28];
    T_VERSION_FORMAT git_ver;
    T_RSA_PUBLIC_KEY rsaPubKey;
    uint8_t sha256[32];
    uint8_t rsvd2[68];
    uint32_t app_cb_signature;
    uint32_t app_cb_table_base_address;
} T_IMG_HEADER_FORMAT;

typedef struct
{
    uint8_t payload_signature[256];
    uint8_t payload_mac[16];
    uint8_t header_signature[256];
    uint8_t header_mac[16];

} T_AUTH_HEADER_FORMAT;

typedef struct
{
    T_IMG_CTRL_HEADER_FORMAT ctrl_header;
    uint8_t uuid[16];
    uint32_t exe_base;
    uint32_t load_base;
    uint8_t rsvd0[8];
    uint16_t auto_enter_dfu_mode_pattern;
    uint16_t single_bank_ota_pattern;
    uint32_t magic_pattern;
    uint8_t rsvd1[44];
    T_VERSION_FORMAT git_ver;
    T_RSA_PUBLIC_KEY rsaPubKey;
    uint8_t sha256[32];
    uint32_t ver_val;
    uint32_t secure_boot_addr;
    uint32_t secure_boot_size;
    uint32_t rom_patch_addr;
    uint32_t rom_patch_size;
    uint32_t app_addr;
    uint32_t app_size;
    uint32_t app_data1_addr;
    uint32_t app_data1_size;
    uint32_t app_data2_addr;
    uint32_t app_data2_size;
    uint32_t app_data3_addr;
    uint32_t app_data3_size;
    uint32_t app_data4_addr;
    uint32_t app_data4_size;
    uint32_t app_data5_addr;
    uint32_t app_data5_size;
    uint32_t app_data6_addr;
    uint32_t app_data6_size;
} T_OTA_HEADER_FORMAT;


/** End of FLASH_DEVICE_Exported_Types
  * @}
  */

/*============================================================================*
  *                                   Constants
  *============================================================================*/
/** @defgroup FLASH_DEVICE_Exported_Constants Flash Device Exported Constants
  * @brief
  * @{
  */
#if 0
#define SIGNATURE_APP_CB            0x0e85d101 /**< patch callback app signature definition*/
#define UUID_SIZE 16

/** End of FLASH_DEVICE_Exported_Constants
  * @}
  */



/*************************************************************************************************
*                          functions
*************************************************************************************************/
/** @defgroup FLASH_DEVICE_Exported_Functions Flash Device Exported Functions
    * @brief
    * @{
    */

/**
 * @brief  calculated checksum(CRC16 or SHA256 determined by image) over the image, and compared
 *         with given checksum value.
 * @param  p_header image header info of the given image.
 * @retval true if image integrity check pass via checksum compare
 * @retval false otherwise
*/
extern bool check_image_chksum(T_IMG_CTRL_HEADER_FORMAT *p_header);
/**
 * @brief  Check the validity of the specified image
 * @param  image_id specify the image
 * @param  header_addr specify image header address
 * @retval true if image check pass
 * @retval false otherwise
*/
extern bool check_header_valid(uint32_t header_addr, T_IMG_ID image_id);
/**
 * @brief  get specified image header address
 * @param  image_id specify the image
 * @return specified image header address
*/
extern uint32_t get_header_addr_by_img_id(T_IMG_ID image_id);
/**
 * @brief  get start address of specified image which located in ota temp bank
 * @param  image_id specify the image
 * @return start address of specified image which located in ota temp bank
*/
extern uint32_t get_temp_ota_bank_addr_by_img_id(T_IMG_ID image_id);

/**
 * @brief  get start address of active ota bank
 * @param  none
 * @return start address of active ota bank
*/
extern uint32_t get_active_ota_bank_addr(void);

extern bool is_ota_support_bank_switch(void);

/**
 * @brief  get image size of specified image which located in ota active bank
 * @param  image_id specify the image which located in ota active bank
 * @return image size of specified image which located in ota active bank
*/
static inline uint32_t get_active_bank_image_size_by_img_id(T_IMG_ID image_id)
{
    uint32_t image_size = 0;

    if (image_id < OTA || image_id >= IMAGE_MAX) //not support
    {
        return image_size;
    }
    else
    {
        uint32_t ota_header_addr = get_active_ota_bank_addr();

        if (!check_header_valid(ota_header_addr, OTA))
        {
            return image_size;
        }
        else
        {
            if (image_id == OTA)
            {
                image_size = OTA_HEADER_SIZE;
            }
            else
            {
                T_OTA_HEADER_FORMAT *ota_header = (T_OTA_HEADER_FORMAT *)ota_header_addr;

                image_size = HAL_READ32((uint32_t)&ota_header->secure_boot_size, (image_id - SecureBoot) * 8);

                if (OTA_HEADER_DEFAULT_VALUE == image_size)
                {
                    image_size = 0;
                }
            }
        }
    }

    return image_size;
}

/**
 * @brief  get image version of specified image which located in ota active bank
 * @param  image_id specify the image which located in ota active bank
 * @param  p_image_version the T_IMAGE_VERSION pointer which indicate specified image's version and sub verdion
 * @note
 * 1.If get ota header version, sub version have no meaning.
 * 2.If patch not support cmd flash_ioctl_get_image_addr_by_image_id_ext,it will get app data3-6 version fail
 * @return true if get specified image version successfully
*/
static inline bool get_active_bank_image_version(T_IMG_ID image_id,
                                                 T_IMAGE_VERSION *p_image_version)
{
    uint32_t image_addr;
    uint32_t image_size;

    if ((image_id < OTA) || (image_id >= IMAGE_MAX)) //not support
    {
        return false;
    }
    else if (image_id == OTA)
    {
        image_addr = get_header_addr_by_img_id(image_id);
        T_OTA_HEADER_FORMAT *p_header = (T_OTA_HEADER_FORMAT *)image_addr;
        if (!p_header)
        {
            return false;
        }
        else
        {
            p_image_version->ver_info.version = p_header->ver_val;
            return true;
        }
    }
#ifdef SDK_8772
    image_addr = get_header_addr_by_img_id(image_id);
#else
    else if (image_id <= AppData2)
    {
        image_addr = get_header_addr_by_img_id(image_id);
    }
    else
    {
        //if patch not support cmd flash_ioctl_get_image_addr_by_image_id_ext, will get app data3-6 version fail!!!
        image_addr = flash_ioctl(flash_ioctl_get_image_addr_by_image_id_ext, image_id, 0);
    }
#endif
    image_size = get_active_bank_image_size_by_img_id(image_id);

    //get active bank image version when image id is not less than OTA
    T_IMG_HEADER_FORMAT *p_header = (T_IMG_HEADER_FORMAT *)image_addr;
    if ((!p_header) || (image_size == 0))
    {
        return false;
    }
    else
    {
        p_image_version->ver_info.version = p_header->git_ver.ver_info.version;
        return true;
    }
}


/** @} */ /* End of group FLASH_DEVICE_Exported_Functions */
/** @} */ /* End of group FLASH_DEVICE */
#endif
#endif // _PATCH_HEADER_CHECK_H_
