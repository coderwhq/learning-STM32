#include "stm32f10x.h"                  // Device header
#include "LED.h"
void LED_Init(void) 
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(LED_CLOCK, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = LED_R_PIN | LED_G_PIN | LED_B_PIN;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(LED_GPIO, &GPIO_InitStructure);
	
	GPIO_SetBits(LED_GPIO, LED_R_PIN | LED_G_PIN | LED_B_PIN);
	
}
// ��
void LED_R_ON(void) 
{
	GPIO_ResetBits(LED_GPIO, LED_R_PIN);
}

void LED_R_OFF(void) 
{
	GPIO_SetBits(LED_GPIO, LED_R_PIN);
}

// ��ת
void LED_R_Turn(void)
{
	// �������һ���������ģʽ�£���ȡ��ǰ�����ֵ
	if(GPIO_ReadOutputDataBit(LED_GPIO, LED_R_PIN) == 0)
	{
		GPIO_SetBits(LED_GPIO, LED_R_PIN);
	}
	else
	{
		GPIO_ResetBits(LED_GPIO, LED_R_PIN);
	}
}

// ��
void LED_G_ON(void) 
{
	GPIO_ResetBits(LED_GPIO, LED_G_PIN);
}

void LED_G_OFF(void) 
{
	GPIO_SetBits(LED_GPIO, LED_G_PIN);
}
// ��ת
void LED_G_Turn(void)
{
	if(GPIO_ReadOutputDataBit(LED_GPIO, LED_G_PIN) == 0)
	{
		GPIO_SetBits(LED_GPIO, LED_G_PIN);
	}
	else
	{
		GPIO_ResetBits(LED_GPIO, LED_G_PIN);
	}
}


// ��


void LED_B_ON(void) 
{
	GPIO_ResetBits(LED_GPIO, LED_B_PIN);
}

void LED_B_OFF(void) 
{
	GPIO_SetBits(LED_GPIO, LED_B_PIN);
}
// ��ת
void LED_B_Turn(void)
{
	if(GPIO_ReadOutputDataBit(LED_GPIO, LED_B_PIN) == 0)
	{
		GPIO_SetBits(LED_GPIO, LED_B_PIN);
	}
	else
	{
		GPIO_ResetBits(LED_GPIO, LED_B_PIN);
	}
}
