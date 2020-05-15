/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     gap_manufacture_adv.h
  * @brief    Head file for GAP manufacture adv used by RTK.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2018-10-10
  * @version  v1.0
  * *************************************************************************************
  */

#ifndef _GAP_MANUFACTURE_ADV_H
#define _GAP_MANUFACTURE_ADV_H

/* Add Includes here */
#include "platform_misc.h"
#include "gap.h"
#include "gap_conn_le.h"

BEGIN_DECLS

/** @addtogroup Gap_Manufacture_Adv
  * @{
  */

/** @defgroup Gap_Manufacture_Adv_Exported_Macros Exported Macros
  * @brief
  * @{
  */
#define MANUFACTURE_ADV_DATA_COMPANY_ID         0x005d //!< Realtek

#define MANUFACTURE_ADV_DATA_TYPE_BT_ADDR       0x00
#define MANUFACTURE_ADV_DATA_TYPE_DFU           0x01
#define MANUFACTURE_ADV_DATA_TYPE_GROUP         0x02

/* For Bee2 Bcut */
#define DFU_PRODUCT_ID_TEST                     0x00000000
#define DFU_PRODUCT_ID_MESH_PROVISIONER         0x00000001
#define DFU_PRODUCT_ID_MESH_DEVICE              0x00000002
#define DFU_PRODUCT_ID_MESH_LIGHT               0x00000003
#define DFU_PRODUCT_ID_MESH_ALI_LIGHT           0x00000004
#define DFU_PRODUCT_ID_MESH_SINGLE_FIRE_SWITCH  0x00000005
#define DFU_PRODUCT_ID_MESH_UNKNOWN             0x00000006
#define DFU_PRODUCT_ID_GROUP_RCU                0x00000007
/** @} */

/** @defgroup Gap_Manufacture_Adv_Exported_Types Exported Types
  * @brief
  * @{
  */
typedef struct
{
    uint32_t product_id;
    uint32_t app_version;
    uint32_t patch_version;
} _PACKED_ manufacture_adv_data_dfu_t;

typedef struct
{
    uint16_t company_id;
    uint8_t type;
    union
    {
        uint8_t data[1]; //!< fake
        /* MANUFACTURE_ADV_DATA_TYPE_BT_ADDR */
        uint8_t bt_addr[6];
        /* MANUFACTURE_ADV_DATA_TYPE_DFU */
        manufacture_adv_data_dfu_t dfu;
    };
} _PACKED_ manufacture_adv_data_t;
/** @} */
/** @} */

END_DECLS

#endif /* _GAP_MANUFACTURE_ADV_H */
