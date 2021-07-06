/**
*****************************************************************************************
*     Copyright(c) 2019, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     bt_mesh_user_api.h
  * @brief    Source file for provisioner cmd.
  * @details  User command interfaces.
  * @author   sherman
  * @date     2019-09-16
  * @version  v1.0
  * *************************************************************************************
  */
#ifndef _BT_MESH_USER_API_H_
#define _BT_MESH_USER_API_H_

#include "mesh_user_cmd_parse.h"
#include "provisioner_cmd.h"
#include "device_cmd.h"
#include "osdep_service.h"
#include "os_task.h"
#include "platform_os.h"
#include "mesh_data_uart.h"
#include "app_msg.h"
#include "mesh_config.h"

#define BT_MESH_PROVISIONER 1
#define BT_MESH_DEVICE      2

#define GEN_MESH_CODE(cmd)	cmd ## _cmd_
#define BT_MESH_CMD_ID_PRIV_MOD(cmd, pointer, mode, timeout, start_time) do {\
    		btMeshCmdIdPriv.meshCmdCode = cmd;\
    		btMeshCmdIdPriv.pmeshCmdItem_s = pointer;\
    		btMeshCmdIdPriv.userApiMode = mode;\
    		btMeshCmdIdPriv.semaDownTimeOut = timeout;\
    		btMeshCmdIdPriv.startTime = start_time;\
	    }while(0)
#define PARAM_MESH_CODE(command, option, help, mesh_code)	{command, option, help, mesh_code},

/** @brief declare function for bt mesh cmd thread*/
typedef int (*bt_mesh_func)(uint16_t mesh_code, void *pmeshCmdItem_s);
/** @brief declare function for user call back*/
typedef uint8_t (*user_cmd_cbk)(uint16_t mesh_code, void *user_data);

/** @brief Indicate the result of user command */
enum bt_mesh_op_result
{
    BT_MESH_USER_CMD_FAIL ,
    BT_MESH_USER_CMD_SUCCESS
};

/** @brief Indicate the result of goo command */
enum bt_mesh_goo_result
{
    BT_MESH_GOO_OFF ,
    BT_MESH_GOO_ON
};

/** @brief Indicate the result of user api */
typedef enum
{
    USER_API_RESULT_OK,
    USER_API_RESULT_NOT_FOUND,
    USER_API_RESULT_ERROR,
    USER_API_RESULT_TIMEOUT,
    USER_API_RESULT_INCORRECT_CODE,
    USER_API_RESULT_ERROR_MESH_MODE,
    USER_API_RESULT_NOT_ENABLE,
    USER_API_RESULT_INDICATION_NOT_MATCHED
} user_api_parse_result_t;

/** @brief Indicate the sync mode of user api */
typedef enum {
    USER_API_ASYNCH ,
    USER_API_SYNCH ,
    USER_API_CMDLINE ,
    USER_API_DEFAULT_MODE
} BT_MESH_API_SYNCHCONFIG;

/** @brief Param string cmd to mesh code */
typedef struct _mesh_cmd_entry {
    char                      *pcommand;
    char                      *poption;
    char                      *phelp;
    uint16_t                  meshCode;
} mesh_cmd_entry;

/** @brief bt mesh command private struct */
typedef struct meshCmdModInfo
{
    struct list_head          meshCmdList;              //!< List contains user command.
    uint8_t                   meshMode;                 //!< Indicate currently mesh mode(provisioner/device).
    uint8_t                   cmdListNum;               //!< user command num included in .meshCmdList.
    uint8_t                   cmdTransmittedNum;        //!< user command num which have been sent to mesh stack.
    uint8_t                   meshCmdEnable;            //!< user command enable flag.
    void                      *cmdMutex;                 //!< mesh command private struct mutex.
    void                      *ppvalueMutex;             //!< pparseValue mutex.
    void                      *cmdItemsMutex;            //!< meshCmdItem_s mutex.
    void                      *meshThreadSema;           //!< sema for cmd thread according to current meshMode.
} CMD_MOD_INFO_S;

