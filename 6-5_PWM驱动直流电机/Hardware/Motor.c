#include "stm32f10x.h"
#include "PWM.h"

void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	PWM_Init();
	// Pin4 Pin5 控制正反方向，Pin2负责输出PWM波形
	/*
	
		电机驱动模块
	
		STBY是正常工作和待机状态，这里直接接3.3V
	
		
		1	  1     x     制动
		0     1     1     反转
		0     1     0     制动
		1     0     1     正转
		1     0     0     制动
		0	  0     x     停止
	
		PWM是一个不断反转的电频信号，那电机就会快速地 转动 停止 转动 停止，
		只要PWM的频率足够快，那么电机就可以稳定的转动了，速度取决于PWM的占空比（PWM波形为高电平的比例）
	
	*/
}

void Motor_SetSpeed(int8_t Speed)
{
	if(Speed >= 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		PWM_SetCompare3(Speed);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		PWM_SetCompare3(-Speed);
	}
}

