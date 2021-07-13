/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
* @file     scene.h
* @brief    Head file for scene models.
* @details  Data types and external functions declaration.
* @author   hector_huang
* @date     2018-8-23
* @version  v1.0
* *************************************************************************************
*/

#ifndef _SCENE_H_
#define _SCENE_H_


#include "mesh_api.h"
#include "generic_transition_time.h"

BEGIN_DECLS

/**
 * @addtogroup SCENE
 * @{
 */

/**
 * @defgroup SCENE_ACCESS_OPCODE Access Opcode
 * @brief Mesh message access opcode
 * @{
 */
#define MESH_MSG_SCENE_GET                              0x8241
#define MESH_MSG_SCENE_RECALL                           0x8242
#define MESH_MSG_SCENE_RECALL_UNACK                     0x8243
#define MESH_MSG_SCENE_STATUS                           0x5E
#define MESH_MSG_SCENE_REGISTER_GET                     0x8244
#define MESH_MSG_SCENE_REGISTER_STATUS                  0x8245
#define MESH_MSG_SCENE_STORE                            0x8246
#define MESH_MSG_SCENE_STORE_UNACK                      0x8247
#define MESH_MSG_SCENE_DELETE                           0x829E
#define MESH_MSG_SCENE_DELETE_UNACK                     0x829F
/** @} */

/**
 * @defgroup SCENE_MODEL_ID Model ID
 * @brief Mesh model id
 * @{
 */
#define MESH_MODEL_SCENE_SERVER                          0x1203FFFF
#define MESH_MODEL_SCENE_SETUP_SERVER                    0x1204FFFF
#define MESH_MODEL_SCENE_CLIENT                          0x1205FFFF
/** @} */

/**
 * @defgroup SCENE_MESH_MSG Mesh Msg
 * @brief Mesh message types used by models
 * @{
 */
#define IS_SCENE_NUMBER_VALID(number) (0x00 != number)

enum
{
    SCENE_STATUS_SUCCESS,
    SCENE_STATUS_REGISTER_FULL,
    SCENE_STATUS_NOT_FOUND
} _SHORT_ENUM_;
typedef uint8_t scene_status_code_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SCENE_STORE)];
    uint16_t scene_number;
} _PACKED_ scene_store_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SCENE_RECALL)];
    uint16_t scene_number;
    uint8_t tid;
    generic_transition_time_t trans_time;
    uint8_t delay;
} _PACKED_ scene_recall_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SCENE_GET)];
} _PACKED_ scene_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SCENE_STATUS)];
    scene_status_code_t status;
    uint16_t current_scene;
    uint16_t target_scene;
    generic_transition_time_t remaining_time;
} _PACKED_ scene_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SCENE_REGISTER_GET)];
} _PACKED_ scene_register_get_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SCENE_REGISTER_STATUS)];
    scene_status_code_t status;
    uint16_t current_scene;
    uint16_t scenes[0];
} _PACKED_ scene_register_status_t;

typedef struct
{
    uint8_t opcode[ACCESS_OPCODE_SIZE(MESH_MSG_SCENE_DELETE)];
    uint16_t scene_number;
} _PACKED_ scene_delete_t;

/** 0 means unused scene number */
typedef struct
{
    uint16_t scene_number;
    void *pmemory;
} scene_storage_memory_t;
/** @} */

/**
 * @defgroup SCENE_TRANSITION_TYPE Transition Type
 * @brief Mesh message transition and delay execution type
 * @{
 */
#define GENERIC_TRANSITION_TYPE_SCENE            0
#define DELAY_EXECUTION_TYPE_SCENE               0
/** @} */


/**
 * @defgroup SCENE_SERVER_DATA Server Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define SCENE_SERVER_GET                                       0 //!< @ref scene_server_get_t
#define SCENE_SERVER_GET_REGISTER_STATUS                       1 //!< @ref scene_server_get_register_status_t
#define SCENE_SERVER_GET_DEFAULT_TRANSITION_TIME               2 //!< @ref scene_server_get_default_transition_time_t
#define SCENE_SERVER_STORE                                     3 //!< @ref scene_server_store_t
#define SCENE_SERVER_RECALL                                    4 //!< @ref scene_server_recall_t
#define SCENE_SERVER_DELETE                                    5 //!< @ref scene_server_delete_t

typedef struct
{
    uint16_t current_scene;
} scene_server_get_t;

typedef struct
{
    scene_status_code_t status;
} scene_server_get_register_status_t;

typedef struct
{
    generic_transition_time_t trans_time;
} scene_server_get_default_transition_time_t;

typedef struct
{
    scene_status_code_t status;
    uint16_t scene_number;
    void *pmemory;
} scene_server_store_t;

typedef struct
{
    uint16_t scene_number;
    void *pmemory;
    generic_transition_time_t total_time;
    generic_transition_time_t remaining_time;
} scene_server_recall_t;

typedef struct
{
    uint16_t scene_number;
} scene_server_delete_t;
/** @} */

