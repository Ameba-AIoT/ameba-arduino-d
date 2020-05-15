#ifndef __PIR_H__
#define __PIR_H__

#define PIR_IRQHandler			EXTI1_IRQHandler
#define PIR_INT_GPIO_CLK		RCC_AHB1Periph_GPIOB
#define PIR_INT_PIN				GPIO_Pin_1
#define PIR_INT_GPIO_PORT		GPIOB
#define PIR_INT_EXT_PORT_SRC	EXTI_PortSourceGPIOB
#define PIR_INT_EXT_PIN_SRC		EXTI_PinSource1
#define PIR_INT_EXT_LINE		EXTI_Line1
#define PIR_INT_EXT_IRQ			EXTI1_IRQn

void pir_isr(void);
int  pir_init_flag(int *state);
int  pir_init_callback(void (* pir_callback)(void));
void pir_exit(void);

#endif // __PIR_H__
