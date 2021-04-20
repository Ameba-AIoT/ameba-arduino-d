/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      link_mgr.h
   * @brief     Define multilink manager struct and functions.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */
#ifndef _BT_OTA_CENTRAL_CLIENT_LINK_MANAGER_H_
#define _BT_OTA_CENTRAL_CLIENT_LINK_MANAGER_H_
/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <app_msg.h>
#include <gap_conn_le.h>
#include <bt_ota_central_client_app_flags.h>
/** @defgroup APP_DISCOV_BITS Application discover services Bits
* @{
 */
#define APP_DISCOV_GAPS_FLAG    0x01
#define APP_DISCOV_BAS_FLAG     0x02
#define APP_DISCOV_OTA_FLAG     0x04
#define APP_DISCOV_DFU_FLAG     0x08

/** @brief  Define device list table size. */
#define BT_OTA_CENTRAL_CLIENT_APP_MAX_DEVICE_INFO 6

/*============================================================================*
 *                              Constants
 *============================================================================*/
/** @addtogroup  CENTRAL_CLIENT_GAP_MSG
    * @{
    */
/**
 * @brief  Application Link control block defination.
 */
typedef struct
{
    T_GAP_CONN_STATE        conn_state;          /**< Connection state. */
	uint8_t                 discovered_flags;    /**< discovered flags. */
    uint8_t                 srv_found_flags;     /**< service founded flogs. */
    T_GAP_REMOTE_ADDR_TYPE  bd_type;             /**< remote BD type*/
    uint8_t                 bd_addr[GAP_BD_ADDR_LEN]; /**< remote BD */
} T_APP_LINK;
/** @} */ /* End of group CENTRAL_CLIENT_GAP_MSG */

#if F_BT_GATT_SRV_HANDLE_STORAGE
/** @addtogroup  CENTRAL_SRV_DIS
    * @{
    */
/** @brief  App link table */
typedef struct
{
    uint8_t      srv_found_flags;
    uint8_t      bd_type;         /**< remote BD type*/
    uint8_t      bd_addr[GAP_BD_ADDR_LEN];  /**< remote BD */
    uint32_t     reserved;
    uint16_t     ota_hdl_cache[HDL_OTA_CACHE_LEN];
    uint16_t     dfu_hdl_cache[HDL_DFU_CACHE_LEN];
    uint16_t     bas_hdl_cache[HDL_BAS_CACHE_LEN];
} T_APP_SRVS_HDL_TABLE;
/** @} */
#endif

/** @addtogroup  CENTRAL_CLIENT_SCAN_MGR
    * @{
    */
/**
 * @brief  Device list block definition.
 */
typedef struct
{
    uint8_t      bd_addr[GAP_BD_ADDR_LEN];  /**< remote BD */
    uint8_t      bd_type;              /**< remote BD type*/
} T_DEV_INFO;
/** @} */

/*============================================================================*
 *                              Variables
 *============================================================================*/
/** @brief  App link table */
extern T_APP_LINK bt_ota_central_client_app_link_table[BT_OTA_CENTRAL_CLIENT_APP_MAX_LINKS];
/** @brief  Device list table, used to save discovered device informations. */
extern T_DEV_INFO bt_ota_central_client_dev_list[BT_OTA_CENTRAL_CLIENT_APP_MAX_DEVICE_INFO];
/** @brief  The number of device informations saved in dev_list. */
extern uint8_t bt_ota_central_client_dev_list_count;

/*============================================================================*
 *                              Functions
 *============================================================================*/
bool bt_ota_central_client_link_mgr_add_device(uint8_t *bd_addr, uint8_t bd_type);
void bt_ota_central_client_link_mgr_clear_device_list(void);
	
#if F_BT_GATT_SRV_HANDLE_STORAGE
uint32_t bt_ota_central_client_app_save_srvs_hdl_table(T_APP_SRVS_HDL_TABLE *p_info);
uint32_t bt_ota_central_client_app_load_srvs_hdl_table(T_APP_SRVS_HDL_TABLE *p_info);
#endif

#endif
