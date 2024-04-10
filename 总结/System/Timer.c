#include "stm32f10x.h"
#include "Encoder.h"

extern uint16_t Second;

void Timer_Init(void)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructrue;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// 选择内部时钟作为时钟源
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitStructrue.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructrue.TIM_CounterMode = TIM_CounterMode_Up;
	// 溢出频率：72000000 / 7200 / 10000 = 1Hz/s 一秒溢出一次
	// 计数频率：72000000 / 7200 = 10000Hz/s 一秒计数10000次
	TIM_TimeBaseInitStructrue.TIM_Period = 10000 - 1; // ARR
	TIM_TimeBaseInitStructrue.TIM_Prescaler = 7200 - 1; // PSC
	TIM_TimeBaseInitStructrue.TIM_RepetitionCounter = 0x00; // 高级定时器里的
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructrue);
	
	/* 
	为什么要清除标志位？
		因为BaseInit 的最后一行代码为了让我们设置的分频器立刻生效，手动生成了一个更新事件，这样预分频器的值就有效了
		但是触发了更新事件，就会触发更新中断，更新中断会置更新中断标志位，所以一旦初始化完了以后，就会立刻进入中断事件
		所以清除一下标志位就行
	*/
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// 抢占优先级
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// 响应优先级
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	
}


uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
}
// 为其他程序运行而暂时注释
//void TIM2_IRQHandler(void)
//{
//	/*
//	// 本程序代码
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//	{
//		Second ++;
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	}
//	*/
//}
