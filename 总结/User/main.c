#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "OLED.h"
#include "Key.h"
#include "LED.h"
#include "LightSensor.h"
#include "CountSensor.h"
#include "Timer.h"
#include "EXT_Timer.h"
#include "Motor.h"
#include "PWM.h"
#include "IC.h"
#include "Encoder.h"
#include "AD.h"

uint8_t KeyNum;
uint8_t State;
uint16_t Second;
uint16_t Count;
int16_t Speed;

int main(void)
{
	OLED_Init();
	Key_Init();
	//LED_Init();
	//LightSensor_Init();
	//CountSensor_Init();
	//Timer_Init();
	//EXT_Timer_Init();
	//Motor_Init();
	//PWM_Init();
	//IC_Init();
	//Encoder_Init();
	//AD_Init();
	
	while(1)
	{
		
	/*******************��������LED*********************/	
		/* 
		//����д�Ǵ���ģ��ܷ��ƹصƿ�����������ĳ������е��������΢����һ�¾Ϳ�������
		if(Key_GetNum() == 1)
		{
			LED_Turn_On();
		}
		if(Key_GetNum() == 2)
		{
			LED_Turn_Off();
		}
		*/
		
		/*
		// �����Ͷ���
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			//LED_Turn_On();
			LED_Trans();
		}
		if(KeyNum == 2)
		{
			//LED_Turn_Off();
			LED_Trans();
		}
		*/
	/*******************��������LED*********************/		
	
	
	/*******************��������������LED*********************/	
		/*
		// PA1��Ҫ���������������DO�ڣ����������������AO�ڣ�ģ�������
		State = LightSensor_GetState();
		if(State == 1)
		{
			LED_Turn_On();
		}
		if(State == 0)
		{
			LED_Turn_Off();
		}
		OLED_ShowNum(1, 1, State, 5);
		*/
		
	/*******************��������������LED*********************/	
	
	
	/*******************EXTI�ⲿ�ж�*********************/
		/*
		OLED_ShowNum(1, 1, CountSensor_GetCount(), 2);
		*/
	/*******************EXTI�ⲿ�ж�*********************/
	
	
	/*******************��ʱ����ʱ�ж�*********************/
		/*
		OLED_ShowNum(1, 1, Timer_GetCounter(), 5);
		OLED_ShowNum(2, 1, Second, 5);
		*/
	/*******************��ʱ����ʱ�ж�*********************/
	
	
	/*******************��ʱ���ⲿʱ��*********************/
		/*
		OLED_ShowNum(1, 1, EXT_Timer_GetCounter(), 5); // ������
		OLED_ShowNum(2, 1, Count, 5); // ������
		*/
	/*******************��ʱ���ⲿʱ��*********************/
	
	
	/*******************PWM����ֱ�����*********************/
		/*
		KeyNum = Key_GetNum();
		
		if(KeyNum == 1)
		{
			Speed += 25;
			if(Speed > 100)
			{
				Speed = -100;
			}
		}
		if(KeyNum == 2)
		{
			Speed = 0;
		}
		Motor_SetSpeed(Speed);
		
		OLED_ShowSignedNum(1, 1, Speed, 5);
		OLED_ShowNum(2, 1, TIM_GetCapture3(TIM2), 5);
		*/
	/*******************PWM����ֱ�����*********************/
	
	
	/*******************PWMIģʽ��Ƶ��ռ�ձ�*********************/
		/*
		OLED_ShowNum(1, 1, IC_GetFreq(), 5);
		OLED_ShowNum(2, 1, IC_GetDuty(), 5);
		*/
	/*******************PWMIģʽ��Ƶ��ռ�ձ�*********************/
	
	
	/*******************������*********************/
		/*
		OLED_ShowSignedNum(1, 1, Speed, 5);
		*/
	/*******************������*********************/
	
	
	/*******************DMA���AD��ͨ��*********************/
		/*
		// ����ת��ɨ��ģʽ, ����ת��DMA��ʵ��AD��ͨ����������Ҫ�ֶ�����
		AD_GetValue();
		
		
		OLED_ShowNum(1, 1, AD_Value[0], 4);
		OLED_ShowNum(2, 1, AD_Value[1], 4);
		OLED_ShowNum(3, 1, AD_Value[2], 4);
		OLED_ShowNum(4, 1, AD_Value[3], 4);
		Delay_ms(100);
		*/
	/*******************DMA���AD��ͨ��*********************/
	
	
	
	
	
	
	/*******************����ͨѶ*********************/
	
	/*******************����ͨѶ*********************/
	
	
	
	
	
	/*******************���I2CͨѶ*********************/
	
	/*******************���I2CͨѶ*********************/
	
	
	
	
	
	
	/*******************���SPIͨѶ*********************/
	
	/*******************���SPIͨѶ*********************/
	
	
	
	}
	
}

// ������  TIM2�жϴ���
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//	{
//		Speed = Encoder_Get();
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	}
//}


