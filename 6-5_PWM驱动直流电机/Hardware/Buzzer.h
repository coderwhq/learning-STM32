#ifndef __BUZZER_H
#define __BUZZER_H

#define Buzzer_CLOCK	RCC_APB2Periph_GPIOB
#define Buzzer_GPIO		GPIOB
#define Buzzer_PIN		GPIO_Pin_12

void Buzzer_Init(void);
void Buzzer_ON(void);
void Buzzer_OFF(void);
void Buzzer_Turn(void);

#endif

