#include "stm32f10x.h"
#include "Encoder.h"

extern uint16_t Second;

void Timer_Init(void)
{
	
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructrue;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	
	// ѡ���ڲ�ʱ����Ϊʱ��Դ
	TIM_InternalClockConfig(TIM2);
	
	TIM_TimeBaseInitStructrue.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructrue.TIM_CounterMode = TIM_CounterMode_Up;
	// ���Ƶ�ʣ�72000000 / 7200 / 10000 = 1Hz/s һ�����һ��
	// ����Ƶ�ʣ�72000000 / 7200 = 10000Hz/s һ�����10000��
	TIM_TimeBaseInitStructrue.TIM_Period = 10000 - 1; // ARR
	TIM_TimeBaseInitStructrue.TIM_Prescaler = 7200 - 1; // PSC
	TIM_TimeBaseInitStructrue.TIM_RepetitionCounter = 0x00; // �߼���ʱ�����
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructrue);
	
	/* 
	ΪʲôҪ�����־λ��
		��ΪBaseInit �����һ�д���Ϊ�����������õķ�Ƶ��������Ч���ֶ�������һ�������¼�������Ԥ��Ƶ����ֵ����Ч��
		���Ǵ����˸����¼����ͻᴥ�������жϣ������жϻ��ø����жϱ�־λ������һ����ʼ�������Ժ󣬾ͻ����̽����ж��¼�
		�������һ�±�־λ����
	*/
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// ��ռ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	// ��Ӧ���ȼ�
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	TIM_Cmd(TIM2, ENABLE);
	
}


uint16_t Timer_GetCounter(void)
{
	return TIM_GetCounter(TIM2);
}
// Ϊ�����������ж���ʱע��
//void TIM2_IRQHandler(void)
//{
//	/*
//	// ���������
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//	{
//		Second ++;
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	}
//	*/
//}
