/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     mesh_beacon.h
  * @brief    Head file for mesh beacon.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2015-8-27
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _MESH_BEACON_H
#define _MESH_BEACON_H

/* Add Includes here */
#include "platform_types.h"

BEGIN_DECLS

/** @addtogroup Mesh_Beacon
  * @{
  */

/** @defgroup Mesh_Beacon_Exported_Types Exported Types
  * @{
  */
enum
{
    MESH_BEACON_TYPE_UNB, //!< Unprovisioned Node Broadcast Beacon
    MESH_BEACON_TYPE_SNB //!< Secure Network Broadcast Beacon
} _SHORT_ENUM_;
typedef uint8_t beacon_type_t;

typedef struct
{
    uint8_t dev_uuid[16];
    uint8_t oob_info[2]; // @ref prov_oob_info_t
    uint8_t uri_hash[4]; //!< optional field
} _PACKED_ beacon_udb_t, *beacon_udb_p;

typedef struct
{
    /*
     * phase 1: old NetKey + Key Refresh flag set to 0
     * phase 2: new NetKey + Key Refresh flag set to 1
     * phase 3: new NetKey + Key Refresh flag set to 0
     */
    uint8_t key_refresh_flag: 1; //!< bit0: key refresh flag
    /*
     * normal: new iv index + iv_update_flag set to 0
     * update: old iv index + iv_update_flag set to 1
     */
    uint8_t iv_update_flag: 1; //!< bit1: IV update flag
    uint8_t rfu: 6; //!< reserved for future use
    uint8_t net_id[8]; //!< Network ID
    uint8_t iv_index[4]; //!< IV Index
    uint8_t auth[8]; //!< Authenticates the packet contents, computed with the Network Key over the [KR, NID, CIVI] fields
} _PACKED_ beacon_snb_t, *beacon_snb_p;

typedef struct
{
    beacon_type_t beacon_type;
    union
    {
        beacon_udb_t  udb;
        beacon_snb_t snb;
    };
} _PACKED_ beacon_t, *beacon_p;

/** @} */

/** @defgroup Mesh_Beacon_Exported_Functions Exported Functions
  * @{
  */

/**
  * @brief generate uri hash
  * @param[in] uri_data: uri
  * @param[in] len: uri length
  * @return none
  */
void beacon_uri_hash_gen(uint8_t uri_data[], uint16_t len);

/**
  * @brief send snb via proxy
  * @return none
  */
void beacon_snb_send_via_proxy(void);

/**
  * @brief send beacon
  * @return none
  */
void beacon_send(void);

/**
  * @brief start beacon
  * @return none
  */
void beacon_start(void);

/**
  * @brief stop beacon
  * @return none
  */
void beacon_stop(void);

/**
  * @brief recevie beacon
  * @param[in] bt_addr: bt addr
  * @param[in] bt_addr_type: bt addr type
  * @param[in] rssi: rssi
  * @param[in] pbuffer: beacon
  * @param[in] len: beacon length
  * @return none
  */
void beacon_receive(uint8_t bt_addr[6], uint8_t bt_addr_type, int8_t rssi, uint8_t *pbuffer,
                    uint8_t len);

/** @} */
/** @} */

END_DECLS

#endif /* _MESH_BEACON_H */

