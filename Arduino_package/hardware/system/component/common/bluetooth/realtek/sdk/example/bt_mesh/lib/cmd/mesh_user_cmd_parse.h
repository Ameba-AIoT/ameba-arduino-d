/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     user_cmd_parse.h
  * @brief    Source file for command parse.
  * @details  Data types and external functions declaration.
  * @author   bill
  * @date     2017-05-16
  * @version  v2.0
  * *************************************************************************************
  */

/* Add Includes here */
#ifndef __MESH_USER_CMD_PARSE_H
#define __MESH_USER_CMD_PARSE_H

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "platform_misc.h"
#include <platform_opts_bt.h>

/****************************************************************************************************************
* macros that other .c files may use all defined here
****************************************************************************************************************/
/**
 * @addtogroup CMD_PARSE
 * @{
 */

/**
 * @defgroup Cmd_Parse_Exported_Macros Command Parser Exported Macros
 * @brief
 * @{
 */

/** @brief  command parse related macros. */
#define USER_CMD_MAX_COMMAND_LINE        70           /* max. length of command line in bytes */

#if defined(CONFIG_BT_MESH_TEST) && CONFIG_BT_MESH_TEST
#define USER_CMD_MAX_PARAMETERS          80           /* max. number of parameters that the parser will scan */
#else 
#define USER_CMD_MAX_PARAMETERS          20           /* max. number of parameters that the parser will scan */
#endif
  
#define USER_CMD_MAX_HISTORY_LINE        3
/** @} */

/**
 * @defgroup Cmd_Parse_Exported_Types Command Parser Exported Types
 * @brief
 * @{
 */

/**
 * @brief Data UART command parse value.
 *
 * This is the structure where the command line parser puts its values.
 */
typedef struct
{
    char      *pcommand;                    /* pointer to command */
    uint8_t    para_count;             /* number of found parameters */
    uint32_t   dw_parameter[USER_CMD_MAX_PARAMETERS]; /* automatically parsed parameters */
    char      *pparameter[USER_CMD_MAX_PARAMETERS];  /* automatically parsed parameters */
} user_cmd_parse_value_t;

/**
 * @brief Data UART command parse result.
 *
 * This is the structure where the command line parser puts its result.
 */
typedef enum
{
    USER_CMD_RESULT_OK,
    USER_CMD_RESULT_ERROR,
    USER_CMD_RESULT_EMPTY_CMD_LINE,
    USER_CMD_RESULT_CMD_NOT_FOUND,
    USER_CMD_RESULT_WRONG_PARAMETER,
    USER_CMD_RESULT_WRONG_NUM_OF_PARAMETERS,
    USER_CMD_RESULT_VALUE_OUT_OF_RANGE
} user_cmd_parse_result_t;

/**
 * @brief command interface.
 */
typedef struct
{
    char      cmd_line[USER_CMD_MAX_COMMAND_LINE + 2];
    uint8_t   cmd_len; /**< accumulated length of command */
    uint8_t   cmd_cur; /**< cmd cursor */
    uint8_t   cmd_history[USER_CMD_MAX_HISTORY_LINE][USER_CMD_MAX_COMMAND_LINE + 2];
    uint8_t   cmd_history_len[USER_CMD_MAX_HISTORY_LINE];
    uint8_t   history_head;
    uint8_t   history_tail;
    uint8_t   history_cur; /**< current cmd pointer */
    char      cmd_prompt[2];
    char      cmd_crlf[3];
} user_cmd_if_t, *user_cmd_if_p;

/**
 * @brief functions that can be called from command table.
 */
typedef user_cmd_parse_result_t (*pf_user_cmd_func_t)(user_cmd_parse_value_t *pparse_value);

/**
 * @brief command table entry.
 *
*/
typedef struct
{
    char              *pcommand;
    char              *poption;
    char              *phelp;
    pf_user_cmd_func_t    user_cmd_func;
} user_cmd_table_entry_t;
/** @} */

/****************************************************************************************************************
* exported functions other .c files may use all defined here.
****************************************************************************************************************/
/**
 * @defgroup Cmd_Parse_Exported_Functions Command Parser Exported Functions
 * @brief
 * @{
 */
/* initiate command interface struct */
extern void mesh_user_cmd_init(char *s);
/* deinitiate command interface struct */
extern void mesh_user_cmd_deinit(char *s);
/* collect user command from Data UART strings */
extern bool mesh_user_cmd_collect(uint8_t *pdata, uint8_t len, const user_cmd_table_entry_t *pcmd_table);
/** @} */
/** @} */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif /* __USER_CMD_PARSE_H */

