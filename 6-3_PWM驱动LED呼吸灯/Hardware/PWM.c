#include "stm32f10x.h"                  // Device header

/*
	OC����Ƚ� Output Compare ͨ���Ƚ�CNT��CCR(����ȽϼĴ���)ֵ�Ĺ�ϵ��
	���������ƽ������1����0��ת�Ĳ������������һ��Ƶ�ʺ�ռ�ձȵ�PWM����

	PWM �����ȵ��ƣ���һ�������ź�
	�ھ��� **���Ե�ϵͳ** �У�����ͨ����һϵ������Ŀ�Ƚ��е��ƣ�����Ч�Ļ������Ҫ��ģ���������Ӧ���ڵ�����ٵ�����
	Ƶ�� = 1/T s ռ�ձ�  = Ton/T s    �ֱ��� = ռ�ձȱ仯����
	ռ�ձ�Խ�󣬵�Ч��ģ���ѹ��Խ�����ڸߵ�ƽ��ռ�ձ�ԽС����Ч��ģ���ѹ��Խ�����ڵ͵�ƽ 


	PWM Ƶ��  	Freq = CK_PSC / (PSC + 1) / (ARR + 1)     �ͼ��������Ƶ��һ����ÿ����0ʱ��������һ�����ڽ���
	PWM ռ�ձ�  Duty = CCR / (ARR + 1) (�ߵ�ƽռ��)
	PWM �ֱ���	Reso = 1 / (ARR + 1)                      ����ĺ�����Ҫ���ݽ��ռ�ձ��������������CCR����ռ�ձ�
*/
	

void PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	// ������ӳ�񣬲�Ҫ����ʹ��
//	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
//	GPIO_PinRemapConfig(GPIO_PartialRemap1_TIM2, ENABLE);
//	GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable, ENABLE);
	
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
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; // ����Ƚ�ģʽ
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High; // �߼��ԣ����Բ���ת����Ч��ƽ�Ǹߵ�ƽ��REF��Чʱ����ߵ�ƽ����֮����
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 10; // ����CCR
	
	TIM_OC1Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE); // ������ʱ��
}


void PWM_SetCompare1(uint16_t Compare) 
{
	TIM_SetCompare1(TIM2, Compare); // ���� CCR
}


