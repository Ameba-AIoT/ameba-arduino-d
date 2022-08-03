#include "atcmd_uart.h"
#include "atcmd_core.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "pinmap.h"

#ifdef __cplusplus
}
#endif

// ATCMD UART default configuration
int atcmd_uart_baudrate = 9600;
int atcmd_uart_databits = 8;
int atcmd_uart_stopbits = 1;
SerialParity atcmd_uart_paritybits = ParityNone;
FlowControl atcmd_uart_flow_ctrl = FlowControlNone;
static serial_t atcmd_uart_obj;

extern SemaphoreHandle_t atcmd_cmd_rx_sema;
extern SemaphoreHandle_t atcmd_cmd_done_sema;
extern uint8_t atcmd_local_echo;

// Variables used for handling TCPIP data for transmission
extern uint16_t transmission_mode_len;
extern uint8_t passthrough_mode;
extern uint32_t atcmd_tcpip_passthrough_last_tick;
extern SemaphoreHandle_t atcmd_tcpip_tx_sema;
extern volatile uint16_t atcmd_tcpip_tx_buflen;
extern uint8_t atcmd_tcpip_tx_buf[ATCMD_TCPIP_TX_BUFFER_SIZE];

void uart_irq(uint32_t id, SerialIrq event) {
    serial_t* sobj = (serial_t*)id;
    unsigned char rc=0;

    if (event == RxIrq) {
        rc = serial_getc(sobj);

        // Code to process data in transmission mode
        if (transmission_mode_len) {
            atcmd_tcpip_tx_buf[atcmd_tcpip_tx_buflen++] = rc;
            transmission_mode_len--;
            if (transmission_mode_len == 0) {
                xSemaphoreGiveFromISR(atcmd_tcpip_tx_sema, NULL);
            }
            return;
        }

        // Code to process data in passthrough mode
        if (passthrough_mode) {
            if (atcmd_tcpip_tx_buflen < ATCMD_TCPIP_TX_BUFFER_SIZE - 10) {
                atcmd_tcpip_tx_buf[atcmd_tcpip_tx_buflen++] = rc;
                atcmd_tcpip_passthrough_last_tick = xTaskGetTickCountFromISR();
            } else {
                // Almost overflow, signal for immediate transmission
                atcmd_tcpip_tx_buf[atcmd_tcpip_tx_buflen++] = rc;
                xSemaphoreGiveFromISR(atcmd_tcpip_tx_sema, NULL);
            }
            return;
        }

        // Code to process regular ATcommands
        // wait for previous ATcommand to be processed
        if (uxSemaphoreGetCount(atcmd_cmd_done_sema) == 0) {
            atcmd_uart_send_str("\r\nbusy p...\r\n");
            return;
        }
        // Check for special keypress
        if (rc == KEY_CR) {
            if (atcmd_local_echo) {
                serial_putc(sobj, rc);
            }
        } else if (rc == KEY_LF) {
            if (atcmd_local_echo) {
                serial_putc(sobj, rc);
            }
            // indicate a completed AT command ready for processing
            if (atcmd_rx_buf_cnt > 0) {
                atcmd_rx_buf[atcmd_rx_buf_cnt] = '\0';
                xSemaphoreTakeFromISR(atcmd_cmd_done_sema, NULL);
                xSemaphoreGiveFromISR(atcmd_cmd_rx_sema, NULL);
            } else {
                atcmd_uart_send_str("\r\nERROR\r\n");
            }
        } else if (rc == KEY_BS) {
            if (atcmd_rx_buf_cnt > 0) {
                atcmd_rx_buf_cnt--;
                atcmd_rx_buf[atcmd_rx_buf_cnt] = '\0';
                if (atcmd_local_echo) {
                    serial_putc(sobj, rc);
                    serial_putc(sobj, ' ');
                    serial_putc(sobj, rc);
                }
            }
        } else {
            if (atcmd_local_echo) {
                serial_putc(sobj, rc);
            }
            if (atcmd_rx_buf_cnt < (ATCMD_RX_BUFFER_SIZE - 1)) {
                // Add new characters to buffer
                atcmd_rx_buf[atcmd_rx_buf_cnt++] = rc;
            } else if (atcmd_rx_buf_cnt == (ATCMD_RX_BUFFER_SIZE - 1)) {
                // Do not add new characters to buffer if full
                atcmd_rx_buf[atcmd_rx_buf_cnt] = '\0';
            }
        }
    }
}

