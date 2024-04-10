#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "CountSensor.h"
#include "Encoder.h"
#include "Motor.h"

uint8_t KeyNum;

int8_t Speed ;

int main(void)
{
	
	OLED_Init();
	Key_Init();
	Motor_Init();
	
	OLED_ShowString(1, 1, "Speed:");
	
	//Motor_SetSpeed(-20);
	
	while(1)
	{
		KeyNum = Key_GetNum();
		if(KeyNum == 1)
		{
			Speed += 25;
			if(Speed > 100)
			{
				Speed = -100;
			}
		}
		else if(KeyNum == 2)
		{
			Speed = 0;
		}
		Motor_SetSpeed(Speed);
		
		OLED_ShowSignedNum(1, 7, Speed, 3);
		OLED_ShowNum(2, 1, TIM_GetCapture3(TIM2), 5);
	}
}

