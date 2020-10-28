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

#ifdef CONFIG_DAC_EN

struct dac_s {
    void *dummy;
};
#endif

struct gdma_s {
    hal_gdma_adaptor_t hal_gdma_adaptor;
};

#ifdef CONFIG_SGPIO_EN
struct sgpio_s {
    hal_sgpio_adapter_t sgpio_adapter;
    hal_gdma_adaptor_t gdma_adaptor;
};

#endif

#ifdef CONFIG_CIR_EN
struct cir_s {
    hal_cir_adapter_t cir_adapter;
    uint32_t *pcir_rx_pointer;
};

#endif

#ifdef CONFIG_QDEC_EN
struct qdec_s {
    hal_qdec_adapter_t qdec_adapter;
};

#endif

#ifdef CONFIG_CHG_EN

struct chg_s {
    hal_chg_adapter_t chg_adapter;
};

#endif  // end of "#ifdef CONFIG_CHG_EN"

#ifdef __cplusplus
}
#endif

#endif  // end of "#ifndef MBED_OBJECTS_H"