void atcmd_uart_init(void) {
    serial_init(&atcmd_uart_obj, ATCMD_UART_TX_PIN, ATCMD_UART_RX_PIN);
    serial_baud(&atcmd_uart_obj, atcmd_uart_baudrate);
    serial_format(&atcmd_uart_obj, atcmd_uart_databits, atcmd_uart_paritybits, atcmd_uart_stopbits);
    pin_mode(ATCMD_UART_CTS_PIN, PullDown); //init CTS low
    serial_set_flow_control(&atcmd_uart_obj, atcmd_uart_flow_ctrl, ATCMD_UART_RTS_PIN, ATCMD_UART_CTS_PIN);
    serial_irq_handler(&atcmd_uart_obj, uart_irq, (uint32_t)&atcmd_uart_obj);
    serial_irq_set(&atcmd_uart_obj, RxIrq, 1);
    printf("ATCMD UART interface ready\r\n");
    at_printf("\r\nready\r\n");
}

void atcmd_uart_reinit(void) {
    serial_baud(&atcmd_uart_obj, atcmd_uart_baudrate);
    serial_format(&atcmd_uart_obj, atcmd_uart_databits, atcmd_uart_paritybits, atcmd_uart_stopbits);
    serial_set_flow_control(&atcmd_uart_obj, atcmd_uart_flow_ctrl, ATCMD_UART_RTS_PIN, ATCMD_UART_CTS_PIN);
}

void atcmd_uart_send_str(const char* str) {
    uint32_t i = 0;
    while (str[i] != '\0') {
        serial_putc(&atcmd_uart_obj, str[i++]);
    }
}

void atcmd_uart_send_buf(uint8_t* buf, uint32_t len) {
    uint8_t* pbuf = buf;
    if((!len) || (!buf)){
        return;
    }
    while (len) {
        serial_putc(&atcmd_uart_obj, *(pbuf++));
        len--;
    }
}

void atcmd_uart_irq_enable(void) {
    serial_irq_set(&atcmd_uart_obj, RxIrq, 1);
}

void atcmd_uart_irq_disable(void) {
    serial_irq_set(&atcmd_uart_obj, RxIrq, 0);
}

//---------------------------- Commands for basic ATCMD functionality ----------------------------//
uint8_t q_AT_UART(void *arg) {
    // Query current UART configuration
    (void)arg;
    at_printf("+UART:%d,%d,%d,%d,%d\r\n",\
        atcmd_uart_baudrate, atcmd_uart_databits, atcmd_uart_stopbits, atcmd_uart_paritybits, atcmd_uart_flow_ctrl);
    return ATCMD_OK;
}

uint8_t q_AT_UART_CUR(void *arg) {
    // Query current UART configuration
    (void)arg;
    at_printf("+UART_CUR:%d,%d,%d,%d,%d\r\n",\
        atcmd_uart_baudrate, atcmd_uart_databits, atcmd_uart_stopbits, atcmd_uart_paritybits, atcmd_uart_flow_ctrl);
    return ATCMD_OK;
}