/**
 * @defgroup SCENE_CLIENT_DATA Client Data
 * @brief Data types and structure used by data process callback
 * @{
 */
#define SCENE_CLIENT_STATUS                                    0 //!< @ref scene_client_status_t
#define SCENE_CLIENT_REGISTER_STATUS                           1 //!< @ref scene_client_register_status_t

typedef struct
{
    uint16_t src;
    scene_status_code_t status;
    uint16_t current_scene;
    bool optional;
    uint16_t target_scene;
    generic_transition_time_t remaining_time;
} scene_client_status_t;

typedef struct
{
    uint16_t src;
    scene_status_code_t status;
    uint16_t current_scene;
    uint16_t *scene_array;
    uint8_t scene_array_len;
} scene_client_register_status_t;
/** @} */

/**
 * @defgroup SCENE_SERVER_API Server API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register scene server model
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to scene server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool scene_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief register scene setup server
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to scene setup server model context
 * @retval true: register success
 * @retval false: register failed
 */
bool scene_setup_server_reg(uint8_t element_index, mesh_model_info_p pmodel_info);
/**
 * @brief publish scene information
 * @param[in] pmodel_info: pointer to scene server model context
 * @param[in] scene: current scene
 * @return publish status
 */
mesh_msg_send_cause_t scene_publish(const mesh_model_info_p pmodel_info,
                                    uint16_t scene);

/**
 * @brief set scene server storage memory
 * @param[in] pmodel_info: pointer to scene server model context
 * @param[in] scenes: scene storage memeories
 * @param[in] num_scenes: number of scenes
 */
void scene_server_set_storage_memory(mesh_model_info_p pmodel_info, scene_storage_memory_t *scenes,
                                     uint16_t num_scenes);

/**
 * @brief set scene setup server storage memory
 * @param[in] pmodel_info: pointer to scene setup server model context
 * @param[in] scenes: scene storage memeories
 * @param[in] num_scenes: number of scenes
 */
void scene_setup_server_set_storage_memory(mesh_model_info_p pmodel_info,
                                           scene_storage_memory_t *scenes, uint16_t num_scenes);
/** @} */

/**
 * @defgroup SCENE_CLIENT_API Client API
 * @brief Functions declaration
 * @{
 */

/**
 * @brief register scene client
 * @param[in] element_index: element index that model registered to
 * @param[in] pmodel_info: pointer to scene client model context
 * @retval TRUE: register success
 * @retval FALSE: register failed
 */
bool scene_client_reg(uint8_t element_index, mesh_model_info_p pmodel_info);

/**
 * @brief store current state to scene
 * @param[in] pmodel_info: pointer to scene client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] scene_number: desired scene number
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t scene_store(const mesh_model_info_p pmodel_info, uint16_t dst,
                                  uint16_t app_key_index, uint16_t scene_number, bool ack);

/**
 * @brief recall specified scene
 * @param[in] pmodel_info: pointer to scene client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] scene_number: desired scene number
 * @param[in] tid: transition identify value
 * @param[in] optional: represent whether contains transition time or not
 * @param[in] trans_time: new transition time
 * @param[in] delay: new delay time
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t scene_recall(const mesh_model_info_p pmodel_info, uint16_t dst,
                                   uint16_t app_key_index, uint16_t scene_number, uint8_t tid, bool optional,
                                   generic_transition_time_t trans_time, uint8_t delay, bool ack);
/**
 * @brief get scene status
 * @param[in] pmodel_info: pointer to scene client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t scene_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                uint16_t app_key_index);

/**
 * @brief get scene register status
 * @param[in] pmodel_info: pointer to scene client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @return send status
 */
mesh_msg_send_cause_t scene_register_get(const mesh_model_info_p pmodel_info, uint16_t dst,
                                         uint16_t app_key_index);

/**
 * @brief delete scene
 * @param[in] pmodel_info: pointer to scene client model context
 * @param[in] dst: remote address
 * @param[in] app_key_index: mesh message used app key index
 * @param[in] scene_number: scene number need to be deleted
 * @param[in] ack: acknowledge flag
 * @return send status
 */
mesh_msg_send_cause_t scene_delete(const mesh_model_info_p pmodel_info, uint16_t dst,
                                   uint16_t app_key_index, uint16_t scene_number, bool ack);

/** @} */
/** @} */

END_DECLS

#endif /* _SCENE_H_ */
