#include "atcmd_core.h"
#include "atcmd_uart.h"

#ifdef __cplusplus
extern "C" {
#endif

#include "sys_api.h"

#ifdef __cplusplus
}
#endif

volatile uint8_t atcmd_rx_buf[ATCMD_RX_BUFFER_SIZE];
volatile uint8_t atcmd_tx_buf[ATCMD_TX_BUFFER_SIZE];
volatile uint16_t atcmd_rx_buf_cnt = 0;
TaskHandle_t atcmd_task_handle = NULL;
SemaphoreHandle_t atcmd_cmd_rx_sema = NULL;
SemaphoreHandle_t atcmd_cmd_done_sema = NULL;
uint8_t atcmd_local_echo = 1;

struct list_head atcmd_hash_tbl[ATCMD_HASH_TBL_SIZE];

int atcmd_hash_index(char *str) {
    unsigned int seed = 131; // 31 131 1313 13131 131313 etc
    unsigned int hash = 0;

    while (*str) {
        hash = hash * seed + (*str++);
    }

    return (hash & 0x7FFFFFFF);
}

void atcmd_add_cmd(atcmd_command_t* newcmd) {
    int index = (atcmd_hash_index(newcmd->atcmd) % ATCMD_HASH_TBL_SIZE);
    list_add(&newcmd->node, &atcmd_hash_tbl[index]);
}

void atcmd_add_cmdtable(atcmd_command_t *tbl, uint8_t len) {
    uint8_t i;
    for(i = 0; i < len; i++) {
        atcmd_add_cmd(&tbl[i]);
    }
}

void* atcmd_search_cmdtable(char *cmd, atcmd_type_t type) {
    int search_cnt = 0;
    int index = (atcmd_hash_index(cmd) % ATCMD_HASH_TBL_SIZE);
    struct list_head *head = &atcmd_hash_tbl[index];
    struct list_head *iterator;
    atcmd_command_t *item;
    void *act = NULL;

    list_for_each(iterator, head) {
        item = list_entry(iterator, atcmd_command_t, node);
        search_cnt++;
        if (_stricmp(item->atcmd, cmd) == 0) {
            if (AT_DEBUG) printf("[atcmd_search_cmdtable] Found matching cmd %s at idx %d search cnt %d\r\n", item->atcmd, index, search_cnt);
            switch (type) {
                case ATCMD_TEST: {
                    act = (void*)item->atcmd_test;
                    break;
                }
                case ATCMD_QUERY: {
                    act = (void*)item->atcmd_query;
                    break;
                }
                case ATCMD_SET: {
                    act = (void*)item->atcmd_set;
                    break;
                }
                case ATCMD_EXECUTE: {
                    act = (void*)item->atcmd_execute;
                    break;
                }
            }
            break;
        }
    }
    return act;
}

void* atcmd_cmd_handler(const char* cmd) {
    atcmd_func_t atcmd_function = NULL;
    char buf[ATCMD_RX_BUFFER_SIZE] = {0};
    char tok[20] = {0};
    char *copy = buf;
    char *token = NULL;
    char *param = NULL;
    atcmd_type_t cmdtype;
    atcmd_response_t ret;
    strncpy(copy, cmd,LOG_SERVICE_BUFLEN-1);

    // Seperate received AT command into command and argument parts
    token = strtok(copy, "=");
    param = strtok(NULL, "\0");

    // Check for valid command token
    if (!token || (strlen(token) >= 20)) {
        return NULL;
    };

    if (param) {
        if ((strlen(param) == 1) && (param[0] == '?')) {
            cmdtype = ATCMD_TEST;
        } else {
            cmdtype = ATCMD_SET;
        }
    } else {
        if (token[strlen(token)-1] == '?') {
            token[strlen(token)-1] = '\0';
            cmdtype = ATCMD_QUERY;
        } else if (cmd[strlen(cmd)-1] == '=') {
            // check for edge case of set ATCMD without any parameters
            // assuming set commands without parameters are all invalid
            return NULL;
        } else {
            cmdtype = ATCMD_EXECUTE;
        }
    }
    strncpy(tok, token, sizeof(tok));

    if (AT_DEBUG) {
    switch (cmdtype) {
        case ATCMD_TEST: {
            printf("[atcmd_cmd_handler] Type: test\r\n");
            break;
        }
        case ATCMD_QUERY: {
            printf("[atcmd_cmd_handler] Type: query\r\n");
            break;
        }
        case ATCMD_SET: {
            printf("[atcmd_cmd_handler] Type: set\r\n");
            break;
        }
        case ATCMD_EXECUTE: {
            printf("[atcmd_cmd_handler] Type: execute\r\n");
            break;
        }
    }
    if (token) printf("[atcmd_cmd_handler] Command: %s \r\n", tok);
    if (param) printf("[atcmd_cmd_handler] Param: %s \r\n", param);
    }

    // Find function corresponding to command and invoke
    atcmd_function = (atcmd_func_t)atcmd_search_cmdtable(tok, cmdtype);

    if (atcmd_function) {
        ret = (atcmd_response_t)atcmd_function(param);
        switch (ret) {
            case ATCMD_NORSP: {
                break;
            }
            case ATCMD_OK: {
                at_printf("\r\nOK\r\n");
                break;
            }
            case ATCMD_ERROR: {
                at_printf("\r\nERROR\r\n");
                break;
            }
        }
    }

    return (void*)atcmd_function;
}

