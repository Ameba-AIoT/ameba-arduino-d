#ifndef _STM32_I2C_H_
#define _STM32_I2C_H_

#include <stdint.h>

#define I2C_MASTER

// I2C mode supported
#define I2C_OP_MODE_POLLING			0x01
#define I2C_OP_MODE_INTERRUPT		0x02
#define I2C_OP_MODE_DMA_POLLING		0x04
#define I2C_OP_MODE_DMA_INTERRUPT	0x08

// Define I2C Speed --------------------------------------------------------
#ifdef FAST_I2C_MODE
	#define I2C_SPEED 340000
	#define I2C_DUTYCYCLE I2C_DutyCycle_16_9  
#else // STANDARD_I2C_MODE
	#define I2C_SPEED 100000
	#define I2C_DUTYCYCLE  I2C_DutyCycle_2
#endif // FAST_I2C_MODE

// Define I2C operation mode 
#ifndef I2C_OP_MODE
#define I2C_OP_MODE					I2C_OP_MODE_INTERRUPT
#endif

// I2Cx Communication boards Interface
#define I2Cx_NUM					I2C1
#define I2Cx_CLK					RCC_APB1Periph_I2C1
#define I2Cx_EV_IRQn				I2C1_EV_IRQn
#define I2Cx_ER_IRQn				I2C1_ER_IRQn
#define I2Cx_EV_IRQHANDLER			I2C1_EV_IRQHandler
#define I2Cx_ER_IRQHANDLER			I2C1_ER_IRQHandler

#define I2Cx_SDA_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define I2Cx_SDA_PIN				GPIO_Pin_9
#define I2Cx_SDA_GPIO_PORT			GPIOB                
#define I2Cx_SDA_SOURCE				GPIO_PinSource9
#define I2Cx_SDA_AF					GPIO_AF_I2C1

#define I2Cx_SCL_GPIO_CLK			RCC_AHB1Periph_GPIOB
#define I2Cx_SCL_PIN				GPIO_Pin_8
#define I2Cx_SCL_GPIO_PORT			GPIOB
#define I2Cx_SCL_SOURCE				GPIO_PinSource8
#define I2Cx_SCL_AF					GPIO_AF_I2C1

#if (I2C_OP_MODE == I2C_OP_MODE_INTERRUPT)
	#define stm32_i2c_tx	stm32_i2c_tx_int
	#define stm32_i2c_rx	stm32_i2c_rx_int
#else
	#error
#endif

void 		stm32_i2c_ev_isr(void);
void 		stm32_i2c_err_isr(void);
uint32_t 	stm32_i2c_tx(uint16_t device_address, uint8_t send_start, const uint8_t *tx_data, uint32_t count, uint8_t send_stop);
uint32_t 	stm32_i2c_rx(uint16_t device_address, uint8_t send_start, uint8_t *rx_data, uint32_t count, uint8_t send_nak, uint8_t send_stop);
int			stm32_i2c_init(void);
void		stm32_i2c_deinit(void);

#endif //_STM32_I2C_H_
