#include "stm32f10x.h"                  // Device header

void IC_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM3);
	// 在这个输入捕获的例子中，这个定时器的作用是用来测速的
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // PSC 决定了测周法的标准频率fc
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// OCInit是每个通道单独占一个函数，ICInit是四个通道共用一个函数
	TIM_ICInit(TIM3, &TIM_ICInitStructure); 
	
	// TIM_SelectOutputTrigger() // 选择输出模式触发源 TRGO
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1); // 选择输入模式触发源 TRGI
	// 当出现上升沿时自动清零，实现自动清0，实现硬件自动化 无需消耗软件资源
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset); // 选择从模式
	
	TIM_Cmd(TIM3, ENABLE);
	
}


uint32_t IC_GetFreq(void)
{
	// 1000 / 20 = 50Hz
	// 1000000 = 72M / PSC 计数器计数的频率(Hz/s)
	// 1/1000000 是1Hz的持续时间，时钟周期，当出现上升沿时，会把cnt的值锁存在CCR中，
	// 用CCR的值乘上这个时钟周期，就是输入频率时钟周期，再取个倒数就是输入频率
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}

uint32_t IC_GetCnt(void)
{
	return (TIM_GetCapture1(TIM3) + 1);
}

