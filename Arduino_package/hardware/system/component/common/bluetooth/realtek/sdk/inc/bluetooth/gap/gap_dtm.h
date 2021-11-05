/**
*****************************************************************************************
*     Copyright(c) 2017, Realtek Semiconductor Corporation. All rights reserved.
*****************************************************************************************
  * @file    gap_dtm.h
  * @brief
  * @details
  * @author  jeff_zheng
  * @date    2017-08-02
  * @version v1.0
  ******************************************************************************
  * @attention
  * <h2><center>&copy; COPYRIGHT 2017 Realtek Semiconductor Corporation</center></h2>
  ******************************************************************************
  */
/* Define to prevent recursive inclusion **/
#ifndef GAP_DTM_H
#define GAP_DTM_H

#include "bt_flags.h"
#include "gap_le.h"
#include <gap_le_types.h>

#if F_BT_LE_4_0_DTM_SUPPORT
/** @addtogroup GAP_LE_DTM GAP LE Direct Test Mode Module
  * @brief GAP LE Direct Test Mode Module
  * @{
  */

/*============================================================================*
 *                         Types
 *============================================================================*/
/** @defgroup GAP_LE_DTM_Exported_Types GAP LE DTM Exported Types
  * @brief
  * @{
  */
typedef enum
{
    GAP_DTM_MODULATION_INDEX_STANDARD   = 0x00,    /**< Assume transmitter will have a standard modulation index. */
    GAP_DTM_MODULATION_INDEX_STABLE     = 0x01,    /**< Assume transmitter will have a stable modulation index. */
} T_GAP_DTM_MODULATION_INDEX_TYPE;

typedef enum
{
    GAP_DTM_PACKET_PAYLOAD_PRBS9 = 0x00,    /**< PRBS9 sequence '11111111100000111101' (in transmission order). */
    GAP_DTM_PACKET_PAYLOAD_1100 = 0x01,     /**< Repeated '11110000' (in transmission order) sequence. */
    GAP_DTM_PACKET_PAYLOAD_10 = 0x02,       /**< Repeated '10101010' (in transmission order) sequence. */
    GAP_DTM_PACKET_PAYLOAD_PRBS15 = 0x03,   /**< PRBS15 sequence. */
    GAP_DTM_PACKET_PAYLOAD_ALL1 = 0x04,     /**< Repeated '11111111' (in transmission order) sequence. . */
    GAP_DTM_PACKET_PAYLOAD_ALL0 = 0x05,     /**< Repeated '00000000' (in transmission order) sequence. . */
    GAP_DTM_PACKET_PAYLOAD_0011 = 0x06,     /**< Repeated '00001111' (in transmission order) sequence. . */
    GAP_DTM_PACKET_PAYLOAD_01 = 0x07        /**< Repeated '01010101' (in transmission order) sequence. . */
} T_GAP_DTM_PACKET_PAYLOAD_TYPE;

typedef enum
{
    GAP_DTM_PHYS_1M       = 0x01,    /**< LE PHY 1M used. */
    GAP_DTM_PHYS_2M       = 0x02,    /**< LE PHY 2M used. */
    GAP_DTM_PHYS_CODED_S8 = 0x03,    /**< LE Coded PHY with S=8 data coding. */
    GAP_DTM_PHYS_CODED_S2 = 0x04,    /**< LE Coded PHY with S=2 data coding. */
} T_GAP_DTM_PHYS_TYPE;

/** End of GAP_LE_DTM_Exported_Types
 * @}
 */

/*============================================================================*
 *                         Functions
 *============================================================================*/

/** @defgroup GAP_LE_DTM_Exported_Functions GAP LE Direct Test Mode Exported Functions
  * @brief GAP LE Direct Test Mode Exported Functions
  * @{
  */

