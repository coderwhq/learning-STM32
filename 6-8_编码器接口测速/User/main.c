#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "CountSensor.h"
#include "Encoder.h"
#include "Timer.h"

int16_t Speed;


/*
Encoder Interface �������ӿ�
�������ӿڿɽ�������(����)���������źţ�
���ݱ�������ת�����������ź����壬�Զ�����CNT�������Լ����Ӷ�ָʾ��������λ�á���ת�������ת�ٶ�
ÿ���߼���ʱ����ͨ�ö�ʱ����ӵ��1���������ӿ�
�����������Ž��������벶���ͨ��1��ͨ��2


����ʵ�ֵ���ʵ���� ��Ƶ��

Tim2 ÿһ�����һ�������ж�
�жϺ���ÿһ���ȡ��Tim3�У��ɱ��������Ƽ�����cnt��ֵ�����ֵʵ���Ͼ��������źŵ�Ƶ��N(Hz/s)

*/

int main(void)
{
	
	OLED_Init();
	Timer_Init();
	Encoder_Init();
	
	OLED_ShowString(1, 1, "Speed:");
	// ģ�����������
	while(1)
	{
		OLED_ShowSignedNum(1, 7, Speed, 5);
		//Delay_ms(1000);
	}
}
// ÿ��һ�봥���ڲ��жϲ���
void TIM2_IRQHandler(void) 
{
	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
	{
		Speed = Encouder_Get();
		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
	}
}

