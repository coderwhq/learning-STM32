#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Count;

void CountSensor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	// EXTI �� NVIC(���ں˵�����) ��ʱ����һֱ�򿪵Ĳ���Ҫ���ǿ���,RCC������ں��������
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line14; // EXTI�ĵ�14����·
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; // �����ж�
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // �ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
	
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ���鷽ʽ����оƬֻ����һ�֣�����������Ŀֻ��Ҫִ��һ��������뼴�ɣ����Է�����������ִ��
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);

}

uint16_t CountSensor_Get(void)
{
	return CountSensor_Count;
}

/**
����жϺ��������ֶ����ã��������ж�ʱϵͳ���Զ����ô˺���
�жϺ��������޲��޷���ֵ��
��stm32��ÿ���жϺ������ǹ̶��ģ�ÿ���ж�ͨ������Ӧһ���жϺ������жϺ��������Բο������ļ�
*/
void EXTI15_10_IRQHandler(void)
{
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		CountSensor_Count ++;
		// �жϳ��������һ��Ҫ�ٵ���һ������жϱ�־λ�ĺ���
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}



