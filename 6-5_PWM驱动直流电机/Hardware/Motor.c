#include "stm32f10x.h"
#include "PWM.h"

void Motor_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;

	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_4 | GPIO_Pin_5;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	PWM_Init();
	// Pin4 Pin5 ������������Pin2�������PWM����
	/*
	
		�������ģ��
	
		STBY�����������ʹ���״̬������ֱ�ӽ�3.3V
	
		
		1	  1     x     �ƶ�
		0     1     1     ��ת
		0     1     0     �ƶ�
		1     0     1     ��ת
		1     0     0     �ƶ�
		0	  0     x     ֹͣ
	
		PWM��һ�����Ϸ�ת�ĵ�Ƶ�źţ��ǵ���ͻ���ٵ� ת�� ֹͣ ת�� ֹͣ��
		ֻҪPWM��Ƶ���㹻�죬��ô����Ϳ����ȶ���ת���ˣ��ٶ�ȡ����PWM��ռ�ձȣ�PWM����Ϊ�ߵ�ƽ�ı�����
	
	*/
}

void Motor_SetSpeed(int8_t Speed)
{
	if(Speed >= 0)
	{
		GPIO_SetBits(GPIOA, GPIO_Pin_4);
		GPIO_ResetBits(GPIOA, GPIO_Pin_5);
		PWM_SetCompare3(Speed);
	}
	else
	{
		GPIO_ResetBits(GPIOA, GPIO_Pin_4);
		GPIO_SetBits(GPIOA, GPIO_Pin_5);
		PWM_SetCompare3(-Speed);
	}
}

