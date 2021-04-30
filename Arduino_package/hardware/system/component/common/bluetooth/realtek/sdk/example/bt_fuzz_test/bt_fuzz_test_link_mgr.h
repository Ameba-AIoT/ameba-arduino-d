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
#ifndef _BT_FUZZ_TEST_LINK_MANAGER_H_
#define _BT_FUZZ_TEST_LINK_MANAGER_H_
/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <app_msg.h>
#include <gap_conn_le.h>
#include <profile_client.h>
#include <bt_fuzz_test_app_flags.h>


/*============================================================================*
 *                              Constants
 *============================================================================*/
/** @brief  Define device list table size. */
#define BT_FUZZ_TEST_APP_MAX_DEVICE_INFO 6

/** @addtogroup  CENTRAL_SRV_DIS
    * @{
    */
/** @defgroup APP_DISCOV_BITS Application discover services Bits
* @{
 */
//#define APP_DISCOV_ALL_PRIMARY_FLAG 0x01
#define BT_FUZZ_TEST_APP_DISCOV_GAPS_FLAG    0x01
#define BT_FUZZ_TEST_APP_DISCOV_BAS_FLAG     0x02
#define BT_FUZZ_TEST_APP_DISCOV_CHAR_FLAG    0x04
#define BT_FUZZ_TEST_APP_DISCOV_CHAR_DES_FLAG1 0x08
#define BT_FUZZ_TEST_APP_DISCOV_CHAR_DES_FLAG2 0x10
#define BT_FUZZ_TEST_APP_DISCOV_CHAR_DES_FLAG3 0x20
#define BT_FUZZ_TEST_APP_DISCOV_CHAR_DES_FLAG4 0x40


#define BT_FUZZ_TEST_APP_READ_FLAG 0x80

#define BT_FUZZ_TEST_APP_WRITE_FLAG 0x100

#define BT_FUZZ_TEST_APP_NOTIFY_FLAG 0x200
#define BT_FUZZ_TEST_APP_INDICATE_FLAG 0x400


/**
  * @}
  */

#define BT_FUZZ_TEST_APP_MODE_JUST_WORK   0x01
#define BT_FUZZ_TEST_APP_MODE_PASSKEY_ENTRY   0x02
#define BT_FUZZ_TEST_APP_MODE_NUMERIC_COMPARISON   0x04


/** @addtogroup  SCATTERNET_GAP_MSG
    * @{
    */
/**
 * @brief  Application Link control block definition.
 */
typedef struct
{
    T_GAP_CONN_STATE        conn_state;          /**< Connection state. */
    uint16_t                 discovered_flags;    /**< discovered flags. */
    uint16_t                 srv_found_flags;     /**< service founded flogs. */
    T_GAP_REMOTE_ADDR_TYPE  bd_type;             /**< remote BD type*/
    uint8_t                 bd_addr[GAP_BD_ADDR_LEN]; /**< remote BD */
	uint8_t					pair_mode;     /**< just work, passkey input, sc numeric */
} T_APP_LINK;
/** @} */ /* End of group SCATTERNET_GAP_MSG */
/** @addtogroup  SCATTERNET_SCAN_MGR
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
extern T_APP_LINK bt_fuzz_test_app_link_table[BT_FUZZ_TEST_APP_MAX_LINKS];
/** @brief  Device list table, used to save discovered device informations. */
extern T_DEV_INFO bt_fuzz_test_dev_list[BT_FUZZ_TEST_APP_MAX_DEVICE_INFO];
/** @brief  The number of device informations saved in dev_list. */
extern uint8_t bt_fuzz_test_dev_list_count;

/*============================================================================*
 *                              Functions
 *============================================================================*/
bool bt_fuzz_test_link_mgr_add_device(uint8_t *bd_addr, uint8_t bd_type);
void bt_fuzz_test_link_mgr_clear_device_list(void);
#endif
