#ifndef __RGB_LED_H__
#define __RGB_LED_H__

#include <stdint.h>

// R: PA6(TIM3_CH1) , G: PA7(TIM3_CH2) , B: PB0(TIM3_CH3)
#define TIMER	TIM3

#define R_PIN	GPIO_Pin_6
#define R_PORT	GPIOA
#define R_CLK	RCC_AHB1Periph_GPIOA
#define R_TCLK	RCC_APB1Periph_TIM3
#define R_AF	GPIO_AF_TIM3
#define R_AF_P	GPIO_PinSource6

#define G_PIN	GPIO_Pin_7
#define G_PORT	GPIOA
#define G_CLK	RCC_AHB1Periph_GPIOA
#define G_TCLK	RCC_APB1Periph_TIM3
#define G_AF	GPIO_AF_TIM3
#define G_AF_P	GPIO_PinSource7

#define B_PIN	GPIO_Pin_0
#define B_PORT	GPIOB
#define B_CLK	RCC_AHB1Periph_GPIOB
#define B_TCLK	RCC_APB1Periph_TIM3
#define B_AF	GPIO_AF_TIM3
#define B_AF_P	GPIO_PinSource0

void led_init(void);
void led_set_color(uint8_t r, uint8_t g, uint8_t b);
void led_gradient_mode_thread(void *param);
void led_pinky_mode_thread(void *param);
void led_christmas_mode_thread(void *param);
void led_warning_mode_thread(void *param);

#endif // __RGB_LED_H__
