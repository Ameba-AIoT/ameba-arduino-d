/**
*********************************************************************************************************
*               Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      dfu_distributor_app.h
* @brief     Smart mesh dfu application
* @details
* @author    bill
* @date      2018-6-6
* @version   v1.0
* *********************************************************************************************************
*/

#ifndef _DFU_DIST_MODELS_H
#define _DFU_DIST_MODELS_H

#ifdef __cplusplus
extern "C" {
#endif

#include "mesh_api.h"
#include "firmware_distribution.h"
#include "firmware_update.h"
#include "blob_transfer.h"
#include "dfudep_service.h"

#if MESH_DFU
/**
 * @addtogroup DFU_DISTRIBUTOR_APP
 * @{
 */

/**
 * @defgroup Dfu_Distributor_Exported_Functions Dfu Distributor Exported Functions
 * @brief
 * @{
 */
#define DFU_DIST_APP_TIMEOUT_MSG                 121

typedef uint32_t (*fw_image_data_get_t)(uint32_t len, uint8_t *pout);
void dfu_dist_models_init(void);
bool dfu_dist_receiver_add(fw_dist_receiver_t *precv);
void dfu_dist_receiver_remove(uint16_t addr);
void dfu_dist_receiver_remove_by_index(uint16_t index);
void dfu_dist_receiver_remove_all(void);
bool dfu_dist_start(uint16_t dst, uint16_t app_key_index, uint16_t update_timeout_base,
                    uint8_t *pfw_metadata, uint8_t metadata_len,
                    uint32_t fw_image_size, fw_image_data_get_t fw_image_data_get);
void dfu_dist_clear(void);
void dfu_dist_handle_timeout(void);
/** @} */
/** @} */
#endif

#ifdef __cplusplus
}
#endif

#endif /* _DFU_DIST_MODELS_H */
