#include "stm32f10x.h"


void PWM_Init(void)
{
	// PA2 -->> TIM2_CH3
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// 复用功能推挽输出
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	/*
		溢出频率 72000000 / (PSC + 1) / (ARR + 1)
		计数频率 72000000 / 7200 = 10000
		直流电机 32
		PWMI 720
	*/
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure); // 由于有很多参数用不上，不需要配置，所以这里进行一个初始化
	
	/**
		TIM_OCMode_Timing 输出比较时间模式 (输出引脚冻结无效)
		TIM_OCMode_Active 输出比较主动模式 (匹配时设置输出引脚为有效电平，当计数值为比较/捕获寄存器值相同时，强制输出为高电平)
		TIM_OCMode_Inactive 输出比较非主动模式 (匹配时设置输出引脚为无效电平，当计数值为比较/捕获寄存器值相同时，强制输出为低电平)
		TIM_OCMode_Toggle 输出比较翻转触发模式(当计数值与比较/捕获寄存器值相同时，翻转输出引脚的电平)
		TIM_OCMode_PWM1 向上计数时，当TIMx_CNT < TIMx_CCR时，输出电平有效，否则为无效向下计数时，当TIMx_CNT > TIMx_CCR时，输出电平无效，否则为有效。
		TIM_OCMode_PWM2 向上计数时，当TIMx_CNT > TIMx_CCR时，输出电平有效，否则为无效向下计数时，当TIMx_CNT < TIMx_CCR时，输出电平无效，否则为有效。
	*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  // 配置CCER寄存器，高电平有效，极性不反转
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/*
		PWM频率 = 溢出频率
		占空比 CCR / (ARR + 1)
		
	*/
	TIM_OCInitStructure.TIM_Pulse = 50; // CCR   直流电机 0  PWMI 50
	
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	
}
// 外界通过控制CCR的值来改变PWM的占空比
void PWM_SetCompareRegister(uint16_t Compare)
{
	TIM_SetCompare3(TIM2, Compare);
}



