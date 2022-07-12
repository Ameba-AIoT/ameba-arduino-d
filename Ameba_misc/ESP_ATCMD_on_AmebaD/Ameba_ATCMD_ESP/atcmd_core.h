#ifndef ATCMD_CORE_H
#define ATCMD_CORE_H

#include "atcmd_uart.h"
#include "atcmd_wifi.h"
#include "atcmd_tcpip.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "dlist.h"

#ifdef __cplusplus
}
#endif

#define AT_DEBUG 0

#define ATCMD_HASH_TBL_SIZE 32
#define ATCMD_MAX_ARG_CNT 20

#define ATCMD_RX_BUFFER_SIZE 256
#define ATCMD_TX_BUFFER_SIZE 256

#define KEY_LF 0xA //'\n'
#define KEY_CR 0xD //'\r'
#define KEY_BS 0x8
#define KEY_ESC 0x1B

extern volatile uint8_t atcmd_tx_buf[ATCMD_TX_BUFFER_SIZE];
extern volatile uint8_t atcmd_rx_buf[ATCMD_RX_BUFFER_SIZE];
extern volatile uint16_t atcmd_rx_buf_cnt;

typedef enum {
    ATCMD_TEST = 0,
    ATCMD_QUERY = 1,
    ATCMD_SET = 2,
    ATCMD_EXECUTE = 3
} atcmd_type_t;

typedef enum {
    ATCMD_NORSP = 0,
    ATCMD_OK = 1,
    ATCMD_ERROR = 2,
} atcmd_response_t;

// function pointer for a generic ATCMD function accepting a string pointer containing arguments
typedef uint8_t (*atcmd_func_t)(void*);
typedef struct atcmd_command{
    char *atcmd;
    atcmd_func_t atcmd_test;
    atcmd_func_t atcmd_query;
    atcmd_func_t atcmd_set;
    atcmd_func_t atcmd_execute;
    struct list_head node;
} atcmd_command_t;

void atcmd_add_cmdtable(atcmd_command_t *tbl, uint8_t len);
void atcmd_init(void);
uint8_t atcmd_parse_params(char *buf, char **argv);
void atcmd_sys_register(void);

#endif
