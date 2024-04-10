#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"
#include "OLED.h"
uint8_t KeyNum;

int main(void)
{
	
	OLED_Init();
	OLED_ShowChar(1, 1, 'A');
	OLED_ShowString(1, 3, "HelloWorld!");
	OLED_ShowNum(2, 1, 12345, 5);
	OLED_ShowSignedNum(2, 7, -66, 4);
	OLED_ShowHexNum(3, 1, 0xAA55, 4);
	OLED_ShowBinNum(4, 1, 0xAA55, 16);
//	OLED_ShowString(1, 6, "Hi XXY");
//	OLED_ShowString(2, 1, "I am a program");
//	OLED_ShowString(3, 1, "by WHQ coding.");
//	OLED_ShowString(4, 1, "I like you. (:");
	
	while(1)
	{

	}
	
	//Buzzer_OFF();
}