void atcmd_service_task(void* param) {
    (void)param;

    while (1) {
        // wait for semaphore indicating new ATcommand is received
        while (xSemaphoreTake(atcmd_cmd_rx_sema, portMAX_DELAY) != pdTRUE);
        if (AT_DEBUG) printf("[atcmd_service_task] Received command: %s\r\n", atcmd_rx_buf);

        // handle new ATcommand by checking hash table for corresponding functions
        if (atcmd_cmd_handler((char *)atcmd_rx_buf) == NULL) {
            // No corresponding ATCMD found
            if (AT_DEBUG) printf("[atcmd_service_task] No handler found\r\n");
            at_printf("\r\nERROR\r\n");
        }
        // indicate processing of ATcommand completed
        atcmd_rx_buf[0] = '\0';
        atcmd_rx_buf_cnt = 0;
        xSemaphoreGive(atcmd_cmd_done_sema);
    }
}

void atcmd_init(void) {
    int result;

    // initialize atcmd hash table and register core atcmd table
    for(uint8_t i = 0; i < ATCMD_HASH_TBL_SIZE; i++) {
        INIT_LIST_HEAD(&atcmd_hash_tbl[i]);
    }
    atcmd_sys_register();

    // initialize semaphore used to indicate newly received ATcommands
    vSemaphoreCreateBinary(atcmd_cmd_rx_sema);
    xSemaphoreTake(atcmd_cmd_rx_sema, 1/portTICK_PERIOD_MS);
    // initialize semaphore used to indicate ATcommands processing completed
    vSemaphoreCreateBinary(atcmd_cmd_done_sema);
    xSemaphoreGive(atcmd_cmd_done_sema);

    // start task to process incoming ATcommands
    result = xTaskCreate(atcmd_service_task, "atcmd_task", 1024, NULL, tskIDLE_PRIORITY + 3, &atcmd_task_handle);
    if (result != pdPASS) {
        printf("atcmd_service_task task create failed\r\n");
    }
}

uint8_t atcmd_parse_params(char *buf, char **argv) {
    // argument count index starts at 1 instead of 0
    uint8_t argc = 1;
    char str_buf[ATCMD_RX_BUFFER_SIZE];
    int str_count = 0;
    int buf_cnt = 0;
    static char temp_buf[ATCMD_RX_BUFFER_SIZE];
    char *buf_pos = temp_buf;
    memset(str_buf, 0, ATCMD_RX_BUFFER_SIZE);
    memset(temp_buf, 0, sizeof(temp_buf));

    if (buf == NULL) {
        return argc;
    }
    strncpy(temp_buf, buf, sizeof(temp_buf));

    while ((argc < ATCMD_MAX_ARG_CNT) && (*buf_pos != '\0')) {
        // replace symbols with null char
        while ((*buf_pos == ',') || (*buf_pos == '[') || (*buf_pos == ']')) {
            // Check for omitted arguments
            if ((*buf_pos == ',') && (*(buf_pos+1) == ',')) {
                argv[argc] = NULL;
                argc++;
            }
            *buf_pos = '\0';
            buf_pos++;
        }

        if (*buf_pos == '\0') {
            break;
        } else if (*buf_pos == '"') {
            memset(str_buf,'\0',ATCMD_RX_BUFFER_SIZE);
            str_count = 0;
            buf_cnt = 0;
            *buf_pos = '\0';
            buf_pos ++;
            if (*buf_pos == '\0') {
                break;
            }
            argv[argc] = buf_pos;
            while ((*buf_pos != '"')&&(*buf_pos != '\0')) {
                if (*buf_pos == '\\') {
                    buf_pos ++;
                    buf_cnt++;
                }
                str_buf[str_count] = *buf_pos;
                str_count++;
                buf_cnt++;
                buf_pos ++;
            }
            *buf_pos = '\0';
            memcpy(buf_pos-buf_cnt,str_buf,buf_cnt);
        } else{
            argv[argc] = buf_pos;
        }
        argc++;
        buf_pos++;

        while ((*buf_pos != ',')&&(*buf_pos != '\0')&&(*buf_pos != '[')&&(*buf_pos != ']')) {
            buf_pos++;
        }
    }
    return argc;
}

