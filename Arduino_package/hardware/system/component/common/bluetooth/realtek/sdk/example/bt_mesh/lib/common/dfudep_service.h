/******************************************************************************
 *
 * Copyright(c) 2019 - 2021 Realtek Corporation. All rights reserved.
 *                                        
 ******************************************************************************/
#ifndef __DFUDEP_SERVICE_H_
#define __DFUDEP_SERVICE_H_

#include "generic_types.h"
#include "ftl.h"
#include "firmware_update.h"

#if defined(MESH_DFU) && MESH_DFU

#define DFU_BLOCK_SIGNATURE_SIZE                        0
#define FW_VERSION_MAX_LEN                              106

/** @brief Indicate the result of dfu api */
typedef enum
{
    DFU_RESULT_OK,
    DFU_RESULT_FAIL
} dfu_result_t;

/**
  * @brief check whether metadata received from update client is acceptable
  *
  * @param[in] pmetadata: metadata pointer of the received metadata from update client
  * @param[in] len: metadata length
  * @return DFU_RESULT_OK for success, DFU_RESULT_FAIL for fail
  */
uint8_t dfu_metadata_check(void *pmetadata, uint32_t len);

/**
  * @brief check if there is sufficient resources for the device firmware update
  *
  * @return DFU_RESULT_OK for success, DFU_RESULT_FAIL for fail
  */
uint8_t dfu_resources_check(void);

/**
  * @brief init dfu resources
  *
  * @return DFU_RESULT_OK for success, DFU_RESULT_FAIL for fail
  */
uint8_t dfu_init_dfu_resources(void);

/**
  * @brief image block file received check action(which is specific defined by upper layer)
  *
  * @param[in] data: pointer to the received block file
  * @param[in] len: image file length within the received block filename
  * @param[in] signature_size: signature length
  * @return DFU_RESULT_OK for success, DFU_RESULT_FAIL for fail
  */
uint8_t dfu_block_verify(uint8_t *data ,uint16_t len, uint8_t signature_size);

/**
  * @brief whole image file received check action
  *
  * @param[in] image_id: the image id of the verifying image
  * @return DFU_RESULT_OK for success, DFU_RESULT_FAIL for fail
  */
uint8_t dfu_verify(uint16_t image_id);

/**
  * @brief after received the cancel request from client, free and deinit some flash resources
  *
  * @return DFU_RESULT_OK for success, DFU_RESULT_FAIL for fail
  */
uint8_t dfu_cancel(void);

/**
  * @brief after received the verify cancel request from client, free and deinit some flash resources
  *
  * @return DFU_RESULT_OK for success, DFU_RESULT_FAIL for fail
  */
uint8_t dfu_verify_cancel(void);

/**
  * @brief apply the received latest image
  *
  * @param[in] reason: reboot reason
  * @param[in] delay_ms: reboot after delay ms
  * @return DFU_RESULT_OK for success, DFU_RESULT_FAIL for fail
  */
uint8_t dfu_apply(uint8_t reason, uint32_t delay_ms);

/**
  * @brief save the received block image file to flash
  *
  * @param[in] len: block image file length
  * @param[in] data: pointer to the block image file
  * @param[in] signature_size: signature length
  * @return DFU_RESULT_OK for success, DFU_RESULT_FAIL for fail
  */
uint8_t dfu_block_data_restore(uint8_t * data , uint32_t len, uint8_t signature_size);

/**
  * @brief when dfu fails, free and deinit flash resources
  *
  * @return None
  */
void dfu_failsafe(void);

/**
  * @brief check whether dfu is enable
  *
  * @return DFU_RESULT_OK for success, DFU_RESULT_FAIL for fail
  */
uint8_t is_dfu_enabled(void);

/**
  * @brief get the current firmware information
  *
  * @param[in] pfw_info: pointer to fw_info
  * @return DFU_RESULT_OK for success, DFU_RESULT_FAIL for fail
  */
uint8_t dfu_get_fw_info(fw_info_t *pfw_info);

/**
  * @brief fetch the image file, which is going to be sent by the dfu client
  *
  * @param[in] len: image block file length
  * @param[in] pout: the destination address. where the image block will be buffed for transmitting
  * @return DFU_RESULT_OK for success, DFU_RESULT_FAIL for fail
  */
uint32_t dfu_fw_image_data_get(uint32_t len, uint8_t *pout);

/**
  * @brief sign the block data
  *
  * @param[in] data: pointer to the image file
  * @param[in] len: image block file length
  * @param[in] signature_size: signature length
  * @return DFU_RESULT_OK for success, DFU_RESULT_FAIL for fail
  */
uint8_t dfu_block_signature(uint8_t *data ,uint16_t len, uint8_t signature_size);

/**
  * @brief check whether the received node's firmware info needs firmware update
  *
  * @param[in] pfw_info: pointer to fw_info
  * @return DFU_RESULT_OK for Nedd Update, DFU_RESULT_FAIL for No
  */
uint8_t dfu_check_fw_info(fw_info_t *pfw_info);

struct dfudep_service_ops {
  uint8_t (*dfu_metadata_check)(void *pmetadata, uint32_t len);
  uint8_t (*dfu_resources_check)(void);
  uint8_t (*dfu_init_dfu_resources)(void);
  uint8_t (*dfu_block_verify)(uint8_t *data ,uint16_t len, uint8_t signature_size);
  uint8_t (*dfu_verify)(uint16_t image_id);
  uint8_t (*dfu_cancel)(void);
  uint8_t (*dfu_verify_cancel)(void);
  uint8_t (*dfu_apply)(uint8_t reason, uint32_t delay_ms);
  uint8_t (*dfu_block_data_restore)(uint8_t * data , uint32_t len, uint8_t signature_size);
  void (*dfu_failsafe)(void);
  uint8_t (*is_dfu_enabled)(void);
  uint8_t (*dfu_get_fw_info)(fw_info_t *pfw_info);
  uint32_t (*dfu_fw_image_data_get)(uint32_t len, uint8_t *pout);
  uint8_t (*dfu_block_signature)(uint8_t *data ,uint16_t len, uint8_t signature_size);
  uint8_t (*dfu_check_fw_info)(fw_info_t *pfw_info);
};
#endif
#endif 