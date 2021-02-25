/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
   * @file      scatternet_app.h
   * @brief     This file handles BLE scatternet application routines.
   * @author    jane
   * @date      2017-06-06
   * @version   v1.0
   **************************************************************************************
   * @attention
   * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
   **************************************************************************************
  */

#ifndef _BT_FUZZ_TEST_APP_H_
#define _BT_FUZZ_TEST_APP_H_

#ifdef __cplusplus
extern "C" {
#endif
/*============================================================================*
 *                              Header Files
 *============================================================================*/
#include <profile_client.h>
#include <profile_server.h>
#include <app_msg.h>

#define BLE_PRINT	printf
#define BD_ADDR_FMT "%02x:%02x:%02x:%02x:%02x:%02x"
#define BD_ADDR_ARG(x) (x)[5],(x)[4],(x)[3],(x)[2],(x)[1],(x)[0]
#define UUID_128_FORMAT "0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X:0x%2X"
#define UUID_128(x)  x[0],x[1],x[2],x[3],x[4],x[5],x[6],x[7],x[8],x[9],x[10],x[11],x[12],x[13],x[14],x[15]


/*============================================================================*
 *                              Variables
 *============================================================================*/
extern T_CLIENT_ID   bt_fuzz_test_gaps_client_id;        /**< Simple ble service client id*/
extern T_CLIENT_ID   bt_fuzz_test_bas_client_id;         /**< battery service client id*/
#if BT_FUZZ_TEST_APP_GENERAL_CLIENT_TEST
extern T_CLIENT_ID   bt_fuzz_test_gcs_client_id;         /**< general client id*/
#endif
#if 0
extern T_SERVER_ID   bt_fuzz_test_simp_srv_id;           /**< Simple ble service id*/
extern T_SERVER_ID   bt_fuzz_test_bas_srv_id;            /**< Battery service id */
extern T_SERVER_ID   bt_fuzz_test_gls_srv_id;            /**< Glucose service id*/
#endif
extern T_SERVER_ID   bt_fuzz_test_ias_srv_id;            /**< Immediate alert level service id */
extern T_SERVER_ID   bt_fuzz_test_hrs_srv_id;            /**< Heart rate service id*/
extern T_SERVER_ID   bt_fuzz_test_gls_srv_id;            /**< Glucose service id*/
extern T_SERVER_ID   bt_fuzz_test_dis_srv_id;            /**< Device information service id*/
extern T_SERVER_ID bt_fuzz_test_bas_srv_id;  /**< Battery service id */
extern T_SERVER_ID bt_fuzz_test_simp_srv_id; /**< Simple ble service id*/

/** @brief I/O Capabilities */
typedef enum
{
    FUZZ_TESTUITE_BTLE_SMP_510,        //!<  Only a Display present, no Keyboard or Yes/No Keys.
    FUZZ_TESTUITE_BTLE_SMPC_510,      //!<  Display and Yes/No Keys present.
    FUZZ_TESTUITE_BTLE_ATT_510,       //!<  Only a Keyboard present, no Display.
    FUZZ_TESTUITE_BTLE_ATTC_510,  //!<  No input/output capabilities.
    FUZZ_TESTUITE_BTLE_PROFILES_510,
    FUZZ_TESTUITE_BTLE_HOGP_510,
    FUZZ_TESTUITE_BTLE_HEALTH_510,
    FUZZ_TESTUITE_BTLE_AD_510,
    FUZZ_TESTUITE_BTLE_RESERVER = 0xff,
} T_FUZZ_TESTSUITE;
#if 0
typedef enum
{
    FUZZ_TESTUITE_BTLE_SMP_LEGACY_510,        //!<  Only a Display present, no Keyboard or Yes/No Keys.
    FUZZ_TESTUITE_BTLE_SMP_LEGACY_540,        //!<  Keyboard dispaly present.
    FUZZ_TESTUITE_BTLE_SMP_LEGACY_D11,        //!<  Only a Display present, no Keyboard or Yes/No Keys.
    FUZZ_TESTUITE_BTLE_SMP_LEGACY_D21,        //!<  Keyboard present.
    FUZZ_TESTUITE_BTLE_SMP_LEGACY_D41,        //!<  Keyboard dispaly present.
    FUZZ_TESTUITE_BTLE_SMP_SC_D11,        //!<  Only a Display present, no Keyboard or Yes/No Keys.
    FUZZ_TESTUITE_BTLE_SMPC_510,      //!<  Display and Yes/No Keys present.
    FUZZ_TESTUITE_BTLE_ATT_510,       //!<  Only a Keyboard present, no Display.
    FUZZ_TESTUITE_BTLE_ATTC_510,  //!<  No input/output capabilities.
    FUZZ_TESTUITE_BTLE_PROFILES_510,
    FUZZ_TESTUITE_BTLE_HOGP_510,
    FUZZ_TESTUITE_BTLE_HEALTH_510,
    FUZZ_TESTUITE_BTLE_AD_510,
    FUZZ_TESTUITE_BTLE_RESERVER = 0xff,
} T_FUZZ_TESTSUITE;
#endif

extern T_FUZZ_TESTSUITE testsuite;

typedef uint8_t TGATTDBdAddr[6];
extern TGATTDBdAddr g_cur_rembd;

extern bool m_start_pair;
extern uint16_t read_handle;
extern uint16_t write_handle;

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
void bt_fuzz_test_app_handle_io_msg(T_IO_MSG io_msg);
/**
  * @brief Callback for gap le to notify app
  * @param[in] cb_type callback msy type @ref GAP_LE_MSG_Types.
  * @param[in] p_cb_data point to callback data @ref T_LE_CB_DATA.
  * @retval result @ref T_APP_RESULT
  */
T_APP_RESULT bt_fuzz_test_app_gap_callback(uint8_t cb_type, void *p_cb_data);

/**
 * @brief  Callback will be called when data sent from profile client layer.
 * @param  client_id the ID distinguish which module sent the data.
 * @param  conn_id connection ID.
 * @param  p_data  pointer to data.
 * @retval   result @ref T_APP_RESULT
 */
T_APP_RESULT bt_fuzz_test_app_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);
T_APP_RESULT bt_fuzz_test_app_profile_callback(T_SERVER_ID service_id, void *p_data);

#if BT_FUZZ_TEST_APP_GENERAL_CLIENT_TEST
T_APP_RESULT bt_fuzz_test_gcs_client_callback(T_CLIENT_ID client_id, uint8_t conn_id, void *p_data);

#endif

#ifdef __cplusplus
}
#endif

#endif

