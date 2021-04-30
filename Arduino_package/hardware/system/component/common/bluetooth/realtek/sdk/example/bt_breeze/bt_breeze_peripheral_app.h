/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      bt_breeze_peripheral_app.h
   * @brief     This file handles BLE peripheral application routines.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

#ifndef _BT_BREEZE_PERIPHERAL_APP_H_
#define _BT_BREEZE_PERIPHERAL_APP_H_

#ifdef __cplusplus
extern "C" {
#endif
/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <app_msg.h>
#include <gap_le.h>
#include <profile_server.h>
#include "breeze_hal_ble.h"
#include "breeze.h"
#include "osdep_service.h"
/*============================================================================*
 *                              Variables
 *============================================================================*/
extern T_SERVER_ID breeze_srv_id; /**< breeze service id*/

// new added
typedef void (*ais_connected_callback_t)();
typedef void (*ais_disconnected_callback_t)();
typedef int (*ais_read_callback_t)(void *buf, uint16_t len);
typedef int (*ais_write_callback_t)(void *buf, uint16_t len);
typedef void (*ais_icccc_change_callback_t)(uint8_t value);
typedef void (*ais_ncccc_change_callback_t)(uint8_t value);
  
typedef struct
{
    /* AIS's Read rc Callback */
	ais_read_callback_t read_ais_rc;
    /* AIS's Read wc Callback */
	ais_read_callback_t	read_ais_wc;
    /* AIS's Read ic Callback */
	ais_read_callback_t read_ais_ic;
    /* AIS's Read nc Callback */
	ais_read_callback_t read_ais_nc;
    /* AIS's Read wwnrc Callback */
	ais_read_callback_t read_ais_wwnrc;
    /* AIS's Write wc Callback */
	ais_write_callback_t write_ais_wc;
    /* AIS's Write wwnrc Callback */
	ais_write_callback_t write_ais_wwnrc;
    /* AIS's ic Changed Callback */
	ais_icccc_change_callback_t ais_ic_ccc_cfg_changed;
    /* AIS's nc Changed Callback */
	ais_ncccc_change_callback_t ais_nc_ccc_cfg_changed;
    /* Callback function when bluetooth connected */
	ais_connected_callback_t connected;
    /* Callback function when bluetooth disconnected */
	ais_disconnected_callback_t disconnected;
} ais_service_cb_t;

typedef struct
{
	uint8_t type;
	uint8_t data_len;
	const uint8_t *data;
}ais_ad_data;

typedef struct
{
	uint8_t *p_data;
	ind_cb cb;
} ind_param_t;

typedef struct
{
	uint16_t length;
	union
	{ 
		uint8_t *p_data;
		ind_param_t ic;
	};
} ind_notify_param_t;

typedef struct
{
	_list list;
	ind_notify_param_t ind_notify_param;
} ind_notify_param_list_t;

typedef enum
{
	BT_BREEZE_MSG_START_ADV,
	BT_BREEZE_MSG_STOP_ADV,
	BT_BREEZE_MSG_DISCONNECT,
	BT_BREEZE_MSG_NOTIFY,
	BT_BREEZE_MSG_INDICATE,
	BT_BREEZE_MSG_MAX
} BT_BREEZE_MSG_TYPE;

typedef struct
{
	uint16_t type;
	uint32_t data;
	void	 *buf;
} T_CALLBACK_MSG;

typedef enum
{
	BT_BREEZE_CALLBACK_MSG_WRITE_WC,
	BT_BREEZE_CALLBACK_MSG_WRITE_WWNRC,
	BT_BREEZE_CALLBACK_MSG_IC_CCC_CFG_CHANGED,
	BT_BREEZE_CALLBACK_MSG_NC_CCC_CFG_CHANGED,
	BT_BREEZE_CALLBACK_MSG_CONNECTED,
	BT_BREEZE_CALLBACK_MSG_DISCONNECTED,
	BT_BREEZE_CALLBACK_MSG_INDICATE_DONE,
	BT_BREEZE_CALLBACK_MSG_MAX
} BT_BREEZE_CALLBACK_MSG_TYPE;

/*============================================================================*
 *                              Functions
 *============================================================================*/

/**
 * @brief    All the application messages are pre-handled in this function
 * @note     All the IO MSGs are sent to this function, then the event handling
 *           function shall be called according to the MSG type.
 * @param[in] io_msg  IO message data
 * @return   void
 */
void ble_breeze_app_handle_io_msg(T_IO_MSG io_msg);

/**
 * @brief    All the BLE Profile service callback events are handled in this function
 * @note     Then the event handling function shall be called according to the
 *           service_id.
 * @param[in] service_id  Profile service ID
 * @param[in] p_data      Pointer to callback data
 * @return   Indicates the function call is successful or not
 * @retval   result @ref T_APP_RESULT
 */
T_APP_RESULT ble_breeze_app_profile_callback(T_SERVER_ID service_id, void *p_data);

/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */
T_APP_RESULT ble_breeze_app_gap_callback(uint8_t cb_type, void *p_cb_data);

#ifdef __cplusplus
}
#endif

#endif

