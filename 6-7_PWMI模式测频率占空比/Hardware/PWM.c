#include "stm32f10x.h"                  // Device header

/*
	OC输出比较 Output Compare 通过比较CNT与CCR(捕获比较寄存器)值的关系，
	来对输出电平进行置1、置0或翻转的操作，用于输出一定频率和占空比的PWM波形

	PWM 脉冲宽度调制，是一个数字信号
	咋具有 **惯性的系统** 中，可以通过对一系列脉冲的宽度进行调制，来等效的获得所需要的模拟参量，常应用于电机控速等领域
	频率= 1/Ts 占空比=Ton/Ts 分辨率 = 占空比变化步距
	占空比越大，等效的模拟电压就越趋近于高电平，占空比越小，等效的模拟电压就越趋近于低电平 


	PWM 频率  	Freq = CK_PSC / (PSC + 1) / (ARR + 1)
	PWM 占空比  Duty = CCR / (ARR + 1) (高电平占比)
	PWM 频率	Reso = 1 / (ARR + 1)
*/
	

void PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure); 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 10; // CCR
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE); // 启动定时器
}


void PWM_SetCompare1(uint16_t Compare) 
{
	TIM_SetCompare1(TIM2, Compare); // 设置 CCR
}

void PWM_SetPrescaler(uint16_t Prescaler)
{
	TIM_PrescalerConfig(TIM2, Prescaler, TIM_PSCReloadMode_Update);
}


