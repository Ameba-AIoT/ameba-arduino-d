#ifndef     _LTPLIB_UTILS_H_
#define     _LTPLIB_UTILS_H_

#include <stdint.h>
#include <stdbool.h>




typedef enum _HRP_MODULE_ID
{
    HRP_MODULE_RESERVED     = 0,
    HRP_MODULE_LOWER_STACK  = 1,
    HRP_MODULE_UPPER_STACK  = 2,
    HRP_MODULE_PROFILE      = 3,
    HRP_MODULE_RESERVED1,

    HRP_MODULE_AUDIO        = 16,
    HRP_MODULE_BLE_AUTO_TEST = 17,


} HRP_MODULE_ID;

bool hrp_handle_downstream_req(HRP_MODULE_ID module_id, uint8_t cmd_group,
                               uint16_t cmd_index, uint16_t param_list_len, uint8_t *p_param_list);

bool hrp_handle_upperstream_events(HRP_MODULE_ID module_id, uint8_t cmd_group,
                                   uint16_t cmd_index, uint16_t param_list_len, uint8_t *p_param_list);



/****************************************************************************/
/* target specific application context to be handed over to the application */
/* this constand is used by the BTLTPTgtxxx function calls of this library  */
#define HRP_TGT_APPHANDLE   void *


/* definition of valid LTP response causes                                  */
#define HRP_CAUSE_SUCCESS                                                 0x00
#define HRP_CAUSE_ACCEPT                                                  0x01
#define HRP_CAUSE_REJECT                                                  0x02
#define HRP_CAUSE_RESOURCE_ERROR                                          0x03
#define HRP_CAUSE_INVALID_PARAMETER                                       0x04
#define HRP_CAUSE_INVALID_STATE                                           0x05
#define HRP_CAUSE_CONNECTION_DISCONNECT                                   0x06
#define HRP_CAUSE_CONNECTION_DISCONNECTED                                 LTP_CAUSE_CONNECTION_DISCONNECT
#define HRP_CAUSE_CONNECTION_LOST                                         0x07
#define HRP_CAUSE_AUTHENTICATION_FAILED                                   0x08
#define HRP_CAUSE_INIT_TIMEOUT                                            0x09
#define HRP_CAUSE_INIT_OUT_OF_SYNC                                        0x0A
#define HRP_CAUSE_INIT_HARDWARE_fAILURE                                   0x0B
#define HRP_CAUSE_CONNECTION_PAUSED                                       0x30
#define HRP_CAUSE_FLOWCONTROL_VIOLATION                                   0x31
#define HRP_CAUSE_UNSPECIFIED                                             0xFD
#define HRP_CAUSE_NOT_SUPPORTED                                           0xFE

/* definition of locally generated internal Event types                     */
#define HRP_INTERNAL_EVENT_COMMUNICATION_OUT_OF_SYNC                      0x40
#define HRP_INTERNAL_EVENT_MALFORMED_MSG_RECEIVED                         0x41
#define HRP_INTERNAL_EVENT_INVALID_DATA_RECEIVED                          0x42



/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/**                                                                        **/
/** 3) Internal functionality used by the LTP-Lib                          **/
/**                                                                        **/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

/* macro definition for read/write uint16_t from/to uint8_t memory                 */
#define NETSHORT2CHAR(p,w)                 \
    *((p)+1) = (uint8_t)((w) & 0xff);         \
    *(p)     = /*lint -e(572,778)*/ (uint8_t)(((w)>>8) & 0xff)

#define NETCHAR2SHORT(p) ((*((p)+1)) & 0xff) + ((*(p)) << 8)

#define NETCHAR2LONG(p) ((uint32_t)(*((p)+3)) & 0xff) + ((uint32_t)(*((p)+2)) << 8) \
    + ((uint32_t)(*((p)+1)) << 16)  + ((uint32_t)(*((p)+0)) << 24)

#define NET24BIT2CHAR(p,w)                  \
    *((p)+2) = (uint8_t)((w) & 0xff);          \
    *((p)+1) = /*lint -e(572,778)*/ (uint8_t)(((w)>>8) & 0xff);     \
    *((p)+0) = /*lint -e(572,778)*/ (uint8_t)(((w)>>16) & 0xff);    \

