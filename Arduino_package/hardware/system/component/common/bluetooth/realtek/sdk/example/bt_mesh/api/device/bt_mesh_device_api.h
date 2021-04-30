/**
*****************************************************************************************
*     Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     bt_mesh_device_api.h
  * @brief    Source file for device cmd.
  * @details  User command interfaces.
  * @author   sherman
  * @date     2019-09-16
  * @version  v1.0
  * *************************************************************************************
  */
#ifndef _BT_MESH_DEV_API_H_
#define _BT_MESH_DEV_API_H_

#include "bt_mesh_user_api.h"

#define BT_MESH_DEV_CMD_RETRY_COUNT 2

/** @brief bt mesh device command table */
enum bt_mesh_device_cmd
{
  GEN_MESH_CODE(_node_reset) ,	/*0*/
  GEN_MESH_CODE(_prov_capa_set) , 
#if MESH_LPN
  GEN_MESH_CODE(_lpn_init) , 
  GEN_MESH_CODE(_lpn_deinit) ,
  GEN_MESH_CODE(_lpn_req) , 
  GEN_MESH_CODE(_lpn_sub) ,  
  GEN_MESH_CODE(_lpn_clear) , /*5*/
#endif
#if MESH_FN
  GEN_MESH_CODE(_fn_init) ,
  GEN_MESH_CODE(_fn_deinit) ,
#endif
  GEN_MESH_CODE(_data_transmission_notify) ,
  GEN_MESH_CODE(_generic_on_off_publish) ,
  GEN_MESH_CODE(_datatrans_write) ,
  GEN_MESH_CODE(_datatrans_read) ,
  GEN_MESH_CODE(_connect) ,
  GEN_MESH_CODE(_disconnect) ,
  GEN_MESH_CODE(_list) ,
  GEN_MESH_CODE(_dev_info_show) ,

  MAX_MESH_DEVICE_CMD
};

/**
  * @brief initialize bt mesh device api
  *
  * @return 1: success 0: fail
  */
uint8_t bt_mesh_device_api_init(void);

/**
  * @brief deinitialize bt mesh device api
  *
  * @return none
  */
void bt_mesh_device_api_deinit(void);

#endif


