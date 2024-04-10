#include "stm32f10x.h"
#include "Buzzer.h"

void Buzzer_Init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(Buzzer_CLOCK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = Buzzer_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(Buzzer_GPIO, &GPIO_InitStructure);
	
	GPIO_SetBits(Buzzer_GPIO, Buzzer_PIN);
	
}

void Buzzer_ON(void) 
{
	GPIO_ResetBits(Buzzer_GPIO, Buzzer_PIN);
}

void Buzzer_OFF(void) 
{
	GPIO_SetBits(Buzzer_GPIO, Buzzer_PIN);
}

void Buzzer_Turn(void)
{
	if(GPIO_ReadOutputDataBit(Buzzer_GPIO, Buzzer_PIN) == 0)
	{
		GPIO_SetBits(Buzzer_GPIO, Buzzer_PIN);
	}
	else
	{
		GPIO_ResetBits(Buzzer_GPIO, Buzzer_PIN);
	}
}

