#include "stm32f10x.h"                  // Device header
#include "Delay.h"



void Key_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure; 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // 默认高电平
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1 | GPIO_Pin_11;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	
}

uint8_t Key_GetNum(void)
{
	uint8_t KeyNum = 0;
	// 要注意考虑 按下不松开 和 防抖 
	// 读取 PB0 输入的电平
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0)
	{
		Delay_ms(20); // 按下防抖
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_1) == 0); // 长按不松开
		Delay_ms(20); // 松开防抖
		KeyNum = 1;
	}
	if(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0)
	{
		Delay_ms(20); // 按下防抖
		while(GPIO_ReadInputDataBit(GPIOB, GPIO_Pin_11) == 0);
		Delay_ms(20); // 松开防抖
		KeyNum = 2;
	}
	return KeyNum;
}

