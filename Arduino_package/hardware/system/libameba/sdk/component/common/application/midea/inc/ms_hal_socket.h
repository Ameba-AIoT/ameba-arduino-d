/*
 * Copyright (c) 2015 - 2016 M-SMART Research Institute of Midea Group.
 * All rights reserved.	
 *
 * File Name 		: ms_hal_socket.h
 * Introduction	: socket communication
 *
 * Current Version	: v0.2
 * Author			: xianzhixiong
 * Create Time	: 2015/08/01
 * Change Log	: (2015/08/26) 1.rename this file  2.added the MS_HAL_ to each enum and struct
 *                           3.rename some functions and add comment to each function
 *
 * All software, firmware and related documentation herein ("M-Smart Software") are
 * intellectual property of M-SMART Research Institute of Midea Group and protected 
 * by law, including, but not limited to, copyright law and international treaties.
 *
 * Any use, modification, reproduction, retransmission, or republication of all
 * or part of M-Smart Software is expressly prohibited, unless prior written
 * permission has been granted by M-Smart.
 *
 */
 
#ifndef __MS_HAL_SOCKET_H__
#define __MS_HAL_SOCKET_H__

#include "lwipopts.h"
#include "lwip/sockets.h"

/* AES 128 return value */
typedef enum
{
	MS_HAL_AES128_RESULT_SUCCESS = 0,
	MS_HAL_AES128_RESULT_ERROR   = 1,
}ms_hal_aes128_result_t;


/* MD5  verify return value */
typedef enum
{
	MS_HAL_MD5_RESULT_SUCCESS = 0,
	MS_HAL_MD5_RESULT_ERROR   = 1,
}ms_hal_md5_result_t;

/* deleted the below struct ,use the system standard struct instead */
/*
struct sockaddr_in 
{
¡¡short int       sin_family;
¡¡uint16_t        sin_port;
¡¡struct in_addr  sin_addr;
  uint8_t         sin_zero[8];
};
*/


/*
*@function:Wifi module  data md5 signature
*@params  input: signature source file address 
*               len:signature file length
*               output:signed file address
*
*@return MS_HAL_MD5_RESULT_SUCCESS :signed success
*            MS_HAL_MD5_RESULT_ERROR     :signed fail
*/
ms_hal_md5_result_t ms_hal_md5_signature (const uint8_t *input, uint16_t len, uint8_t *output);


/*
*@function:WiFi module data AES128 encrypt
*@params data:The address of the source data will encrypted and after encrypted sucessful,the encypted data will store in the same address
*              len:The length of the data will be encrypted and after encrypted sucessful,it will be the encrypted sucessful data length
*              key:the key used to encrypted
*		   encrypt_type:Encryption algorithm type. 0:ECB(we only use ECB)
*@return  MS_HAL_AES128_RESULT_SUCCESS:encrypt success
*             MS_HAL_AES128_RESULT_ERROR:encrypt fail
*/
ms_hal_aes128_result_t  ms_hal_aes128_encrypt (uint8_t *data, uint16_t *len, const uint8_t *key, const uint8_t encrypt_type);



/*
*@function:WiFi module data AES128 decrypt
*@params data:The address of the source data will decrypt and after decrypted sucessful,the decrypted data will store in the same address
*              len:The length of the data will be decrypted and after decrypted sucessful,it will be the decrypted sucessful data length
*              key:the key used to decrypted
*		   decrypt_type:decryption algorithm type. 0:ECB(we only use ECB)
*@return  MS_HAL_AES128_RESULT_SUCCESS:decrypt success
*             MS_HAL_AES128_RESULT_ERROR:decrypt fail
*/
ms_hal_aes128_result_t  ms_hal_aes128_decrypt (uint8_t *data, uint16_t *len, const uint8_t *key, const uint8_t decrypt_type);



/*
*@functon:From the domain name to obtain IP address
*@params host_name:domain name address
*              dest_ip:storage of the target IP address
*@return     if get the IP address successful return a  nonnegative number ,otherwise return a negative number
*/
int8_t ms_hal_get_ip_by_name(const char *host_name, uint32_t *dest_ip);


/*
*@function: create a nw socket
*@params domain:protocol type
*              socket_type:socket type
*		   protocol:this value must be 0
*@return   if create successful return a  positive number ,otherwise return a negative number
*/
int  ms_hal_socket_create(int domain, int socket_type, int protocol);



/*
*@function:deleted a socket
*@params sock_fd:the delete socket descriptor
*@return  if  successful return 0 ,otherwise return a negative number
*/
int  ms_hal_socket_close(int sock_fd);



/*
*@function:set the socket attributes,eg:blocknon-blocking
*@params sock_fd:socket descriptor
*              level:protocol
*              optname:Option name
*              optval:adress of the operand
*              optlen:the length of optval
*/
#define ms_hal_socket_setopt(a,b,c,d,e)         lwip_setsockopt(a,b,c,d,e)
//int16_t ms_hal_socket_setopt (const int16_t sock_fd, const uint16_t level, const uint16_t optname, const uint8_t *optval, socklen_t optlen);



/*
*@function:bind the socket to the specified port number
*@params sock_fd:the socket descriptor will to be bind
*              my_addr: socket address
*              addrlen:socket address length
*@return  if  successful return 0 ,otherwise return a negative number
*/
int ms_hal_socket_bind(int sock_fd, const struct sockaddr *my_addr, socklen_t addrlen);



