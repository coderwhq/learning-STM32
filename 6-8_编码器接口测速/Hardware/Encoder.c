#include "stm32f10x.h"                  // Device header

int16_t Encoder_Count;

void Encoder_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_ICInitTypeDef TIM_ICInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 1 - 1; // PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICStructInit(&TIM_ICInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	//TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 在编码器中这里的上升沿参数代表的是高低电平极性不反转。上升沿：不反相，下降沿：反相
	
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	//TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising; // 在编码器中这里的上升沿参数代表的是高低电平极性不反转。上升沿：不反相，下降沿：反相
	
	TIM_ICInit(TIM3, &TIM_ICInitStructure);
	
	// 配置编码器接口                 TIM_EncoderMode_TI12      TI1和TI2都计数        对应手册上的三个模式
	TIM_EncoderInterfaceConfig(TIM3, TIM_EncoderMode_TI12, TIM_ICPolarity_Rising, TIM_ICPolarity_Rising); //这里的上升沿也是不反相的意思
	
	
	TIM_Cmd(TIM3, ENABLE); // 启动定时器
}


int16_t Encouder_Get(void)
{
	int16_t Temp;
	Temp = TIM_GetCounter(TIM3);
	TIM_SetCounter(TIM3, 0); // 每次清0是为了测速
	return Temp;
	
}



