/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     provision_device.h
  * @brief    Head file for mesh device.
  * @details
  * @author   bill
  * @date     2015-11-09
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _PROVISION_DEVICE_H_
#define _PROVISION_DEVICE_H_

/* Add Includes here */
#include "provision_generic.h"
#include "provision_adv.h"

BEGIN_DECLS

/** @addtogroup Mesh_Prov_Device
  * @brief
  * @{
  */

/** @defgroup Mesh_Prov_Device_Exported_Functions Exported Functions
  * @brief
  * @{
  */


/**
  * @brief disconnect the prov bearer
  *
  * The spec requires the provisioner to disconnect the bearer after the provision procedure.
  * The mesh stack leaves the app to decide whether to disconnect at the case @ref PROV_CB_TYPE_COMPLETE.
  * @param[in] reason: pb-adv bearer need the disconnect reason
  * @return operation result
  */
bool prov_disconnect_dev(pb_adv_link_close_reason_t reason);

/**
  * @brief set the auth random value
  *
  * The function shall be called at the appropriate time.
  * @param[in] random: random value
  * @return operation result
  */
bool prov_auth_random_set_dev(uint8_t random[16]);

/**
  * @brief change the auth value
  *
  * The function shall be called at the appropriate time. The auth value will changed immediately
  * @param[in] pvalue: auth value
  * @param[in] len: value length
  * @return operation result
  */
bool prov_auth_value_change_dev(uint8_t *pvalue, uint8_t len);

/**
  * @brief set the auth value
  *
  * The function shall be called at the appropriate time. The auth value shall be
  * set with the appropriate length and data format.
  * @param[in] pvalue: auth value
  * @param[in] len: value length
  * @return operation result
  */
bool prov_auth_value_set_dev(uint8_t *pvalue, uint8_t len);

/**
 * @brief device process provision data
 * @param[in] pdata: provision data
 * @param[in] len: provision data length
 */
void device_receive(uint8_t *pdata, uint16_t len);

/**
 * @brief device send data
 * @param[in] pdata: provision data
 * @param[in] len: provision data length
 */
bool prov_send_dev(uint8_t *pdata, uint16_t len);

/**
 * @brief initialize provision resources
 */
void prov_init_dev(void);

/** @} */
/** @} */

END_DECLS

#endif /* _PROVISION_DEVICE_H_ */