/** @brief bt mesh cmd thread private struct */
typedef struct meshCmdItem
{
    uint16_t                  meshCmdCode;              //!< user command code index.
    user_cmd_parse_value_t    *pparseValue;             //!< pointer of parameter array.
    bt_mesh_func              meshFunc;                 //!< func will be called in cmd thread according to current meshMode.
    user_cmd_cbk              userCbk;                  //!< suer callback will be involked in bt_mesh_indication if not NULL.
    void                      *userData;                //!< pointer os user private data struct.
} CMD_ITEM;

/** @brief bt mesh user command struct */
struct meshUserItem
{
    uint8_t                   userCmdResult;            //!< indicate results of command to upper layer.
    uint8_t                   userApiMode;              //!< 2: cmd line mode 1: sync mode 0: async mode.
    void                      *userSema;                 //!< sema for sync mode.
    void                      *cmdLineSema;              //!< sema for command line mode.
    user_cmd_parse_value_t    *pparseValue;             //!< pointer of parameter array.
    void                      *userParam;               //!< pointer of user parameter.
};

/** @brief typedef struct meshUserItem */
typedef struct meshUserItem USER_ITEM, *PUSER_ITEM;

/** @brief bt mesh cmd thread private struct*/
typedef struct meshCmdItemSet
{
    struct list_head          list;                     //!< list of .pmeshCmdItem.
    uint8_t                   semaDownTimeOut;          //!< wait time out for sync mode.
    uint8_t                   userApiMode;              //!< 2: cmd line mode 1: sync mode 0: async mode.
    uint8_t                   msgRecvFlag;              //!< indicate the callback has received of previous message.
    CMD_ITEM                  *pmeshCmdItem;            //!< pointer of meshCmdItem.
} CMD_ITEM_S;

/** @brief bt mesh indication private struct */
typedef struct meshCmdIndicationPriv
{
    CMD_ITEM_S                *pmeshCmdItem_s;          //!< pointer of meshCmdItem.
    uint8_t                   semaDownTimeOut;          //!< wait time out for sync mode.
    uint16_t                  meshCmdCode;              //!< mesh code index.
    uint32_t                  userApiMode;              //!< 2: cmd line mode 1: sync mode 0: async mode.
    uint32_t                  startTime;                //!< record the start time for mesh code in process.
} INDICATION_ITEM;

/** @brief bt mesh task private struct */
struct mesh_task_struct {
    void                      *task;	/* I: workqueue thread */
    void                      *wakeup_sema;    /* for internal use only */
    void                      *terminate_sema; /* for internal use only */
    u32                       blocked;          /* for internal use only */
};

/**
  * @brief call back handler of bt mesh io msg
  *
  * @param[in] io_msg: input io msg struct value
  * @return none
  */
void bt_mesh_io_msg_handler(T_IO_MSG io_msg);

/**
  * @brief function configure io_msg and call bt_mesh_send_io_msg
  *
  * @param[in] mesh_code: mesh code index
  * @param[in] pmesh_cmd_item_s: pointer of meshCmdItem_s
  * @return none
  */
int bt_mesh_user_cmd(uint16_t mesh_code, void *pmesh_cmd_item_s);

/**
  * @brief enqueue user command to cmd list and wake up bt mesh cmd threads
  *
  * @param[in] queue: user command list
  * @param[in] head_or_tail: 1 for head, 0 for tail
  * @return 0 for success 1 for fail
  */
uint8_t bt_mesh_enqueue_cmd(struct list_head *queue, uint8_t head_or_tail);

/**
  * @brief dequeue user command from cmd list
  *
  * @return list of dequeue command
  */
struct list_head *bt_mesh_dequeue_cmd(void);

/**
  * @brief search handler included in provisionercmds/devicecmds according to mesh_code index
  *
  * @param[in] mesh_code: mesh code index
  * @param[in] pmesh_cmd_item_s: pointer of meshCmdItem_s
  * @return result of calling handler
  */
