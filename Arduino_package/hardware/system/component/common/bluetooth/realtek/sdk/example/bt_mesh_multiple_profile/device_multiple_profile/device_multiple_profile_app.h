/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      device_app.h
* @brief     Smart mesh demo application
* @details
* @author    bill
* @date      2015-11-12
* @version   v0.1
* *********************************************************************************************************
*/

#ifndef _MESH_APPLICATION__
#define _MESH_APPLICATION__

#ifdef __cplusplus
extern "C" {
#endif

#include <profile_client.h>
#include <profile_server.h>
#include "app_msg.h"
#include "mesh_api.h"
#include "bt_mesh_device_multiple_profile_app_flags.h"

/*============================================================================*
 *                              Variables
 *============================================================================*/
#if defined(CONFIG_BT_MESH_CENTRAL) && CONFIG_BT_MESH_CENTRAL
extern T_CLIENT_ID   bt_mesh_central_gcs_client_id;         /**< General Common Services client client id*/
#define BLE_PRINT	printf
#define BD_ADDR_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#define BD_ADDR_ARG(x) (x)[5],(x)[4],(x)[3],(x)[2],(x)[1],(x)[0]
#define UUID_128_FORMAT "0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X"
#define UUID_128(x)  x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]
#endif
#if defined(CONFIG_BT_MESH_PERIPHERAL) && CONFIG_BT_MESH_PERIPHERAL
extern T_SERVER_ID bt_mesh_simp_srv_id; /**< Simple ble service id*/
extern T_SERVER_ID bt_mesh_bas_srv_id;  /**< Battery service id */
extern const uint8_t adv_data[];
#endif
#if defined(CONFIG_BT_MESH_SCATTERNET) && CONFIG_BT_MESH_SCATTERNET
#define BLE_PRINT	printf
#define BD_ADDR_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#define BD_ADDR_ARG(x) (x)[5],(x)[4],(x)[3],(x)[2],(x)[1],(x)[0]
#define UUID_128_FORMAT "0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X"
#define UUID_128(x)  x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]
extern T_SERVER_ID bt_mesh_simp_srv_id; /**< Simple ble service id*/
extern T_SERVER_ID bt_mesh_bas_srv_id;  /**< Battery service id */
extern const uint8_t adv_data[];
extern T_CLIENT_ID   bt_mesh_scatternet_gcs_client_id;         /**< General Common Services client client id*/
#endif
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
void bt_mesh_device_multiple_profile_app_handle_io_msg(T_IO_MSG io_msg);
/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */
T_APP_RESULT bt_mesh_device_multiple_profile_app_gap_callback(uint8_t cb_type, void *p_cb_data);

/**
 * @brief  Callback will be called when data sent from profile client layer.
 * @param  client_id the ID distinguish which module sent the data.
 * @param  conn_id connection ID.
 * @param  p_data  pointer to data.
 * @retval   result @ref T_APP_RESULT
 */
T_APP_RESULT bt_mesh_device_multiple_profile_app_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);

/**
    * @brief    All the BT Profile service callback events are handled in this function
    * @note     Then the event handling function shall be called according to the
    *           service_id
    * @param    service_id  Profile service ID
    * @param    p_data      Pointer to callback data
    * @return   T_APP_RESULT, which indicates the function call is successful or not
    * @retval   APP_RESULT_SUCCESS  Function run successfully
    * @retval   others              Function run failed, and return number indicates the reason
    */
T_APP_RESULT bt_mesh_device_multiple_profile_app_profile_callback(T_SERVER_ID service_id, void *p_data);

#if defined(CONFIG_BT_MESH_CENTRAL) && CONFIG_BT_MESH_CENTRAL
/**
 * @brief  Callback will be called when data sent from profile client layer.
 * @param  client_id the ID distinguish which module sent the data.
 * @param  conn_id connection ID.
 * @param  p_data  pointer to data.
 * @retval   result @ref T_APP_RESULT
 */
T_APP_RESULT bt_mesh_central_gcs_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);
#elif defined(CONFIG_BT_MESH_SCATTERNET) && CONFIG_BT_MESH_SCATTERNET
/**
 * @brief  Callback will be called when data sent from profile client layer.
 * @param  client_id the ID distinguish which module sent the data.
 * @param  conn_id connection ID.
 * @param  p_data  pointer to data.
 * @retval   result @ref T_APP_RESULT
 */
T_APP_RESULT bt_mesh_scatternet_gcs_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);
#if F_BT_GAPS_CHAR_WRITEABLE
T_APP_RESULT bt_mesh_scatternet_gap_service_callback(T_SERVER_ID service_id, void *p_para);
#endif
#endif

void device_info_cb(uint8_t bt_addr[6], uint8_t bt_addr_type, int8_t rssi, device_info_t *pinfo);
bool prov_cb(prov_cb_type_t cb_type, prov_cb_data_t cb_data);
void fn_cb(uint8_t frnd_index, fn_cb_type_t type, uint16_t lpn_addr);
void lpn_cb(uint8_t frnd_index, lpn_cb_type_t type, uint16_t fn_addr);
void hb_cb(hb_data_type_t type, void *pargs);
void app_vendor_callback(uint8_t cb_type, void *p_cb_data);
#ifdef __cplusplus
}
#endif

#endif /* _MESH_APPLICATION__S */
