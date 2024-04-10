#include "stm32f10x.h"                  // Device header

/*
	OC����Ƚ� Output Compare ͨ���Ƚ�CNT��CCR(����ȽϼĴ���)ֵ�Ĺ�ϵ��
	���������ƽ������1����0��ת�Ĳ������������һ��Ƶ�ʺ�ռ�ձȵ�PWM����

	PWM �����ȵ��ƣ���һ�������ź�
	զ���� **���Ե�ϵͳ** �У�����ͨ����һϵ������Ŀ�Ƚ��е��ƣ�����Ч�Ļ������Ҫ��ģ���������Ӧ���ڵ�����ٵ�����
	Ƶ��= 1/Ts ռ�ձ�=Ton/Ts �ֱ��� = ռ�ձȱ仯����
	ռ�ձ�Խ�󣬵�Ч��ģ���ѹ��Խ�����ڸߵ�ƽ��ռ�ձ�ԽС����Ч��ģ���ѹ��Խ�����ڵ͵�ƽ 


	PWM Ƶ��  	Freq = CK_PSC / (PSC + 1) / (ARR + 1)     �ͼ��������Ƶ��һ����ÿ����0ʱ��������һ�����ڽ���
	PWM ռ�ձ�  Duty = CCR / (ARR + 1) (�ߵ�ƽռ��)
	PWM Ƶ��	Reso = 1 / (ARR + 1)                      ����ĺ�����Ҫ���ݽ��ռ�ձ��������������CCR����ռ�ձ�
*/
	

void PWM_Init(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	TIM_TimeBaseInitTypeDef TIM_TimeBaseInitStructure;
	TIM_OCInitTypeDef TIM_OCInitStructure;
	
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA, ENABLE);
	
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	
	TIM_TimeBaseInitStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseInitStructure.TIM_CounterMode = TIM_CounterMode_Up;
	/*
		��Ϊ���Ҫ��PWM����Ϊ 20ms  1Hz/20ms = 1Hz / 0.02s = 50Hz
	
		PWMƵ�� = 50Hz = 72MHz / 72Hz / 20000Hz
	
		��Ϊ���Ҫ��ߵ�ƽʱ�� 0.5ms--2.5ms
		
		500 / 20000 = 2.5%     ռ�ձ�
		2.5% * 20ms = 0.5ms    
		2500 / 20000 = 12.5%   ռ�ձ�
		12.5% * 20ms = 2.5ms
	
	*/
	TIM_TimeBaseInitStructure.TIM_Period = 20000 - 1; // ARR
	TIM_TimeBaseInitStructure.TIM_Prescaler = 72 - 1; // PSC
	TIM_TimeBaseInitStructure.TIM_RepetitionCounter = 0; 
	
	TIM_TimeBaseInit(TIM2, &TIM_TimeBaseInitStructure);
	
	TIM_OCStructInit(&TIM_OCInitStructure); 
	
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 500; // CCR
	
	TIM_OC2Init(TIM2, &TIM_OCInitStructure);
	
	TIM_Cmd(TIM2, ENABLE); // ������ʱ��
}


void PWM_SetCompare2(uint16_t Compare)
{
	TIM_SetCompare2(TIM2, Compare);
}


