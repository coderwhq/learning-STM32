#include "stm32f10x.h"   // Device header

extern uint16_t Num; // extern 关键字意味着有个外部变量 Num，让编译器自动去其他文件里寻找

void Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	GPIO_InitTypeDef GPIO_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	// PA0 的复用功能中有 TIM2_CH1_ETR ，也就是PA0的高低电平变化会作为TIM2的外部时钟脉冲进入计数器
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	/*
	这些函数对应时基单元的时钟选择部分
		TIM_InternalClockConfig		选择内部时钟
		TIM_ITRxExternalClockConfig	选择ITRx作为时钟，用于时钟的联级，InternalTrigger 内部触发
		TIM_TIxExternalClockConfig	选择TIx捕获通道作为时钟，
		TIM_ETRClockMode1Config		选择ETR模式1
		TIM_ETRClockMode2Config		选择ETR模式2
		模式1是GPIO通过ETR直接到时基单元 模式2是GPIO通过ETR到内部触发引脚（TRGI）
		外部时钟模式1使用外部输入脚作为计数器时钟的时钟源，
		而外部时钟模式2使用外部触发输入或内部触发输入作为计数器时钟的时钟源
	*/
	// 选择外部时钟模式2
	TIM_ETRClockMode2Config(TIM2, TIM_ExtTRGPSC_OFF, TIM_ExtTRGPolarity_NonInverted, 0x0f); // 外部时钟
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 10 - 1;
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1;
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // 高级定时器才有
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);  // 初始化时基单元
	
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // 使能中断
	
	// 设置TIM2在NVIC的中断通道
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	
	TIM_Cmd(TIM2, ENABLE); // 启动定时器
	
}

uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
}

// TIM2 的中断函数
void TIM2_IRQHandler(void)
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Num ++;
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