/**
* @brief   Start a test where the DUT receives test reference packets at a fixed interval.
            The tester generates the test reference packets.
*
* @param[in]   rx_chann - channel to receive packets.
*
* @retval GAP_CAUSE_SUCCESS: Operation success.
* @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void dtm_test_req(uint16_t command)
    {
        rx_chann = (command & 0x3f00) >> 8;
        le_dtm_receiver_test_req(rx_chann);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        uint16_t status  = 0;
        uint16_t event = 0;
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_DTM_RECEIVER_TEST:
           status = p_data->p_le_receive_test_rsp->cause;
           if (status == 0)
           {
               APP_PRINT_INFO2("dtm_event_trace: ev = 0x%x, st = 0x%x", (event & 0x8000) >> 15, event & 0x1);
           }
           else
           {
               event |= 1;
               APP_PRINT_INFO2("dtm_evt_trace: EV = 0x%x, st = 0x%x", (event & 0x8000) >> 15, event & 0x1);
           }
           dtm_uart_send_bytes(event);
           break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_dtm_receiver_test(uint8_t rx_chann);

/**
* @brief   Start a test where the DUT generates test reference packets at a fixed interval.
            The Controller shall transmit at maximum power.
*
* @param[in]   tx_chann - channel to transmit packets.
* @param[in]   data_len - length in bytes of payload data in each packet.
* @param[in]   pkt_pl - the value of packet payload.
*
* @retval GAP_CAUSE_SUCCESS: Operation success.
* @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void dtm_test_req(uint16_t command)
    {
        tx_chann = (command & 0x3f00) >> 8;
        data_len = up_2_bits << 6 | (command & 0xfc) >> 2;
        pkt_pl = command & 0x03;
        le_dtm_transmitter_test(tx_chann, data_len, pkt_pl);
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        uint16_t status  = 0;
        uint16_t event = 0;
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_DTM_TRANSMITTER_TEST:
        status = p_data->le_cause.cause;
           if (status == 0)
           {
               APP_PRINT_INFO2("dtm_evt_trace: ev = 0x%x, st = 0x%x", (event & 0x8000) >> 15, event & 0x1);
           }
           else
           {
               event |= 1;
               APP_PRINT_INFO2("dtm_evt_trace: st = 0x%x, st = 0x%x", (event & 0x8000) >> 15, event & 0x1);
           }
           dtm_uart_send_bytes(event);
           break;
        }
        ...
    }
  * \endcode
  */

T_GAP_CAUSE le_dtm_transmitter_test(uint8_t tx_chann, uint8_t data_len, uint8_t pkt_pl);

/**
* @brief   Stop any test which is in progress.
*
* @retval GAP_CAUSE_SUCCESS: Operation success.
* @retval GAP_CAUSE_SEND_REQ_FAILED: Operation fail.
  *
  * <b>Example usage</b>
  * \code{.c}
    void dtm_test_req(uint16_t command)
    {
        ...
        le_dtm_test_end();
    }

    T_APP_RESULT app_gap_callback(uint8_t cb_type, void *p_cb_data)
    {
        T_APP_RESULT result = APP_RESULT_SUCCESS;
        T_LE_CB_DATA *p_data = (T_LE_CB_DATA *)p_cb_data;

        uint16_t status  = 0;
        uint16_t event = 0;
        switch (cb_type)
        {
        ...
        case GAP_MSG_LE_DTM_TEST_END:
             status = p_data->p_le_dtm_test_end_rsp->cause;
             if (status == 0)
             {
                 event |= 1 << 15;
                 event |= p_data->p_le_dtm_test_end_rsp->num_pkts;
                 APP_PRINT_INFO2("dtm_evt_trace: ev = 0x%x, packet count = 0x%x", (event & 0x8000) >> 15,
                                 event & 0x7fff);
             }
             else
             {
                 event |= 1;
                 APP_PRINT_INFO2("dtm_evt_trace: ev = 0x%x, st = 0x%x", (event & 0x8000) >> 15, event & 0x1);
             }
             dtm_uart_send_bytes(event);
             break;
        }
        ...
    }
  * \endcode
  */
T_GAP_CAUSE le_dtm_test_end(void);

/** End of GAP_LE_DTM_Exported_Functions
  * @}
  */

/** End of GAP_LE_DTM
  * @}
  */
#endif

#endif /* GAP_DTM_H */
