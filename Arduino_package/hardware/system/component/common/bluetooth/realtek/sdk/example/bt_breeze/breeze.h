/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     breeze.h
  * @brief    Head file for using breeze service.
  * @details  Breeze data structs and external functions declaration.
  * @author
  * @date
  * @version  v1.0
  * *************************************************************************************
  */

/* Define to prevent recursive inclusion */
#ifndef _BREEZE_H_
#define _BREEZE_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

/* Add Includes here */
#include "profile_server.h"



/*============================================================================*
 *                         Macros
 *============================================================================*/

#define BREEZE_NOTIFY_INDICATE_DISABLE  		0x00
#define BREEZE_NOTIFY_ENABLE  					0x01
#define BREEZE_INDICATE_ENABLE  				0x02
#define BREEZE_NOTIFY_DISABLE  					0x03
#define BREEZE_INDICATE_DISABLE  				0x04

#define BREEZE_WRITE_REQ            1
#define BREEZE_WRITE_NORSP			2


/*============================================================================*
 *                         Types
 *============================================================================*/

enum
{
    SVC_ATTR_IDX = 0,
    RC_CHRC_ATTR_IDX,
    RC_DESC_ATTR_IDX,
    WC_CHRC_ATTR_IDX,
    WC_DESC_ATTR_IDX,
    IC_CHRC_ATTR_IDX,
    IC_DESC_ATTR_IDX,
    IC_CCC_ATTR_IDX,
    WWNRC_CHRC_ATTR_IDX,
    WWNRC_DESC_ATTR_IDX,
    NC_CHRC_ATTR_IDX,
    NC_DESC_ATTR_IDX,
    NC_CCC_ATTR_IDX,
    /* Add more entry here if necessary */
    AIS_ATTR_NUM,
};

typedef enum
{
	BREEZE_READ_1 = 1,
	BREEZE_READ_2,
	BREEZE_READ_3,
	BREEZE_READ_4,
	BREEZE_READ_5
} T_BREEZE_READ_VAL;

typedef enum
{
    BREEZE_PARAM_V1_READ_CHAR_VAL = 0x01,
} T_BREEZE_PARAM_TYPE;

typedef struct
{
    uint8_t opcode; 
    T_WRITE_TYPE write_type;
    uint16_t len;
    uint8_t *p_value;
} TBREEZE_WRITE_MSG;


typedef union
{
    uint8_t notification_indication_index;
    uint8_t read_value_index;
	TBREEZE_WRITE_MSG write;
} T_BREEZE_UPSTREAM_MSG_DATA;


typedef struct
{
    uint8_t                 conn_id;
    T_SERVICE_CALLBACK_TYPE msg_type;
    T_BREEZE_UPSTREAM_MSG_DATA msg_data;
} T_BREEZE_CALLBACK_DATA;


/*============================================================================*
 *                         Functions
 *============================================================================*/

T_SERVER_ID breeze_add_service(void *p_func);

bool breeze_set_parameter(T_BREEZE_PARAM_TYPE param_type, uint16_t len, uint8_t *p_value);

typedef void (*ind_cb)(uint8_t res);

bool breeze_send_indicate(uint8_t conn_id, T_SERVER_ID service_id, void *p_value,
                                       uint16_t length, ind_cb cb);

bool breeze_send_notify(uint8_t conn_id, T_SERVER_ID service_id, void *p_value,
										uint16_t length);


#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif  /* _BREEZE_H_ */
