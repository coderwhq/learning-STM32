#include "stm32f10x.h"                  // Device header
#include "Delay.h"
#include "LED.h"
#include "Key.h"
#include "Buzzer.h"
#include "LightSensor.h"

uint8_t KeyNum;

int main(void)
{
	LED_Init();
	Buzzer_Init();
	LightSensorInit();

	
	while(1)
	{
		// 为什么在这里使用两个外设的turn函数都无法实现功能
		
		// 光敏电阻传感器内部设置了上拉模式，所以端口初始为1，光敏电阻有光时，电阻为0，输入为0
		// 当光暗时为0，输出高电平，这是传感器自带上拉电阻的高电平。光亮时为1，输出低电平，传感器内部mos管打开，引脚接低电平输出为0
		if(LightSensor_Get() == 1) // 高电平，当光源被遮挡时，亮灯开启蜂鸣器
		{
//			Buzzer_Turn();
//			LED_R_Turn();
			LED_G_ON();
//			Buzzer_ON();
		}
		else
		{
//			Buzzer_Turn(); 
//			LED_R_Turn();
			LED_G_OFF();
//			Buzzer_OFF();
		}
	}
	
	//Buzzer_OFF();
}
