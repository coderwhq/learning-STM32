#include "stm32f10x.h"   // Device header

extern uint16_t Num; // extern �ؼ�����ζ���и��ⲿ���� Num���ñ������Զ�ȥ�����ļ���Ѱ��

/*
�ж� TIM  --->  NVIC  --->  CPU
ʱ������=Ƶ�ʵĵ���

����һ���������õ�ʱ�����**ʱ������**
һ��**��������**���ڶ��ʱ�����ڣ�ָ���һ�ξ���һ����������Ҫ�����ʱ��

72MHz  1s�ڿ��Է��� 72000000 �������źţ�1һ�������ź�Ϊ1Hz

72MHz��ʱ������Ϊ1s/72MHz = 1us/72Hz      us ��Ӧ M   ms ��Ӧ K

���������72����������Ҫ72�����壬�ķ�ʱ��Ϊ 1/72 * 72 = 1us

���������72000������ķ�ʱ��Ϊ1/72 * 72000 = 1ms



16�ļ�����CNT��Ԥ��Ƶ��PSC(16λ)���Զ���װ�Ĵ���ARR(16λ) ����һ���Ϊ ʱ����Ԫ

Ԥ��Ƶ�����Լ�������ʱ�ӽ��з�Ƶ���ü���������� 0һ��Ƶ 1����Ƶ
�Զ���װ�Ĵ����ǣ�������Ŀ��ֵ����������Ҫ�ƶ��ٸ�ʱ�������ж�

2^16 * 2^16 = �ɼ�¼�����������N��N / (ÿ���𵴴���)72MHz = ����ʱ 59.65s

 

CK_CNT�������������Ƶ��
CK_PSC������Ԥ��Ƶ����Ƶ��
PSC��Ԥ��Ƶ���Ĳ���
ARR���Զ���װ�Ĵ����Ĳ���

����������Ƶ�ʣ�CK_CNT = CK_PSC / (PSC + 1)

���������Ƶ�ʣ�CK_CNT_OV = CK_CNT / (ARR + 1) = CK_PSC / (PSC + 1) / (ARR + 1)
���Ƶ�ʣ�����Ƶ��/(��װĿ��ֵ+1) 
*/
void Timer_Init(void)
{
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	NVIC_InitTypeDef NVIC_InitStructure;
	/******* 1.��APB1�е�TIM2ʱ�� *******/
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	/******* 2.ѡ���ڲ�ʱ�� *******/
	TIM_InternalClockConfig(TIM2); // �ڲ�ʱ��
	/******* 3.����ʱ����Ԫ������ʱ���ж�ͨ�� *******/
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;  // ָ��ʱ�ӷ�Ƶ
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	/*
		����Ҫ֪����λ s �� Hz �����Ӧ��
		Ҫʵ��һ��һ����Ҳ���� 
		CK_PSC = 72MHz
		(ARR + 1) * (PSC + 1) = CK_PSC 
		1Hz = CS_PSC / (PSC + 1) / (ARR + 1)
		���磺
		TIM_Period��10000 - 1
		TIM_Prescaler��7200 - 1
		��⣺���ڲ�ʱ��ϵͳ��1���ڷ����� 72MHz ���� 7200��Ƶ�����������������Ƶ��CK_CNT����
		72MHz / 7200 = 10000Hz�����10000Hz��ζ��һ���ڻ���10000�������źŷ��͸���������
		Ҳ���Ǽ�������ִ��10000�μ�������ô�������Զ���װ�Ĵ���Ϊ 10000��
		Ҳ���ǵ��������ӵ�10000ʱ��ִ���жϺ�������������������� 1Hz ���� �����жϵ�Ƶ��
		���жϺ�����ִ��Second++�Ĳ����������Ϳ���ʵ��ÿһ���ӷ���һ���жϺ���
	*/
	TIM_TimeBaseInitStructure.TIM_Period = 10000 - 1; // �Զ���װ�Ĵ���
	TIM_TimeBaseInitStructure.TIM_Prescaler = 7200 - 1; // Ԥ��Ƶ��
	
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; // �߼���ʱ������
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);  // ��ʼ��ʱ����Ԫ
	/* ΪʲôҪ�����־λ��
		��ΪBaseInit �����һ�д���Ϊ�����������õķ�Ƶ��������Ч���ֶ�������һ�������¼�������Ԥ��Ƶ����ֵ����Ч��
		���Ǵ����˸����¼����ͻᴥ�������жϣ������жϻ��ø����жϱ�־λ������һ����ʼ�������Ժ󣬾ͻ����̽����ж��¼�
		�������һ�±�־λ����
	*/
	TIM_ClearFlag(TIM2, TIM_FLAG_Update);
	
	TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE); // ���ö�ʱ�����ж�������ƣ�ʹ�ܺ�ѡ��ͨ��
	
	/******* 4.����NVIC�ж�ͨ�� *******/
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
	NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
	
	NVIC_Init(&NVIC_InitStructure);
	
	/*******5.������ʱ��*******/
	TIM_Cmd(TIM2, ENABLE); // ����ʹ�ܼ�����
	
}


// TIM2 ���жϺ���
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//	{
//		Num ++;
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	}
//}

