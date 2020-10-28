/**************************************************************************//**
 * @file     objects.h
 * @brief    Mbed HAL API Object structure definition.
 * @version  V1.00
 * @date     2017-05-03
 *
 * @note
 *
 ******************************************************************************
 *
 * Copyright(c) 2007 - 2017 Realtek Corporation. All rights reserved.
 *
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the License); you may
 * not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an AS IS BASIS, WITHOUT
 * WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *
 ******************************************************************************/
#ifndef MBED_OBJECTS_H
#define MBED_OBJECTS_H

#include "cmsis.h"
#include "PortNames.h"
//#include "PeripheralNames.h"
#include "PinNames.h"
#include "hal.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_GPIO_EN
struct gpio_irq_s {
    hal_gpio_irq_adapter_t gpio_irq_adp;
};

typedef struct gpio_irq_s gpio_irq_t;

struct gpio_s {
    hal_gpio_adapter_t adapter;
};

typedef struct gpio_s gpio_t;

struct port_s {
    hal_gpio_port_adapter_t hal_port;
};
#endif  // end of "#ifdef CONFIG_GPIO_EN"

#ifdef CONFIG_UART_EN
struct serial_s {
    hal_uart_adapter_t uart_adp;
    hal_gdma_adaptor_t tx_gdma;
    hal_gdma_adaptor_t rx_gdma;
    uint32_t irq_en;
    void *tx_irq_handler;
    uint32_t tx_irq_id;
};
#endif  // end of "#ifdef CONFIG_UART_EN"


#ifdef CONFIG_PWM_EN
struct pwmout_s {
    uint8_t pwm_idx;
    uint8_t pin_sel;
    uint8_t is_init; // 1: Initialize Success
    uint32_t period;
    uint32_t pulse;
    uint32_t offset_us;
    uint32_t polarity;
    hal_pwm_adapter_t  pwm_hal_adp;
};
#endif

#ifdef CONFIG_I2C_EN
struct i2c_s {
    hal_i2c_adapter_t i2c_adp;
};
#endif

#ifdef CONFIG_ADC_EN
struct analogin_s {
    uint8_t idx;
};
#endif

#ifdef CONFIG_GTIMER_EN
struct gtimer_s {
    hal_timer_adapter_t timer_adp;
};
#endif

#ifdef CONFIG_I2S_EN
struct i2s_s {
    hal_i2s_adapter_t i2s_adapter;
};

#endif

#ifdef CONFIG_PCM_EN
struct pcm_s {
    hal_pcm_adapter_t pcm_adapter;
};

#endif


#ifdef CONFIG_DAC_EN

struct dac_s {
    void *dummy;
};
#endif

struct gdma_s {
    hal_gdma_adaptor_t hal_gdma_adaptor;
};

struct flash_s {
    hal_spic_adaptor_t *phal_spic_adaptor;
    u8 flash_pin_sel;
};

#ifdef CONFIG_SGPIO_EN
struct sgpio_s {
    hal_sgpio_adapter_t sgpio_adapter;
    hal_gdma_adaptor_t gdma_adaptor;
};

#endif

#ifdef CONFIG_SPI_EN
struct spi_s {
    hal_ssi_adaptor_t hal_ssi_adaptor;

    volatile u32 state;
    u32 irq_handler;
    u32 irq_id;
    u32 bus_tx_done_handler;
    u32 bus_tx_done_irq_id;
    u32 dma_en;
    u32 sclk;
#ifdef CONFIG_GDMA_EN
    hal_gdma_adaptor_t spi_gdma_adp_tx;
    hal_gdma_adaptor_t spi_gdma_adp_rx;
#endif
};
#endif

#ifdef CONFIG_ICC_EN

// define ICC command indication callback register table size, adjust this size as needed by the user application
#define ICC_CMD_TBL_SIZE        10      // the size of the ICC command table, user application can add ICC command and its callback to this table

// define ICC message indication callback register table size, adjust this size as needed by the user application
#define ICC_MSG_TBL_SIZE        10      // the size of the ICC message table, user application can add ICC message and its callback to this table

#if defined (CONFIG_BUILD_NONSECURE)
// ICC object for Non-Secure API
struct icc_s {
    icc_ns_adapter_t *picc_adp;
    icc_user_cmd_ns_entry_t icc_cmd_tbl[ICC_CMD_TBL_SIZE];
    icc_user_msg_ns_entry_t icc_msg_tbl[ICC_MSG_TBL_SIZE];
};
#else
// ICC object for Secure/Ignore-Secure API
struct icc_s {
    hal_icc_adapter_t *picc_adp;
    icc_user_cmd_entry_t icc_cmd_tbl[ICC_CMD_TBL_SIZE];
    icc_user_msg_entry_t icc_msg_tbl[ICC_MSG_TBL_SIZE];
};
#endif

#endif  // #ifdef CONFIG_ICC_EN

#ifdef CONFIG_AUDIO_EN
struct audio_s {
    hal_audio_adapter_t audio_adapter;
    hal_gdma_adaptor_t audio_gdma_adp_tx;
    hal_gdma_adaptor_t audio_gdma_adp_rx;    
};

#endif


#ifdef __cplusplus
}
#endif

#endif  // end of "#ifndef MBED_OBJECTS_H"

