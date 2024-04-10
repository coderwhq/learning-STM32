#include "stm32f10x.h"


void PWM_Init(void)
{
	// PA2 -->> TIM2_CH3
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	// ���ù����������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_2;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	/*
		���Ƶ�� 72000000 / (PSC + 1) / (ARR + 1)
		����Ƶ�� 72000000 / 7200 = 10000
		ֱ����� 32
		PWMI 720
	*/
	TIM_TimeBaseInitStructure.TIM_Period = 100 - 1; // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 720 - 1; // PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0;
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure); // �����кܶ�����ò��ϣ�����Ҫ���ã������������һ����ʼ��
	
	/**
		TIM_OCMode_Timing ����Ƚ�ʱ��ģʽ (������Ŷ�����Ч)
		TIM_OCMode_Active ����Ƚ�����ģʽ (ƥ��ʱ�����������Ϊ��Ч��ƽ��������ֵΪ�Ƚ�/����Ĵ���ֵ��ͬʱ��ǿ�����Ϊ�ߵ�ƽ)
		TIM_OCMode_Inactive ����ȽϷ�����ģʽ (ƥ��ʱ�����������Ϊ��Ч��ƽ��������ֵΪ�Ƚ�/����Ĵ���ֵ��ͬʱ��ǿ�����Ϊ�͵�ƽ)
		TIM_OCMode_Toggle ����ȽϷ�ת����ģʽ(������ֵ��Ƚ�/����Ĵ���ֵ��ͬʱ����ת������ŵĵ�ƽ)
		TIM_OCMode_PWM1 ���ϼ���ʱ����TIMx_CNT < TIMx_CCRʱ�������ƽ��Ч������Ϊ��Ч���¼���ʱ����TIMx_CNT > TIMx_CCRʱ�������ƽ��Ч������Ϊ��Ч��
		TIM_OCMode_PWM2 ���ϼ���ʱ����TIMx_CNT > TIMx_CCRʱ�������ƽ��Ч������Ϊ��Ч���¼���ʱ����TIMx_CNT < TIMx_CCRʱ�������ƽ��Ч������Ϊ��Ч��
	*/
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1; 
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;  // ����CCER�Ĵ������ߵ�ƽ��Ч�����Բ���ת
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	/*
		PWMƵ�� = ���Ƶ��
		ռ�ձ� CCR / (ARR + 1)
		
	*/
	TIM_OCInitStructure.TIM_Pulse = 50; // CCR   ֱ����� 0  PWMI 50
	
	TIM_OC3Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	
}
// ���ͨ������CCR��ֵ���ı�PWM��ռ�ձ�
void PWM_SetCompareRegister(uint16_t Compare)
{
	TIM_SetCompare3(TIM2, Compare);
}