//---------------------------- Commands for basic ATCMD functionality ----------------------------//

uint8_t e_AT(void *arg) {
    // Test AT
    (void)arg;
    return ATCMD_OK;
}

extern uint8_t e_AT_CWQAP(void *arg);
uint8_t e_AT_RST(void *arg){
    // Soft reset
    (void)arg;

    // Disconnect WiFi if necessary
    e_AT_CWQAP(NULL);

    sys_reset();
    return ATCMD_OK;
}

uint8_t e_AT_GMR(void *arg) {
    // Check Version Information
    (void)arg;

    at_printf("AT version:2.4.0.0(4c6eb5e - ESP32 - May 20 2022 03:11:58)\r\n");
    at_printf("SDK version:v4.3.2-553-g588436db47\r\n");
    at_printf("compile time(5641e0a):%s %s\r\n", __DATE__, __TIME__);
    at_printf("Bin version:2.4.0(WROOM-32)\r\n");

    return ATCMD_OK;
}

uint8_t e_ATE0(void *arg) {
    // Set AT Command echo off
    (void)arg;
    atcmd_local_echo = 0;
    return ATCMD_OK;
}

uint8_t e_ATE1(void *arg) {
    // Set AT Command echo on
    (void)arg;
    atcmd_local_echo = 1;
    return ATCMD_OK;
}

extern uint8_t q_AT_UART(void *arg);
extern uint8_t q_AT_UART_CUR(void *arg);
extern uint8_t s_AT_UART_CUR(void *arg);
extern uint8_t q_AT_UART_DEF(void *arg);
extern uint8_t s_AT_UART_DEF(void *arg);

uint8_t q_AT_SYSRAM(void *arg) {
    // Query Current Remaining Heap Size and Minimum Heap Size
    (void)arg;
    at_printf("+SYSRAM:%d,%d", xPortGetFreeHeapSize(), xPortGetMinimumEverFreeHeapSize());
    return ATCMD_OK;
}

uint8_t q_AT_SYSSTORE(void *arg) {
    // Query the AT parameter store mode.
    // AT+SYSSTORE?
    (void)arg;
    at_printf("+SYSSTORE:%d\r\n", 0);
    return ATCMD_OK;
}

uint8_t s_AT_SYSSTORE(void *arg) {
    // Set the AT parameter store mode.
    // AT+SYSSTORE=<store_mode>
    (void)arg;
    uint8_t argc = 0;
    char* argv[ATCMD_MAX_ARG_CNT] = {0};
    uint8_t mode = 0;          // Arg 1

    if (!arg) {
        return ATCMD_ERROR;
    }
    argc = atcmd_parse_params((char*)arg, argv);
    if (argc != 2) {
        return ATCMD_ERROR;
    }

    mode = atoi(argv[1]);
    if (mode > 1) {
        return ATCMD_ERROR;
    }

    return ATCMD_OK;
}

atcmd_command_t atcmd_sys_commands[] = {
      {"AT",            NULL, NULL,             NULL,           e_AT,       {NULL, NULL}},
      {"AT+RST",        NULL, NULL,             NULL,           e_AT_RST,   {NULL, NULL}},
      {"AT+GMR",        NULL, NULL,             NULL,           e_AT_GMR,   {NULL, NULL}},
      {"ATE0",          NULL, NULL,             NULL,           e_ATE0,     {NULL, NULL}},
      {"ATE1",          NULL, NULL,             NULL,           e_ATE1,     {NULL, NULL}},
      {"AT+UART",       NULL, q_AT_UART,        s_AT_UART_CUR,  NULL,       {NULL, NULL}},
      {"AT+UART_CUR",   NULL, q_AT_UART_CUR,    s_AT_UART_CUR,  NULL,       {NULL, NULL}},
      {"AT+UART_DEF",   NULL, q_AT_UART_DEF,    s_AT_UART_DEF,  NULL,       {NULL, NULL}},
      {"AT+SYSRAM",     NULL, q_AT_SYSRAM,      NULL,           NULL,       {NULL, NULL}},
      {"AT+SYSSTORE",   NULL, q_AT_SYSSTORE,    s_AT_SYSSTORE,  NULL,       {NULL, NULL}},
};

void atcmd_sys_register(void) {
    atcmd_add_cmdtable(atcmd_sys_commands, sizeof(atcmd_sys_commands)/sizeof(atcmd_sys_commands[0]));
}
