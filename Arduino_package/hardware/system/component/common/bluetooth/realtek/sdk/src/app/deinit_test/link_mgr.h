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
#ifndef _LINK_MANAGER_H_
#define _LINK_MANAGER_H_
/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <app_msg.h>
#include <gap_conn_le.h>
#if F_BT_LE_GATT_CLIENT_SUPPORT
#include "profile_client.h"
#endif
/*============================================================================*
 *                              Constants
 *============================================================================*/
/** @brief  Define links number. range: 0-4 */
#define APP_MAX_LINKS  2
#define APP_HDL_CACHE_LEN 24/** @brief  Define device list table size. */
#define APP_MAX_DEVICE_INFO 15
typedef struct
{
    uint16_t     uuid16;
    uint8_t      bd_addr[GAP_BD_ADDR_LEN];  /**< remote BD */
    uint8_t      bd_type;         /**< remote BD type*/
    uint8_t      reserved[3];     /**< used to T_GATT_SERVER_DATABASE_INFO four byte alignment*/
    uint16_t     hdl_cache[APP_HDL_CACHE_LEN];
} T_GATT_SERVER_DATABASE_INFO;
/**
 * @brief  Application Link control block defination.
 */
typedef struct
{
    T_GAP_CONN_STATE conn_state;  /**<  Connection state. */
#if F_BT_LE_GATT_CLIENT_SUPPORT
    T_CLIENT_ID   client_id;
#endif
    uint8_t       end_handle_idx;
    uint8_t       disc_state;
    uint8_t       state;
    T_GATT_SERVER_DATABASE_INFO server_info;
} T_APP_LINK;
/**
 * @brief  Device list block defination.
 */
typedef struct
{
    uint8_t      bd_addr[GAP_BD_ADDR_LEN];  /**< remote BD */
    uint8_t      bd_type;              /**< remote BD type*/
} T_DEV_INFO;
/*============================================================================*
 *                              Variables
 *============================================================================*/
/** @brief  App link table */
extern T_APP_LINK app_link_table[APP_MAX_LINKS];
/** @brief  Device list table, used to save discovered device informations. */
extern T_DEV_INFO dev_list[APP_MAX_DEVICE_INFO];
/** @brief  The number of device informations saved in dev_list. */
extern uint8_t dev_list_count;

/*============================================================================*
 *                              Functions
 *============================================================================*/
bool link_mgr_add_device(uint8_t *bd_addr, uint8_t bd_type);
void link_mgr_clear_device_list(void);

#endif

