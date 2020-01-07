#ifndef WSCLIENT_H
#define WSCLIENT_H
#include <websocket/libwsclient.h>

/****************Define if using the polarssl*******************/
#define USING_SSL

/******************Define the function used*********************/
#ifdef USING_SSL
int wss_set_fun_ops(wsclient_context *wsclient);
#define wsclient_set_fun_ops(wsclient) wss_set_fun_ops(wsclient)
#else
int ws_set_fun_ops(wsclient_context *wsclient);
#define wsclient_set_fun_ops(wsclient) ws_set_fun_ops(wsclient)
#endif
/***************************************************************/

/*************************************************************************************************
** Function Name  : create_wsclient
** Description    : Creating the websocket client context structure
** Input          : url:websocket server's url
**					port:websocket server's port, if not given, default 80 for "ws", 443 for "wss"
**					origin: the address or url of your self
**					buf_len: the length of tx/rx/received buffer. It determine the maximum bytes of data send and receive.
** Return         : Created: websocket client context structure
**					Failed:  NULL
**************************************************************************************************/
wsclient_context *create_wsclient(char *url, int port,char *path, char* origin, int buf_len);

/*************************************************************************************************
** Function Name  : ws_connect_url
** Description    : Connecting to the websocket server
** Input          : wsclient: the websocket client context created by create_wsclientfunction
** Return         : Connected: the socket value
**					Failed:    -1
**************************************************************************************************/
int ws_connect_url(wsclient_context *wsclient);

/*************************************************************************************************
** Function Name  : ws_setsockopt_keepalive
** Description    : Set global value for KeepAlive socket option. These options will be set within ws_connect_url(), so the api must be invoked before ws_connect_url() or it would not take effect.
** Input          : keepalive_idle: value for TCP_KEEPIDLE option
**                  keepalive_interval: value for TCP_KEEPINTVL option
**                  keepalive_count: value for TCP_KEEPCNT option
** Return         : None
**************************************************************************************************/
void ws_setsockopt_keepalive(uint32_t keepalive_idle, uint32_t keepalive_interval, uint32_t keepalive_count);

/*************************************************************************************************
** Function Name  : ws_send
** Description    : Create the sending string data and copy to tx_buf
** Input          : message: the string that send to server(cannot exceeding the MAX_DATA_LEN
**					message_len: the length of the string
**					use_mask: 0/1; 1 means using mask for bynary
**					wsclient: the websocket client context
** Return         : None
**************************************************************************************************/
void ws_send(char* message, int message_len, int use_mask, wsclient_context *wsclient);

/*************************************************************************************************
** Function Name  : ws_sendBinary
** Description    : Create the sending binary data and copy to tx_buf
** Input          : message: the binary that send to server(cannot exceeding the MAX_DATA_LEN
**					message_len: the length of the binary
**					use_mask: 0/1; 1 means using mask for bynary
**					wsclient: the websocket client context
** Return         : None
**************************************************************************************************/
void ws_sendBinary(uint8_t* message, int message_len, int use_mask, wsclient_context *wsclient);

/*************************************************************************************************
** Function Name  : ws_sendPing
** Description    : Sending Ping to websocket server
** Input          : use_mask: 0/1; 1 means using mask for bynary
**					wsclient: the websocket client context
** Return         : None
**************************************************************************************************/
void ws_sendPing(int use_mask, wsclient_context *wsclient);

/*************************************************************************************************
** Function Name  : ws_poll
** Description    : Receicing data from server and send the data in tx_buf
** Input          : timeout(in milliseconds)
					wsclient: the websocket client context
** Return         : None
**************************************************************************************************/
void ws_poll(int timeout, wsclient_context **wsclient);

/*************************************************************************************************
** Function Name  : ws_dispatch
** Description    : callback function when getting message from server
** Input          : function that resolve the message received and the message length
** Return         : None
**************************************************************************************************/
void ws_dispatch(void (*callback)(wsclient_context **, int)) ;

/*************************************************************************************************
** Function Name  : ws_getReadyState
** Description    : Getting the connection status
** Input          : wsclient: the websocket client context
** Return         : readyStateValues(3 types:CLOSING, CLOSED, OPEN) 
**************************************************************************************************/
readyStateValues ws_getReadyState(wsclient_context *wsclient);

/*************************************************************************************************
** Function Name  : ws_close
** Description    : Closing the connection with websocket server
** Input          : wsclient: the websocket client context
** Return         : None
**************************************************************************************************/
void ws_close(wsclient_context **wsclient);

#endif