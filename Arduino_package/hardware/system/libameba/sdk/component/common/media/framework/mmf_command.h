#ifndef _MMF_COMMAND_H_
#define _MMF_COMMAND_H_

#include <FreeRTOS.h>
#include <task.h>
#include "basic_types.h"
#include "osdep_service.h"

#define MMF_CMD_DEBUG 0

#if MMF_CMD_DEBUG
#define MMF_CMD_PRINTF(fmt, args...)    printf("\n\r%s: " fmt, __FUNCTION__, ## args)
#define MMF_CMD_ERROR(fmt, args...)    printf("\n\r%s: " fmt, __FUNCTION__, ## args)
#else
#define MMF_CMD_PRINTF(fmt, args...)
#define MMF_CMD_ERROR(fmt, args...)    printf("\n\r%s: " fmt, __FUNCTION__, ## args)
#endif

#define MMF_COMMAND_SERVICE_PRIORITY	tskIDLE_PRIORITY + 1
#define MMF_COMMAND_BUF_SIZE			512
#define DEF_MMF_COMMAND_PORT			16385

#define MMF_COMMAND_SELECT_SOCK 8 

// TODO: command list
/*mmf command list*/

typedef struct {
	struct list_head list;
	u8		bCmd;
	u8		bSubCommand;
	u8		bidx;
	u8		bLen;// sizeof[data]
	u16		wParam;
	u16		wAddr;
	u8		*data;		// max: 32byte
} mcmd_t;

typedef struct mmf_command_socket
{
	int server_socket;
	u8 *server_ip;
	u16 server_port;
	u8 *remote_ip;
	u16 remote_port;
	int client_socket;

	mcmd_t* command;
	u8 *request;
	u8 *response;
	int (*cb_command)(void*,void*);
} mcmd_socket_t;

typedef struct mmf_command_context
{
	struct list_head cmd_list;
	_mutex list_lock;
	_sema cmd_in_sema;
	xTaskHandle     queue_task;
	int (*cb_custom)(void*);
	mcmd_socket_t* mcmd_socket;
	xTaskHandle     socket_task;
} mcmd_context;

mcmd_t* mcmd_cmd_create(u8 bCmd, u8 bSubCommand, u8 bidx, u8 bLen, u16 wParam, u16 wAddr, u8 *data);
void mcmd_queue_service(void *param);
int mcmd_queue_task_open(mcmd_context *ctx);
mcmd_context * mcmd_context_init();
void mcmd_context_deinit(mcmd_context *ctx);
void mcmd_in_cmd_queue(mcmd_t *cmd, mcmd_context *ctx);
mcmd_t *mcmd_out_cmd_queue(mcmd_context *ctx);

mcmd_socket_t* mcmd_socket_create();
void mcmd_socket_free(mcmd_socket_t *mcmd_socket);
char mcmd_socket_bind_server(mcmd_socket_t *mcmd_socket);
char mcmd_socket_accept_client(mcmd_socket_t *mcmd_socket);
mcmd_t* mcmd_parse_cmd(u8 *data);

#endif