#define NETLONG2CHAR(p,w)                   \
    *((p)+3) = (uint8_t)((w) & 0xff);          \
    *((p)+2) = /*lint -e(572,778)*/ (uint8_t)(((w)>>8) & 0xff);     \
    *((p)+1) = /*lint -e(572,778)*/ (uint8_t)(((w)>>16) & 0xff);    \
    *((p)+0) = /*lint -e(572,778)*/ (uint8_t)(((w)>>24) & 0xff)


/* macro definition to identity internal event location                     */
#define HRP_GENERATE_EVENT_ID  ((0x00)<<24 | (((LTP_SOURCE_FILE_ID) & 0xFF)<<16) | ((__LINE__) & 0xFFFF))

/* message definitions                                                      */
#define HRP_DATA_MIN_HEADER_LENGTH                                           7
#define HRP_OPT_MASK_HEADER_CRC                                          0x03
#define HRP_OPT_MASK_HEADER_CRC8                                          0x01

#define HRP_OPCODE_RESERVED                                               0x00

/* definitions of message properties                                        */
#define LTP_MDC_MSG         0x01
#define LTP_MDH_MSG         0x02
#define LTP_VAR_LEN_MSG     0x04
#define LTP_CNF_MSG         0x08




#ifdef __cplusplus
extern "C" {
#endif

/* needed message structures                                                */

/* data structure to describe message properties                            */
typedef struct
{
    uint8_t opcode;
    uint8_t length;
    uint8_t properties;
} LTPCmdInfo;
typedef LTPCmdInfo *PLTPCmdInfo;

/* internal Status definition for LTP-Lib                                   */
typedef enum
{
    HRPLibStatusIdle,             /* ready to be called with 'LTPLibxxx' call*/
    HRPLibStatusResync,           /* Lib is ready for re-sync                */
    HRPLibStatusBusy,             /* busy with 'LTPLibxxx' call (re-entrant) */
    HRPLibStatusOffSync           /* Lib detected sync loss in receive data  */
} T_ENUM_HRP_STATUS;

/* internal data container definition for LTP-Lib                           */
typedef struct
{
    uint16_t   Offset;                 /* offset to data in data buffer           */
    uint16_t   Length;                 /* length of data                          */
    uint8_t *BufferAddress;           /* buffer address                          */
} T_HRP_DATA_CB;
typedef T_HRP_DATA_CB   *LTP_DATA_CB_P;

/* internal data container definition for queue elements                    */
struct hrpQueueElement                                /* dummy definition      */
{
    struct hrpQueueElement  *Next;                      /* point to next element */
    uint8_t                    data[2];                    /* user data             */
};
typedef struct hrpQueueElement T_HRP_QUEUE_ELEMENT;
typedef T_HRP_QUEUE_ELEMENT *P_HRP_ELEMENT;

/* internal data container definition for queue elements                    */
typedef struct
{
    P_HRP_ELEMENT First;                                 /* first element         */
    P_HRP_ELEMENT Last;                                  /* last element          */
    uint16_t          ElementCount;                          /* element count         */
} HRP_QUEUE_T, *HRP_QUEUE_P;

void  hrp_queue_in(HRP_QUEUE_P QueuePtr, void *pQueueElement);
void *hrp_queue_out(HRP_QUEUE_P QueuePtr);

/* internal queue-element for LTP-Lib for data container                    */
typedef struct
{
    /* This MUST be the FIRST structure element ! */
    T_HRP_QUEUE_ELEMENT  QueueElement;
    /* the 'real' data...                         */
    T_HRP_DATA_CB  DataCB;
} T_HRP_ELEMENT;
typedef T_HRP_ELEMENT *P_HRP_ElEMENT;



/* internal context definition for LTP-Lib. When ever a LTP-Lib function is */
/*  called, a pointer to this context must be provided                      */
typedef struct
{
    HRP_TGT_APPHANDLE          AppHandle;
    T_ENUM_HRP_STATUS          Status;
    uint16_t                ReceiveOffset;
    uint16_t                ReceiveMaxLength;
    uint16_t                SendOffset;
    uint16_t                HRPMsgStart;
    P_HRP_ElEMENT         pActiveElement;
    uint8_t               *pHRPMsg;
    uint16_t                HRPMsgPos;
    uint16_t                HRPMsgLength;

    HRP_QUEUE_T         UsedElementQueue;
    uint16_t                HRPDataCollected;
} T_HRP_LIB;
typedef T_HRP_LIB *P_HRP_LIB;
typedef struct
{
    uint8_t   seq_id;
    bool crc8;
    bool crc16;
} HRP_SEQ_ID;

void hrp_lib_trigger_hrp_proccess(P_HRP_LIB p_hrp_lib);
static bool hrp_lib_trigger(P_HRP_LIB p_hrp_lib);
static bool bt_hrp_transfer_hrp_element_data(P_HRP_LIB p_hrp_lib, uint16_t copy_length);
static void bt_hrp_crc_error(P_HRP_LIB p_hrp_lib);
bool hrp_lib_send_internal_event_info(P_HRP_LIB p_hrp_lib, uint8_t copmsk, uint8_t *p_opt,
                                      uint8_t cause,
                                      uint8_t event_type, uint32_t event_info);


/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/**                                                                        **/
/** 4) Target specifics that must be implemented by the user of the LTP-Lib**/
/**                                                                        **/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

/****************************************************************************/
/* uint8_t * bt_hrp_tgt_send_buffer_alloc                                           */
/* (                                                                        */
/*    HRP_TGT_APPHANDLE AppHandle : Handle to identify application context  */
/*    uint16_t              len       : size of buffer to be allocated (bytes)  */
/* )                                                                        */
/* return:------------------------------------------------------------------*/
/* pointer to allocated memory in case of success                           */
/* NULL pointer in case of an error                                         */
/*                                                                          */
/* Description:-------------------------------------------------------------*/
/* This target specific function is used to allocate buffers for LTP        */
/* messages that are send to an application with the BT_LTP_Sendxxx         */
/* functions of this library.                                               */
/*                                                                          */
/* TODO: please implement this function for your target                     */
/*                                                                          */
/****************************************************************************/
uint8_t *bt_hrp_tgt_send_buffer_alloc(HRP_TGT_APPHANDLE app_handle, uint16_t len);

/****************************************************************************/
/* uint8_t * BTLTPTgtAssemblyBfferAlloc                                        */
/* (                                                                        */
/*    HRP_TGT_APPHANDLE AppHandle : Handle to identify application context  */
/* )                                                                        */
/* return:------------------------------------------------------------------*/
/* pointer to allocated memory in case of success                           */
/* NULL pointer in case of no buffer available (this is no error condition) */
/*                                                                          */
/* Description:-------------------------------------------------------------*/
/* This target specific function is used to allocate buffers for LTP-       */
/* message assembly that is processed by functions of this library.         */
/*                                                                          */
/* TODO: please implement this function for your target                     */
/*                                                                          */
/****************************************************************************/
uint8_t *bt_hrp_tgt_assembly_buffer_alloc(HRP_TGT_APPHANDLE app_handle);

void bt_hrp_buffer_callback(void *Handle);

/****************************************************************************/
/* void bt_hrp_tgt_receive_buffer_release                                        */
/* (                                                                        */
/*    HRP_TGT_APPHANDLE AppHandle : Handle to identify application context  */
/*    uint8_t *            pBuffer   : pointer to receive buffer to be released*/
/* )                                                                        */
/* return:------------------------------------------------------------------*/
/* non                                                                      */
/*                                                                          */
/* Description:-------------------------------------------------------------*/
/* This target specific function is used to released buffers for LTP        */
/* messages that are received and consumed by the 'hrp_lib_handle_receive_data' */
/* function of this library.                                                */
/*                                                                          */
/* TODO: please implement this function for your target                     */
/*                                                                          */
/****************************************************************************/
void bt_hrp_tgt_receive_buffer_release(HRP_TGT_APPHANDLE app_handle, uint8_t *pBuffer);

/****************************************************************************/
/* bool bt_hrp_tgt_send_hrp_message                                              */
/* (                                                                        */
/*    HRP_TGT_APPHANDLE AppHandle : Handle to identify application context  */
/*    uint8_t *            pMsg      : pointer to of LTP msg buffer to be send */
/*    uint16_t              offset                                              */
/*    uint16_t              dataLen                                             */
/* )                                                                        */
/* return:------------------------------------------------------------------*/
/* true in case the message could be send successfully,                     */
/* false in case the message could not be send but was dumped               */
/*                                                                          */
/* Description:-------------------------------------------------------------*/
/* This function is used to send an LTP message to an application with the  */
/* BT_LTP_Sendxxx functions of this library                                 */
/*                                                                          */
/* TODO: please implement this function for your target                     */
/*                                                                          */
/****************************************************************************/
bool bt_hrp_tgt_send_hrp_message(HRP_TGT_APPHANDLE app_handle, uint8_t *pMsg, uint16_t offset,
                                 uint16_t data_len);

/****************************************************************************/
/* bool bt_hrp_tgt_handle_hrp_message                                            */
/* (                                                                        */
/*    HRP_TGT_APPHANDLE AppHandle : Handle to identify application context  */
/*    uint8_t *            pMsgBuffer: pointer to message buffer               */
/*    uint8_t              cmd       : identifier for LTP-command to be handled*/
/*    uint8_t              copmsk    : copmsk of LPT-command to be handled     */
/*    uint8_t *            pOpt      : pointer to optional parameters of LTP-  */
/*                                  command to be handled, or NULL in case  */
/*                                  of no optional parameters included      */
/*    uint16_t              lenPara   : length of mandatory parameters of LTP-  */
/*                                  command to be handled                   */
/*    uint8_t *            pPara     : pointer to mandatory parameters of LTP- */
/*                                  command to be handled, or NULL in case  */
/*                                  of no mandatory parameters included     */
/* )                                                                        */
/* return:------------------------------------------------------------------*/
/* false in case the message buffer shall not be re-used for LTP-msg        */
/* assembly by the LTP-Lib, otherwise true                                  */
/*                                                                          */
/* Description:-------------------------------------------------------------*/
/* This function is called by the LTP-Lib if a complete LTP message is      */
/* assembled and is ready to be consumed by the application                 */
/*                                                                          */
/* TODO: please implement this function for your target                     */
/*                                                                          */
/****************************************************************************/
bool bt_hrp_tgt_handle_hrp_message(HRP_TGT_APPHANDLE app_handle, uint8_t *p_msg_buffer, uint8_t cmd,
                                   uint8_t copmsk, uint8_t *p_opt, uint16_t len_para, uint8_t *p_para);

/****************************************************************************/
/* PLTPElement BTLTPTgtQueueElementAlloc                                    */
/* (                                                                        */
/*    HRP_TGT_APPHANDLE AppHandle : Handle to identify application context  */
/* )                                                                        */
/* return:------------------------------------------------------------------*/
/* pointer to allocated queue element in case of success                    */
/* NULL pointer in case of no queue element available (this is no error     */
/* condition)                                                               */
/*                                                                          */
/* Description:-------------------------------------------------------------*/
/* This target specific function is used to allocate queue elements for LTP-*/
/* message assembly that is processed by functions of this library.         */
/*                                                                          */
/* TODO: please implement this function for your target                     */
/*                                                                          */
/****************************************************************************/
P_HRP_ElEMENT bt_hrp_tgt_queue_element_alloc(HRP_TGT_APPHANDLE app_handle);

/****************************************************************************/
/* void bt_hrp_tgt_queue_element_release                                         */
/* (                                                                        */
/*    HRP_TGT_APPHANDLE AppHandle : Handle to identify application context  */
/* )                                                                        */
/* return:------------------------------------------------------------------*/
/* non                                                                      */
/*                                                                          */
/* Description:-------------------------------------------------------------*/
/* This target specific function is used to release queue elements for LTP- */
/* message assembly that is processed by functions of this library.         */
/*                                                                          */
/* TODO: please implement this function for your target                     */
/*                                                                          */
/****************************************************************************/
void bt_hrp_tgt_queue_element_release(HRP_TGT_APPHANDLE app_handle, P_HRP_ElEMENT p_hrp_element);

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/**                                                                        **/
/** 5) Utility fuctions that have to be integrated for using the LTP-Lib   **/
/**                                                                        **/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/

/****************************************************************************/
/* bool hrp_lib_initialize                                                    */
/* (                                                                        */
/*    P_HRP_LIB      pLTPLib        : pointer to LTP context to be initialized*/
/*    HRP_TGT_APPHANDLE AppHandle : Handle to identify application context  */
/*    uint16_t              receiveOffset : offset for assembled LTP-messages   */
/*    uint16_t              receiveMaxLen : max len for assembled LTP-messages  */
/*    uint16_t              sendOffset    : offset for LTP-messages to be send  */
/* )                                                                        */
/* return:------------------------------------------------------------------*/
/*  false if an internal error occured and the lib is not functional,       */
/*  true otherwise (in case of success)                                     */
/*                                                                          */
/* Description:-------------------------------------------------------------*/
/* This function has to be called to initialize the LTP-Lib at system       */
/* startup and/or reset                                                     */
/*                                                                          */
/* TODO: please integrate this function into the target implementation      */
/*                                                                          */
/****************************************************************************/
bool hrp_lib_initialize(P_HRP_LIB p_hrp_lib, HRP_TGT_APPHANDLE app_handle, uint16_t receive_offset,
                        uint16_t receive_max_len, uint16_t send_offset);

/****************************************************************************/
/* bool hrp_lib_shutdown                                                      */
/* (                                                                        */
/*    P_HRP_LIB      pLTPLib        : pointer to LTP context to be initialized*/
/* )                                                                        */
/* return:------------------------------------------------------------------*/
/*  false if an internal error occured and the lib is not functional,       */
/*  true otherwise (in case of success)                                     */
/*                                                                          */
/* Description:-------------------------------------------------------------*/
/* This function may be called to free all resources used by LTP-Lib        */
/*                                                                          */
/* TODO: please integrate this function into the target implementation      */
/*                                                                          */
/****************************************************************************/
bool hrp_lib_shutdown(P_HRP_LIB p_hrp_lib);

//void hrp_lib_trigger_hrp_proccess(P_HRP_LIB pLTPLib);

#if F_LTPLIB_ASYNC_ASSEMBLY_SUPPORT
/****************************************************************************/
/* void LTPLibHandleTimeout                                                 */
/* (                                                                        */
/*    P_HRP_LIB     pLTPLib  : pointer to LTP context to be used              */
/*    TLTPTimerID timerID  : timer that timed out                           */
/* )                                                                        */
/****************************************************************************/
void hrp_lib_handle_receive_data(P_HRP_LIB p_hrp_lib, TLTPTimerID timerID);
#endif

/****************************************************************************/
/* bool LTPLibHandleReceiveData                                             */
/* (                                                                        */
/*    P_HRP_LIB pLTPLib      : pointer to LTP context to be used              */
/*    uint8_t *  pRxBuffer    : pointer to buffer that contains new data       */
/*    uint16_t    rxLength     : length of new data received                    */
/*    uint16_t    rxOffset     : offset of new data in buffer                   */
/* )                                                                        */
/* return:------------------------------------------------------------------*/
/*  false data could NOT be handled properly,                               */
/*  true otherwise                                                          */
/*                                                                          */
/* Description:-------------------------------------------------------------*/
/* This function has to be called to introduce new rx data received into    */
/* the LTP-Lib statemachine when ever new data is received                  */
/*                                                                          */
/* TODO: please integrate this function into the target implementation      */
/*                                                                          */
/****************************************************************************/
bool hrp_lib_handle_receive_data(P_HRP_LIB p_hrp_lib, uint8_t *p_rx_buffer, uint16_t rx_length,
                                 uint16_t rx_offset);

/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/**                                                                        **/
/** 6) Utility functions that can be used by the user of the LTP-Lib       **/
/**                                                                        **/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/
/****************************************************************************/



/****************************************************************************/
/* uint16_t hrp_lib_insert_header                                                  */
/* (                                                                        */
/*    P_HRP_LIB pLTPLib      : pointer to LTP context to be used              */
/*    uint8_t *  pBuffer      : pointer to data buffer containing payload      */
/*    uint16_t*  offset       : pointer to the offset of the payload           */
/*                           after call: offset of the message in buffer    */
/*    uint16_t    dataLen      : payload length in buffer                       */
/*    uint8_t    cmd          : ltp command                                    */
/*    uint8_t    copmsk       : bitmask defining content of optional parameter */
/*    uint8_t *  pOpt         : pointer to optional parameter structure        */
/*    uint16_t*  posParam     : after call: offset of the first parameter      */
/* )                                                                        */
/* return:------------------------------------------------------------------*/
/* length of the ltp message in pBuffer beginning from *offset              */
/* if length is 0, the header could not be inserted                         */
/*                                                                          */
/* Description:-------------------------------------------------------------*/
/* This function can be used to insert a LTP message header with optional   */
/* parameters infront of a given message buffer with payload                */
/*                                                                          */
/****************************************************************************/


#ifdef __cplusplus
}
#endif

#endif /* LTPLIB_H */
