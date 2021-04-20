/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_flash.h
  * @brief    Head file for mesh flash storage.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2017-2-21
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _MESH_FLASH_H
#define _MESH_FLASH_H

/* Add Includes here */
#include "platform_misc.h"
#include "ftl.h"
#include "mesh_api.h"

BEGIN_DECLS

/** @addtogroup Mesh_Flash
  * @{
  */

/** @defgroup Mesh_Flash_Exported_Macros Exported Macros
  * @{
  */

/** fake num */
#ifdef MESH_PROVISIONER
#define MESH_DEV_KEY_NUM_MAX                    3
#else
#define MESH_DEV_KEY_NUM_MAX                    1
#endif
#define MESH_NET_KEY_NUM_MAX                    3
#define MESH_APP_KEY_NUM_MAX                    3
#define MESH_VIR_ADDR_NUM_MAX                   3
#define MESH_RPL_ENTRY_NUM_MAX                  20
#define MESH_MODEL_NUM_MAX                      1
#define MESH_MODEL_SUB_ADDR_NUM_MAX             10

// Flash Storage Offset
#define MESH_FLASH_OFFSET                       mesh_node.flash_offset
#define MESH_FLASH_SIZE_MAX                     mesh_node.flash_size

/** @} */

/** @defgroup Mesh_Flash_Exported_Types Exported Types
  * @{
  */

typedef enum
{
    MESH_FLASH_PARAMS_NODE_INFO,
    MESH_FLASH_PARAMS_IV_INDEX,
    MESH_FLASH_PARAMS_IV_UPDATE_FLAG,
    MESH_FLASH_PARAMS_SEQ,
    MESH_FLASH_PARAMS_TTL,
    MESH_FLASH_PARAMS_FEATURES,
    MESH_FLASH_PARAMS_NWK_TX_PARAMS,
    MESH_FLASH_PARAMS_HB_PARAMS,
    MESH_FLASH_PARAMS_DEV_KEY,
    MESH_FLASH_PARAMS_NET_KEY,
    MESH_FLASH_PARAMS_APP_KEY,
    MESH_FLASH_PARAMS_VIR_ADDR,
    MESH_FLASH_PARAMS_RPL,
    MESH_FLASH_PARAMS_RPL_ENTRY,
    MESH_FLASH_PARAMS_RPL_SEQ,
    MESH_FLASH_PARAMS_MODEL_PUB_PARAMS,
    MESH_FLASH_PARAMS_MODEL_APP_KEY_BINDING,
    MESH_FLASH_PARAMS_MODEL_SUBSCRIBE_ADDR
} flash_params_type_t;

typedef struct
{
    uint8_t rpl_loop;
    uint16_t entry_loop;
} rpl_entry_info_t;

/** @} */

/** @defgroup Mesh_Flash_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief save the data to the Flash just like the RAM
  *
  * There is no need to worry about the read/erase/write operation of the flash
  * @param[in] pdata: the data pointer
  * @param[in] size: the data size
  * @param[in] offset: the data offset
  * @return the operation result
  * @retval 0: success
  * @retval !0: fail
  */
#define mesh_flash_save(pdata, size, offset)            ftl_save(pdata, offset, size)

/**
  * @brief load the data to the Flash just like the RAM
  *
  * There is no need to worry about the read/erase/write operation of the flash
  * @param[out] pdata: the data pointer
  * @param[in] size: the data size
  * @param[in] offset: the data offset
  * @return the operation result
  * @retval 0: success
  * @retval !0: fail
  */
#define mesh_flash_load(pdata, size, offset)            ftl_load(pdata, offset, size)

/**
  * @brief load the node state from the NVM
  *
  * It can be usde to get the node state in advance of mesh_init.
  * !!!Attention: the API shall be called after all the models have bee registered!!!
  *
  * @return the node state
  */
mesh_node_state_t mesh_flash_restore_node_state(void);
#define mesh_node_state_restore mesh_flash_restore_node_state

///@cond
void mesh_flash_store(flash_params_type_t param_type, void *param);
void mesh_flash_restore(void);
void mesh_flash_clear(void);
bool mesh_flash_restore_msg_id(uint16_t *unicast_addr, uint32_t *iv_index, uint32_t *seq,
                               bool *iv_update_flag);
bool mesh_flash_retore_net_key(uint16_t net_key_index, uint16_t *net_key_index_g,
                               uint8_t key[MESH_COMMON_KEY_SIZE]);
uint32_t mesh_flash_total_size(void);
///@endcond

/** @} */
/** @} */

END_DECLS

#endif /* _MESH_FLASH_H */

