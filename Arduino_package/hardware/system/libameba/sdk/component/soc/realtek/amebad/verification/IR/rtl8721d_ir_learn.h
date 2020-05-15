/*
 *  Routines to access hardware
 *
 *  Copyright (c) 2013 Realtek Semiconductor Corp.
 *
 *  This module is a confidential and proprietary property of RealTek and
 *  possession or use of this module requires written permission of RealTek.
 */
#ifndef _RTL8721D_IR_TEST_H_
#define _RTL8721D_IR_TEST_H_


/** @addtogroup  DUALMODE_RMC Dualmode RMC
    * @{
    */

/** @defgroup  IR_LEARN Ir Learn
    * @brief  This file handles ir learn function.
    * @{
    */

/*============================================================================*
  *                                 Macros
  *============================================================================*/
/** @defgroup IR_LEARN_Exported_Macros Ir Learn Exported Macros
    * @{
    */
#define IR_RX_FIFO_THR_LEVEL    10  /**<trigger interrupt if rx has IR_RX_FIFO_THR_LEVEL num data*/
#define LOOP_QUEUE_MAX_SIZE     512
#define QUEUE_CAPABILITY (LOOP_QUEUE_MAX_SIZE-1)

#define IR_FREQ_10MHZ 10000 /**<10M*/

/* Enable calculate frequency when decode packet or not */
#define CALCULATE_FREQUENCY         TRUE
/* Enable correcting frequency by software */
#define CORRECT_FREQ_BY_SOFTWARE    TRUE

#define RMC_IR_LEARN_TIMEOUT_MS 20000 /**<20s*/

#define ABS_FREQUENCY(a,b)          ((a>b) ? (a-b):(b-a))


/* IR learn time out is 10s */
#define IR_LEARN_TIME_OUT           (10*configTICK_RATE_HZ)/*10s*/


/*--------------Configure by user to confirm IR learn parameter --------*/
/* Filter threshold value. If 100< time interval< 2000, treat it as a part of a carrier time */
#define TIME_LOWEST_VALUE           100  /**<0.01ms   learn frequency is 10M*/
#define TIME_HIGHEST_VALUE          2000 /**<0.2ms    learn frequency is 10M*/
/* If low voltage time>0.25ms,we treat it as a time which have no carrier */
#define TIME_LOW_VOLTAGE_VALUE      2500 /**<0.25ms   learn frequency is 10M*/

#define LEARN_WAVE_MAX_SIZE        (100)    /*max size of learn data of per key in uint32_t*/

#define LEARN_WAVE_MIN_SIZE        (10)    /*max size of learn data of per key in uint32_t*/

/** End of IR_LEARN_Exported_Macros
    * @}
    */

/*============================================================================*
  *                                     Types
  *============================================================================*/
/** @defgroup IR_LEARN_Exported_Types Ir Learn Exported Types
    * @{
    */
typedef enum t_ir_ret_code
{
    IR_RET_OK,          /**< ir return ok*/
    IR_LEARN_ERR,       /**< ir learn error*/
} T_IR_RET_CODE;

/* Data struct for IR learn, to-do: 4 bytes align*/
/*need to find if all needed ###*/
typedef struct t_ir_packet_typedef
{
    /* Manage loop queue */
    volatile uint16_t   read_index;          /**< index of read queue */
    volatile uint16_t   write_index;         /**< index of write queue */
    uint32_t *queue;    /**< pointer to the queue */

    /* Record IR learned data */
    uint32_t            detect_time[LEARN_WAVE_MAX_SIZE];/**< record total time*/
    uint32_t            square_wave_num;    /**< record square wave number*/
    uint16_t            detect_time_num;    /**< record IR data number */
    bool                is_high_level;  /**< record decode voltage level */
    volatile bool       over_flow;  /**< record loop queue over folw or not */

#if CALCULATE_FREQUENCY
    /* Record learned IR frequency */
    float               wave_frequency[LEARN_WAVE_MAX_SIZE / 2 + 1];    /**< record IR frequency*/
#else
    /* Record IR high waveform time parameters */
    uint32_t            high_wave_time[LEARN_WAVE_MAX_SIZE / 2 + 1];
    uint16_t            high_wave_count[LEARN_WAVE_MAX_SIZE / 2 + 1];
#endif
    uint16_t            frequency_num;       /**< Record number of calculated frequency */
    volatile bool       data_receive_completed; /**< Record IR learn data receive completed flag */

} T_IR_PACKET_TYPEDEF;
/** End of IR_LEARN_Exported_Types
    * @}
    */

/*============================================================================*
  *                                 Variables
  *============================================================================*/
/** @defgroup IR_LEARN_Exported_Variables Ir Learn Exported Variables
    * @{
    */

/*record IR Learn Key Index*/
//extern T_VIRTUAL_KEY_DEFS g_ir_learn_key_index;
extern u32 g_ir_learn_key_index;
/*ir learn data packet*/
extern T_IR_PACKET_TYPEDEF g_ir_learn_packet;
/*set this flag if press key while receiving ir learn data*/
extern volatile bool g_exit_ir_learn_by_key;
/** End of IR_LEARN_Exported_Variables
    * @}
    */

/*============================================================================*
  *                                     Functions
  *============================================================================*/
/** @defgroup IR_LEARN_Exported_Functions Ir Learn Exported Functions
    * @{
    */
/**
  * @brief  this API is for user to Initial IR learn mode.
  * @param  none
  * @return none
  */
bool ir_learn_begin(void);
/**
  * @brief  this API is for user to end IR Learn.
  * @param  none
  * @return none
  */
void ir_learn_end(void);
/**
  * @brief  this API is for user to reset IR learn mode, before capture wave every time.
  * @param  none
  * @return none
  */
void ir_learn_reset(void);
/**
  * @brief  this API is for user to capture IR wave and save to flash ,
  *         address is key_index*LEARN_WAVE_MAX_SIZE.
  * @param  key_index   key index for ir learn
  * @return none
  */
void ir_learn_wave_capture(u32  key_index);
/**
  * @brief  this API is for user to exit IR wave wait or capture wave.
  * @param  none
  * @return none
  */
void ir_learn_capture_exit(void);
/**
    * @brief    get flash address index according to key index value.
    * @note
    * @param    key index
    * @return   flash index.
    * @retval   0~8
    */
uint16_t get_flash_addr_index( u32 key_index);
/** End of IR_LEARN_Exported_Functions
    * @}
    */

/** @} */ /* End of group IR_LEARN */
/** @} */ /* End of group DUALMODE_RMC */

#endif
