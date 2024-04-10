#ifndef __LED_H
#define __LED_H

#define LED_CLOCK	RCC_APB2Periph_GPIOA
#define LED_GPIO	GPIOA
#define LED_R_PIN	GPIO_Pin_0
#define LED_G_PIN	GPIO_Pin_1
#define LED_B_PIN	GPIO_Pin_2

void LED_Init(void);
void LED_R_ON(void);
void LED_R_OFF(void);
void LED_R_Turn(void);

void LED_G_ON(void);
void LED_G_OFF(void);
void LED_G_Turn(void);

void LED_B_ON(void);
void LED_B_OFF(void);
void LED_B_Turn(void);

#endif

