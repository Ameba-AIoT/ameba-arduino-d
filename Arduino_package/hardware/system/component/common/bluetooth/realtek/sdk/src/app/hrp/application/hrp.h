/**
*********************************************************************************************************
*               Copyright(c) 2014, Realtek Semiconductor Corporation. All rights reserved.
*********************************************************************************************************
* @file      aci_if.h
* @brief     some macro/struct/functions declaration of aci interface
* @details   none.
* @author    tifnan
* @date      2014-10-17
* @version   v0.1
* *********************************************************************************************************
*/

#ifndef     _HRP_H_
#define     _HRP_H_

/****************************************************************************
 * includes
 ****************************************************************************/

#include <string.h>
#include <hrp_utils.h>
#include <hrp_app_flags.h>

#define HRP_SUB_TYPE                          1 /* 0:sdk-v0,acut,bb3-dev; 1:sdk-v1,bcut */
#define ACI_EN                                0

#ifdef TEST_PROFILE
/* select which profiles to be tested */

#define LTP_TEST_APP  1
#if LTP_TEST_APP
#define LTP_TEST_AVRCP 1    // 1 -- built success
#define LTP_TEST_A2DP 1
#define LTP_TEST_HFP  1
#define LTP_TEST_KEY_MGR 1
#define LTP_TEST_GAP 1
#define LTP_TEST_SPP 1

#define LE_SUPPORT 1

#else
#define LTP_TEST_AVRCP 0    // 1 -- built success
#define LTP_TEST_A2DP 0
#define LTP_TEST_HFP  0
#define LTP_TEST_KEY_MGR 0
#define LTP_TEST_GAP 0
#define LTP_TEST_SPP 0
#endif
#endif

/* buffer size */
#define RX_BUFFER_SIZE                        0x800  //must be 2^n,because {x&(RX_BUFFER_SIZE-1)}
#define RX_DISABLE_COUNT                      0x200
#define TX_BUFFER_SIZE1                        0x400    //lorna change 0x200 --> 0x400  8K
#define RX_HANDLE_BUFFER_SIZE                 0x400

#define BTLTP_DEFAULT_COPMSK                  0x80          /* enable CRC  */

#define BTHRP_ACTION_POOL_SIZE                (2)  /* enough */
#define BT_GATT_SERVER_MAX_SERVICES_COUNT     8
#define BTIF_MDL_COUNT 4

#define LTP_ACT_INFO_FLAG_ACT_INFO            0x01
#define LTP_ACT_INFO_FLAG_GAP_REG             0x02

#ifdef TEST_PROFILE
#define LTP_ACT_INFO_FLAG_ALL                 (LTP_ACT_INFO_FLAG_ACT_INFO)
#else
#define LTP_ACT_INFO_FLAG_ALL                 (LTP_ACT_INFO_FLAG_ACT_INFO|LTP_ACT_INFO_FLAG_GAP_REG)
#endif

#define BTIF_L2C_COUNT                    6
#define BT_MAX_MTU_SIZE                       1021  //lorna change 335-->672-->1021
#define BTLTP_US_BUFFER_SIZE                  (BT_MAX_MTU_SIZE + 32)
#define BTLTP_MAX_MSG_SIZE                    1021  //lornachange   361-->672-->1021

#define BTHRP_QUEUE_ELEMENT_COUNT             10

#define LTP_TRACE_NONE                        0
#define LTP_TRACE_ERROR                       1
#define LTP_TRACE_INFO                        2
#define LTP_TRACE_DEBUG                       3
#define LTP_TRACE_ALL                         4

/******************************** the define of events ltp used **********************************/
#define LTP_EVENT_BTIF_MESSAGE          0x10      /* BTIF message */
#define LTP_EVENT_UART_RX               0x21      /* data available */
#define LTP_EVENT_UART_TX               0x12      /* transmit request */
#define LTP_EVENT_UART_TX_COMPLETED     0x13      /* transmit completed */
#define LTP_EVENT_PROFILE_MESSAGE       0x14  /* spp tx innner data event */
#define LTP_EVENT_BTIF_L2C_DATA_RSP     0x15  /* l2cap tx innner data event */
#define LTP_EVENT_MPA_L2C_DATA_RSP      0x16  /* l2cap tx innner data event */

/****************************************************************************/
/* state                                                                    */
/****************************************************************************/
typedef enum _TBTHrpState
{
    btltpStateInit,
    btltpStateRegistering,
    btltpStateIdle,
    btltpStateReleasing,
    btltpStateCount
} T_BT_HRP_STATE;

/****************************************************************************/
/* Tx Buffer callback handling                                              */
/****************************************************************************/

typedef enum
{
    btltpActionNotUsed,
    btltpActionExit,
    btltpActionReset,
    btltpActionSendDataConf,
    btltpActionSendDIDDeviceConf,
    btltpActionSendSDPEndpointConf,
    btltpActionReleaseBuffer
} TBTLtpActionCommand;

typedef enum
{
    LTPProfileSppTxData
} TProfileEvent;

typedef struct
{
    void *pBuffer;
    uint32_t  p_srv_handle;
} TBTLtpServiceAction;

typedef union
{
    uint8_t               *pReleaseBuffer;
    uint16_t                MDL_ID;
    TBTLtpServiceAction serviceAction;
} TBTLtpActionData;

typedef struct
{
    TBTLtpActionCommand Action;
    TBTLtpActionData    p;
} T_BT_HRP_ACTION;
typedef T_BT_HRP_ACTION *P_BT_HRP_ACTION;

