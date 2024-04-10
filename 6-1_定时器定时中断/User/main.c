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

uint8_t KeyNum;

uint16_t Num;

int main(void)
{
	
	OLED_Init();
	Timer_Init();
	
	OLED_ShowString(1, 1, "NUM:");
	
	while(1)
	{
		OLED_ShowNum(1, 5, Num, 5);
		OLED_ShowNum(2, 5, TIM_GetCounter(TIM2), 5);
	}
}
