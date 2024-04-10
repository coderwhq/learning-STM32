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
	OLED_ShowString(1, 1, "SYSCLK:");
	OLED_ShowNum(1, 8, SystemCoreClock, 8);
	
	while(1)
	{
		OLED_ShowString(2, 1, "Running");
		Delay_ms(500); // 因为delay函数内部默认使用的是72M主频，所以当把主频改为32M时，Delay就会翻一倍
		OLED_ShowString(2, 1, "       ");
		Delay_ms(500);
	}
	
	//Buzzer_OFF();
}
/*
PWR

PWR(Power Control)电源控制

PWR负责管理STM32内部的电源供电部分，可以实现可编程电压监测器和低功耗模式的功能

可编程电压监测器(PVD)可以监控VDD电源电压，当VDD下降到PVD阀值以下或上升到PVD阀值之上时，PVD会触发中断，用于执行紧急关闭任务

低功耗模式包括睡眠模式(Sleep)、停机模式(Stop)和待机模式(Standby)，可在系统空闲时，降低STM32的功耗，延长设备使用时间
*/

