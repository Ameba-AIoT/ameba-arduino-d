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
#include <ble_scatternet_app_flags.h>

/*============================================================================*
 *                              Constants
 *============================================================================*/
 /** @brief  Define device list table size. */
#define BLE_SCATTERNET_APP_MAX_DEVICE_INFO 6
/** @addtogroup  CENTRAL_CLIENT_GAP_MSG
    * @{
    */
/**
 * @brief  Application Link control block defination.
 */
typedef struct
{
    T_GAP_CONN_STATE        conn_state;          /**< Connection state. */
    T_GAP_REMOTE_ADDR_TYPE  bd_type;             /**< remote BD type*/
    uint8_t                 bd_addr[GAP_BD_ADDR_LEN]; /**< remote BD */
	T_GAP_ROLE              role;                   //!< Device role
} T_APP_LINK;
/** @} */
/* End of group CENTRAL_CLIENT_GAP_MSG */
/** @addtogroup  CENTRAL_CLIENT_SCAN_MGR
	 * @brief  Device list block definition.
	 */
typedef struct
{
	uint8_t 	 bd_addr[GAP_BD_ADDR_LEN];	/**< remote BD */
	uint8_t 	 bd_type;			   /**< remote BD type*/
} T_DEV_INFO;
/** @} */
/** @addtogroup  SCATTERNET_RANDOM
	* @{
	*/
#if F_BT_LE_USE_STATIC_RANDOM_ADDR
typedef struct
{
	uint8_t 	 is_exist;
	uint8_t 	 reserved;		   /**< remote BD type*/
	uint8_t 	 bd_addr[GAP_BD_ADDR_LEN];	/**< remote BD */
} T_APP_STATIC_RANDOM_ADDR;
#endif

  
/*============================================================================*
 *                              Variables
 *============================================================================*/
/** @brief  App link table */
extern T_APP_LINK ble_scatternet_app_link_table[BLE_SCATTERNET_APP_MAX_LINKS];
extern T_DEV_INFO ble_scatternet_dev_list[BLE_SCATTERNET_APP_MAX_DEVICE_INFO];
/** @brief  The number of device informations saved in dev_list. */
extern uint8_t ble_scatternet_dev_list_count;

/*============================================================================*
 *                              Functions
 *============================================================================*/
/*============================================================================*
 *                              Functions
 *============================================================================*/
bool ble_scatternet_link_mgr_add_device(uint8_t *bd_addr, uint8_t bd_type);
void ble_scatternet_link_mgr_clear_device_list(void);

#if F_BT_LE_USE_STATIC_RANDOM_ADDR
uint32_t ble_scatternet_app_save_static_random_address(T_APP_STATIC_RANDOM_ADDR *p_addr);
uint32_t ble_scatternet_app_load_static_random_address(T_APP_STATIC_RANDOM_ADDR *p_addr);
#endif
#endif
