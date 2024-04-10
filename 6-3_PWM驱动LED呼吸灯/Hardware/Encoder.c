#include "stm32f10x.h"                  // Device header

int16_t Encoder_Count;

void Encoder_Init(void)
{

	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6 | GPIO_Pin_7;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource6);
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOA, GPIO_PinSource7);
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line6 | EXTI_Line7;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; // �����ж�
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // �ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
	
	EXTI_Init(&EXTI_InitStructure);
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ���鷽ʽ����оƬֻ����һ�֣�����������Ŀֻ��Ҫִ��һ��������뼴�ɣ����Է�����������ִ��
	
	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
//	NVIC_InitStructure.NVIC_IRQChannel = EXTI9_5_IRQn;
//	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
//	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
//	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 2;
//	
//	NVIC_Init(&NVIC_InitStructure);
}


int16_t Encouder_Get(void)
{
	int16_t Temp;
	Temp = Encoder_Count;
	Encoder_Count = 0;
	return Temp;
	
}

// ��ת A�ȴ����½��� ��λ��90��B�ڴ���
// ��ת B�ȴ����½��� ��λ��90��A�ڴ���
void EXTI9_5_IRQHandler(void)
{
	// ��A�����ж�ʱ���½��ش�������BΪ�͵�ƽʱ��Ϊ��ת��ʵ��B�ȴ����½��أ�
	if(EXTI_GetITStatus(EXTI_Line6) == SET)
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_7) == 0)
		{
			Encoder_Count --;
		}
		// �жϳ��������һ��Ҫ�ٵ���һ������жϱ�־λ�ĺ���
		EXTI_ClearITPendingBit(EXTI_Line6);
	}
	// ��B�����ж�ʱ���½��ش�������AΪ�͵�ƽʱ��Ϊ��ת��ʵ��A�ȴ����½��أ�
	else if(EXTI_GetITStatus(EXTI_Line7) == SET) 
	{
		if(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_6) == 0)
		{
			Encoder_Count ++;
		}
		EXTI_ClearITPendingBit(EXTI_Line7);
	}
}


