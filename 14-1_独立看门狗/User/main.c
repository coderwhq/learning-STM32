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
	// 如果独立看门狗已经由硬件选项或软件启动，LSI振荡器将被强制在打开状态，并且不能被关闭。在LSI振荡器稳定后，时钟供应给IWDG。
	OLED_Init();
	
	OLED_ShowString(1, 1, "IWDG TEST");
	
	if(RCC_GetFlagStatus(RCC_FLAG_IWDGRST) == SET)
	{
		// 独立看门狗触发复位
		
		OLED_ShowString(2, 1, "IWDGRST");
		Delay_ms(500);
		OLED_ShowString(2, 1, "       ");
		Delay_ms(100);
		
		RCC_ClearFlag();
	}
	else
	{
		// 按键触发复位
		OLED_ShowString(3, 1, "RST");
		Delay_ms(500);
		OLED_ShowString(3, 1, "   ");
		Delay_ms(100);
	}
	
	IWDG_WriteAccessCmd(IWDG_WriteAccess_Enable); // 解除写保护
	// 超时时间1s		Tiwdg = Tlsi * PR预分频系数 * (RL + 1)			Tlsi = 1 / Flsi = 1 / 40KHz = 0.025ms
	IWDG_SetPrescaler(IWDG_Prescaler_16);
	IWDG_SetReload(2500 - 1);
	IWDG_ReloadCounter(); // 提前喂一下狗，保证第一个喂狗周期就是1s，严谨一点
	IWDG_Enable(); // 喂狗或使能时，就会给键寄存器写入0x5555之外的值，所以我们不需要手动执行写保护
	while(1)
	{
		Key_GetNum();
		
		IWDG_ReloadCounter();
		
		OLED_ShowString(4, 1, "FEED");
		Delay_ms(200);  
		OLED_ShowString(4, 1, "    ");
		Delay_ms(600); 
	}
}

/*

WDG(Watchdog)看门狗

看门狗可以监控程序的运行状态，当程序因为设计漏洞、硬件故障电磁干扰等原因，出现卡死或跑飞现象时，看门狗能及时复位程序避免程序陷入长时间的罢工状态，保证系统的可靠性和安全性

看门狗本质上是一个定时器，当指定时间范围内，程序没有执行喂狗(重置计数器)操作时，看门狗硬件电路就自动产生复位信号
STM32内置两个看门狗

  独立看门狗(IWDG)：独立工作，对时间精度要求较低

  窗口看门狗(WWDG)：使用APB1的时钟，没有专用的时钟，不算是独立，要求看门狗在精确计时窗口起作用

*/


