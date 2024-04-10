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
	// ��������벶��������У������ʱ�����������������ٵ�
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInitStructure.TIM_Period = 65536 - 1; // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // PSC �����˲��ܷ��ı�׼Ƶ��fc
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseInitStructure);
	
	TIM_ICInitStructure.TIM_Channel = TIM_Channel_1;
	TIM_ICInitStructure.TIM_ICFilter = 0xF;
	TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
	TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
	TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
	// OCInit��ÿ��ͨ������ռһ��������ICInit���ĸ�ͨ������һ������
	TIM_ICInit(TIM3, &TIM_ICInitStructure); 
	
	// TIM_SelectOutputTrigger() // ѡ�����ģʽ����Դ TRGO
	TIM_SelectInputTrigger(TIM3, TIM_TS_TI1FP1); // ѡ������ģʽ����Դ TRGI
	// ������������ʱ�Զ����㣬ʵ���Զ���0��ʵ��Ӳ���Զ��� �������������Դ
	TIM_SelectSlaveMode(TIM3, TIM_SlaveMode_Reset); // ѡ���ģʽ
	
	TIM_Cmd(TIM3, ENABLE);
	
}


uint32_t IC_GetFreq(void)
{
	// 1000 / 20 = 50Hz
	// 1000000 = 72M / PSC ������������Ƶ��(Hz/s)
	// 1/1000000 ��1Hz�ĳ���ʱ�䣬ʱ�����ڣ�������������ʱ�����cnt��ֵ������CCR�У�
	// ��CCR��ֵ�������ʱ�����ڣ���������Ƶ��ʱ�����ڣ���ȡ��������������Ƶ��
	return 1000000 / (TIM_GetCapture1(TIM3) + 1);
}

uint32_t IC_GetCnt(void)
{
	return (TIM_GetCapture1(TIM3) + 1);
}