uint8_t s_AT_UART_CUR(void *arg) {
    // Set UART configuration
    uint8_t argc = 0;
    char *argv[ATCMD_MAX_ARG_CNT] = {0};
    u32 baud = 0;
    int databits = 0;
    int stopbits = 0;
    int parity = 0;
    int flowcontrol = 0;

    if (!arg) {
        return ATCMD_ERROR;
    }
    if ((argc = atcmd_parse_params((char*)arg, argv)) != 6) {
        return ATCMD_ERROR;
    }
    for (uint8_t i = 1; i < argc; i++) {
        if (argv[i] == NULL) {
        return ATCMD_ERROR;
        }
    }

    baud = atoi(argv[1]);
    databits = atoi(argv[2]);
    stopbits = atoi(argv[3]);
    parity = atoi(argv[4]);
    flowcontrol = atoi(argv[5]);

    if (((databits < 5) || (databits > 8))||\
        ((stopbits < 1) || (stopbits > 3))||\
        ((parity < 0) || (parity > 2))||\
        ((flowcontrol < 0) || (flowcontrol > 3))||\
        ((baud < 80) || (baud > 5000000))\
        ){
        return ATCMD_ERROR;
    }

    atcmd_uart_baudrate = baud;
    atcmd_uart_databits = databits;
    if (stopbits == 1) {
        atcmd_uart_stopbits = 1;
    } else {
        atcmd_uart_stopbits = 2;
    }
    atcmd_uart_paritybits = (SerialParity)parity;
    switch (flowcontrol) {
        case 0: {
            atcmd_uart_flow_ctrl = FlowControlNone;
            break;
        }
        case 1: {
            atcmd_uart_flow_ctrl = FlowControlRTS;
            break;
        }
        case 2: {
            atcmd_uart_flow_ctrl = FlowControlCTS;
            break;
        }
        case 3: {
            atcmd_uart_flow_ctrl = FlowControlRTSCTS;
            break;
        }
    }
    at_printf("\r\nOK\r\n");
    vTaskDelay(5/portTICK_PERIOD_MS);   // delay to allow data in UART FIFO to clear out
    atcmd_uart_reinit();
    return ATCMD_NORSP;
}

uint8_t q_AT_UART_DEF(void *arg) {
    // Query default UART configuration, saved in flash
    (void)arg;
    at_printf("+UART_DEF:%d,%d,%d,%d,%d\r\n",\
        atcmd_uart_baudrate, atcmd_uart_databits, atcmd_uart_stopbits, atcmd_uart_paritybits, atcmd_uart_flow_ctrl);
    return ATCMD_OK;
}

uint8_t s_AT_UART_DEF(void *arg) {
    // Set default UART configuration, saved in flash
    uint8_t argc = 0;
    char *argv[ATCMD_MAX_ARG_CNT] = {0};
    u32 baud = 0;
    int databits = 0;
    int stopbits = 0;
    int parity = 0;
    int flowcontrol = 0;

    if (!arg) {
        return ATCMD_ERROR;
    }
    if ((argc = atcmd_parse_params((char*)arg, argv)) != 6) {
        return ATCMD_ERROR;
    }
    for (uint8_t i = 1; i < argc; i++) {
        if (argv[i] == NULL) {
        return ATCMD_ERROR;
        }
    }

    baud = atoi(argv[1]);
    databits = atoi(argv[2]);
    stopbits = atoi(argv[3]);
    parity = atoi(argv[4]);
    flowcontrol = atoi(argv[5]);

    if (((databits < 5) || (databits > 8))||\
        ((stopbits < 1) || (stopbits > 3))||\
        ((parity < 0) || (parity > 2))||\
        ((flowcontrol < 0) || (flowcontrol > 3))||\
        ((baud < 80) || (baud > 5000000))\
        ){
        return ATCMD_ERROR;
    }

    atcmd_uart_baudrate = baud;
    atcmd_uart_databits = databits;
    if (stopbits == 1) {
        atcmd_uart_stopbits = 1;
    } else {
        atcmd_uart_stopbits = 2;
    }
    atcmd_uart_paritybits = (SerialParity)parity;
    switch (flowcontrol) {
        case 0: {
            atcmd_uart_flow_ctrl = FlowControlNone;
            break;
        }
        case 1: {
            atcmd_uart_flow_ctrl = FlowControlRTS;
            break;
        }
        case 2: {
            atcmd_uart_flow_ctrl = FlowControlCTS;
            break;
        }
        case 3: {
            atcmd_uart_flow_ctrl = FlowControlRTSCTS;
            break;
        }
    }
    at_printf("\r\nOK\r\n");
    vTaskDelay(5/portTICK_PERIOD_MS);   // delay to allow data in UART FIFO to clear out
    atcmd_uart_reinit();
    return ATCMD_OK;
}
