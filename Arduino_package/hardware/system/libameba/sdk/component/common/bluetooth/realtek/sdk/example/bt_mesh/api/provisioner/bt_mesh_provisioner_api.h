/**
*****************************************************************************************
*     Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     bt_mesh_provisioner_api.h
  * @brief    Source file for provisioner cmd.
  * @details  User command interfaces.
  * @author   sherman
  * @date     2019-09-16
  * @version  v1.0
  * *************************************************************************************
  */
#ifndef _BT_MESH_PROV_API_H_
#define _BT_MESH_PROV_API_H_

#include "bt_mesh_user_api.h"
#include "osdep_service.h"
#include "platform_os.h"

#define BT_MESH_PROV_CMD_RETRY_COUNT 2

/** @brief bt mesh provisioner command table */
enum bt_mesh_provisioner_cmd
{
	GEN_MESH_CODE(_pb_adv_con) ,	/*0*/
 	GEN_MESH_CODE(_prov) ,    
 	GEN_MESH_CODE(_app_key_add) , 
	GEN_MESH_CODE(_model_app_bind) , 
	GEN_MESH_CODE(_model_pub_set) ,  
	GEN_MESH_CODE(_generic_on_off_set) , /*5*/
	GEN_MESH_CODE(_generic_on_off_get) ,
	GEN_MESH_CODE(_node_reset) ,
 	GEN_MESH_CODE(_model_sub_delete) ,
 	GEN_MESH_CODE(_model_sub_add) ,
 	GEN_MESH_CODE(_model_sub_get) , /*10*/
    GEN_MESH_CODE(_prov_discover) ,
    GEN_MESH_CODE(_prov_cccd_operate) ,
    GEN_MESH_CODE(_proxy_discover) ,
    GEN_MESH_CODE(_proxy_cccd_operate) ,
    GEN_MESH_CODE(_connect) , /*15*/
 	GEN_MESH_CODE(_disconnect) ,
 	GEN_MESH_CODE(_list) ,
	
	MAX_MESH_PROVISIONER_CMD
};

/**
  * @brief initialize bt mesh provisioner api
  *
  * @return 1: success 2: fail
  */
uint8_t bt_mesh_provisioner_api_init(void);

/**
  * @brief deinitialize bt mesh provisioner api
  *
  * @return none
  */
void bt_mesh_provisioner_api_deinit(void);

#endif

