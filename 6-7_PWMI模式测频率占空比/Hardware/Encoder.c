#include "stm32f10x.h"                  // Device header

int16_t Encoder_Count;

void Encoder_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line6 | EXTI_Line7;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; // 开启中断
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // 中断模式
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // 下降沿触发
	
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // 分组方式整个芯片只能用一种，所以整个项目只需要执行一次这个代码即可，可以放在主函数内执行
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	
//	NVIC_Init(&NVIC_InitStructure);
}


int16_t Encouder_Get(void)
{
	int16_t Temp;
	Temp = Encoder_Count;
	Encoder_Count = 0;
	return Temp;
	
}

// 正转 A先触发下降沿 相位差90后B在触发
// 反转 B先触发下降沿 相位差90后A在触发
void EXTI9_5_IRQHandler(void)
{
	// 当A开启中断时（下降沿触发）且B为低电平时，为反转（实际B先触发下降沿）
	if(EXTI_GetITStatus(EXTI_Line6) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0)
		{
			Encoder_Count --;
		}
		// 中断程序结束后一定要再调用一下清除中断标志位的函数
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	// 当B开启中断时（下降沿触发）且A为低电平时，为正转（实际A先触发下降沿）
	else if(EXTI_GetITStatus(EXTI_Line7) == SET) 
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
		{
			Encoder_Count ++;
		}
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}