/*
*@function:send a connet request,need the target host port number and host ip when call this function
*@params sock_fd:the socket handle which want to connect
*              addr    :this address point to a struct include the host ip and port number 
*              addrlen:length of addr
*@return  if  successful return 0 ,otherwise return a negative number
*/
//#define ms_hal_socket_connect(a,b,c) lwip_connect(a,b,c)
int16_t ms_hal_socket_connect( int16_t sock_fd, const struct sockaddr *addr, uint16_t addrlen );



/*
*@function:Listen to bind socket connection
*@params sock_fd:to listen this socket descriptor
*              backlog:Set the maximum length of the queue waiting for connecting 
*@return  if  successful return 0 ,otherwise return a negative number
*/
int  ms_hal_socket_listen( int sock_fd, int backlog );



/*
*@function:Accept the client's connection request, if successful return a new socket descriptor
*@params sock_fd:The listening socket descriptor
*             addr:point to a new connect target 
*             addrlen:length of addr
*@return   if  successful return a  positive number ,otherwise return a negative number or 0
*/
#define ms_hal_socket_accept(a,b,c)         lwip_accept(a,b,c)
//int16_t ms_hal_socket_accept( int16_t sock_fd, struct sockaddr *addr, socklen_t *addrlen);




/*
*@function:read data from socket
*@params  sock_fd:get the data from this socket descriptor
*              buf:store the recieve data
*              nbyte:to read the data length 
*              wait_time:The blocking of data read time,unit is ms
*@return  0: connect is closed
*             positive number:return the read data length
*             negative numbe:Network receiving errors
*/
int ms_hal_socket_recv( int sock_fd, void *buf, size_t nbyte, int flags);



/*
*@function:send nbytes data to socket from buffer
*@params sock_fd:Data will written to this socket descriptor
*              buf:the data buffer which to send
*              nbyte:the data length to send
*              flag:please keep it to 0
*@return  if return a nonegative numbe,means the length of the write data successfully,otherwise send data fail
*/
int ms_hal_socket_send( int sock_fd, const void *buf, size_t nbyte, int flag );


/*
*@function:send len bytes data to socket from buffer
*@params sock_fd:Data will written to this socket descriptor
*              buf:the data buffer which to send
*              len:the data length to send
*              flag:please keep it to 0
*              target_addr:this struct include the target host ip address
*              target_length:struct sockaddr length
*@return  if return a nonegative numbe,means the length of the write data successfully,otherwise send data fail
*/
int ms_hal_socket_sendto (int sock_fd , const void * buf, size_t len, int flag, const struct sockaddr *target_addr , socklen_t  target_length );



/*
*@function:read UDP data packet from specify IP address and port number
*@params  sock_fd:The listening socket descriptor
*               buf     :the data will store in the buffer address
*               len: the data length wish to read
*               flag: keep it to 0
*               source_addr:this struct include the source host ip address
*               source_length:struct sockaddr length
*@return    positive number:The actual received data bytes
*               0:network connect error
*               negative numbe:Network receiving errors
*/
int ms_hal_socket_recvfrom ( int sock_fd, void *buf, size_t len, int flags, struct sockaddr *source_addr, socklen_t *source_length );



/*
*@function:Monitor descriptor file  of changes, if someone have read/write files will notified to the application layer
*@params maxfdp:this value must be the number of descriptors plus one
*             readfds:Used to monitor the file descriptor read change
*             writefds:Used to monitor the file descriptor write change
*             errorfds:Used to monitor the file descriptor errors
*             timeout:0--immediately  >0--use this value to timeout
*@return    positive number:get some file descriptors match the read/write attribute
*               0:haven`t found some read/write attribute file descriptors ,wait timeout
*               negative numbe:occur some errors
*/
#define ms_hal_select(a,b,c,d,e)     lwip_select(a,b,c,d,e)
//int16_t ms_hal_select(int16_t maxfdp, fd_set *readfds, fd_set *writefds, fd_set *errorfds, struct timeval *timeout);




/*
*@function:clean the file descriptor set
*@params fd:Participate in reading and writing of the socket data collection
*@return :null
*/
#define ms_hal_fd_clean(a) FD_ZERO(a)
//void  ms_hal_fd_clean(fd_set *fd);


/*
*@function:Will be set in the corresponding file descriptor fd position to 1
*@params fd:file descriptor
*             set:The file descriptor set
*@return :null
*/
#define ms_hal_fd_set(a,b) FD_SET(a,b)
//void  ms_hal_fd_set(uint16_t fd,fd_set *set);


/*
*@function:Will be set in the corresponding file descriptor fd position to 0
*@params fd:file descriptor
*             set:The file descriptor set
*@return :null
*/
#define ms_hal_fd_release(a,b) FD_CLR(a,b)
//void  ms_hal_fd_release(uint16_t fd,fd_set *set);


/*
*@function:to judge the fd  in the corresponding file descriptor is 1 or 0
*@params fd:file descriptor
*             set:The file descriptor set
*@return : 1: this bit was set
*              0: this bit is clean
*/
#define ms_hal_fd_is_set(a,b) FD_ISSET(a,b)
//uint16_t  ms_hal_fd_is_set(uint16_t fd,fd_set *set);

#endif
