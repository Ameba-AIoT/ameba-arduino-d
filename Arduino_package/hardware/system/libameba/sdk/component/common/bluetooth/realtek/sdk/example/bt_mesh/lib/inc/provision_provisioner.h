/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     provision_provisioner.h
  * @brief    Head file for provisioner.
  * @details  Data structs and external functions declaration.
  * @author   bill
  * @date     2015-11-09
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PROVISION_PROVISIONER_H_
#define _PROVISION_PROVISIONER_H_

/* Add Includes here */
#include "provision_generic.h"
#include "mesh_api.h"
#include "mesh_provision.h"

BEGIN_DECLS

/** @addtogroup Mesh_Prov_Provisioner
  * @brief
  * @{
  */

/** @defgroup Mesh_Prov_Provisioner_Exported_Types Exported Types
  * @brief
  * @{
  */
extern uint16_t assign_net_key_index;
extern uint16_t assign_addr;
extern int16_t assign_idx;
/** @} */

/** @defgroup Mesh_Prov_Provisioner_Exported_Functions Exported Functions
  * @brief
  * @{
  */

/**
  * @brief start the provisionging
  *
  * The function shall be called at the appropriate timing.
  * @param[in] attn_dur: attention duration
  * @return operation result
  */
bool prov_invite(uint8_t attn_dur);

/**
  * @brief assign the idx and addr to the new device
  *
  * The function can be called after @ref prov_invite and before @ref prov_send_prov_data,
  * normally at the @ref PROV_CB_TYPE_PATH_CHOOSE case handler.
  * The default idx assign scheme is to find the unused entry or the least addr entry.
  * The default addr assign scheme is the maximum address of all devices plus 1.
  * @param[in] idx: the entry index, < 0 represents to use the default assign scheme
  * @param[in] addr: the address, 0 represents to use the default assign scheme
  * @return the assign index
  */
uint16_t prov_assign(int16_t idx, uint16_t addr);

/**
  * @brief stop the provisioning
  * @return operation result
  */
bool prov_reject(void);

/**
  * @brief unprov the device in the pb-gatt
  * @deprecated
  * @return operation result
  */
bool prov_unprovisioning(void);

/**
  * @brief choose one path from the eight prov pathes
  *
  * publick key: no oob publick key & oob publick key
  * auth data: no oob & input & output & static
  * @param[in] pprov_start: using the start pdu to choose
  * @return operation result
  */
bool prov_path_choose(prov_start_p pprov_start);

/**
  * @brief set the public key of the provisioning device
  *
  * The function shall be called at the appropriate timing.
  * @param[in] public_key: the ecc public key of the device
  * @return operation result
  */
bool prov_device_public_key_set(uint8_t public_key[64]);

/**
  * @brief set the random and confirmation without the knownledge of auth value
  *
  * The app has no need to provide the auth value to the stack.
  * @param[in] random: the random
  * @param[in] conf: the confirmation
  * @return operation result
  */
bool prov_auth_random_conf_set(uint8_t random[16], uint8_t conf[16]);

/** @} */
/** @} */

END_DECLS

#endif  /* _PROVISION_PROVISIONER_H_ */
