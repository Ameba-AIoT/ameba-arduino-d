/**
*****************************************************************************************
*     Copyright(c) 2015, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file     data_uart.h
  * @brief    Head file for Data uart operations.
  * @details  Data uart init and print vital data through data uart.
  * @author   bill
  * @date     2015-03-19
  * @version  v2.0
  * *************************************************************************************
  */

#ifndef _DATA_UART_H_
#define _DATA_UART_H_

#ifdef  __cplusplus
extern "C" {
#endif      /* __cplusplus */

#include "platform_misc.h"
/****************************************************************************************************************
* macros that other .c files may use all defined here
****************************************************************************************************************/
/**
 * @addtogroup DATA_UART
 * @{
 */

/**
 * @defgroup Data_Uart_Exported_Types Data Uart Exported Types
 * @brief
 * @{
 */

/** @brief  Print data use Data UART. */
typedef void (*pf_send_msg_from_isr_t)(uint8_t data);
/** @} */

/****************************************************************************************************************
* exported functions other .c files may use all defined here.
****************************************************************************************************************/
/**
 * @defgroup Data_Uart_Exported_Functions Data Uart Exported Functions
 * @brief
 * @{
 */
/* Print formated data use Data UART  */
extern void data_uart_debug(char *fmt, ...);
extern void data_uart_dump(uint8_t *pbuffer, uint32_t len);
extern uint32_t data_uart_send_string(const uint8_t *data, uint32_t len);
extern void mesh_data_uart_init(uint32_t tx_pin, uint32_t rx_pin, pf_send_msg_from_isr_t pf_send);
/** @} */
/** @} */

#ifdef  __cplusplus
}
#endif      /*  __cplusplus */

#endif

