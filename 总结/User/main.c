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
#include "Serial.h"

uint8_t KeyNum;
uint8_t State;
uint16_t Second;
uint16_t Count;
int16_t Speed;

int main(void)
{
	// 配置优先级分组，整个程序共用同一种模式
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);
	
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
	Serial_Init();
	Serial_TxPacket[0] = 0x10;
	Serial_TxPacket[1] = 0x20;
	Serial_TxPacket[2] = 0x30;
	Serial_TxPacket[3] = 0x40;
	while(1)
	{
	
	/*******************按键控制LED*********************/	
		/* 
		//这样写是错误的，能否点灯关灯看运气，看你的程序运行到了哪里，稍微分析一下就看出来了
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
		// 这样就对了
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
	/*******************按键控制LED*********************/		
	
	
	/*******************光敏传感器控制LED*********************/	
		/*
		// PA1需要接入光敏传感器的DO口（数字输出），而非AO口（模拟输出）
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
		
	/*******************光敏传感器控制LED*********************/	
	
	
	/*******************EXTI外部中断*********************/
		/*
		OLED_ShowNum(1, 1, CountSensor_GetCount(), 2);
		*/
	/*******************EXTI外部中断*********************/
	
	
	/*******************定时器定时中断*********************/
		/*
		OLED_ShowNum(1, 1, Timer_GetCounter(), 5);
		OLED_ShowNum(2, 1, Second, 5);
		*/
	/*******************定时器定时中断*********************/
	
	
	/*******************定时器外部时钟*********************/
		/*
		OLED_ShowNum(1, 1, EXT_Timer_GetCounter(), 5); // 计数器
		OLED_ShowNum(2, 1, Count, 5); // 计数器
		*/
	/*******************定时器外部时钟*********************/
	
	
	/*******************PWM驱动直流电机*********************/
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
	/*******************PWM驱动直流电机*********************/
	
	
	/*******************PWMI模式测频率占空比*********************/
		/*
		OLED_ShowNum(1, 1, IC_GetFreq(), 5);
		OLED_ShowNum(2, 1, IC_GetDuty(), 5);
		*/
	/*******************PWMI模式测频率占空比*********************/
	
	
	/*******************编码器*********************/
		/*
		OLED_ShowSignedNum(1, 1, Speed, 5);
		*/
	/*******************编码器*********************/
	
	
	/*******************DMA配合AD多通道*********************/
		/*
		// 单次转换扫描模式, 单次转运DMA，实现AD多通道触发，需要手动触发
		AD_GetValue();
		
		
		OLED_ShowNum(1, 1, AD_Value[0], 4);
		OLED_ShowNum(2, 1, AD_Value[1], 4);
		OLED_ShowNum(3, 1, AD_Value[2], 4);
		OLED_ShowNum(4, 1, AD_Value[3], 4);
		Delay_ms(100);
		*/
	/*******************DMA配合AD多通道*********************/
	
	
	/*******************串口通讯*********************/
	/*
	KeyNum = Key_GetNum();
	if(KeyNum == 1)
	{
		// 发送数字测试
		Serial_SendDigitalPacket();
		Serial_TxPacket[0] ++;
		Serial_TxPacket[1] ++;
		Serial_TxPacket[2] ++;
		Serial_TxPacket[3] ++;
		
		Serial_SendString("nihaoa,Captain\r\n");
		printf("Num = %d\r\n", 666); // 需要再工程选项中打开 USE MicroLIB
	}
	if(Serial_GetReceiveFlag() == 1)
	{
		// 接收数字测试
		OLED_ShowHexNum(1, 1, Serial_RxPacket[0], 2);
		OLED_ShowHexNum(1, 4, Serial_RxPacket[1], 2);
		OLED_ShowHexNum(1, 7, Serial_RxPacket[2], 2);
		OLED_ShowHexNum(1, 10, Serial_RxPacket[3], 2);
		
		// 不需要自己清零标志位
		OLED_ShowString(1, 1, Serial_RxString);
	}
	*/
	/*******************串口通讯*********************/
	
	
	
	
	
	/*******************软件I2C通讯*********************/
	
	/*******************软件I2C通讯*********************/
	
	
	
	
	
	
	/*******************软件SPI通讯*********************/
	
	/*******************软件SPI通讯*********************/
	
	
	
	}
	
}

// 编码器  TIM2中断代码
//void TIM2_IRQHandler(void)
//{
//	if(TIM_GetITStatus(TIM2, TIM_IT_Update) == SET)
//	{
//		Speed = Encoder_Get();
//		TIM_ClearITPendingBit(TIM2, TIM_IT_Update);
//	}
//}


