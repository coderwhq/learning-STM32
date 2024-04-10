#ifndef __KEY_H
#define __KEY_H
#include "stm32f10x.h"

#define KEY_GPIO_CLK	RCC_APB2Periph_GPIOB

#define KEY_GPIO_PORT	GPIOB

#define KEY1_GPIO_PIN	GPIO_Pin_1
#define KEY2_GPIO_PIN	GPIO_Pin_11

#define KEY_OFF 1
#define KEY_ON 0

void Key_Init(void);
uint8_t Key_GetNum(void);

#endif