#define API_TYPE_GATT_SEND_VENDOR_HCI_CMD 3
typedef struct _TApiBufVendorCmd
{
    uint16_t  op;
    uint8_t   len;
    uint8_t   *para;
} TApiBufVendorCmd;

typedef struct _TApiBufUserDefined
{
    uint16_t Type;
    union
    {
        TApiBufVendorCmd ApiBufVendorCmd;
    } p;
} TApiBufUserDefined;


/* Ltp data struct */
typedef struct _TData
{
    uint8_t *pBuffer;
    uint32_t Length;
} T_HRP_DATA;


/** @brief tcb to manage tx buffer */
typedef struct
{
    uint16_t
    tx_blk_idx;          /* sending block index [0 -- TX_BUFFER_SIZE_MASK](block is sending now ) */
    uint16_t    tx_free_blk_idx;     /* free tx block index [0----TX_BUFFER_SIZE_MASK] */
    uint16_t    tx_un_used_size;     /* size of block which is discarded */
    uint16_t    free_size;           /* bytes can be used */
} TxMemTCB, *PTxMemTCB;

/** @brief aci control struct */
typedef struct _ACI_TCB
{
    T_HRP_DATA           TxData;
    void               *Handle;              /* task handle */
    void               *UarthandleEvent;    /* task queue */
    void               *QueueHandleEvent;    /* task queue */
    void               *QueueHandleMessage;  /* message queue */
    void               *QueueHandleTxData;   /* Tx queue */
    void               *QueueHandleRxData;   /* Rx queue */
    void               *QueueHandleTxRel;    /* tx buffer release queue */
    uint32_t                     RxDataIndication;    /* pending responses */
    uint32_t                     RxDataLength;
    uint32_t                     RxOffset;
    uint16_t                      RxReadIndex;
    uint16_t                      RxWriteIndex;
    uint8_t                  *p_rx_buf;           /* pointer to the rx buffer allocated dynamically */
    uint8_t                  *p_tx_buf;           /* pointer to the tx buffer allocated dynamically */
    uint8_t                  *p_rx_handle_buf;    /* buffer address, handle uart rx data */
    uint8_t                  *P_RxBuffer;         /* for rx */
    TxMemTCB                  tx_mem_tcb;         /* manage tx bufer */

    bool                      RxDisabled;
    bool                      TxDisabled;
} ACI_TCB, *P_ACI_TCB;

typedef struct _TBTHrp
{
    uint8_t               *p_send_buffer;    /* for saving the address of tx buffer */
    HRP_QUEUE_T         FreeElementQueue;

    T_BT_HRP_STATE         State;

#if 0
    T_BT_HRP_MDL_CONTEXT    MDLContextPool[BTIF_MDL_COUNT];

    T_BT_HRP_L2C_CONTEXT    L2cContextPool[BTIF_L2C_COUNT];
    uint16_t current_trx_cid;           /* added for br/edr l2cap throughput test*/
    uint16_t current_trx_data_len;      /* added for br/edr l2cap throughput test*/
#endif

    void *QueueHandleProfileMessage;

    /* Buffer callback Action Handling                                       */
    T_BT_HRP_ACTION        ActionPool[BTHRP_ACTION_POOL_SIZE];
    P_BT_HRP_ACTION        pBufferAction;
    /* LTP re-assemble                                                       */
    uint8_t            *pMsgBuffer;
    T_HRP_LIB             HRPLib;
    T_HRP_ELEMENT         ElementPool[BTHRP_QUEUE_ELEMENT_COUNT];
    /* COM interface                                                         */
    uint8_t             ActInfoFlags;
    uint8_t             ownBDAddress[6];

    uint8_t             service_register_idx;
    //  TGattServiceTable   gattServiceTable[BT_GATT_SERVER_MAX_SERVICES_COUNT]; //use for ACI
    void               *gattServiceHandle[BT_GATT_SERVER_MAX_SERVICES_COUNT];//use for LTP

    P_ACI_TCB           p_aci_tcb;
    uint16_t    le_ds_pool_id;
    uint16_t    le_ds_data_offset;
    uint8_t     le_ds_credits;
} T_BT_HRP;
typedef T_BT_HRP *P_BT_HRP;

typedef struct _TAciConfig
{
    uint8_t ltp_interface : 2;
    uint8_t ltp_wake_up_pin_en : 1;
    uint8_t ltp_trace_level: 3;
    uint8_t reserved: 2;

    uint8_t uart_flow_control_en: 1;
    uint8_t uart_word_len: 1;
    uint8_t uart_parity: 2;
    uint8_t reserved2: 4;

    /* ltp uart pin configuration */
    uint8_t uart_tx_pin_index;
    uint8_t uart_rx_pin_index;
    uint8_t uart_cts_pin_index;
    uint8_t uart_rts_pin_index;
    uint8_t gpio_h_pin_index;
    uint8_t gpio_b_pin_index;

    uint32_t uart_baudrate;

    /* ltp spi pin configuration */
    uint8_t spi_mosi_pin_index;
    uint8_t spi_miso_pin_index;
    uint8_t spi_cs_pin_index;
    uint8_t spi_clk_pin_index;
    uint8_t spi_int_pin_index; /* used to notify master to read data drom bee */

    uint8_t reserved3[7];

} TAciConfig;
typedef TAciConfig *PAciConfig;
extern PAciConfig P_AciConfig;
extern P_BT_HRP  P_BtHrp;

/* hrp_application.c  */
extern void hrp_buffer_release(void *pBuffer);
extern void hrp_write(uint8_t *p_buf, uint32_t buf_len);

#endif

