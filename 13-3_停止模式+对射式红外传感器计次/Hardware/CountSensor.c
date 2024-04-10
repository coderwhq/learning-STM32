#include "stm32f10x.h"                  // Device header

uint16_t CountSensor_Count;

void CountSensor_Init(void)
{
	
	/*
		GPIOx_Pin_(1-16)����   --->   AFIO(ѡ������)   --->   EXTI    --->    NVIC
	*/
	
	GPIO_InitTypeDef GPIO_InitStructure;
	EXTI_InitTypeDef EXTI_InitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	
	// EXTI �� NVIC(���ں˵�����) ��ʱ����һֱ�򿪵Ĳ���Ҫ���ǿ���,RCC��������ں��������
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
	
	/* 
		AFIO �ж�����ѡ�񣬾���һ������ѡ����
		������Pin������ͬ��PA0 PB0 PC0....�����ڶ�������ѡ������һ�����ӵ������EXTIͨ�� 
	
		��STM32��AFIO��Ҫ����������񣺸��ù���������ӳ�䡢�ж�����ѡ��
	*/
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_14;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	
	// �����ڲ��ڲ���AFIO
	GPIO_EXTILineConfig(GPIO_PortSourceGPIOB, GPIO_PinSource14);
	
	/*
		EXTI�ⲿ�ж� Extern Interrupt ���ؼ�⼰����
		EXTI���Լ��ָ��GPIO�ڵĵ�ƽ�źţ�����ָ����GPIO�ڲ�����ƽ�仯ʱ��
		EXTI�����ж����룬��NVIC�þ��󼴿��ж�CPU������ʹCPUִ��EXTI��Ӧ���жϳ���
		
		EXTI֧�ִ������ŵ�ƽ�ı仯�ķ�ʽΪ��������/�½���/˫����/�������(����ִ��һ�����Ϳ��Դ����ж�)
		
		֧�����е�GPIO�ڣ�Pin������ͬ�Ĳ���ͬʱ�����жϣ�����PA0 PB0 PC0����ͬʱ����
		
		���룺16��GPIO_Pin��PVD�����RTC���ӡ�USB���ѡ���̫�����ѣ���20���ж���·
		�����11��ͨ��NVIC(EXTI0��1��2��3��4��EXTI9_5��EXTI15_10��PVD��RTC��USB����̫��)��20��ͨ����������(�¼���Ӧ)
		
		������Ӧ�ķ�ʽ���ж���Ӧ��ͨ��CPU��/�¼���Ӧ�����ᴥ���жϣ�������������������������֮������Ϲ�����
	*/
	
	EXTI_InitStructure.EXTI_Line = EXTI_Line14; // EXTI�ĵ�14����·
	EXTI_InitStructure.EXTI_LineCmd = ENABLE; // ����
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt; // �ж�ģʽ
	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling; // �½��ش���
	
	EXTI_Init(&EXTI_InitStructure);
	
	
	/*
		NVICǶ���ж�����������������ͳһ�����ж����ȼ��͹����жϣ���һ���ں����裬cpu������
		NVIC�кܶ������ڣ����Խ���EXTI��TIM��ADC��USART������һ�����軹���ܻ�ͬʱռ�ö���ж�ͨ����
		����������EXTIռ���� 11 ���ж�ͨ��
		NVICֻ��һ�������ͨ��CPU
	*/
	
	
	
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); // ���鷽ʽ����оƬֻ����һ�֣�����������Ŀֻ��Ҫִ��һ��������뼴�ɣ����Է�����������ִ��
	// ����ͨ����ʹ�ܻ�ʧ�ܺ�ͨ�������ȼ�
	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	// ��ռ���ȼ��ߵĿ����ж�Ƕ�ף���Ӧ���ȼ��ߵĿ��������Ŷӣ�����ͬ�İ����жϺ��Ŷӣ�
	// PreemptionPriority��ռ���ȼ���SubPriority��Ӧ���ȼ�
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
���ƻص�����
*/
void EXTI15_10_IRQHandler(void)
{
	// �жϺ����в�Ҫִ�к�ʱ�����Ĵ��룬Ҳ��ò�Ҫ����Ӳ�� �� ��Ҫ��������������ͬ�ĺ���
	if(EXTI_GetITStatus(EXTI_Line14) == SET)
	{
		CountSensor_Count ++;
		// �жϳ��������һ��Ҫ�ٵ���һ������жϱ�־λ�ĺ���
		EXTI_ClearITPendingBit(EXTI_Line14);
	}
}



