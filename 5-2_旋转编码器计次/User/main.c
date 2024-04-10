#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
#include "CountSensor.h"
#include "Encoder.h"
uint8_t KeyNum;

int16_t Num;

int main(void)
{
	
	OLED_Init();
	Encoder_Init();
	
	OLED_ShowString(1, 1, "Num:");
	
	while(1)
	{
		Num += Encouder_Get();
		OLED_ShowSignedNum(1, 5, Num, 5);
	}
	
	//Buzzer_OFF();
}
