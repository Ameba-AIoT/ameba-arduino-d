#ifndef ATCMD_UART_H
#define ATCMD_UART_H

#include "atcmd_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "serial_api.h"

#ifdef __cplusplus
}
#endif

// default configurations for ATCMD UART port
// LOG_UART pins should not be used for ATCMD
#define ATCMD_UART_TX_PIN PB_1
#define ATCMD_UART_RX_PIN PB_2
#define ATCMD_UART_RTS_PIN PA_14
#define ATCMD_UART_CTS_PIN PA_15

#define at_printf(fmt, args...)  do{\
        snprintf((char*)atcmd_tx_buf, ATCMD_TX_BUFFER_SIZE, fmt, ##args); \
        atcmd_uart_send_str((char*)atcmd_tx_buf);\
    }while(0)

#define at_print_data(data, size)  do{\
        atcmd_uart_send_buf(data, size);\
    }while(0)

void atcmd_uart_init(void);
void atcmd_uart_send_str(const char* str);
void atcmd_uart_send_buf(uint8_t* buf, uint32_t len);
void atcmd_uart_irq_enable(void);
void atcmd_uart_irq_disable(void);

#endif