uint8_t bt_mesh_user_cmd_hdl(uint16_t mesh_code, CMD_ITEM_S *pmesh_cmd_item_s);

/**
  * @brief allocate struct value for user and configure structure member with the input parameters
  *
  * @param[in] mesh_code: mesh code index
  * @param[in] pparse_value: pointer of parameter array
  * @param[in] func: functions will be involked in bt mesh cmd thread
  * @param[in] cbk: user callback function
  * @param[in] user_data: pointer of user's private structure
  * @return CMD_ITEM_S
  */
CMD_ITEM_S* bt_mesh_cmdreg(uint16_t mesh_code, user_cmd_parse_value_t *pparse_value, bt_mesh_func func, user_cmd_cbk cbk, void *user_data);

/**
  * @brief free struct value
  *
  * @param[in] pmesh_cmd_item_s: pointer to previous allocated memory
  * @return 0
  */
uint8_t bt_mesh_cmdunreg(CMD_ITEM_S *pmesh_cmd_item_s);

/**
  * @brief set user command
  *
  * @param[in] mesh_code: mesh code index
  * @param[in] pparse_value: pointer of parameter array
  * @param[in] func: functions will be involked in bt mesh cmd thread
  * @param[in] cbk: user callback function
  * @param[in] user_data: pointer of user's private structure
  * @return user_api_parse_result_t
  */
user_api_parse_result_t bt_mesh_set_user_cmd(uint16_t mesh_code, user_cmd_parse_value_t *pparse_value, user_cmd_cbk cbk, void *user_data);

/**
  * @brief indicate mesh command result
  *
  * @param[in] mesh_code: mesh code index
  * @param[in] state: result
  * @param[in] pparam: parameters need to be indicated
  * @return user_api_parse_result_t
  */
user_api_parse_result_t bt_mesh_indication(uint16_t mesh_code, uint8_t state, void *pparam);

/**
  * @brief allocate PUSER_ITEM for user to configure
  *
  * @param[in] synch_enable: 1 for synch, 0 for asynch
  * @return pointer to PUSER_ITEM
  */
PUSER_ITEM bt_mesh_alloc_hdl(uint8_t synch_enable);

/**
  * @brief free allocated PUSER_ITEM
  *
  * @param[in] puser_item: pointer to PUSER_ITEM
  * @return none
  */
void bt_mesh_free_hdl(PUSER_ITEM puser_item);

/**
 * @brief  Read uint8_t and convert to string.
 *
 * @param[in] buf: address of uint8_t array.
 * @param[in] buflen: num of array.
 * @param[in] out: target string buf.
 * @return
*/
void user_cmd_array2string(uint8_t *buf, unsigned int buflen, char *out);

/**
 * @brief  Read ASCII string and convert to uint32_t.
 *
 * @param[in] p
 * @return
*/
extern uint32_t user_cmd_string2uint32(char *p);

/**
 * @brief  Skip white spaces in buffer.
 *
 * @param[in] buffer
 * @return pointer to skipped white spaces' new buffer.
*/
extern char *user_cmd_skip_spaces(char *buffer);

/**
 * @brief  Find end of a word.
 *
 * @param[in] buffer
 * @return
*/
extern char *user_cmd_find_end_of_word(char *buffer);

/**
 * @brief  pram user cmd.
 *
 * @param[in] argc
 * @param[in] argv
 * @return
*/
void bt_mesh_param_user_cmd(unsigned int argc, char **argv);

#if (defined(CONFIG_BT_MESH_PROVISIONER) && CONFIG_BT_MESH_PROVISIONER || \
    defined(CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE) && CONFIG_BT_MESH_PROVISIONER_MULTIPLE_PROFILE)
/**
 * @brief  pram mesh ota user cmd.
 *
 * @param[in] argc
 * @param[in] argv
 * @return
*/
void bt_mesh_dfu_param_user_cmd(unsigned int argc, char **argv);
#endif

#endif

