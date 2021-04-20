/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_service.h
  * @brief    Head file for mesh service.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2016-3-14
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _MESH_SERVICE_H
#define _MESH_SERVICE_H

/* Add Includes here */
#include "mesh_provision.h"
#include "mesh_config.h"

BEGIN_DECLS

/** @addtogroup Mesh_Service
  * @{
  */

/** @defgroup Mesh_Service_Exported_Macros Exported Macros
  * @{
  */
#if defined(MESH_DEVICE) && MESH_DEVICE
#if MESH_DEVICE_PROV_PROXY_SERVER_COEXIST
#define MESH_GATT_SERVER_COUNT                              2
#else
#define MESH_GATT_SERVER_COUNT                              1
#endif
#else
#define MESH_GATT_SERVER_COUNT                              0
#endif

#if defined(MESH_PROVISIONER) && MESH_PROVISIONER
#define MESH_GATT_CLIENT_COUNT                              2
#else
#define MESH_GATT_CLIENT_COUNT                              1 //!< or 0 ?
#endif

#define MESH_SERVICE_ADV_DATA_HEADER_SERVICE_DATA_OFFSET    7
/** @} */

/** @defgroup Mesh_Service_Exported_Types Exported Types
  * @{
  */

/** little endian */
typedef struct
{
    uint8_t dev_uuid[16];
    prov_oob_info_t oob_info;
} _PACKED_ mesh_service_data_provision_t;

enum
{
    PROXY_ADV_TYPE_NET_ID = 0,
    PROXY_ADV_TYPE_NODE_IDENTITY = 1,
#if MESH_PRB
    PROXY_ADV_TYPE_PRIVATE_NET_ID = 2,
    PROXY_ADV_TYPE_PRIVATE_NODE_IDENTITY = 3,
#endif
} _SHORT_ENUM_;
typedef uint8_t proxy_adv_type_t;

typedef struct
{
    proxy_adv_type_t type;
    union
    {
        uint8_t net_id[8];
        struct
        {
            uint8_t hash[8];
            uint8_t random[8];
        };
    };
} _PACKED_ mesh_service_data_proxy_t;

typedef union
{
    mesh_service_data_provision_t  provision;
    mesh_service_data_proxy_t proxy;
} _PACKED_ mesh_service_data_t, *mesh_service_data_p;

typedef enum
{
    MESH_SERVCIE_ADV_CFG_TYPE_PROV_RETRANS_COUNT = 0, //!< uint8_t
    MESH_SERVCIE_ADV_CFG_TYPE_PROV_RETRANS_INTERVAL = 1, //!< uint16_t, unit: ms
    MESH_SERVCIE_ADV_CFG_TYPE_PROXY_RETRANS_COUNT = 2, //!< uint8_t
    MESH_SERVCIE_ADV_CFG_TYPE_PROXY_RETRANS_INTERVAL = 3, //!< uint16_t, unit: ms
    MESH_SERVCIE_ADV_CFG_TYPE_PROXY_WITH_BT_ADDR = 4, //!< bool
#if MESH_PRB
    MESH_SERVCIE_ADV_CFG_TYPE_PRIVATE_PROXY_RETRANS_COUNT = 5, //!< uint8_t
    MESH_SERVCIE_ADV_CFG_TYPE_PRIVATE_PROXY_RETRANS_INTERVAL = 6, //!< uint16_t, unit: ms
#endif
} mesh_service_adv_cfg_type_t;
/** @} */

/** @defgroup Mesh_Service_Exported_Functions Exported Functions
  * @{
  */

///@cond
void mesh_service_adv_send(void);
void mesh_service_adv_receive(uint8_t bt_addr[6], uint8_t addr_type, int8_t rssi, uint8_t *pbuffer,
                              uint16_t len);
void mesh_service_identity_adv_send(uint16_t net_key_index);
void mesh_service_identity_adv_rr(void);
void mesh_service_identity_adv_start(void);
void mesh_service_identity_adv_trigger(bool on_off);
#if MESH_PRB
void mesh_private_service_adv_start(void);
void mesh_private_service_adv_stop(void);
void mesh_private_service_adv_send(void);
void mesh_private_service_identity_adv_send(uint16_t net_key_index);
void mesh_private_service_identity_adv_rr(void);
void mesh_private_service_identity_adv_start(void);
void mesh_private_service_identity_adv_trigger(bool on_off);
#endif
void mesh_service_init(void);
void mesh_service_deinit(void);
///@endcond

/**
  * @brief start the mesh service adv
  * @return none
  */
void mesh_service_adv_start(void);

/**
  * @brief stop the mesh service adv
  * @return none
  */
void mesh_service_adv_stop(void);

/**
  * @brief configure the mesh service adv
  * @param[in] type: the parameter type, @ref mesh_service_adv_cfg_type_t
  * @param[in] param: the parameter pointer
  * @param[in] param_len: the parameter length
  * @return operation result
  */
bool mesh_service_adv_cfg(mesh_service_adv_cfg_type_t type, uint8_t *param, uint16_t param_len);

/** @} */
/** @} */

END_DECLS

#endif /* _MESH_SERVICE_H */

